//Jeffrey Walraven
//CSIS 434 - Spring 2017
//LexicalAanalyzer - a lexical analyzer system for simple arithmatic expressions

#include "LexicalAnalyzer.h"
using namespace std;


LexicalAnalyzer::LexicalAnalyzer(ostream *fout, istream *fin)
{
	this->fout = fout;
	this->fin = fin;
	this->currentLine = 1;
	this->lexLen = 0;
	getChar();
}


int LexicalAnalyzer::getNextToken()
{
	return nextToken;
}


int LexicalAnalyzer::getCurrentLine()
{
	return currentLine;
}


int LexicalAnalyzer::getLexLength()
{
	return lexLen;
}


char* LexicalAnalyzer::getLexeme()
{
	return lexeme;
}


char* LexicalAnalyzer::getOldLexeme()
{
	return oldLexeme;
}


void LexicalAnalyzer::addChar()
{
	if (lexLen <= 98) 
	{
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;

		// Set the historical lexeme
		oldLexeme[lexLen - 1] = nextChar;
		oldLexeme[lexLen] = 0;
	}
	else 
	{
		cout << "Error - lexeme is too long" << endl;
	}

}


void LexicalAnalyzer::getChar()
{
	// Set the next char and check that it is not EOF
	if ((nextChar = (*fin).get()) != EOF) 
	{
		if (isalpha(nextChar))
			charClass = LETTER;
		else if (isdigit(nextChar))
			charClass = DIGIT;
		else charClass = UNKNOWN;
	}
	else 
	{
		charClass = EOF;
	}
}


void LexicalAnalyzer::getNonBlank()
{
	while (isspace(nextChar))
	{
		if (nextChar == '\n' || nextChar == '\r')
			++currentLine;
		getChar();
	}
}


int LexicalAnalyzer::lookup(char ch)
{
	switch (ch) 
	{
	case ',':
		addChar();
		nextToken = COMMA;
		break;
	case '"':
		addChar();
		nextToken = QUOTE_MARK;
		break;
	case ':':
		addChar();
		nextToken = COLON;
		break;
	default:
		addChar();
		nextToken = EOF;
		break;
	}
	return nextToken;
}


void LexicalAnalyzer::printLexLine()
{
	cout << "Line " << currentLine << ". " 
		<< "Next token is: " << left << setw(15) << charArrayForToken(nextToken)
		<< "Next lexeme is " << lexeme << endl;
	(*fout) << left << "| " << setw(10) << currentLine
		<< "| " << setw(20) << charArrayForToken(nextToken)
		<< "| " << setw(25) << lexeme << "|" << endl;
	(*fout) << setfill('-') << setw(61) << "|" << "|" << setfill(' ') << endl;
}


int LexicalAnalyzer::lex()
{
	// Important! Reset lexLen to 0 to restart lexeme array
	lexLen = 0;

	// Remove all blank characters
	getNonBlank();

	switch (charClass) 
	{
	case LETTER:
		addChar();
		getChar();
		while (charClass == LETTER || charClass == DIGIT)
		{
			addChar();
			getChar();
		}
		// Check if a String literal or a name
		if (nextChar == '"')
			nextToken = STRING_LIT;
		else 
			nextToken = NAME;

		// Find if the word is actually a reserved word
		for (int i = 0; i < NUM_RESERVED_WORDS; ++i)
		{
			if (strcmp(reservedWords[i], lexeme) == 0)
				nextToken = RESERVED_WORD;
		}
		break;
	case UNKNOWN:
		lookup(nextChar);
		getChar();
		break;
	case EOF:
		nextToken = EOF;
	    lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		break;
	}

	printLexLine();
	return nextToken;
}
