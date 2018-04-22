
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
#include "globals.h"

MainWindow::MainWindow()
{

    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    widget->setLayout(layout);

    setupStatusBar();
    createActions();
    createMenus();
    createTabBar();

    layout->addWidget(tabBar);
    CodeEditor *codeEditor = new CodeEditor();
    createTab(codeEditor);

    setWindowTitle(tr("NarwhalEdit"));
    setMinimumSize(160, 160);

    QSettings settings;
    settings.beginGroup("MainWindow");
    QRect screen = QApplication::desktop()->screenGeometry();
    resize(settings.value("size", screen.size()).toSize());
    settings.endGroup();

    setWindowTitle("NarwhalEdit - " + tabBar->tabText(tabBar->currentIndex()));
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
    CodeEditor *codeEditor = new CodeEditor();
    createTab(codeEditor);
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));

    if (fileName.isEmpty())
        return;
    else {

        for (int i = 0; i < tabBar->count(); i++) {
            CodeEditor *editor = qobject_cast<CodeEditor *>(tabBar->widget(i));
            if (editor->filePath == fileName) {
                tabBar->setCurrentIndex(i);
                return;
            }
        }

        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }

        QTextStream in(&file);
        QString text;

        while (!in.atEnd())
            text.append(in.readLine() + "\n");

        file.flush();
        file.close();

//        int currentTabIndex = tabBar->currentIndex();
//        if (currentEditor->document()->isEmpty() && tabBar->tabText(currentTabIndex) == "New Tab")
//            tabBar->removeTab(currentTabIndex);

        CodeEditor *codeEditor = new CodeEditor();
        codeEditor->document()->setPlainText(text);
        codeEditor->setDocumentSaved(true);
        codeEditor->filePath = fileName;
        QFileInfo fileInfo(fileName);
        createTab(codeEditor, fileInfo.fileName());

    }
}

void MainWindow::save()
{
    if (tabBar->count() > 0) {
        qDebug() << currentEditor->filePath;
        if(currentEditor->filePath == "")
            saveAs();
        else {

            QFile file(currentEditor->filePath);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, tr("Unable to open file"), file.errorString());
                return;
            }

            QTextStream out(&file);
            out << currentEditor->toPlainText();

            file.flush();
            file.close();

            QString fileName = currentEditor->filePath;
            QFileInfo fileInfo(fileName);
            int currentIndex = tabBar->currentIndex();
            tabBar->setTabText(currentIndex, fileInfo.fileName());
            currentEditor->setDocumentSaved(true);
            currentEditor->filePath = fileName;

        }
    }
}

void MainWindow::saveAs()
{
    if (tabBar->count() > 0) {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"));
        qDebug() << fileName;
        if (fileName.isEmpty())
            return;
        else {

            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, tr("Unable to open file"), file.errorString());
                return;
            }

            QTextStream out(&file);
            out << currentEditor->toPlainText();

            file.flush();
            file.close();

            QFileInfo fileInfo(fileName);
            int currentIndex = tabBar->currentIndex();
            tabBar->setTabText(currentIndex, fileInfo.fileName());
            currentEditor->setDocumentSaved(true);
            currentEditor->filePath = fileName;
        }

    }
}

void MainWindow::find()
{
    if (currentEditor->findWidget->isVisible())
        currentEditor->findWidget->hide();
    else {
        currentEditor->findWidget->show();
        currentEditor->clearFocus();
        currentEditor->findWidget->getLineEdit()->setFocus();
    }

}

void MainWindow::undo()
{
    currentEditor->undo();
    lineNumberLabel->setText("Line: " + QString::number(currentEditor->getCurrentLine()));
}

void MainWindow::redo()
{
    currentEditor->redo();
    lineNumberLabel->setText("Line: " + QString::number(currentEditor->getCurrentLine()));
}

void MainWindow::cut()
{
    currentEditor->cut();
    lineNumberLabel->setText("Line: " + QString::number(currentEditor->getCurrentLine()));
}

void MainWindow::copy()
{
    currentEditor->copy();
    lineNumberLabel->setText("Line: " + QString::number(currentEditor->getCurrentLine()));
}

void MainWindow::paste()
{
    currentEditor->paste();
    qDebug() << "asfd";
    lineNumberLabel->setText("Line: " + QString::number(currentEditor->getCurrentLine()));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Menu"),
            tr("NarwhalEdit is a multi-user text editor"));
}

void MainWindow::updateLineNumber(int lineNumber)
{
    lineNumberLabel->setText("Line: " + QString::number(lineNumber));
}

