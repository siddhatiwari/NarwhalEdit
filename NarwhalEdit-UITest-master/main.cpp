#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file("/Users/wateraura/Desktop/Projects/Qt/UITest/TabBarStyleSheet.qss");
    qDebug() << file.bytesAvailable();
    if(file.open(QFile::ReadOnly)) {
        QString StyleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(StyleSheet);
    }
    MainWindow w;
    w.show();

    return a.exec();
}
