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
	this->errorMessage = "An error was thrown at line ";
}


void SyntaxAnalyzer::analyzeSyntax()
{
	do 
	{
		// Get next lexeme
		lexAnalyzer->lex();

		// check the program grammar
		program();

		// Loop until the end of the file
	} while (!tokenMatches(EOF));

	// Report syntax check
	cout << endl << "Syntax check completed! The syntax of the program is correct." << endl;
	(*fout) << endl << "Syntax check completed! The syntax of the program is correct." << endl;
}


bool SyntaxAnalyzer::tokenMatches(int token)
{
	return lexAnalyzer->getNextToken() == token;
}


bool SyntaxAnalyzer::lexemeMatches(char *lexeme)
{
	return strcmp(lexAnalyzer->getLexeme(), lexeme) == 0;
}


bool SyntaxAnalyzer::program()
{
	// Check block grammar
	if (block())
		return true;
	else
		error();
	return false;
}


bool SyntaxAnalyzer::block()
{
	// Check if block has BEGIN reserved word
	if (tokenMatches(RESERVED_WORD) && lexemeMatches("BEGIN"))
	{
		// Get next lexeme
		lexAnalyzer->lex();

		// Check declaration sequence grammar
		if (!declarationSequence())
			error();

		// Check statement sequence grammar
		if (!statementSequence())
			error();

		// Check if block closes with END reserved word
		if (tokenMatches(RESERVED_WORD) && lexemeMatches("END"))
		{
			lexAnalyzer->lex();
			return true;
		}
		else
		{
			errorMessage = "ERROR: Expected closing block statement 'END' at line ";
		}
	}
	else
	{
		errorMessage = "ERROR: Expected opening block statement 'BEGIN' at line ";
	}
	return false;
}


bool SyntaxAnalyzer::declarationSequence()
{
	// Check declaration grammar
	if (declaration())
	{
		// Check the rest of the sequence
		while (declaration());
		return true;
	}
	else
		errorMessage = "ERROR: Expected declaration at line ";

	return false;
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
	if (tokenMatches(RESERVED_WORD))
	{
		// Check if the lexeme is CHAR or STRING
		if (lexemeMatches("CHAR") || lexemeMatches("STRING"))
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

		// Check the rest of the name list
		while (tokenMatches(COMMA))
		{
			lexAnalyzer->lex();

			// Check name syntax
			if (!name())
				errorMessage = "ERROR: Expected name after comma at line ";
			
			lexAnalyzer->lex();
		}
		return true;
	}
	else
		errorMessage = "ERROR: Expected name at line ";
	return false;
}


bool SyntaxAnalyzer::statementSequence()
{
	if (statement())
	{
		// Check if lexeme is a statement
		do
		{
			lexAnalyzer->lex();
		} while (statement());
		return true;
	}
	else
		errorMessage = "ERROR: Expected statement at line ";
	return false;
}


bool SyntaxAnalyzer::statement()
{
	// Check if lexeme is a reserved word
	if (tokenMatches(RESERVED_WORD))
	{
		// Check if the lexeme is OUTPUT
		if (lexemeMatches("OUTPUT"))
		{
			lexAnalyzer->lex();

			// Check the grammar of char expression
			return charExpression();
		}
		else if (lexemeMatches("INPUT"))
		{
			lexAnalyzer->lex();

			// Check the grammar of name
			return name();
		}
		else if (test())
		{
			lexAnalyzer->lex();

			// Check the pair grammar
			if (pair())
			{
				lexAnalyzer->lex();

				// Check for a colon
				if (tokenMatches(COLON))
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
	return (lexemeMatches("EQ") || lexemeMatches("NEG"));
}


bool SyntaxAnalyzer::pair()
{
	// Check name grammar
	if (name())
	{
		lexAnalyzer->lex();

		// Check for a comma
		if (tokenMatches(COMMA))
		{
			lexAnalyzer->lex();

			// Check name grammar
			if (name())
				return true;
			else
				errorMessage = "ERROR: Expected name for pair after comma at line ";
		}
	}
	return false;
}


bool SyntaxAnalyzer::charExpression()
{
	// Check for a string literal
	if (tokenMatches(QUOTE_MARK))
	{
		lexAnalyzer->lex();

		if (tokenMatches(STRING_LIT))
		{
			lexAnalyzer->lex();

			if (tokenMatches(QUOTE_MARK))
			{
				return true;
			}
			else
			{
				errorMessage = "ERROR: Expected closing quote mark '\"' at line ";
				error();
			}
		}
		else
		{
			errorMessage = "ERROR: Expected closing quote mark '\"' at line ";
			error();
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
	return tokenMatches(NAME);
}

void SyntaxAnalyzer::error()
{
	char exit;

	// Write out error report
	cout << endl << errorMessage
		<< lexAnalyzer->getCurrentLine() << ":" << endl << endl;
	(*fout) << endl << errorMessage
		<< lexAnalyzer->getCurrentLine() << "." << endl << endl;
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
