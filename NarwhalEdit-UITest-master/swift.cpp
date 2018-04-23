#include "swift.h"

Swift::Swift()
{
    this->keywordPatterns   << "\\bCharacter\\b" << "\\bclass\\b" << "\\blet\\b"
                            <<"\\bassociatedtype\\b"<<"\\bbreak\\b"<<"\\bfalse\\b"
                            << "\\bDouble\\b" << "\\benum\\b" << "\\bvar\\b"
                            <<"\\blet\\b"<<"\\breturn\\b"<<"\\btrue\\b"
                            << "\\bFloat\\b" << "\\binit\\b" << "\\bInt\\b"
                            <<"\\binternal\\b"<<"\\bAny\\b"<<"\\btry\\b"
                            << "\\bcase\\b" << "\\bnamespace\\b" <<"\\bfileprivate\\b"
                            <<"\\bcontinue\\b"<<"\\bis\\b"
                            << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                            <<"\\brepeat\\b"<<"\\bas\\b"<<"\\btrhows\\b"
                            << "\\bswitch\\b" << "\\bsignals\\b"
                            <<"\\bassociatedtype\\b"<<"\\bdefault\\b"<<"\\bcatch\\b"
                            << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                            <<"\\binout\\b"<<"\\bwhere\\b"<<"\\bnil\\b"
                            << "\\bdeinit\\b" << "\\bextension\\b" << "\\bfunc\\b"
                            <<"\\bdefer\\b"<<"\\bwhile\\b"<<"\\bsuper\\b"
                            << "\\bimport\\b" << "\\bopen\\b" << "\\boperator\\b"
                            <<"\\bdo\\b"<<"\\bfallthrough\\b"<<"\\bthrow\\b"
                            << "\\bvoid\\b" << "\\bprotocol\\b" << "\\bBool\\b"
                            << "\\bsubscript\\b"<<"\\bguard\\b"<<"\\bself\\b"
                            << "\\btypealias\\b" << "\\bDict\\b" << "\\barray\\b"
                            <<"\\bfor\\b"<<"\\bin\\b"<<"\\brethrows\\b"
                            << "\\belse\\b"<<"\\bif\\b";
        this->commentStartExpression = QRegularExpression("/\\*");
        this->commentEndExpression = QRegularExpression("\\*/");
        this->singleLineComment="//";
}
