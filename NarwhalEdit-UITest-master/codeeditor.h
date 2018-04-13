#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include <QCompleter>
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
    ~CodeEditor();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void setCompleter(QCompleter *c);
    QCompleter *completer() const;

private:
    QWidget *lineNumberArea;
    Highlighter *highlighter;
    QStringList keywords;
    const std::map<QChar, QChar> AUTOCOMPLETE_CHARACTERS = {
        {'{', '}'},
        {'(', ')'},
        {'[', ']'},
    };
    QString textUnderCursor() const;
    QCompleter *cmpltr;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;

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
     * Completes the text based on prior input
     *
     * @brief Completes the text
     * @param none
     * @return void
     */
    void completeText();

    /**
     * Tries to autocomplete certain characters that are typed
     *
     * @brief Try to autocomplete character
     * @param none
     * @return void
     */
    void tryAutocompete();



    void tryIgnore();

    void findCompletionKeywords();

    void insertCompletion(const QString &completion);

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
