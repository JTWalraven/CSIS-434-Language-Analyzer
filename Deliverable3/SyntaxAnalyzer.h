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
	char *errorMessage;
	LexicalAnalyzer *lexAnalyzer;
	bool tokenMatches(int);
	bool lexemeMatches(char *);
	bool program();
	bool block();
	bool declarationSequence();
	bool declaration();
	bool declarer();
	bool nameList();
	bool statementSequence();
	bool statement();
	bool test();
	bool pair();
	bool charExpression();
	bool name();
	void error();
public:
	SyntaxAnalyzer(ostream *fout, istream *fin);
	void analyzeSyntax();
};

#endif