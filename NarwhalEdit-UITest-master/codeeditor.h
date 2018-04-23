#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include <QCompleter>
#include <QTcpSocket>
#include "highlighter.h"
#include "server.h"
#include "findwidget.h"

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
    QCompleter *completer() const;
    Server *editorServer;
    QTcpSocket *editorSocket;
    FindWidget *findWidget;
    int connectedPort = 0;
    QString filePath = "";
    bool getDocumentSaved();
    void setDocumentSaved(bool saved);
    int getCurrentLine();
    void highlightCurrentLine();
    Highlighter* getHighlighter();
    void doRehighlight();

private:
    void setupEditor();
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
    bool documentSaved = false;
    int currentLine = 1;
    QString findingText = "";

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;

signals:
    void updateLineNumber(int lineNumber);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);

    /**
     * Highlights the current line
     *
     * @brief Highlights current line
     * @param none
     * @return void
     */

    void updateLineNumberArea(const QRect &, int);

    /**
     * Rehighlights all of the text in the document
     *
     * @brief Rehighlights text
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

    bool writeData();

    void sendData(QByteArray data, QTcpSocket *sender);

    void updateText();

    void findCompletionKeywords();

    void insertCompletion(const QString &completion);

    void calculateNewLineNumber();

    void find(QString text);

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
