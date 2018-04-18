#include "globals.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

bool whiteTheme = true;
MainWindow *mainWindow = 0;

void setTheme(bool wTheme)
{
    whiteTheme = wTheme;

    QFile *file;
    if (!whiteTheme) {
        file = new QFile(":/Files/DarkTabBarStyleSheet.qss");

        //[&mainWindow setOpaque:NO]; // YES by default
        //NSColor *semiTransparentBlue = [NSColor colorWithDeviceRed:0.0 green:0.0 blue:1.0 alpha:0.5];
        //[static_cast<id>(mainWindow) setBackgroundColor:semiTransparentBlue];
        //[mainWindow titlebarAppearsTransparent: true] // gives it "flat" look
        //window.backgroundColor = <NSColor> // set the background color
    }
    else {
        file = new QFile(":/Files/TabBarStyleSheet.qss");
    }

    qDebug() << file->bytesAvailable();
    if(file->open(QFile::ReadOnly)) {
        QString StyleSheet = QLatin1String(file->readAll());
        qApp->setStyleSheet(StyleSheet);
    }

}
