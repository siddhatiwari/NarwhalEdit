#ifndef LANGUAGE_H
#define LANGUAGE_H
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <string>

class Language
{
protected:
    QStringList keywordPatterns;
    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;
    std::string singleLineComment;
public:
    QStringList getKeywordPatterns();
    QRegularExpression getCommentStartExpression();
    QRegularExpression getCommentEndExpression();
    std::string getSingleLineComment();
};

#endif // LANGUAGE_H
