//Jeffrey Walraven
//CSIS 434 - Spring 2017
//SyntaxAnalyzer - a recursive-descent parser for checking syntax

#include <cstdlib>
#include "SyntaxAnalyzer.h"
using namespace std;


SyntaxAnalyzer::SyntaxAnalyzer(ostream *fout, istream *fin)
{
	this->fout = fout;
	this->fin = fin;
	this->lexAnalyzer = new LexicalAnalyzer(fout, fin);
}


void SyntaxAnalyzer::analyzeSyntax()
{
	do {
		lexAnalyzer->lex();
		//expr();
	} while (lexAnalyzer->getNextToken() != EOF);
	cout << endl << "Syntax check completed! The syntax of the program is correct." << endl;
	(*fout) << endl << "Syntax check completed! The syntax of the program is correct." << endl;
}

/*
void SyntaxAnalyzer::expr()
{
	term();

	while (lexAnalyzer->getNextToken() == ADD_OP || lexAnalyzer->getNextToken() == SUB_OP)
	{
		lexAnalyzer->lex();
		term();
	}
}


void SyntaxAnalyzer::term()
{
	factor();

	while (lexAnalyzer->getNextToken() == MULT_OP || lexAnalyzer->getNextToken() == DIV_OP) {
		lexAnalyzer->lex();
		factor();
	}
}


void SyntaxAnalyzer::factor()
{
	if (lexAnalyzer->getNextToken() == IDENT || lexAnalyzer->getNextToken() == INT_LIT) {
		lexAnalyzer->lex();
	}
	else 
	{
		if (lexAnalyzer->getNextToken() == LEFT_PAREN) {
			lexAnalyzer->lex();
			expr();

			// Check for the right parenthesis
			if (lexAnalyzer->getNextToken() == RIGHT_PAREN) 
			{
				lexAnalyzer->lex();
			}
			else 
			{
				cout << endl << "ERROR: Expected closing parenthesis ')' at the end of line "
					<< lexAnalyzer->getCurrentLine() << ":" << endl << endl;
				(*fout) << endl << "ERROR: Expected closing parenthesis ')' at the end of line "
					<< lexAnalyzer->getCurrentLine() << "." << endl << endl;
				error();
			}
		} 
		else
		{
			cout << endl << "ERROR: Unexpected closing parenthesis ')' on line "
				<< lexAnalyzer->getCurrentLine() << ":" << endl << endl;
			(*fout) << endl << "ERROR: Unexpected closing parenthesis ')' on line "
				<< lexAnalyzer->getCurrentLine() << "." << endl << endl;
			error();
		}
	} 
} */

void SyntaxAnalyzer::error()
{
	char exit;
	cout << TAB << TAB << lexAnalyzer->getOldLexeme() << endl << endl
		<< TAB << "Error on lexeme: " << charArrayForToken(lexAnalyzer->getNextToken())  << endl
		<< TAB << "Lexeme total length: " << lexAnalyzer->getLexLength() << endl << endl;
	cout << endl << "Syntax check failed! The syntax of the program is incorrect." << endl;
	(*fout) << endl << "Syntax check failed! The syntax of the program is incorrect." << endl;

	// Exit routine
	(*fout) << endl << endl;
	(*fout) << "END OF MAIN REPORT";
	cout << endl << endl << "End of Program. Press any key to continue ...    ";
	cin >> exit;	//to pause program
	std::exit(0);
}
