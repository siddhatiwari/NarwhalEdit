#ifndef TABBAR_H
#define TABBAR_H

#include <QWidget>
#include <QTabWidget>
#include "codeeditor.h"

class TabBar : public QTabWidget
{

public:
    TabBar();
    ~TabBar();
    void createEditorTab(CodeEditor *codeEditor, QString title = "New Tab");
    bool quitRequested();
    bool handleTabCloseRequest(int tabIndex);

private slots:
    bool handleCloseRequested(int tabIndex);
};

#endif // TABBAR_H