void MainWindow::setupStatusBar()
{
    lineNumberLabel = new QLabel("Line: 1");
    lineNumberLabel->setAccessibleName(QString("line-number-label"));

    highlightingButton = new QToolButton();
    highlightingButton->setAccessibleName(QString("highlighting-button"));
    highlightingButton->setPopupMode(QToolButton::MenuButtonPopup);

    syntaxMenu = new QMenu();
    syntaxMenu->setAccessibleName(QString("highlighting-menu"));
    highlightingButton->setMenu(syntaxMenu);

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
    saveAct->setShortcut(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    saveAsAct = new QAction(tr("&Save As"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document to disk with a file name"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);

    findAct = new QAction(tr("&Find"), this);
    findAct->setShortcuts(QKeySequence::Find);
    findAct->setStatusTip(tr("Find a string in the text"));
    connect(findAct, &QAction::triggered, this, &MainWindow::find);

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

    startServerAct = new QAction(tr("&Start Server"), this);
    startServerAct->setStatusTip(tr("Start a server so others can edit the current file"));
    connect(startServerAct, SIGNAL(triggered(bool)), this, SLOT(startServerAction()));

    closeServerAct = new QAction(tr("&Close Server"), this);
    closeServerAct->setStatusTip(tr("Close the currently running server"));
    connect(closeServerAct, SIGNAL(triggered(bool)), this, SLOT(closeServerAction()));

    connectAct = new QAction(tr("&Connect"), this);
    connectAct->setStatusTip(tr("Connect to an editor server"));
    connect(connectAct, SIGNAL(triggered(bool)), this, SLOT(connectAction()));

    disconnectAct = new QAction(tr("&Disconnect"), this);
    disconnectAct->setStatusTip(tr("Disconnect from the editor server"));
    connect(disconnectAct, SIGNAL(triggered(bool)), this, SLOT(disconnectAction()));

    connectionInfoAct = new QAction(tr("&Connection Info"), this);
    connectAct->setStatusTip(tr("Check your Server and Client connection information"));
    connect(connectionInfoAct, SIGNAL(triggered(bool)), this, SLOT(connectionInfoAction()));

    darkThemeAct = new QAction(tr("Dark Theme"), this);
    darkThemeAct->setStatusTip(tr("Sets the editor theme to Dark"));
    connect(darkThemeAct, SIGNAL(triggered(bool)), this, SLOT(darkThemeAction()));

    defaultThemeAct = new QAction(tr("Default Theme"), this);
    defaultThemeAct->setStatusTip(tr("Sets the editor theme to Default"));
    connect(defaultThemeAct, SIGNAL(triggered(bool)), this, SLOT(defaultThemeAction()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), this, SLOT(ctrlWAction()));
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

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();

    networkMenu = menuBar()->addMenu(QString("Network"));
    networkMenu->addAction(startServerAct);
    networkMenu->addAction(closeServerAct);
    networkMenu->addAction(connectAct);
    networkMenu->addAction(disconnectAct);
    networkMenu->addAction(connectionInfoAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(findAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(darkThemeAct);
    viewMenu->addAction(defaultThemeAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

}

void MainWindow::createTabBar()
{
    tabBar = new TabBar();
    connect(tabBar, &TabBar::currentChanged, [this]() {
        currentEditor = qobject_cast<CodeEditor *>(tabBar->currentWidget());
        lineNumberLabel->setText("Line: " + QString::number(currentEditor->getCurrentLine()));
        setWindowTitle("NarwhalEdit - " + tabBar->tabText(tabBar->currentIndex()));
        updateNetworkMenuOptions();
    });
}

void MainWindow::startServerAction()
{
    bool currentEditorConnected = currentEditor->editorSocket->state() == QAbstractSocket::ConnectedState;
    if (!currentEditorConnected) {
        if (!currentEditor->editorServer->isListening()) {
            int portInput = QInputDialog::getInt(this, QString("Connection Port"),
                                                     addDarkThemeHTML(QString("Port:")), QLineEdit::Normal);
            if (portInput >= 1000 && portInput <= 9999) {
                currentEditor->editorServer->startServer(portInput);
                if (currentEditor->editorServer->isListening()) {
                    currentEditor->editorSocket->connectToHost(QHostAddress::Any, portInput);
                    if (!currentEditor->editorSocket->waitForConnected())
                        QMessageBox::information(this, tr(""), addDarkThemeHTML(QString("Error: Unable to connect to host")));
                    else {
                        currentEditor->connectedPort = portInput;
                        qDebug() << "Connected to host";
                    }
                }
                else
                    QMessageBox::information(this, "", addDarkThemeHTML(QString("Error: Could not start server on port " + QString::number(portInput))));
            }
            else
                QMessageBox::information(this, tr(""), addDarkThemeHTML(QString("Error: Invalid server port " + QString::number(portInput))));
        }
        else {
            int currentTabIndex = tabBar->currentIndex();
            QMessageBox::information(this, tr(""), addDarkThemeHTML(QString("Error: Server for '" + tabBar->tabText(currentTabIndex) + "' already running!")));
        }
    }
    else if (currentEditorConnected)
        QMessageBox::information(this, tr(""), addDarkThemeHTML(QString("Error: Already connected to a server")));
    updateNetworkMenuOptions();
}

void MainWindow::closeServerAction()
{
    if (currentEditor->editorServer->isListening()) {
        currentEditor->editorServer->close();
        currentEditor->editorSocket->close();
        updateNetworkMenuOptions();
    }
}

void MainWindow::connectAction()
{
    bool currentEditorConnected = currentEditor->editorSocket->state() == QAbstractSocket::ConnectedState;
    if (!currentEditorConnected) {
        int portInput = QInputDialog::getInt(this, QString("Connection Port"),
                                                 addDarkThemeHTML(QString("Port:")), QLineEdit::Normal);
        if (portInput >= 1000 && portInput <= 9999) {
            currentEditor->editorSocket->connectToHost(QHostAddress::Any, portInput);
            if (!currentEditor->editorSocket->waitForConnected())
                QMessageBox::information(this, tr(""), addDarkThemeHTML(QString("Error: Unable to connect to host")));
            else {
                currentEditor->connectedPort = portInput;
                updateNetworkMenuOptions();
                qDebug() << "Connected to host";
            }
        }
        else
            QMessageBox::information(this, tr(""), addDarkThemeHTML(QString("Error: Invalid server port " + QString::number(portInput))));
    }
    else if (currentEditorConnected) {
        QString currentAddress = QString::number(currentEditor->editorSocket->localPort());
        QMessageBox::information(this, tr(""), addDarkThemeHTML(QString("Error: Already connected to host on port " + currentAddress)));
    }
    else
        QMessageBox::information(this, tr(""), addDarkThemeHTML(QString("Error: Connection failed")));
}

void MainWindow::disconnectAction()
{
    if (!currentEditor->editorServer->isListening() && currentEditor->editorSocket->state() == QAbstractSocket::ConnectedState) {
        currentEditor->editorSocket->close();
        currentEditor->connectedPort = 0;
    }
    updateNetworkMenuOptions();
}

void MainWindow::connectionInfoAction()
{
    QMessageBox messageBox;
    QString hosting = currentEditor->editorServer->isListening() ? "true" : "false";
    QString port = currentEditor->connectedPort == 0 ? "N/A" : QString::number(currentEditor->connectedPort);
    QString connections = !currentEditor->editorServer->isListening() ? "N/A" : QString::number(currentEditor->editorServer->connections);

    QString text = addDarkThemeHTML(QString("Hosting file: " + hosting + "<br />" + "Connected port: " + port + "<br />" +
                           "Server connections: " + connections));
    messageBox.setText(text);
    messageBox.exec();
}

void MainWindow::darkThemeAction()
{
    if (whiteTheme) {
        setTheme(false);
        for (int i = 0; i < tabBar->count(); i++) {
            CodeEditor *editor = qobject_cast<CodeEditor *>(tabBar->widget(i));
            editor->highlightCurrentLine();
        }

        QSettings settings;
        settings.beginGroup("MainWindow");
        settings.setValue("whiteTheme", false);
        settings.endGroup();

        qApp->processEvents();
    }
}

void MainWindow::defaultThemeAction()
{
    if (!whiteTheme) {
        setTheme(true);
        for (int i = 0; i < tabBar->count(); i++) {
            CodeEditor *editor = qobject_cast<CodeEditor *>(tabBar->widget(i));
            editor->highlightCurrentLine();
        }

        QSettings settings;
        settings.beginGroup("MainWindow");
        settings.setValue("whiteTheme", true);
        settings.endGroup();

        qApp->processEvents();
    }
}

void MainWindow::ctrlWAction()
{
    if (tabBar->handleTabCloseRequest(tabBar->currentIndex()))
        tabBar->close();
    if (tabBar->count() == 0)
        QApplication::quit();
}

void MainWindow::updateHighlightingAction(int index)
{
    qDebug() << syntaxHighlightingActs.at(index)->text();
    highlightingButton->setDefaultAction(syntaxHighlightingActs.at(index));
}

void MainWindow::updateNetworkMenuOptions()
{
    bool serverRunning = currentEditor->editorServer->isListening();
    // Start Server option
    networkMenu->actions().at(0)->setEnabled(!serverRunning);
    // Close Server option
    networkMenu->actions().at(1)->setEnabled(serverRunning);
    bool clientConnected = currentEditor->editorSocket->state() == QAbstractSocket::ConnectedState;
    // Connect to a Server option
    networkMenu->actions().at(2)->setEnabled(!clientConnected);
    // Disconnect from Server option
    if (!serverRunning && clientConnected)
        networkMenu->actions().at(3)->setEnabled(true);
    else
        networkMenu->actions().at(3)->setEnabled(false);
}

#include <QCloseEvent>
void MainWindow::closeEvent (QCloseEvent *event)
{
    if (tabBar->quitRequested()) {
        tabBar->programExiting = true;
        QSettings settings;
        settings.beginGroup("MainWindow");
        settings.setValue("size", size());
        settings.endGroup();
        event->accept();
    }
    else
        event->ignore();
}
