#include "objectivec.h"

ObjectiveC::ObjectiveC()
{
    this->keywordPatterns << "\\bchar\\b" << "\\binterface\\b" << "\\bimplementation\\b"
                                             << "\\bprotocol\\b" << "\\bend\\b" << "\\bprivate\\b"
                                             << "\\bpublic\\b" << "\\bprotected\\b" << "\\btry\\b"
                                             << "\\bthrow\\b" << "\\bcatch\\b" << "\\bfinally\\b"
                                             << "\\bclass\\b" << "\\bselector\\b" << "\\bencode\\b"
                                             << "\\bstring\\b" << "\\bsynchronized\\b" << "\\bin\\b"
                                             << "\\bout\\b" << "\\binout\\b" << "\\bbycopy\\b"
                                             << "\\bbyref\\b" << "\\boneway\\b" << "\\balloc\\b"
                                             << "\\bretain\\b" << "\\brelease\\b" << "\\bautorelease\\b"
                                             << "\\bbool\\b" << "\\bsuper\\b" << "\\bself\\b" << "\\bimport\\b"
                                             <<"\\belse\\b"<<"\\bif\\b";
        this->commentStartExpression = QRegularExpression("/\\*");
        this->commentEndExpression = QRegularExpression("\\*/");
        this->singleLineComment="//";
}
