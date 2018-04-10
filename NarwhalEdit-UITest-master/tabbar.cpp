#include "tabbar.h"
#include <QTextEdit>
#include <QTabBar>
#include <QTextCursor>
#include <Qdebug>
#include "codeeditor.h"

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
    //qDebug() << this->currentWidget()->toPlainText();
    /*QTextCursor *curse = new QTextCursor();
    curse->setPosition(5);
    int pos = curse->position();
    qDebug()<<pos;*/

    //I initially tried using the cursor class to keep track of where the user's pointer is in the text box, but I don't know if
    //it works like that.

}

void TabBar::handleCloseRequested(int tabIndex)
{
    this->removeTab(tabIndex);
}


