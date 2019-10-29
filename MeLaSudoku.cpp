// TODO
//
// check size works...
// generator should take complete rule set into account?? (may be slower)

#include <iostream>
#include <string>
using namespace std;
#include "MeLaSudoku.h"

// this is fiendish
//
// 750090046
// 901000302
// 000000000
// 200601007
// 080000020
// 100308005
// 000000000
// 309000204
// 840030079

// this is "impossible with known logic rules"
//
// 008060400
// 030000070
// 400801003
// 007409600
// 000000000
// 002306500
// 900608004
// 020000090
// 004020700

// this came from pocket pc sudoku generator
// and seems to have more than one solution!
//
// 000390160
// 000000000
// 006007400
// 000040900
// 950010026
// 001070000
// 005700200
// 000000000
// 082036000

// this is easy
// 040003100
// 900510406
// 800009570
// 008060045
// 060905030
// 210030900
// 039400002
// 402071008
// 007200050

void test();

int main()
{
	//test();
	int i, j;
	char input, level;
	string str;
	int size;
	int dump;

	Rule *apTrivialRules[] = 
	{
		new IfNotPossibleElsewhereRule,
		new IfAppearsElsewhereRule,
		new OnlyOnePossibilityRule,
	};
	Rule *apEasyRules[] = 
	{
		new IfNotPossibleElsewhereRule,
		new IfAppearsElsewhereRule,
		new IfOnlyPossibleInRowOrColumnInBlockRule,
		new IfOnlyPossibleInBlockInRowOrColumnRule,
		new OnlyOnePossibilityRule,
	};
	Rule *apMediumRules[] = 
	{
		new IfNotPossibleElsewhereRule,
		new IfAppearsElsewhereRule,
		new IfOnlyPossibleInRowOrColumnInBlockRule,
		new IfOnlyPossibleInBlockInRowOrColumnRule,
		new PairsTriplesEtcRule(2),
		new PairsTriplesEtcRule2(2),
		new OnlyOnePossibilityRule,
	};
	Rule *apHardRules[] = 
	{
		new IfNotPossibleElsewhereRule,
		new IfAppearsElsewhereRule,
		new IfOnlyPossibleInRowOrColumnInBlockRule,
		new IfOnlyPossibleInBlockInRowOrColumnRule,
		new PairsTriplesEtcRule,
		new PairsTriplesEtcRule2,
		new OnlyOnePossibilityRule,
	};
	TrialAndErrorRule *pTrialAndError = new TrialAndErrorRule;
	Rule *apReallyHardRules[] = 
	{
		new IfNotPossibleElsewhereRule,
		new IfAppearsElsewhereRule,
		new IfOnlyPossibleInRowOrColumnInBlockRule,
		new IfOnlyPossibleInBlockInRowOrColumnRule,
		new PairsTriplesEtcRule,
		new PairsTriplesEtcRule2,
		new OnlyOnePossibilityRule,
		pTrialAndError,
	};
	pTrialAndError->SetRules(sizeof(apReallyHardRules)/sizeof(apReallyHardRules[0]), apReallyHardRules);

	do {
		dump = 0;
		printf("(S)olve or (G)enerate? ");
		cin >> input;
		if (input ==  'S' || input ==  's') {
			printf("size? ");
			cin >> input;
			size = (int)(input-'0');
			printf("output method? ");
			cin >> input;
			if (input == 'Y' || input == 'y') {
				dump = 1;
			}
			int **ppiProblem = new int*[size*size];
			for (i = 0; i < size*size; i++) {
				ppiProblem[i] = new int[size*size];
				printf("row %d: ", i+1);
				cin >> str;
				for (j = 0; j < size*size; j++) {
					if (j >= (signed)str.length()) {
						ppiProblem[i][j] = 0;
						continue;
					}
					if (size <= 3) {
						ppiProblem[i][j] = (int)(str[j]-'0');
					} else {
						ppiProblem[i][j] = (int)(str[j]-'A'+1);
					}
					if (ppiProblem[i][j] < 0 || ppiProblem[i][j] > size*size) {
						ppiProblem[i][j] = 0;
					}
				}
			}
			printf("\n");
			MeLaSudokuSolver Sudoku(size, ppiProblem);
			Sudoku.Dump();
			printf("\n");
			if (dump) {
				Sudoku.SetDump(1);
			}
			if (!Sudoku.Check()) {
				printf("not a valid sudoku!\n");
			} else {
				Sudoku.Solve(sizeof(apReallyHardRules)/sizeof(apReallyHardRules[0]), apReallyHardRules);
				if (dump) {
					printf("\n");
				}
				Sudoku.Dump();
				if (Sudoku.IsSolved()) {
					printf("\nsolved it!\n");
				}
				if (Sudoku.IsImpossible()) {
					printf("\nnot a valid sudoku!\n");
				}
				if (Sudoku.IsNotUnique()) {
					printf("\nnot a sudoku with a unique solution!\n");
				}
				if (!Sudoku.Check()) {
					printf("\nnot a valid solution!\n");
				}
			}
			for (i = 0; i < size*size; i++) {
				delete [] ppiProblem[i];
			}
			delete [] ppiProblem;
		} else {
			printf("size? ");
			cin >> input;
			size = (int)(input-'0');
			printf("difficulty (1 = trivial - 5 = really hard)? ");
			cin >> level;
			MeLaSudokuGenerator *generator;
			switch (level) {
				case '1':
					generator = new MeLaSudokuGenerator(size, sizeof(apTrivialRules)/sizeof(apTrivialRules[0]), apTrivialRules,
						0, 0);
					break;
				case '2':
					generator = new MeLaSudokuGenerator(size, sizeof(apEasyRules)/sizeof(apEasyRules[0]), apEasyRules,
						sizeof(apTrivialRules)/sizeof(apTrivialRules[0]), apTrivialRules);
					break;
				case '3':
					generator = new MeLaSudokuGenerator(size, sizeof(apMediumRules)/sizeof(apMediumRules[0]), apMediumRules,
						sizeof(apEasyRules)/sizeof(apEasyRules[0]), apEasyRules);
					break;
				case '4':
					generator = new MeLaSudokuGenerator(size, sizeof(apHardRules)/sizeof(apHardRules[0]), apHardRules,
						sizeof(apMediumRules)/sizeof(apMediumRules[0]), apMediumRules);
					break;
				case '5':
					generator = new MeLaSudokuGenerator(size, sizeof(apReallyHardRules)/sizeof(apReallyHardRules[0]), apReallyHardRules,
						sizeof(apHardRules)/sizeof(apHardRules[0]), apHardRules);
					break;
				default:
					exit(1);
			}
			printf("\n");
			generator->GetMeLaSudokuSolver().Dump();
			printf("\n%d filled in cells\n\n", generator->GetMeLaSudokuSolver().GetNumSolved());
			printf("do you want to see the solution? ");
			cin >> input;
			if (input == 'Y' || input == 'y') {
				printf("output method? ");
				cin >> input;
				if (input == 'Y' || input == 'y') {
					dump = 1;
				}
				printf("\n");
				if (dump) {
					generator->GetMeLaSudokuSolver().SetDump(1);
				}
				switch (level) {
					case '1':
						generator->GetMeLaSudokuSolver().Solve(sizeof(apTrivialRules)/sizeof(apTrivialRules[0]), apTrivialRules);
						break;
					case '2':
						generator->GetMeLaSudokuSolver().Solve(sizeof(apEasyRules)/sizeof(apEasyRules[0]), apEasyRules);
						break;
					case '3':
						generator->GetMeLaSudokuSolver().Solve(sizeof(apMediumRules)/sizeof(apMediumRules[0]), apMediumRules);
						break;
					case '4':
						generator->GetMeLaSudokuSolver().Solve(sizeof(apHardRules)/sizeof(apHardRules[0]), apHardRules);
						break;
					case '5':
						generator->GetMeLaSudokuSolver().Solve(sizeof(apReallyHardRules)/sizeof(apReallyHardRules[0]), apReallyHardRules);
						break;
					default:
						exit(1);
				}
				if (dump) {
					printf("\n");
				}
				generator->GetMeLaSudokuSolver().Dump();
				if (generator->GetMeLaSudokuSolver().IsSolved()) {
					printf("\nsolved it!\n");
				}
				if (generator->GetMeLaSudokuSolver().IsImpossible()) {
					printf("\nnot a valid sudoku!\n");
				}
				if (generator->GetMeLaSudokuSolver().IsNotUnique()) {
					printf("\nnot a sudoku with a unique solution!\n");
				}
				if (!generator->GetMeLaSudokuSolver().Check()) {
					printf("\nnot a valid solution!\n");
				}
			}
			delete generator;
		}
		printf("\n");
	} while (1);

	for (i = 0; i < sizeof(apTrivialRules)/sizeof(apTrivialRules[0]); i++) {
		delete apTrivialRules[i];
	}
	for (i = 0; i < sizeof(apEasyRules)/sizeof(apEasyRules[0]); i++) {
		delete apEasyRules[i];
	}
	for (i = 0; i < sizeof(apMediumRules)/sizeof(apMediumRules[0]); i++) {
		delete apMediumRules[i];
	}
	for (i = 0; i < sizeof(apHardRules)/sizeof(apHardRules[0]); i++) {
		delete apHardRules[i];
	}
	for (i = 0; i < sizeof(apReallyHardRules)/sizeof(apReallyHardRules[0]); i++) {
		delete apReallyHardRules[i];
	}
}

