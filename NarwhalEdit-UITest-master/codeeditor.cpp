#include <QtWidgets>
#include <exception>
#include <algorithm>
#include "codeeditor.h"

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    // Sets tab
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(15);
    this->setFont(font);

    // Sets tab size to 4 spaces
    const int tabSize = 4;
    QFontMetrics metrics(this->font());
    this->setTabStopWidth(tabSize * metrics.width(' '));

    // Autocomplete text
    autocompleteChars = {
        {'{', '}'},
        {'(', ')'},
        {'[', ']'},
        {'<', '>'},
    };

    lineNumberArea = new LineNumberArea(this);
    highlighter = new Highlighter(this->document());

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(textChanged()), this, SLOT(rehighlight()));
    connect(this, SIGNAL(textChanged()), this, SLOT(completeText()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
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
    this->blockSignals(true);

    this->highlighter->rehighlight();

    this->blockSignals(false);
}

void CodeEditor::completeText()
{
    this->blockSignals(true);

    tryIgnore();
    tryAutocompete();

    this->blockSignals(false);
}

// Autocompletes certain characters if they are entered
void CodeEditor::tryAutocompete()
{
    QString currentText = this->toPlainText();
    if (currentText.size() > 0 ) {
        int position = this->textCursor().position();
        QChar previousChar = currentText.at(position - 1);
        if (autocompleteChars.find(previousChar) != autocompleteChars.end()) {
            this->insertPlainText(autocompleteChars[previousChar]);
            this->moveCursor(QTextCursor::Left);
        }
    }
}

void CodeEditor::tryIgnore()
{
    QString currentText = this->toPlainText();
    int position = this->textCursor().position();

    if (position < currentText.size()) {
        QChar previousChar = currentText.at(position - 1);
        QChar nextChar = currentText.at(position);

        std::vector<QChar> ignoredChars;
        ignoredChars.reserve(autocompleteChars.size());
        for(auto const& imap: autocompleteChars)
            ignoredChars.push_back(imap.second);

        if(std::find(ignoredChars.begin(), ignoredChars.end(), nextChar) != ignoredChars.end()) {
            if (previousChar == nextChar) {
                this->moveCursor(QTextCursor::Right);
                this->textCursor().deletePreviousChar();
            }
        }
    }

}
