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

    // Creates the global font
    createFont();

    // Gets settings from current settings if they exist
    QSettings settings;
    settings.beginGroup("MainWindow");
    whiteTheme = settings.value("whiteTheme", true).toBool();
    MainWindow *w = new MainWindow();
    w->setWindowIcon(QIcon(":/Files/NarwhalEditLogo.png"));
    mainWindow = w;

    // Set the theme from current settings if they exist
    setTheme(settings.value("whiteTheme", true).toBool());
    settings.endGroup();

    w->show();

    return a.exec();
}
