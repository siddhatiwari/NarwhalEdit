#include "objectivec.h"

ObjectiveC::ObjectiveC()
{
    this->keywordPatterns   << "\\bauto\\b"<<"\\bchar\\b" << "\\binterface\\b"
                            << "\\bimplementation\\b"<<"\\bbreak\\b"
                            <<"\\bunion\\b"<<"\\bunsigned\\b"
                            << "\\bprotocol\\b" << "\\bend\\b" << "\\bprivate\\b"
                            <<"\\bcase\\b"<<"\\btypedef\\b"<<"\\bvoidvoid"
                            << "\\bpublic\\b" << "\\bprotected\\b" << "\\btry\\b"
                            <<"\\bdefault\\b"<<"\\bint\\b"<<"\\bwhile\\b"
                            << "\\bthrow\\b" << "\\bcatch\\b" << "\\bfinally\\b"
                            <<"\\bconst\\b"<<"\\blong\\b"<<"\\bswitch\\b"
                            << "\\bclass\\b" << "\\bselector\\b" << "\\bencode\\b"
                            <<"\\bcontinue\\b"<<"\\bsigned\\b"
                            << "\\bstring\\b" << "\\bsynchronized\\b" << "\\bin\\b"
                            <<"\\bdo\\b"<<"\\binline\\b"<<"\\bsizeof\\b"
                            << "\\bout\\b" << "\\binout\\b" << "\\bbycopy\\b"
                            <<"\\bdouble\\b"<<"\\bregister\\b"<<"\\bstruct\\b"
                            << "\\bbyref\\b" << "\\boneway\\b" << "\\balloc\\b"
                            <<"\\benum\\b"<<"\\bfloat\\b"
                            <<"\\bstatic\\b"<<"\\bBOOL\\b"
                            << "\\bretain\\b" << "\\brelease\\b" << "\\bautorelease\\b"
                            <<"\\bextern\\b"<<"\\brestrict\\b"<<"\\bYES\\b"<<"\\bNO\\b"
                            << "\\bbool\\b" << "\\bsuper\\b" << "\\bself\\b"
                            << "\\bimport\\b"<<"\\bfor\\b"<<"\\breturn\\b"
                            <<"\\belse\\b"<<"\\bif\\b"
                            <<"\\bgoto\\b"<<"\\bshort\\b";
        this->commentStartExpression = QRegularExpression("/\\*");
        this->commentEndExpression = QRegularExpression("\\*/");
        this->singleLineComment="//";
}
