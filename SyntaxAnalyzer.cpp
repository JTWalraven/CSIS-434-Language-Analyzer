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
		// Get next lexeme
		lexAnalyzer->lex();

		// check the block grammar
		block();

		// Loop until the end of the file
	} while (lexAnalyzer->getNextToken() != EOF);

	// Report syntax check
	cout << endl << "Syntax check completed! The syntax of the program is correct." << endl;
	(*fout) << endl << "Syntax check completed! The syntax of the program is correct." << endl;
}


bool SyntaxAnalyzer::block()
{
	// Check if block has BEGIN reserved word
	if (lexAnalyzer->getNextToken() == RESERVED_WORD && lexAnalyzer->getLexeme() == "BEGIN")
	{
		// Get next lexeme
		lexAnalyzer->lex();

		// Check declaration sequence grammar
		declarationSequence();

		// Check statement sequence grammar
		statementSequence();

		// Check if block closes with END reserved word
		if (lexAnalyzer->getNextToken() == RESERVED_WORD && lexAnalyzer->getLexeme() == "END")
		{
			lexAnalyzer->lex();
			return true;
		}
		else
		{
			// ERROR
			return false;
		}
	}
	else
	{
		// ERROR
		return false;
	}
}


bool SyntaxAnalyzer::declarationSequence()
{
	// Check declaration grammar
	if (declaration())
	{
		lexAnalyzer->lex();

		// Check the rest of the sequence
		declarationSequence();
		return true;
	}
	else
	{
		// ERROR
		return false;
	}
}


bool SyntaxAnalyzer::declaration()
{
	// Check if lexeme is a declarer
	if (declarer())
	{
		lexAnalyzer->lex();

		// Check name list grammar
		return nameList();
	}
	return false;
}


bool SyntaxAnalyzer::declarer()
{
	// Check if lexeme is a reserved word
	if (lexAnalyzer->getNextToken() == RESERVED_WORD)
	{
		// Check if the lexeme is CHAR or STRING
		if (lexAnalyzer->getLexeme() == "CHAR" || lexAnalyzer->getLexeme() == "STRING")
			return true;
	}
	return false;
}


bool SyntaxAnalyzer::nameList()
{
	// Check if lexeme is a name
	if (name())
	{
		lexAnalyzer->lex();

		if (lexAnalyzer->getNextToken() == COMMA)
		{
			// Check name list grammar
			return nameList();
		}
	}
	return false;
}


bool SyntaxAnalyzer::statementSequence()
{

}


bool SyntaxAnalyzer::statement()
{
	// Check if lexeme is a reserved word
	if (lexAnalyzer->getNextToken() == RESERVED_WORD)
	{
		// Check if the lexeme is OUTPUT
		if (lexAnalyzer->getLexeme() == "OUTPUT")
		{
			lexAnalyzer->lex();

			// Check the grammar of char expression
			return charExpression();
		}
		else if (lexAnalyzer->getLexeme() == "INPUT")
		{
			lexAnalyzer->lex();

			// Check the grammar of name
			return name();
		}
	}
	else 
	{
		// Check the test grammar
		if (test())
		{
			lexAnalyzer->lex();

			// Check the pair grammar
			if (pair())
			{
				lexAnalyzer->lex();

				// Check for a colon
				if (lexAnalyzer->getNextToken() == COLON)
				{
					lexAnalyzer->lex();

					// Check the statement grammar
					return statement();
				}
			}
		}
	}
	return false;
}


bool SyntaxAnalyzer::test()
{
	return (lexAnalyzer->getLexeme() == "EQ" || lexAnalyzer->getLexeme() == "NEG");
}


bool SyntaxAnalyzer::pair()
{
	// Check name grammar
	if (name())
	{
		lexAnalyzer->lex();

		// Check for a comma
		if (lexAnalyzer->getNextToken() == COMMA)
		{
			lexAnalyzer->lex();

			// Check name grammar
			return name();
		}
	}
	return false;
}


bool SyntaxAnalyzer::charExpression()
{
	// Check for a string literal
	if (lexAnalyzer->getNextToken() == QUOTE_MARK)
	{
		lexAnalyzer->lex();

		if (lexAnalyzer->getNextToken() == STRING_LIT)
		{
			lexAnalyzer->lex();

			if (lexAnalyzer->getNextToken() == QUOTE_MARK)
			{
				return true;
			}
			else 
			{
				// ERROR
			}
		}
		else
		{
			// ERROR
		}
	}
	else if (name())
	{
		return true;
	}
	return false;
}


bool SyntaxAnalyzer::name()
{
	return (lexAnalyzer->getNextToken() == NAME);
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
