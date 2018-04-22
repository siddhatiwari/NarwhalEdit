#ifndef GLOBALS_H
#define GLOBALS_H
#include "mainwindow.h"

extern bool whiteTheme;
extern MainWindow *mainWindow;
void setTheme(bool whiteTheme);
void createFont();
QString addDarkThemeHTML(QString text);
extern QFont globalFont;

#endif // GLOBALS_H
