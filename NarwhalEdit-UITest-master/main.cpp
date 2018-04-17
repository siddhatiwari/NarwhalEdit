#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Siddha Tiwari");
    QApplication::setApplicationName("NarwhalEdit");

    QFile file(":/Files/TabBarStyleSheet.qss");
    qDebug() << file.bytesAvailable();
    if(file.open(QFile::ReadOnly)) {
        QString StyleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(StyleSheet);
    }
    MainWindow w;
    w.show();

    return a.exec();
}
