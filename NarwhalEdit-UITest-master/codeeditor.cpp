
#include <QtWidgets>
#include <exception>
#include <algorithm>
#include "codeeditor.h"
#include "mainwindow.h"
#include "globals.h"
#include "c_plus_plus.h"
#include "java.h"
#include "javascript.h"
#include "python.h"
#include "c.h"
#include "c_sharp.h"
#include "ruby.h"
#include "swift.h"
#include "objectivec.h"


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    setupEditor();
}

CodeEditor::~CodeEditor()
{
}

void CodeEditor::setupEditor()
{
    setLineWrapMode(QPlainTextEdit::NoWrap);

    QHBoxLayout *hLayout = new QHBoxLayout();
    findWidget = new FindWidget(this);
    findWidget->setAccessibleName("find-widget");
    findWidget->hide();
    connect(findWidget, SIGNAL(findButtonClicked(QString)), this, SLOT(find(QString)));
    hLayout->addWidget(findWidget);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addLayout(hLayout);
    vLayout->addStretch(1);

    setFont(globalFont);

    // Sets tab size to 4 spaces
    const int tabSize = 4;
    QFontMetrics metrics(this->font());
    this->setTabStopWidth(tabSize * metrics.width(' '));

    lineNumberArea = new LineNumberArea(this);

    // Sets up highlighter
    highlighter = new Highlighter(document(), new C_Plus_Plus());//DefaultHighlighter

    // Sets up autocompleter
    QCompleter *cmp = new QCompleter(this->parent());
    cmp->popup()->setAccessibleName("completer-popup");
    cmpltr = cmp;
    cmpltr->setWidget(this);
    cmpltr->setCompletionMode(QCompleter::PopupCompletion);
    cmpltr->setCaseSensitivity(Qt::CaseInsensitive);
    cmpltr->setWrapAround(false);
    connect(cmpltr, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));

    editorServer = new Server();
    connect(editorServer, &Server::dataReceived, this, &CodeEditor::sendData);

    editorSocket = new QTcpSocket();
    connect(editorSocket, SIGNAL(readyRead()), this, SLOT(updateText()));
    connect(editorSocket, &QTcpSocket::disconnected, [this]() {
       QMessageBox::information(this, "", QString("Disconnected from server on port "
                                                  + QString::number(connectedPort))) ;
    });

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, &CodeEditor::cursorPositionChanged, [this]() {
        highlightCurrentLine();
        calculateNewLineNumber();
    });
    connect(this, &CodeEditor::textChanged, [this]() {
        rehighlight();
        completeText();
        findCompletionKeywords();
        calculateNewLineNumber();
        documentSaved = false;
    });

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

Highlighter* CodeEditor::getHighlighter() {
    return highlighter;
}

bool CodeEditor::getDocumentSaved()
{
    return documentSaved;
}

void CodeEditor::setDocumentSaved(bool saved)
{
    documentSaved = saved;
}

int CodeEditor::getCurrentLine()
{
    return firstVisibleBlock().blockNumber() +  cursorRect().y() / cursorRect().size().height() + 1;
}

