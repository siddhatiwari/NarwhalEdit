
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include "tabbar.h"
#include <QTextCursor>

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

private slots:
    void newFile();
    void open();
    void save();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void setLineSpacing();
    void setParagraphSpacing();
    void about();
    void aboutQt();
    void updateLineNumber(int lineNumber);
    void startAction();
    void connectAction();
    void connectionInfoAction();
    void updateCurrentTab();

private:
    void createActions();
    void createMenus();
    void createTabBar();
    void createTextEditor();
    void createSideBar();
    void createTab(CodeEditor *codeEditor, QString title = "New Tab");

    TabBar *tabBar;
    CodeEditor *currentEditor;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;
    QActionGroup *alignmentGroup;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *startAct;
    QAction *connectAct;
    QAction *connectionInfoAct;
    QAction *setLineSpacingAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QLabel *infoLabel;

};

#endif
