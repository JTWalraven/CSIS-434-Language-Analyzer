#ifndef LEXICAL_ANLYZER_H
#define LEXICAL_ANALYZER_H

#include <iostream>
#include <iomanip>
#include "Constants.h"
using namespace std;

class LexicalAnalyzer 
{
private:
	ostream *fout;
	istream *fin;
	char lexeme[LEXEME_LENGTH];
	char oldLexeme[LEXEME_LENGTH];
	char nextChar;
	int charClass,
		lexLen,
		currentLine,
		nextToken;
	void addChar();
	void getChar();
	void getNonBlank();
	void printLexLine();
	int lookup(char ch);
public:
	LexicalAnalyzer(ostream *fout, istream *fin);
	int lex();
	int getNextToken();
	int getCurrentLine();
	int getLexLength();
	char* getLexeme();
	char* getOldLexeme();
};

#endif
