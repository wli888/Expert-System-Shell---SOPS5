/*-----------------------------------------------------------------
C file: error.c

Deze file bevat procedures voor het afhandelen van fouten.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


#include <stdio.h>
#include <process.h>
#include <string.h>

#include "decl.h"
#include "error.h"


/* druk een syntax error af */
void print_syntax_error(char error, char *string)
{
	char *p;
	register int i;
	int linecount = 1;

	static char *se[] =
	{
		"syntax error",
		"left parenthesis \"(\" expected",
		"right parenthesis \")\" expected",
		"not a command",
		"constant symbolic atom expected",
		"\"-->\" expected",
		"left curly bracket \"{\" expected",
		"right curly bracket \"}\" expected",
		"up arrow \"^\" expected",
		"two left angle bracket \"<<\" expected",
		"two right angle bracket \">>\" expected",
		"less than character \"<\" expected",
		"greater than character \">\" expected",
		"minus sign \"-\" expected",
		"COMPUTE operator (+,-,*,//) expected",
		"element class not defined",
		"attribute not defined",
		"number expected",
		"not allowed in a COMPUTE expression",
		"element variabele already defined in production rule",
		"element variabele not defined in left hand side of production rule",
		"element class has already been defined",
		"take literally \"//\" expected",
		"not a function",
		"this variable has its first occurrence in a production rule and therefore it has to be preceded by an equal-to operator \"=\" (it is not necessary to explicitly specify an equal-to operator)",
		"this variable has its first occurrence in the right hand side of a production rule",
		"attribute name for this element class has already been defined",
		"production rule name has already been defined",
		"variables may not appear in a make command at the top level",
		"functions may not appear in a make command at the top level",
		"(CRLF) function may appear only in write command",
		"this command may not be used at the top level",
		"not a action command"
	};

	p = i_buf;
	while(p != input)
	{
		p++;
		if ((*p == '\r') || (*p == '\n'))
			linecount++;
	}
	if (!strcmp(string,""))
		printf("\nSOPS5> Syntax error in input file \"%s\" line %d: %s.\n",
			input_filename, linecount, se[error]);
	else
		printf("\nSOPS5> Syntax error in input file \"%s\" line %d: \"%s\" %s.\n",
			input_filename, linecount, string, se[error]);
	exit(1);
}


/* druk een run-time error af */
void print_run_time_error(char error, char *string)
{
	static char *re[] =
	{
		"is the value of a variable which is not allowed in a COMPUTE expression",
		"attempt to divide by zero in a COMPUTE expression",
		"predicate operator not allowed to be used with comparison of two strings"
	};

	if (!strcmp(string,""))
		printf("\n\nSOPS5> Run-time error: %s.\n", re[error]);
	else
		printf("\n\nSOPS5> Run-time error: \"%s\" %s.\n", string, re[error]);
	exit(1);
}


/* druk een error af */
void print_error(char error, char *string)
{
	static char *e[] =
	{
		"out of memory!\nSOPS5> Leaving program",
		"number of input files specified is not correct!\n\nSOPS5> Usage: sops5 <SOPS5 program filename> <SOPS5 data filename>",
		"unable to open input file"
	};

	if (!strcmp(string,""))
		printf("\n\nSOPS5> Error: %s.\n\n", e[error]);
	else
		printf("\n\nSOPS5> Error: %s \"%s\".\n\n", e[error], string);
	exit(1);
}