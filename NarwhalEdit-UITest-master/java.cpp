#include "java.h"

Java::Java()
{
    this->keywordPatterns   <<"\\babstract\\b"<<"\\bassert\\b"<<"\\bboolean\\b"
                            <<"\\bbreak\\b"<<"\\bbyte\\b"<<"\\bcase\\b"
                            <<"\\bcatch\\b"<<"\\bchar\\b"<<"\\bclass\\b"
                            <<"\\bcontinue\\b"<<"\\bdefault\\b"<<"\\bdo\\b"
                            <<"\\bdouble\\b"<<"\\belse\\b"<<"\\benum\\b"
                            <<"\\bextends\\b"<<"\\bfinal\\b"<<"\\bfinally\\b"
                            <<"\\bfloat\\b"<<"\\bfor\\b"<<"\\bif\\b"<<"\\bimplements\\b"
                            <<"\\bimport\\b"<<"\\binstanceof\\b"<<"\\bint\\b"
                            <<"\\binterface\\b"<<"\\blong\\b"<<"\\bnative\\b"
                            <<"\\bnew\\b"<<"\\bpackage\\b"<<"\\bprivate\\b"
                            <<"\\bprotected\\b"<<"\\bpublic\\b"<<"\\breturn\\b"
                            <<"\\bshort\\b"<<"\\bstatic\\b"<<"\\bstrictfp\\b"
                            <<"\\bsuper\\b"<<"\\bswitch\\b"<<"\\bsynchronized\\b"
                            <<"\\bthis\\b"<<"\\bthrow\\b"<<"\\bthrows\\b"
                            <<"\\btransient\\b"<<"\\btry\\b"<<"\\bvoid\\b"
                            <<"\\bvolatile\\b"<<"\\bwhile\\b"<<"\\btrue\\b"<<"\\bfalse\\b";
    this->commentStartExpression = QRegularExpression("/\\*");
    this->commentEndExpression = QRegularExpression("\\*/");
    this->singleLineComment= "//";

}
