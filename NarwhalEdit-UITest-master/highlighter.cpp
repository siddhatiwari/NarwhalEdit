
#include "highlighter.h"
#include <QDebug>
#include <vector>

Highlighter::Highlighter(QTextDocument *parent, Language *language)
    : QSyntaxHighlighter(parent)
{
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
    rule.pattern = language->getSingleLineRegularExpression();
    rule.format = singleLineCommentFormat;

    multiLineCommentFormat.setForeground(Qt::red);

    commentStartExpression = language->getCommentStartExpression();
    commentEndExpression = language->getCommentEndExpression();
}

void Highlighter::highlightBlock(const QString &text)
{

    setCurrentBlockState(0);

    QTextCharFormat defaultFormat;
    defaultFormat.setBackground(Qt::transparent);
    defaultFormat.setForeground(QColor(210, 210, 210));
    setFormat(0, text.size(), defaultFormat);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    std::vector<char> quotesStack;
    QChar previousChar;
    QChar currentChar;
    QChar nextChar;
    int start;
    int end;
    int singleLineStartIndex = -1;
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

        if (quotesStack.size() == 0 && currentChar == '/'  && nextChar == '/')
            singleLineStartIndex = i;

        if (quotesStack.size() == 0 && !isFalseQuote && currentChar == '"'){
            quotesStack.push_back('"');
            start = i;
        }
        else if (quotesStack.size() == 1 && !isFalseQuote && currentChar == '"' && i != start && singleLineStartIndex == -1){
            quotesStack.pop_back();
            end = i;
            setFormat(start, end - start + 1, quotationFormat);
        }

    }

    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }

    }

    if (singleLineStartIndex != -1)
        setFormat(singleLineStartIndex, text.size(), singleLineCommentFormat);

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
