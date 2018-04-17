#ifndef LANGUAGE_H
#define LANGUAGE_H
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class Language
{
protected:
    QStringList keywordPatterns;
    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;
    QRegularExpression singleLineRegularExpression;
public:
    QStringList getKeywordPatterns();
    QRegularExpression getCommentStartExpression();
    QRegularExpression getCommentEndExpression();
    QRegularExpression getSingleLineRegularExpression();
};

#endif // LANGUAGE_H
