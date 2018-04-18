#ifndef TABBAR_H
#define TABBAR_H

#include <QWidget>
#include <QTabWidget>
#include <QStylePainter>
#include <QStyleOptionTab>
#include "codeeditor.h"

class TabBar : public QTabWidget
{
    QVector<int> mUnSaved;
public:
    TabBar();
    ~TabBar();
    void createEditorTab(CodeEditor *codeEditor, QString title = "New Tab");
    bool quitRequested();
    bool handleTabCloseRequest(int tabIndex);
    bool programExiting = false;

private slots:
    bool handleCloseRequested(int tabIndex);

};

#endif // TABBAR_H
