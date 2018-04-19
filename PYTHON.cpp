#include "PYTHON.h"

PYTHON::PYTHON()
{
    this->keywordPatterns << "\\bTrue\\b" << "\\bFalse\\b" << "\\bNone\\b"
	<< "\\band\\b" <<"\\bor\\b" << "\\bnot\\b" << "\\bassert\\b" 
	<<"\\bbreak\\b" << "\\bcontinue\\b" <<"\\bclass\\b" 
	<<"\\bdef\\b" << "\\bif\\b" <<"\\belse\\b" << "\\belif\\b" 
	<<"\\bdel\\b" << "\\btry\\b" << "\\bexcept\\b" <<"\\braise\\b" 
	<<"\\bfinally\\b" << "\\bfor\\b" <<"\\bwhile\\b" <<"\\bpass\\b" 
	<<"\\bimport\\b" <<"\\bfrom\\b" <<"\\bas\\b" <<"\\blamda\\b" 
	<<"\\breturn\\b" <<"\\byield\\b" <<"\\bwith\\b" <<"\\bin\\b" <<"\\bis\\b"
	<<"\\bglobal\\b" <<"\\bnon-local\\b";  
    this->commentStartExpression = QRegularExpression();
    this->commentEndExpression = QRegularExpression();
    this->singleLineRegularExpression= QRegularExpression("//(([^\n]*[^\"]*\"[^\"]*\")*[^\"]*$)*"); //(?=([^\"]*\"[^\"]*\")*[^\"]*$)
	//We don't know how to do this yet but for single line comments the character is '#'
	//No multine comments in Python
	}

