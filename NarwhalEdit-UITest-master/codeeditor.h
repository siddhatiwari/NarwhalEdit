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

    /**
     * Paints the current line with params with the current event
     *
     * @brief Handles line painting
     * @param event
     * @return void
     */

    void lineNumberAreaPaintEvent(QPaintEvent *event);

    /**
     * @brief Returns the line area width
     * @return int
     */

    int lineNumberAreaWidth();

    QCompleter *completer() const;
    Server *editorServer;
    QTcpSocket *editorSocket;
    FindWidget *findWidget;
    int connectedPort = 0;
    QString filePath = "";

    /**
     * @brief Get documentSaved
     * @return bool
     */

    bool getDocumentSaved();

    /**
     * @brief Set documentSaved
     * @param bool
     */

    void setDocumentSaved(bool saved);

    /**
     * @brief Get currentLine
     * @return int
     */

    int getCurrentLine();

    /**
     * @brief highlightCurrentLine
     * @return void
     */

    void highlightCurrentLine();
    Highlighter* getHighlighter();

    /**
     * @brief Does re-highliting
     * @return void
     */

    void doRehighlight();

private:

    /**
     * @brief Sets up the editor
     * @return void
     */

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

    /**
     * @brief Handle the resize event
     * @param QResizeEvent
     */

    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief Handle the key press event
     * @param QKeyEvent
     */

    void keyPressEvent(QKeyEvent *e) override;

    /**
     * @brief Handle the focus in event
     * @param QFocusEvent
     */

    void focusInEvent(QFocusEvent *e) override;

signals:

    /**
     * @brief Update the line number
     * @param int
     */

    void updateLineNumber(int lineNumber);

private slots:

    /**
     * @brief Updates the line number area width
     * @param int
     * @return none
     */

    void updateLineNumberAreaWidth(int newBlockCount);

    /**
     * Highlights the current line
     *
     * @brief Highlights current line
     * @return none
     */

    void updateLineNumberArea(const QRect &, int);

    /**
     * Rehighlights all of the text in the document
     *
     * @brief Rehighlights text
     * @return none
     */
    void rehighlight();

    /**
     * Completes the text based on prior input
     *
     * @brief Completes the text
     * @return none
     */
    void completeText();

    /**
     * Tries to autocomplete certain characters that are typed
     *
     * @brief Try to autocomplete character
     * @return none
     */
    void tryAutocompete();

    /**
     * @brief Try to ignore some characters that are entered
     * @return none
     */

    void tryIgnore();

    /**
     * @brief Write data to socket
     * @return bool
     */

    bool writeData();

    /**
     * @brief Send data to sockets
     * @param QByteArray
     * @param QTcpSocket
     * @return none
     */

    void sendData(QByteArray data, QTcpSocket *sender);

    /**
     * @brief Update the text
     * @return none
     */

    void updateText();

    /**
     * @brief Finds words that need completion
     * @return none
     */

    void findCompletionKeywords();

    /**
     * @brief Inserts the completion text
     * @param QString
     */

    void insertCompletion(const QString &completion);

    /**
     * @brief Calculated the current line number
     * @return none
     */

    void calculateNewLineNumber();

    /**
     * @brief Finds some text and highlights it
     * @param QString
     */

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

    /**
     * @brief Handles the paint event
     * @param QPaintEvent
     */

    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};
#endif
