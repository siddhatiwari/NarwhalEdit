#include <QtWidgets>
#include <exception>
#include <algorithm>
#include "codeeditor.h"
#include "mainwindow.h"

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    // Sets tab
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(15);
    setFont(font);

    // Sets tab size to 4 spaces
    const int tabSize = 4;
    QFontMetrics metrics(this->font());
    this->setTabStopWidth(tabSize * metrics.width(' '));

    lineNumberArea = new LineNumberArea(this);

    // Sets up highlighter
    highlighter = new Highlighter(document());

    QCompleter *cmp = new QCompleter(this->parent());
    setCompleter(cmp);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(textChanged()), this, SLOT(rehighlight()));
    connect(this, SIGNAL(textChanged()), this, SLOT(completeText()));
    connect(this, SIGNAL(textChanged()), this, SLOT(findCompletionKeywords()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateLineNumber()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

CodeEditor::~CodeEditor()
{
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

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

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
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

    tryIgnore();
    tryAutocompete();

    blockSignals(false);
}

void CodeEditor::tryAutocompete()
{
    QString currentText = toPlainText();
    if (currentText.size() > 0 ) {
        int position = textCursor().position();
        QChar previousChar = currentText.at(position - 1);
        if (AUTOCOMPLETE_CHARACTERS.find(previousChar) != AUTOCOMPLETE_CHARACTERS.end()) {
            insertPlainText(AUTOCOMPLETE_CHARACTERS.at(previousChar));
            moveCursor(QTextCursor::Left);
        }
    }
}

void CodeEditor::tryIgnore()
{
    QString currentText = toPlainText();
    int position = textCursor().position();

    if (position < currentText.size()) {
        QChar previousChar = currentText.at(position - 1);
        QChar nextChar = currentText.at(position);

        std::vector<QChar> ignoredChars;
        ignoredChars.reserve(AUTOCOMPLETE_CHARACTERS.size());
        for(auto const& imap: AUTOCOMPLETE_CHARACTERS)
            ignoredChars.push_back(imap.second);

        if(std::find(ignoredChars.begin(), ignoredChars.end(), nextChar) != ignoredChars.end()) {
            if (previousChar == nextChar) {
                moveCursor(QTextCursor::Right);
                textCursor().deletePreviousChar();
            }
        }

    }
}

void CodeEditor::findCompletionKeywords()
{
    QString currentText = toPlainText();
    QStringList wordList;

    QString tempKeyword;
    std::vector<char> excludeChars = {' ', '.', '(', ')', '{', '}',
                                 '[', ']', '<', '>', '\n', '\t'};
    for (int i = 0; i < currentText.size(); i++) {
        if (std::find(excludeChars.begin(), excludeChars.end(), currentText[i]) != excludeChars.end()) {
            if (!wordList.contains(tempKeyword))
                wordList << tempKeyword;
                tempKeyword = "";
            }
        else
            tempKeyword += currentText[i];
    }

    QCompleter *cmp = new QCompleter(wordList, this->parent());
    setCompleter(cmp);
}

void CodeEditor::setCompleter(QCompleter *completer)
{
    if (cmpltr) {
        cmpltr->popup()->setMaximumSize(0, 0);
        QObject::disconnect(cmpltr, 0, this, 0);
    }

    cmpltr = completer;

    if (!cmpltr)
        return;

    cmpltr->setWidget(this);
    cmpltr->setCompletionMode(QCompleter::PopupCompletion);
    cmpltr->setCaseSensitivity(Qt::CaseInsensitive);
    cmpltr->setWrapAround(false);
    connect(cmpltr, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
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
    if (cmpltr && cmpltr->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_D); // CTRL+E
    if (!cmpltr || !isShortcut) // do not process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!cmpltr || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
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
    cmpltr->complete(cr); // popup it up!
}

void CodeEditor::updateLineNumber()
{
    QString currentText = toPlainText();
    int newLineCount = 0;
    for (int i = 0; i < currentText.size(); i++) {
        if (currentText[i] == '\n')
            newLineCount++;
    }
    qDebug() << newLineCount;
}
