#include "swift.h"

Swift::Swift()
{
    this->keywordPatterns << "\\bCharacter\\b" << "\\bclass\\b" << "\\blet\\b"
                                             << "\\bDouble\\b" << "\\benum\\b" << "\\bvar\\b"
                                             << "\\bFloat\\b" << "\\binit\\b" << "\\bInt\\b"
                                             << "\\bcase\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                                             << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                                             << "\\bswitch\\b" << "\\bsignals\\b" << "\\bassociatedtype\\b"
                                             << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                                             << "\\bdeinit\\b" << "\\bextension\\b" << "\\bfunc\\b"
                                             << "\\bimport\\b" << "\\bopen\\b" << "\\boperator\\b"
                                             << "\\bvoid\\b" << "\\bprotocol\\b" << "\\bBool\\b" << "\\bsubscript\\b"
                                             << "\\btypealias\\b" << "\\bDict\\b" << "\\barray\\b"<<"\\bfor\\b"<<"\\bin\\b"
                                             << "\\belse\\b"<<"\\bif\\b";
        this->commentStartExpression = QRegularExpression("/\\*");
        this->commentEndExpression = QRegularExpression("\\*/");
        this->singleLineComment="//";
}
