
#include <QtWidgets>
#include <QTextCursor>
#include <QDebug>
#include <QShortcut>
#include <QKeySequence>
#include <QTextEdit>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QRect>
#include <QFile>
#include <QBoxLayout>
#include "mainwindow.h"
#include "codeeditor.h"

MainWindow::MainWindow()
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                              "invoke a context menu</i>"));
    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Box);
    infoLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(infoLabel);
    widget->setLayout(layout);

    setupStatusBar();
    createActions();
    createMenus();
    createTabBar();

    layout->addWidget(tabBar);
    CodeEditor *codeEditor = new CodeEditor();
    createTab(codeEditor);

    setWindowTitle(tr("Menus"));
    setMinimumSize(160, 160);
    QRect screen = QApplication::desktop()->screenGeometry();
    resize(screen.width(), screen.height());

}

#ifndef QT_NO_CONTEXTMENU
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(cutAct);
    menu.addAction(copyAct);
    menu.addAction(pasteAct);
    menu.exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU

void MainWindow::createTab(CodeEditor *codeEditor, QString title)
{
    tabBar->createEditorTab(codeEditor, title);
    connect(codeEditor, SIGNAL(updateLineNumber(int)), this, SLOT(updateLineNumber(int)));
    int lastTabIndex = tabBar->count() - 1;
    tabBar->setCurrentIndex(lastTabIndex);
}

void MainWindow::newFile()
{
    infoLabel->setText(tr("Invoked <b>File|New</b>"));

    qDebug() << "1";

    CodeEditor *codeEditor = new CodeEditor();
    createTab(codeEditor);

    qDebug() << "2";
}

void MainWindow::open()
{
    infoLabel->setText(tr("Invoked <b>File|Open</b>"));
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), tr("Text File (*.txt)"));

    if (fileName.isEmpty())
        return;
    else {

        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }

        QDataStream in(&file);
        QString text;

        in >> text;

        file.flush();
        file.close();

        qDebug() << "1";

        int currentTabIndex = tabBar->currentIndex();
        if (currentEditor->document()->isEmpty() && tabBar->tabText(currentTabIndex) == "New Tab")
            tabBar->removeTab(currentTabIndex);

        CodeEditor *codeEditor = new CodeEditor();
        codeEditor->document()->setPlainText(text);

        QFileInfo fileInfo(fileName);
        createTab(codeEditor, fileInfo.fileName());

        qDebug() << "2";

    }
}

void MainWindow::save()
{
    infoLabel->setText(tr("Invoked <b>File|Save</b>"));

    if (tabBar->count() > 0) {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text (*.txt);;All Files (*)"));
        if (fileName.isEmpty())
            return;
        else {

            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, tr("Unable to open file"), file.errorString());
                return;
            }

            QDataStream out(&file);
            CodeEditor *currentEditor = qobject_cast<CodeEditor *>(tabBar->currentWidget());

            out << currentEditor->toPlainText();

            file.flush();
            file.close();

            QFileInfo fileInfo(fileName);
            int currentIndex = tabBar->currentIndex();
            tabBar->setTabText(currentIndex, fileInfo.fileName());
            currentEditor->setDocumentSaved(true);
        }

    }
}

void MainWindow::undo()
{
    infoLabel->setText(tr("Invoked <b>Edit|Undo</b>"));
    currentEditor->undo();
}

void MainWindow::redo()
{
    infoLabel->setText(tr("Invoked <b>Edit|Redo</b>"));
    currentEditor->redo();
}

void MainWindow::cut()
{
    infoLabel->setText(tr("Invoked <b>Edit|Cut</b>"));
    currentEditor->cut();
}

void MainWindow::copy()
{
    infoLabel->setText(tr("Invoked <b>Edit|Copy</b>"));
    currentEditor->copy();
}

void MainWindow::paste()
{
    infoLabel->setText(tr("Invoked <b>Edit|Paste</b>"));
    currentEditor->paste();
}

void MainWindow::setLineSpacing()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Set Line Spacing</b>"));
}

void MainWindow::about()
{
    infoLabel->setText(tr("Invoked <b>Help|About</b>"));
    QMessageBox::about(this, tr("About Menu"),
            tr("NarwhalEdit is a multi-user text editor"));
}

void MainWindow::aboutQt()
{
    infoLabel->setText(tr("Invoked <b>Help|About Qt</b>"));
}

void MainWindow::updateLineNumber(int lineNumber)
{
    lineNumberLabel->setText("Line: " + QString::number(lineNumber));
}

void MainWindow::setupStatusBar()
{
    lineNumberLabel = new QLabel("Line: 1");
    highlightingButton = new QToolButton();
    highlightingButton->setPopupMode(QToolButton::MenuButtonPopup);
    syntaxMenu = new QMenu();
    statusBar()->addWidget(lineNumberLabel, 1);
    statusBar()->addWidget(highlightingButton, 1);
}

