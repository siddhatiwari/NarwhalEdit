#include "language.h"
QStringList Language::getKeywordPatterns(){
    return this->keywordPatterns;
}
QRegularExpression Language::getCommentStartExpression(){
    return this->commentStartExpression;
}
QRegularExpression Language::getCommentEndExpression(){
    return this->commentEndExpression;
}
std::string Language::getSingleLineComment(){
    return this->singleLineComment;
}
