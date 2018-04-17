
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include "tabbar.h"
#include <QTextCursor>
#include <QToolButton>
#include <vector>

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
    void about();
    void updateLineNumber(int lineNumber);
    void startServerAction();
    void closeServerAction();
    void connectAction();
    void disconnectAction();
    void connectionInfoAction();
    void updateHighlightingAction(int index);

private:
    void setupStatusBar();
    void createActions();
    void createMenus();
    void createTabBar();
    void createTextEditor();
    void createSideBar();
    void createTab(CodeEditor *codeEditor, QString title = "New Tab");
    void closeEvent(QCloseEvent *bar) override;
    void updateNetworkMenuOptions();

    TabBar *tabBar;
    CodeEditor *currentEditor;
    QLabel *lineNumberLabel;
    QToolButton *highlightingButton;
    QMenu *syntaxMenu;
    QMenu *fileMenu;
    QMenu *networkMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;
    QActionGroup *alignmentGroup;
    std::vector<QAction *> syntaxHighlightingActs;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *startServerAct;
    QAction *closeServerAct;
    QAction *connectAct;
    QAction *disconnectAct;
    QAction *connectionInfoAct;
    QAction *setLineSpacingAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

};

#endif
