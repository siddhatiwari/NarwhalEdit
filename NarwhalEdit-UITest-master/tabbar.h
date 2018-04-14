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
    void createEditorTab(CodeEditor *codeEditor);

private slots:
    void handleCloseRequested(int tabIndex);
};

#endif // TABBAR_H
