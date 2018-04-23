
#include "highlighter.h"
#include <QDebug>
#include <vector>
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

    QTextCharFormat defaultFormat;
    defaultFormat.setBackground(Qt::transparent);
    if (!whiteTheme)
        defaultFormat.setForeground(QColor(210, 210, 210));
    else
        defaultFormat.setForeground(Qt::black);
    setFormat(0, text.size(), defaultFormat);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }

    }

    std::vector<char> quotesStack;
    QChar previousChar;
    QChar currentChar;
    QChar nextChar;
    int start;
    int end;
    int singleLineStartIndexOne = -1;
    for (int i = 0; i < text.size(); i++) {
        previousChar = -1;
        currentChar = text.at(i);
        nextChar = -1;

        if (text.size() - 1 >= i + 1)
            nextChar = text.at(i + 1);
        else
            nextChar = -1;

        if (i != 0)
            previousChar = text.at(i - 1);
        else
            previousChar = -1;

        bool isFalseQuote = previousChar == '\\' && currentChar == '"' && quotesStack.size() == 1;

        if(lengthOfComment > 1 && previousChar!='*' && quotesStack.size() == 0 && currentChar == singleLineCommentQString.at(0)  && nextChar == singleLineCommentQString.at(1)){
            singleLineStartIndexOne = i;
            break;
        }

        if(lengthOfComment == 1 && previousChar!='*' && quotesStack.size() == 0 && currentChar == singleLineCommentQString[0]){
            singleLineStartIndexOne = i;
            break;
        }

        if (quotesStack.size() == 0 && !isFalseQuote && currentChar == '"'){
            quotesStack.push_back('"');
            start = i;
        }
        else if (quotesStack.size() == 1 && !isFalseQuote && currentChar == '"' && i != start && singleLineStartIndexOne == -1){
            quotesStack.pop_back();
            end = i;
            setFormat(start, end - start + 1, quotationFormat);
        }


    }

    if (singleLineStartIndexOne != -1)
        setFormat(singleLineStartIndexOne, text.size(), singleLineCommentFormat);

    std::vector<char> singleQuotesStack;
    int singleLineStartIndex = -1;
    start = -1;
    end = -1;
    for (int i = 0; i < text.size(); i++) {
        previousChar = -1;
        currentChar = text.at(i);
        nextChar = -1;

        if (text.size() - 1 >= i + 1)
            nextChar = text.at(i + 1);
        else
            nextChar = -1;

        if (i != 0)
            previousChar = text.at(i - 1);
        else
            previousChar = -1;

        bool isFalseQuote = previousChar == '\\' && currentChar == '\'' && singleQuotesStack.size() == 1;

        if(lengthOfComment > 1 && previousChar!='*' && singleQuotesStack.size() == 0 && currentChar == singleLineCommentQString.at(0)  && nextChar == singleLineCommentQString.at(1)){
            singleLineStartIndex = i;
            break;
        }

        if(lengthOfComment == 1 && previousChar!='*' && singleQuotesStack.size() == 0 && currentChar == singleLineCommentQString[0]){
            singleLineStartIndex = i;
            break;
        }

        if (singleQuotesStack.size() == 0 && !isFalseQuote && currentChar == '\''){
            singleQuotesStack.push_back('\'');
            start = i;
        }
        else if (singleQuotesStack.size() == 1 && !isFalseQuote && currentChar == '\'' && i != start && singleLineStartIndex == -1){
            singleQuotesStack.pop_back();
            end = i;
            setFormat(start, end - start + 1, quotationFormat);
        }


    }


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

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(QColor(153, 51, 255));
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(QColor(0, 204, 102));
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(QColor(51, 153, 255));
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::red);
    rule.format = singleLineCommentFormat;

    multiLineCommentFormat.setForeground(Qt::red);

    commentStartExpression = language->getCommentStartExpression();
    commentEndExpression = language->getCommentEndExpression();
}
