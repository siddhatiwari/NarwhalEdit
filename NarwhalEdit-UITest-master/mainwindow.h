
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

    // The following handle their respective actions

    void newFile();
    void open();
    void save();
    void saveAs();
    void find();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void about();
    void updateLineNumber(int lineNumber);

    // The following are actions that are taken to handle their events

    void startServerAction();
    void closeServerAction();
    void connectAction();
    void disconnectAction();
    void connectionInfoAction();
    void darkThemeAction();
    void defaultThemeAction();
    void ctrlWAction();
    void updateHighlightingAction(int index);

private:

    /**
     * @brief Sets up the status bar
     * @return none
     */

    void setupStatusBar();

    // Creates Actions
    void createActions();

    // Creates Menus
    void createMenus();

    // Creates tab bar
    void createTabBar();

    // Creates a text editor
    void createTextEditor();

    // Creates a sidebar
    void createSideBar();

    /**
     * @brief Creates a tab based on an inputed CodeEditor
     * @param CodeEditor
     * @param QString
     * @return none
     */

    void createTab(CodeEditor *codeEditor, QString title = "New Tab");

    /**
     * @brief Handles the close event
     * @param QCloseEvent
     * @return none
     */

    void closeEvent(QCloseEvent *bar) override;

    /**
     * @brief Updates the network menu options
     * @return none
     */

    void updateNetworkMenuOptions();

    TabBar *tabBar;
    CodeEditor *currentEditor;
    QWidget *findWidget;
    QLabel *lineNumberLabel;
    QToolButton *highlightingButton;
    QMenu *syntaxMenu;
    QMenu *fileMenu;
    QMenu *networkMenu;
    QMenu *viewMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QActionGroup *alignmentGroup;
    std::vector<QAction *> syntaxHighlightingActs;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *findAct;
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
    QAction *darkThemeAct;
    QAction *defaultThemeAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

};

#endif
