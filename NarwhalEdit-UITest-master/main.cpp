#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QDir>
#include "globals.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Siddha Tiwari");
    QApplication::setApplicationName("NarwhalEdit");

    MainWindow *w = new MainWindow();
    mainWindow = w;

    setTheme(true);

    w->show();

    return a.exec();
}
