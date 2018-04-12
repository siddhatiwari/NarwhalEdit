#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include "highlighter.h"

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class LineNumberArea;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

private:
    QWidget *lineNumberArea;
    Highlighter *highlighter;
    std::map<QChar, QChar> autocompleteChars;
    bool autocompleting;

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);

    /**
     * Highlights the current line
     *
     * @brief Highlights current line
     * @param none
     * @return void
     */
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

    /**
     * Rehighlights all of the text in the document
     *
     * @brief Rehighlight text
     * @param none
     * @return void
     */
    void rehighlight();

    /**
     * Tries to autocomplete certain characters that are typed
     *
     * @brief Try to autocomplete character
     * @param none
     * @return void
     */
    void tryAutocompete();

    void completeText();

    void tryIgnore();

};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }


protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};


#endif
