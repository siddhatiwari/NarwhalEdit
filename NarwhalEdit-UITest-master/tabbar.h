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

    /**
     * @brief Creates an editor tab based on inputed CodeEditor
     * @param CodeEditor
     * @param QString
     */

    void createEditorTab(CodeEditor *codeEditor, QString title = "New Tab");

    /**
     * @brief Handles a quit requested
     * @return bool
     */

    bool quitRequested();

    /**
     * @brief Handles a close request
     * @param int
     * @return bool
     */

    bool handleTabCloseRequest(int tabIndex);
    bool programExiting = false;

private slots:

    /**
     * @brief Handles a tab close request
     * @param int
     * @return none
     */

    bool handleCloseRequested(int tabIndex);

};

#endif // TABBAR_H