void CodeEditor::doRehighlight()
{
    rehighlight();
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 100) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits + 15;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor;
        if (whiteTheme)
            lineColor = QColor(QColor(230, 247, 255));
        else
            lineColor = QColor(48, 48, 48);

        //selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QRect adjustedRect = event->rect();
    adjustedRect.setWidth(event->rect().width());
    QPainter painter(lineNumberArea);
    if (!whiteTheme)
        painter.fillRect(adjustedRect, QColor(16, 16, 16));
    else
        painter.fillRect(adjustedRect, QColor(240, 240, 240));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            if (!whiteTheme)
                painter.setPen(QColor(130, 130, 130));
            else
                painter.setPen(Qt::darkGray);
            painter.setFont(font());
            //QRect selectedLineRect(0, adjustedRect.y(), lineNumberArea->width(), 17);
            //painter.fillRect(selectedLineRect, QColor(255, 0, 0));
            painter.drawText(0, top, lineNumberArea->width() - 5, fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void CodeEditor::rehighlight()
{
    blockSignals(true);

    highlighter->rehighlight();

    blockSignals(false);
}

void CodeEditor::completeText()
{
    blockSignals(true);

    find(findingText);
    writeData();

    blockSignals(false);
}

void CodeEditor::tryAutocompete()
{
    QString currentText = toPlainText();
    if (currentText.size() > 0 ) {
        int position = textCursor().position();
        if (position - 1 >= 0) {
            QChar previousChar = currentText.at(position - 1);
            if (AUTOCOMPLETE_CHARACTERS.find(previousChar) != AUTOCOMPLETE_CHARACTERS.end()) {
                insertPlainText(AUTOCOMPLETE_CHARACTERS.at(previousChar));
                moveCursor(QTextCursor::Left);
            }
        }
    }
}

void CodeEditor::tryIgnore()
{

    QString currentText = toPlainText();
    qDebug() << textCursor().position();
    int position = textCursor().position() - 1;

    if (position < currentText.size()) {
        if (position - 1 >= 0 && position + 1 < currentText.size()) {
            //qDebug() << "1";
            QChar previousChar = currentText.at(position - 1);
            //qDebug() << "2";
            QChar nextChar = currentText.at(position + 1);

            std::vector<QChar> ignoredChars;
            ignoredChars.reserve(AUTOCOMPLETE_CHARACTERS.size());
            for(auto const& imap: AUTOCOMPLETE_CHARACTERS)
                ignoredChars.push_back(imap.second);

            if(std::find(ignoredChars.begin(), ignoredChars.end(), nextChar) != ignoredChars.end()) {
                if (AUTOCOMPLETE_CHARACTERS.count(previousChar)) {
                    if (AUTOCOMPLETE_CHARACTERS.at(previousChar) == nextChar) {
                        moveCursor(QTextCursor::Right);
                        textCursor().deletePreviousChar();
                    }
                }
            }
        }
    }
}

bool CodeEditor::writeData()
{
    if (editorSocket->state() == QAbstractSocket::ConnectedState) {
        qDebug() << "writing data";
        QString currentText = toPlainText();
        QByteArray data = currentText.toLocal8Bit();
        editorSocket->write(data);
        return editorSocket->waitForBytesWritten();
    }
    else
        return false;
}

void CodeEditor::sendData(QByteArray data, QTcpSocket *sender)
{
    if (editorServer->isListening()) {
        for (int i = 0; i < int(editorServer->sockets.size()); i++) {
            QTcpSocket *socket = editorServer->sockets.at(i);
            if (socket != sender)
                socket->write(data);
        }
    }
}

void CodeEditor::updateText()
{
    blockSignals(true);

    QString updatedText = editorSocket->readAll();
    document()->setPlainText(updatedText);

    blockSignals(false);
}

void CodeEditor::findCompletionKeywords()
{
    std::vector<char> excludeChars = {' ', '.', '(', ')', '{', '}',
                                 '[', ']', '<', '>', '\n', '\t', '-'};
    QString currentText = toPlainText();
    int typedStart = -1;
    int typedEnd = textCursor().position();
    if (typedEnd - 1 >= 0) {
        for (int x = typedEnd - 1; x >= 0; x--){
            if (std::find(excludeChars.begin(), excludeChars.end(), currentText[x]) != excludeChars.end()) {
                typedStart = x + 1;
                break;
            }
        }
    }

    currentText = currentText.mid(0, typedStart) + currentText.mid(typedEnd);

    QStringList wordList;
    QString tempKeyword;
    for (int i = 0; i < currentText.size(); i++) {
        if (std::find(excludeChars.begin(), excludeChars.end(), currentText[i]) != std::end(excludeChars) && currentText.size()>0) {
            if (!wordList.contains(tempKeyword))
                wordList << tempKeyword;
            tempKeyword = "";
        }
        else
            tempKeyword += currentText[i];
    }

    cmpltr->setModel(new QStringListModel(wordList, cmpltr));
}

void CodeEditor::find(QString text)
{
    findingText = text;
    highlighter->textToFind = text;
    highlighter->rehighlight();
}

void CodeEditor::insertCompletion(const QString &completion)
{
    if (cmpltr->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - cmpltr->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

QString CodeEditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void CodeEditor::focusInEvent(QFocusEvent *e)
{
    if (cmpltr)
        cmpltr->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Backspace && e->key() != Qt::Key_Backspace) {
        tryIgnore();
        tryAutocompete();
    }

    if (cmpltr && cmpltr->popup()->isVisible()) {
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return;
       default:
           break;
       }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_D);
    if (!cmpltr || !isShortcut)
        QPlainTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!cmpltr || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-=");
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 1
                      || eow.contains(e->text().right(1)))) {
        cmpltr->popup()->hide();
        return;
    }

    if (completionPrefix != cmpltr->completionPrefix()) {
        cmpltr->setCompletionPrefix(completionPrefix);
        cmpltr->popup()->setCurrentIndex(cmpltr->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(cmpltr->popup()->sizeHintForColumn(0)
                + cmpltr->popup()->verticalScrollBar()->sizeHint().width());
    cmpltr->complete(cr);
}

void CodeEditor::calculateNewLineNumber()
{
    int currentLine = firstVisibleBlock().blockNumber() +  cursorRect().y() / cursorRect().size().height() + 1;
    emit updateLineNumber(currentLine);
}
