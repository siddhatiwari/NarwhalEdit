#include "tabbar.h"
#include <QTextEdit>
#include <QTabBar>
#include <QTextCursor>
#include <Qdebug>
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

void TabBar::createEditorTab(CodeEditor *codeEditor)
{
    this->addTab(codeEditor, "New tab");
}

void TabBar::handleCloseRequested(int tabIndex)
{
    this->removeTab(tabIndex);
}


