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

    /**
     * @brief Gets the keyword patters
     * @return QStringList
     */

    QStringList getKeywordPatterns();

    /**
     * @brief Gets the comment start expression
     * @return QRegularExpression
     */

    QRegularExpression getCommentStartExpression();

    /**
     * @brief Gets the comment end expression
     * @return QRegularExpression
     */

    QRegularExpression getCommentEndExpression();

    /**
     * @brief Gets the single line comment
     * @return std::string
     */

    std::string getSingleLineComment();
};

#endif // LANGUAGE_H
