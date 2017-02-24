#ifndef SYNTAX_ANLYZER_H
#define SYNTAX_ANLYZER_H

#include <iostream>
#include <iomanip>
#include "LexicalAnalyzer.h"
using namespace std;

class SyntaxAnalyzer 
{
private:
	ostream *fout;
	istream *fin;
	LexicalAnalyzer *lexAnalyzer;
	void expr();
	void term();
	void factor();
	void error();
public:
	SyntaxAnalyzer(ostream *fout, istream *fin);
	void analyzeSyntax();
};

#endif