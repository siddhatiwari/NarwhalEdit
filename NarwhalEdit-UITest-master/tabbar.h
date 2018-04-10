#ifndef TABBAR_H
#define TABBAR_H

#include <QWidget>
#include <QTabWidget>

class TabBar : public QTabWidget
{

public:
    TabBar();
    //virtual ~TabBar();
    void createEditorTab();

private slots:
    void handleCloseRequested(int tabIndex);
};

#endif // TABBAR_H
