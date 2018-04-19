#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include "globals.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Siddha Tiwari");
    QApplication::setApplicationName("NarwhalEdit");

    QSettings settings;
    settings.beginGroup("MainWindow");
    whiteTheme = settings.value("whiteTheme", true).toBool();
    MainWindow *w = new MainWindow();
    mainWindow = w;

    setTheme(settings.value("whiteTheme", true).toBool());
    settings.endGroup();

    w->show();

    return a.exec();
}
