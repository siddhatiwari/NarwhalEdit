#ifndef GLOBALS_H
#define GLOBALS_H
#include "mainwindow.h"

extern bool whiteTheme;
extern MainWindow *mainWindow;
void setTheme(bool whiteTheme);
QString addDarkThemeHTML(QString text);

#endif // GLOBALS_H