void test()
{
	int i;
#if 0
	int aaiProblem[9][9] =
	{
		{ 0, 0, 0, 3, 9, 0, 1, 6, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 6, 0, 0, 7, 4, 0, 0 },
		{ 0, 0, 0, 0, 4, 0, 9, 0, 0 },
		{ 9, 5, 0, 0, 1, 0, 0, 2, 6 },
		{ 0, 0, 1, 0, 7, 0, 0, 0, 0 },
		{ 0, 0, 5, 7, 0, 0, 2, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 8, 2, 0, 3, 6, 0, 0, 0 },
	};
#endif
	int aaiProblem[9][9] =
	{
		{ 0, 0, 0, 0, 0, 0, 0, 0, 9 },
		{ 0, 0, 6, 0, 0, 1, 0, 2, 0 },
		{ 5, 0, 1, 0, 0, 7, 0, 0, 0 },
		{ 2, 0, 0, 1, 5, 0, 7, 0, 0 },
		{ 0, 5, 0, 0, 0, 0, 0, 3, 0 },
		{ 0, 0, 7, 0, 9, 3, 0, 0, 4 },
		{ 0, 0, 0, 7, 0, 0, 6, 0, 5 },
		{ 0, 3, 0, 8, 0, 0, 4, 0, 0 },
		{ 4, 0, 0, 0, 0, 0, 0, 0, 0 },
	};
	int *ppiProblem[9];
	TrialAndErrorRule *pTrialAndError = new TrialAndErrorRule;
	Rule *apReallyHardRules[] = 
	{
		new IfNotPossibleElsewhereRule,
		new IfAppearsElsewhereRule,
		new IfOnlyPossibleInRowOrColumnInBlockRule,
		new PairsTriplesEtcRule,
		new PairsTriplesEtcRule2,
		new OnlyOnePossibilityRule,
		pTrialAndError,
	};
	pTrialAndError->SetRules(sizeof(apReallyHardRules)/sizeof(apReallyHardRules[0]), apReallyHardRules);

	for (i = 0; i < 9; i++) {
		ppiProblem[i] = aaiProblem[i];
	}
	MeLaSudokuSolver Sudoku(3, ppiProblem);
	Sudoku.Dump();
	printf("\n");
	Sudoku.SetDump(1);
	Sudoku.Solve(sizeof(apReallyHardRules)/sizeof(apReallyHardRules[0]), apReallyHardRules);
	printf("\n");
	Sudoku.Dump();
	if (Sudoku.IsSolved()) {
		printf("\nsolved it!\n");
	}
	if (Sudoku.IsImpossible()) {
		printf("\nnot a valid sudoku!\n");
	}
	if (Sudoku.IsNotUnique()) {
		printf("\nnot a sudoku with a unique solution!\n");
	}
	if (!Sudoku.Check()) {
		printf("\nnot a valid solution!\n");
	}
	printf("\n");
	getchar();
	for (i = 0; i < sizeof(apReallyHardRules)/sizeof(apReallyHardRules[0]); i++) {
		delete apReallyHardRules[i];
	}
}
