#include "c_plus_plus.h"

C_Plus_Plus::C_Plus_Plus()
{
    this->keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                                         << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                                         << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                                         << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                                         << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                                         << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                                         << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                                         << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                                         << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                                         << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b";
    this->commentStartExpression = QRegularExpression("/\\*");
    this->commentEndExpression = QRegularExpression("\\*/");
    this->singleLineRegularExpression= QRegularExpression("(?=([^(\")]*\"[^(\")]*\")*[^\"]*$)//[^\n]*"); //(?=([^\"]*\"[^\"]*\")*[^\"]*$)
}
//[^\n]*
//([^\n]*[^\\\"]*\\\"[^\\\"]*\\\")*[^\\\"]*
