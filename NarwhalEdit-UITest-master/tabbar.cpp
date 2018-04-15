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

void TabBar::createEditorTab(CodeEditor *codeEditor, QString title)
{
    this->addTab(codeEditor, title);
}

void TabBar::handleCloseRequested(int tabIndex)
{
    this->removeTab(tabIndex);
}


