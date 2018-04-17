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
    this->addTab(codeEditor, title);
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

    if (serversOpen) {
        int response =  QMessageBox::question(
                    this, "", QString("There are editor server(s) open, do you still want to close?"),
                    QMessageBox::Yes | QMessageBox::No);
        return response == QMessageBox::Yes;
    }

    if (connectionsOpen) {
        int response =  QMessageBox::question(
                    this, "", QString("There are editor connection(s) open, do you still want to close?"),
                    QMessageBox::Yes | QMessageBox::No);
        return response == QMessageBox::Yes;
    }

    if (unsavedDocuments) {
        int response =  QMessageBox::question(
                    this, "", QString("There are unsaved documents open, do you still want to close?"),
                    QMessageBox::Yes | QMessageBox::No);
        return response == QMessageBox::Yes;
    }

    return false;
}

bool TabBar::quitRequested()
{
    bool quitConfirmed = false;
    for (int i = 0; i < count(); i++) {
        quitConfirmed = handleTabCloseRequest(i);
    }
    return quitConfirmed;
}
