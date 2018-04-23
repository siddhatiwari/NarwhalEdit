#ifndef GLOBALS_H
#define GLOBALS_H
#include "mainwindow.h"

extern bool whiteTheme;
extern MainWindow *mainWindow;

/**
 * @brief Sets the theme for the editor
 * @param bool
 * @return none
 */

void setTheme(bool whiteTheme);

/**
 * @brief Creates the font for the editor
 * @return none
 */

void createFont();

/**
 * @brief Adds a dark text HTML theme to some text
 * @param QString
 * @return QString
 */

QString addDarkThemeHTML(QString text);
extern QFont globalFont;

#endif // GLOBALS_H
