#include "globals.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <AppKit/NSWindow.h>
#import <AppKit/NSView.h>
#import <AppKit/NSColor.h>
#import <AppKit/NSAppearance.h>

bool whiteTheme = true;
MainWindow *mainWindow = 0;

void setTheme(bool wTheme)
{
    whiteTheme = wTheme;
    QCoreApplication::setAttribute( Qt::AA_DontCreateNativeWidgetSiblings );
    NSView *nsview = ( __bridge NSView * )reinterpret_cast<void *>( mainWindow->window()->winId() );
    NSWindow *nswindow = [nsview window];
    QFile *file;

    if (!whiteTheme) {

        file = new QFile(":/Files/DarkTabBarStyleSheet.qss");

        [nswindow setOpaque:NO];
        [nswindow invalidateShadow];
        nswindow.titlebarAppearsTransparent = true;
        //nswindow.titleVisibility = NSWindow.TitleVisibility.hidden;
        nswindow.appearance = [NSAppearance appearanceNamed:NSAppearanceNameVibrantDark];
        nswindow.backgroundColor = NSColor.blackColor;
        //[nswindow setTitleVisibility:NSWindowTitleHidden];
        //[nswindow setStyleMask:NSFullSizeContentViewWindowMask];
        //mainWindow->statusBar()->setStyleSheet("background-color: rgb(0, 255, 0);");
    }
    else {

        file = new QFile(":/Files/TabBarStyleSheet.qss");

        [nswindow setOpaque:NO];
        [nswindow invalidateShadow];
        nswindow.titlebarAppearsTransparent = true;
        nswindow.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        nswindow.backgroundColor = [NSColor colorWithCalibratedRed:200 green:200 blue:200 alpha:1.0f];

    }

    qDebug() << file->bytesAvailable();
    if(file->open(QFile::ReadOnly)) {
        QString StyleSheet = QLatin1String(file->readAll());
        qApp->setStyleSheet(StyleSheet);
    }

}