void MainWindow::createActions()
{
    QStringList options = {"asdf", "fda", "fdas"};
    for (int i = 0; i < options.size(); i++) {
        QAction *syntaxHighightAct = new QAction(options.at(i));
        syntaxHighlightingActs.push_back(syntaxHighightAct);
    }

    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);

    cutAct = new QAction(tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, &QAction::triggered, this, &MainWindow::cut);

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, &QAction::triggered, this, &MainWindow::copy);

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, &QAction::triggered, this, &MainWindow::paste);

    startAct = new QAction(tr("&Start"), this);
    startAct->setStatusTip(tr("Start a server so others can edit the current file"));
    connect(startAct, SIGNAL(triggered(bool)), this, SLOT(startAction()));

    connectAct = new QAction(tr("&Connect"), this);
    connectAct->setStatusTip(tr("Connect to an editor server"));
    connect(connectAct, SIGNAL(triggered(bool)), this, SLOT(connectAction()));

    connectionInfoAct = new QAction(tr("&Connection Info"), this);
    connectAct->setStatusTip(tr("Check your Server and Client connection information"));
    connect(connectionInfoAct, SIGNAL(triggered(bool)), this, SLOT(connectionInfoAction()));

    setLineSpacingAct = new QAction(tr("Set &Line Spacing..."), this);
    setLineSpacingAct->setStatusTip(tr("Change the gap between the lines of a "
                                       "paragraph"));
    connect(setLineSpacingAct, &QAction::triggered, this, &MainWindow::setLineSpacing);

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(aboutQtAct, &QAction::triggered, this, &MainWindow::aboutQt);
}

void MainWindow::createMenus()
{

    for (int i = 0; i < syntaxHighlightingActs.size(); i++) {
        QAction *currentAction = syntaxHighlightingActs.at(i);
        if (i == 0)
            highlightingButton->setDefaultAction(currentAction);
        syntaxMenu->addAction(currentAction);
        connect(currentAction, &QAction::triggered, [this, i]() {
            updateHighlightingAction(i);
        });
    }
    highlightingButton->setMenu(syntaxMenu);

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(startAct);
    fileMenu->addAction(connectAct);
    fileMenu->addAction(connectionInfoAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    formatMenu = editMenu->addMenu(tr("&Format"));
    formatMenu->addAction(setLineSpacingAct);

}

void MainWindow::createTabBar()
{
    tabBar = new TabBar();
    connect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(updateCurrentTab()));
}

void MainWindow::updateCurrentTab()
{
    currentEditor = qobject_cast<CodeEditor *>(tabBar->currentWidget());
}

void MainWindow::startAction()
{
    bool currentEditorConnected = currentEditor->editorSocket->state() == QAbstractSocket::ConnectedState;
    if (!currentEditorConnected) {
        if (!currentEditor->editorServer->isListening()) {
            int portInput = QInputDialog::getInt(this, tr("Connection Port"),
                                                     tr("Port:"), QLineEdit::Normal);
            if (portInput >= 1000 && portInput <= 9999) {
                currentEditor->editorServer->startServer(portInput);
                currentEditor->editorSocket->connectToHost(QHostAddress::Any, portInput);
                if (!currentEditor->editorSocket->waitForConnected())
                    QMessageBox::information(this, tr(""), QString("Error: Unable to connect to host"));
                else {
                    currentEditor->connectedPort = portInput;
                    qDebug() << "Connected to host";
                }
            }
            else
                QMessageBox::information(this, tr(""), QString("Error: Invalid server port " + QString::number(portInput)));
        }
        else {
            int currentTabIndex = tabBar->currentIndex();
            QMessageBox::information(this, tr(""), QString("Error: Server for '" + tabBar->tabText(currentTabIndex) + "' already running!"));
        }
    }
    else if (currentEditorConnected)
        QMessageBox::information(this, tr(""), QString("Error: Already connected to a server"));
}

void MainWindow::connectAction()
{
    bool currentEditorConnected = currentEditor->editorSocket->state() == QAbstractSocket::ConnectedState;
    if (!currentEditorConnected) {
        int portInput = QInputDialog::getInt(this, tr("Connection Port"),
                                                 tr("Port:"), QLineEdit::Normal);
        if (portInput >= 1000 && portInput <= 9999) {
            currentEditor->editorSocket->connectToHost(QHostAddress::Any, portInput);
            if (!currentEditor->editorSocket->waitForConnected())
                QMessageBox::information(this, tr(""), QString("Error: Unable to connect to host"));
            else {
                currentEditor->connectedPort = portInput;
                qDebug() << "Connected to host";
            }
        }
        else
            QMessageBox::information(this, tr(""), QString("Error: Invalid server port " + QString::number(portInput)));
    }
    else if (currentEditorConnected) {
        QString currentAddress = QString::number(currentEditor->editorSocket->localPort());
        QMessageBox::information(this, tr(""), QString("Error: Already connected to host on port " + currentAddress));
    }
    else
        QMessageBox::information(this, tr(""), QString("Error: Connection failed"));
}

void MainWindow::connectionInfoAction()
{
    QMessageBox messageBox;
    QString hosting = currentEditor->editorServer->isListening() ? "true" : "false";
    QString port = currentEditor->connectedPort == 0 ? "N/A" : QString::number(currentEditor->connectedPort);
    QString text = QString("Hosting file: " + hosting + "\n" + "Connected port: " + port);
    messageBox.setText(text);
    messageBox.exec();
}

void MainWindow::updateHighlightingAction(int index)
{
    qDebug() << syntaxHighlightingActs.at(index)->text();
    highlightingButton->setDefaultAction(syntaxHighlightingActs.at(index));
}

#include <QCloseEvent>
void MainWindow::closeEvent (QCloseEvent *event)
{
    if (tabBar->quitRequested())
        event->accept();
    else
        event->ignore();
}
