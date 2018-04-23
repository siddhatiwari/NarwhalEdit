
#include "highlighter.h"
#include <QDebug>
#include <vector>
#include <string>
#include "globals.h"

Highlighter::Highlighter(QTextDocument *parent, Language *language)
    : QSyntaxHighlighter(parent)
{
    setLanguage(language);
}

void Highlighter::highlightBlock(const QString &text)
{

    setCurrentBlockState(0);
    std::string singleLineCommentQString=language->getSingleLineComment();
    int lengthOfComment=singleLineCommentQString.size();

    // Sets the default text format based on theme to have a clean re-highlight
    QTextCharFormat defaultFormat;
    defaultFormat.setBackground(Qt::transparent);
    if (!whiteTheme)
        defaultFormat.setForeground(QColor(210, 210, 210));
    else
        defaultFormat.setForeground(Qt::black);
    setFormat(0, text.size(), defaultFormat);

    int startIndex = 0;

    // Match each highlighting keywords
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }

    }

    // Highlight the strings and comments that may be in the strings accordingly
    bool stringMode = false;
    QChar startCharacter;
    int startQuote = -1;
    int startComment = -1;
    for (int index = 0; index < text.size(); index++){
        QChar character = text.at(index);
        if ((character == '"' || character == '\'') && !stringMode){
            stringMode = true;
            startCharacter = character;
            startQuote = index;
        } else if (stringMode && character == startCharacter && text.at(index - 1) != '\\'){
            stringMode = false;
            setFormat(startQuote, index - startQuote + 1, quotationFormat);
        } else if (lengthOfComment>1&&text.at(index) == singleLineCommentQString.at(0) && index + 1 < text.size() && text.at(index + 1) == singleLineCommentQString.at(1) && !stringMode && (index - 1 >= 0 ? text.at(index - 1) != '*' : true)){
            startComment = index;
            break;
        }else if (lengthOfComment==1&&text.at(index) == singleLineCommentQString.at(0) && !stringMode){
            startComment = index;
            break;
        }
    }
    if (startComment != -1){
        setFormat(startComment, text.size(), singleLineCommentFormat);
    }

    // If there is a startIndex, highlight the comments
    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }

    // Format the text to find if there is text to find
    if (textToFind != "") {
        QTextCharFormat colorFormat;
        colorFormat.setBackground(Qt::yellow);

        for (int i = 0; i < text.size() - textToFind.size() + 1; i++) {
            QString test = text.mid(i, textToFind.size());
            if (test == textToFind) {
                setFormat(i, textToFind.size(), colorFormat);
            }
        }
    }

}

void Highlighter::setLanguage(Language *lang)
{
    this->language = lang;

    highlightingRules.clear();


    // Set highligting rules for keywords
    HighlightingRule rule;
    keywordFormat.setForeground(QColor(255, 128, 0));
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns<<language->getKeywordPatterns();
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Set the Class format for Qt classes
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(QColor(153, 51, 255));
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    // Set the format for comments
    quotationFormat.setForeground(QColor(0, 204, 102));
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;

    // Set the format for functions
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(QColor(51, 153, 255));
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // Set format for single-line comments
    singleLineCommentFormat.setForeground(Qt::red);
    rule.format = singleLineCommentFormat;

    multiLineCommentFormat.setForeground(Qt::red);

    commentStartExpression = language->getCommentStartExpression();
    commentEndExpression = language->getCommentEndExpression();
}
