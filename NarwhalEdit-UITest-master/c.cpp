#include "c.h"

C::C()
{
    this->keywordPatterns <<  "\\bauto\\b"
                           << "\\bbreak\\b"
                           << "\\bcase\\b"
                           << "\\bchar\\b"
                           << "\\bconst\\b"
                           << "\\bcontinue\\b"
                           << "\\bdefault\\b"
                           << "\\bdo\\b"
                           << "\\bdouble\\b"
                           << "\\belse\\b"
                           << "\\benum\\b"
                           << "\\bextern\\b"
                           << "\\bfloat\\b"
                           << "\\bfor\\b"
                           << "\\bgoto\\b"
                           << "\\bif\\b"
                           << "\\binline\\b"
                           << "\\bint\\b"
                           << "\\blong\\b"
                           << "\\bbool\\b"
                           << "\\bregister\\b"
                           << "\\brestrict\\b"
                           << "\\breturn\\b"
                           << "\\bshort\\b"
                           << "\\bsigned\\b"
                           << "\\bsizeof\\b"
                           << "\\bstatic\\b"
                           << "\\bstruct\\b"
                           << "\\bswitch\\b"
                           << "\\btypedef\\b"
                           << "\\bunion\\b"
                           << "\\bunsigned\\b"
                           << "\\bvoid\\b"
                           << "\\bvolatile\\b"
                           << "\\bwhile\\b"
                           << "\\b_Alignas\\b"
                           << "\\b_Alignof\\b"
                           << "\\b_Atomic\\b"
                           << "\\b_Bool\\b"
                           << "\\b_Complex\\b"
                           << "\\b_Generic\\b"
                           << "\\b_Imaginary\\b"
                           << "\\b_Noreturn\\b"
                           << "\\btrue\\b"
                           << "\\bfalse\\b"
                           << "\\b_Static_assert\\b"
                           << "\\b_Thread_local\\b";
    this->commentStartExpression = QRegularExpression("/\\*");
    this->commentEndExpression = QRegularExpression("\\*/");
    this->singleLineComment = "//";
}
