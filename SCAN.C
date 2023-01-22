/*-----------------------------------------------------------------
C file: scan.c

Deze file bevat procedures voor het scannen van een string (ingelezen
uit een invoer file) om een token te retourneren.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


#include <string.h>
#include <ctype.h>

#include "decl.h"
#include "scan.h"


/* haal een token uit de input string */
void get_token()
{
	register char *temp;

	token_class = ZERO_TK;
	temp = token;
	*temp = '\0';

	while ((iswhite(*input)) || (*input==';') || (*input=='\n') ||
				(*input=='\r') || (*input=='\0'))
	{
		while (*input==';')
		{
			++input;
			while ((*input!='\0') && (*input!='\n') && (*input!='\r'))
				++input;
		}
		if (iswhite(*input))
		{
			++input;
			while(iswhite(*input)) ++input;
		}
		if (*input=='\n')
		{
			++input;
			while(iswhite(*input)) ++input;
		}
		if (*input=='\r')
		{
			++input; ++input;
			while(iswhite(*input)) ++input;
		}
		if (*input=='\0')
		{
			*token = '\0';
			token_class = EOT_TK;
			return;
		}
	}

	if (*input=='(')
	{
		*temp = *input;
		input++;
		temp++;
		*temp = '\0';
		token_class = L_PAREN_TK;
		return;
	}

	if (*input==')')
	{
		*temp = *input;
		input++;
		temp++;
		*temp = '\0';
		token_class = R_PAREN_TK;
		return;
	}

	if (*input=='{')
	{
		*temp = *input;
		input++;
		temp++;
		*temp = '\0';
		token_class = L_CURLY_BRACKET_TK;
		return;
	}

	if (*input=='}')
	{
		*temp = *input;
		input++;
		temp++;
		*temp = '\0';
		token_class = R_CURLY_BRACKET_TK;
		return;
	}


	if (strchr("<>=", *input))
	{
		switch(*input)
		{
			case '=':
				*temp = EQ_TK;
				input++;
				temp++;
				*temp = '\0';
				token_class = EQ_TK;
				return;

			case '<':
				if (*(input+1) == '>')
				{
					*temp = NE_TK;
					input += 2;
					temp ++;
					*temp = '\0';
					token_class = NE_TK;
					return;
				}
				else if (*(input+1) == '=')
				{
					*temp = LE_TK;
					input += 2;
					temp ++;
					*temp = '\0';
					token_class = LE_TK;
					return;
				}
				else if (*(input+1) == '<')
				{
					*temp = *input;
					*(temp+1) = *(input+1);
					input += 2;
					temp += 2;
					*temp = '\0';
					token_class = TWO_L_ANGLE_BRACKET_TK;
					return;
				}
				else
				{
					*temp = LT_TK;
					input ++;
					temp ++;
					*temp = '\0';
					token_class = LT_TK;
					return;
				}

			case '>':
				if (*(input+1) == '=')
				{
					*temp = GE_TK;
					input += 2;
					temp ++;
					*temp = '\0';
					token_class = GE_TK;
					return;
				}
				else if (*(input+1) == '>')
				{
					*temp = *input;
					*(temp+1) = *(input+1);
					input += 2;
					temp += 2;
					*temp = '\0';
					token_class = TWO_R_ANGLE_BRACKET_TK;
					return;
				}
				else
				{
					*temp = GT_TK;
					input ++;
					temp ++;
					*temp = '\0';
					token_class = GT_TK;
					return;
				}
		}
	}

	if (*input=='-')
	{
		if ((*(input+1)=='-') && (*(input+2)=='>'))
		{
			*temp = *input;
			*(temp+1) = *(input+1);
			*(temp+2) = *(input+2);
			input += 3;
			temp += 3;
			*temp = '\0';
			token_class = ARROW_TK;
			return;
		}
		else
		{
			if (isdigit(*(input+1)))
			{
				while(!isdelim(*input)) *temp++ = *input++;
				*temp = '\0';
				token_class = NUMBER_TK;
			}
			else if (iswhite(*(input+1)))
			{
				*temp = *input;
				input += 2;
				temp++;
				*temp = '\0';
				token_class = NEG_TK;
			}
			else if ((*(input+1)=='.') && (isdigit(*(input+2))))
			{
				while(!isdelim(*input)) *temp++ = *input++;
				*temp = '\0';
				token_class = NUMBER_TK;
			}
			return;
		}
	}

	if (*input=='+')
	{
		if (isdigit(*(input+1)))
		{
			while(!isdelim(*input)) *temp++ = *input++;
			*temp = '\0';
			token_class = NUMBER_TK;
		}
		else if (iswhite(*(input+1)))
		{
				*temp = *input;
				input += 2;
				temp++;
				*temp = '\0';
				token_class = PLUS_TK;
		}
		else if ((*(input+1)=='.') && (isdigit(*(input+2))))
		{
			while(!isdelim(*input)) *temp++ = *input++;
			*temp = '\0';
			token_class = NUMBER_TK;
		}
		return;
	}

	if (*input=='*')
	{
		if (iswhite(*(input+1)))
		{
			*temp = *input;
			input += 2;
			temp++;
			*temp = '\0';
			token_class = MUL_TK;
			return;
		}
	}

	if (*input=='^')
	{
		*temp = *input;
		input++;
		temp++;
		*temp ='\0';
		token_class = UPARROW_TK;
		return;
	}

	if (*input=='/')
	{
		if (*(input+1)=='/')
		{
			if (iswhite(*(input+2)))
			{
				*temp = *input;
				*(temp+1) = *(input+1);
				input += 3;
				temp += 2;
				*temp = '\0';
				token_class = TAKE_LITERALLY_TK;
				return;
			}
		}
	}

	if (isdigit(*input))
	{
		while(!isdelim(*input)) *temp++ = *input++;
		*temp = '\0';
		token_class = NUMBER_TK;
		return;
	}
	else
	{
		if ((*input=='.') && (isdigit(*(input+1))))
		{
			while(!isdelim(*input)) *temp++ = *input++;
			*temp = '\0';
			token_class = NUMBER_TK;
			return;
		}
	}

	if (isalpha(*input))
	{
		while(!isdelim(*input)) *temp++ = *input++;
		token_class = TEMP;
	}

	*temp = '\0';

	if (token_class==TEMP)
	{
		token_class = look_up(token);
		if (!token_class)
			token_class = CSA_TK;
	}
	return;
}


