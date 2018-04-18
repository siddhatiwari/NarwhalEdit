#include "tabbar.h"
#include <QTextEdit>
#include <QTabBar>
#include <QTextCursor>
#include <Qdebug>
#include <QApplication>
#include <QMessageBox>
#include "codeeditor.h"
#include "highlighter.h"

TabBar::TabBar()
{
    this->setTabsClosable(true);
    connect(this, &TabBar::tabCloseRequested, this, &TabBar::handleCloseRequested);
}

TabBar::~TabBar()
{
}

void TabBar::createEditorTab(CodeEditor *codeEditor, QString title)
{
    addTab(codeEditor, title);
//    tabBar()->tabRect(currentIndex()).
}

bool TabBar::handleCloseRequested(int tabIndex)
{
    if (handleTabCloseRequest(tabIndex))
        this->removeTab(tabIndex);
    if (count() == 0)
        QApplication::quit();
}

bool TabBar::handleTabCloseRequest(int tabIndex)
{
    if (!programExiting) {
        bool serversOpen = false;
        bool connectionsOpen = false;
        bool unsavedDocuments = false;
        CodeEditor *currentEditor = qobject_cast<CodeEditor *>(widget(tabIndex));

        if (currentEditor->editorServer->isListening())
            serversOpen = true;
        if (currentEditor->editorSocket->state() == QAbstractSocket::ConnectedState)
            connectionsOpen = true;
        if (!currentEditor->getDocumentSaved())
            unsavedDocuments = true;

        QMessageBox *message = new QMessageBox();
        message->addButton(QMessageBox::Yes);
        message->addButton(QMessageBox::No);
        message->setDefaultButton(QMessageBox::Yes);
        if (serversOpen) {
            message->setText("There are editor server(s) open, do you still want to close?");
            return message->exec() == QMessageBox::Yes;
        }
        if (connectionsOpen) {
            message->setText("There are editor connection(s) open, do you still want to close?");
            return message->exec() == QMessageBox::Yes;
        }
        if (unsavedDocuments) {
            message->setText("There are unsaved document(s) open, do you still want to close?");
            return message->exec() == QMessageBox::Yes;
        }

        return false;
    }
    else
        return true;
}

bool TabBar::quitRequested()
{
    bool quitConfirmed = false;
    for (int i = 0; i < count(); i++) {
        quitConfirmed = handleTabCloseRequest(i);
        if (quitConfirmed)
            break;
    }
    return quitConfirmed;
}
