#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include "SyntaxAnalyzer.h"
using namespace std;

int main() 
{
	ofstream fout;
	ifstream fin;
	char exit;

	cout << endl << "Running Lexical Analyzer . . . " << endl;
	cout << PROGRAM_ID << endl;
	cout << "                  Console Output" << endl << endl << endl;

	//  ADMINISTRATIVE STUFF
	fout.open(PROGRAM_ID + string(".out"));
	fout << PROGRAM_ID << endl;
	fout << "                   Main Report" << endl << endl << endl;

	//  PROGRAM LOGIC
	fin.open("program.dat");

	if (!fin)	// Test the State of I/O stream
	{
		cout << endl
			<< "       ***  Error:  Can't open the input file  ***"

			<< endl << endl << endl;
		cout << endl << endl << "End Program due to error. Press any key to continue ...    ";
		cin >> exit;	//to pause program
		return 1;
	}

	// Create table header for output file
	fout << setfill('=') << setw(62) << "" << setfill(' ') << endl;
	fout << "| " << setw(10) << left << "Line No." 
		<< "| " << setw(20)  << left << "Token" 
		<< "| " << setw(25) << left << "Lexeme" << "|" << endl;
	fout << setfill('=') << setw(62) << "" << setfill(' ') << endl;

	// Run the Syntax Analyzer
	SyntaxAnalyzer *syntaxAnalyzer = new SyntaxAnalyzer(&fout, &fin);
	syntaxAnalyzer->analyzeSyntax();

	//EXIT ROUTINE
	fout << endl << endl;
	fout << "END OF MAIN REPORT";
	cout << endl << endl << "End of Program. Press any key to continue ...    ";
	cin >> exit;	//to pause program
	return 0;
}