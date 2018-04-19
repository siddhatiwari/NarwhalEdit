#include "RUBY.h"

RUBY::RUBY()
{
    this->keywordPatterns << "\\b_ENCODING_\\b" <<"\\b_LINE_\\b"
	<<"\\b_FILE_\\b"<<"\\bBEGIN\\b"<<"\\bEND\\b"<<"\\balias\\b"
	<<"\\band\\b"<<"\\bbegin\\b"<<"\\bbreak\\b"<<"\\bcase\\b"
	<<"\\bclass\\b"<<"\\bdef\\b"<<"\\bdefined?\\b"<<"\\bdo\\b"
	<<"\\belse\\b"<<"\\belsif\\b"<<"\\bend\\b"<<"\\bensure\\b"
	<<"\\bfalse\\b"<<"\\bfor\\b"<<"\\bif\\b"<<"\\bin\\b"
	<<"\\bmodule\\b"<<"\\bnext\\b"<<"\\bnil\\b"<<"\\bnot\\b"
	<<"\\bor\\b"<<"\\bredo\\b"<<"\\brescue\\b"<<"\\bretry\\b"
	<<"\\breturn\\b"<<"\\bself\\b"<<"\\bsuper\\b"<<"\\bthen\\b"
	<<"\\btrue\\b"<<"\\bundef\\b"<<"\\bunless\\b"<<"\\buntil\\b"
	<<"\\bwhen\\b"<<"\\bwhile\\b"<<"\\byield\\b";
    this->commentStartExpression = QRegularExpression("=begin");
    this->commentEndExpression = QRegularExpression("=end");
    this->singleLineRegularExpression= QRegularExpression("//(([^\n]*[^\"]*\"[^\"]*\")*[^\"]*$)*"); //(?=([^\"]*\"[^\"]*\")*[^\"]*$)
	//We don't know how to do this yet but for single line comments the character is '#'
	
	}

