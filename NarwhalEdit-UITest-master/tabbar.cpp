#include "tabbar.h"
#include <QTextEdit>
#include <QTabBar>
#include <QTextCursor>
#include <Qdebug>
#include "codeeditor.h"
#include "highlighter.h"

TabBar::TabBar()
{
    //this->tabBar()->tabButton(0, QTabBar::LeftSide)->resize(0, 0);
    this->setTabsClosable(true);
    connect(this, &TabBar::tabCloseRequested, this, &TabBar::handleCloseRequested);
}

void TabBar::createEditorTab()
{
    CodeEditor *textEdit = new CodeEditor();
    this->addTab(textEdit, "New tab");

}

void TabBar::handleCloseRequested(int tabIndex)
{
    this->removeTab(tabIndex);
}