/* controleer of een token een commando is en retourneer deze commando. */
char look_up(char *s)
{
	register int i;
	commands_t table[] =
	{
		"literalize", LITERALIZE_TK,
		"p", P_TK,
		"make", MAKE_TK,
		"remove", REMOVE_TK,
		"modify", MODIFY_TK,
		"write", WRITE_TK,
		"halt", HALT_TK,
		"crlf", CRLF_TK,
		"compute", COMPUTE_TK,
		"", EOT_TK
	};

	for (i=0; *table[i].command; i++)
		if (!strcmp(table[i].command,s))
			return table[i].tclass;
	return 0;
}


/* deze procedure haalt een any_atom */
void get_any_atom()
{
	register char *temp;

	temp = token;
	*temp = '\0';
	while(!iswhite(*input)) *temp++ = *input++;
	*temp = '\0';
}


/* deze procedure haalt een nummer of een any_atom */
void get_number_or_any_atom()
{
	register char *temp;

	temp = token;
	*temp = '\0';

	while ((iswhite(*input)) || (*input=='\n') || (*input=='\r') ||
				(*input=='\0'))
	{
		if (iswhite(*input))
		{
			++input;
			while(iswhite(*input)) ++input;
		}
		if (*input=='\n')
		{
			++input;
			while(iswhite(*input)) ++input;
		}
		if (*input=='\r')
		{
			++input; ++input;
			while(iswhite(*input)) ++input;
		}
		if (*input=='\0')
		{
			*token = '\0';
			token_class = EOT_TK;
			return;
		}
	}


	if ((*input=='>') && (*(input+1) == '>'))
	{
		*temp = *input;
		*(temp+1) = *(input+1);
		input += 2;
		temp += 2;
		*temp = '\0';
		token_class = TWO_R_ANGLE_BRACKET_TK;
		return;
	}

	if (isdigit(*input))
	{
		while(!iswhite(*input)) *temp++ = *input++;
		*temp = '\0';
		token_class = NUMBER_TK;
		return;
	}
	else
	{
		if ((*input=='.') && (isdigit(*(input+1))))
		{
			while(!iswhite(*input)) *temp++ = *input++;
			*temp = '\0';
			token_class = NUMBER_TK;
			return;
		}
	}

	if ((*input=='+') || (*input=='-'))
	{
		if (isdigit(*(input+1)))
		{
			while(!iswhite(*input)) *temp++ = *input++;
			*temp = '\0';
			token_class = NUMBER_TK;
			return;
		}
		else if ((*(input+1)=='.') && (isdigit(*(input+2))))
		{
			while(!iswhite(*input)) *temp++ = *input++;
			*temp = '\0';
			token_class = NUMBER_TK;
			return;
		}
	}

	while(!iswhite(*input)) *temp++ = *input++;
	*temp = '\0';
}


/* deze procedure bekijkt of een karakter een delimiter is. */
BOOLEAN isdelim(char c)
{
	if (strchr(" (){}<>/", c) || c==9 || c=='\n' || c=='\r' || c==0)
		return TRUE;
	else
		return FALSE;
}


/* deze procedure bekijkt of een karakter een spatie of een tab is. */
BOOLEAN iswhite(char c)
{
	if (c==' ' || c=='\t')
		return TRUE;
	else
		return FALSE;
}


/* deze procedure bekijkt of een token een predicate is. */
BOOLEAN ispredicate()
{
	if (iswhite(*input))
	{
		switch (token_class)
		{
			case EQ_TK:
				return TRUE;
			case NE_TK:
				return TRUE;
			case LT_TK:
				return TRUE;
			case LE_TK:
				return TRUE;
			case GT_TK:
				return TRUE;
			case GE_TK:
				return TRUE;
			default:
				return FALSE;
		}
	}
	else
		return FALSE;
}
