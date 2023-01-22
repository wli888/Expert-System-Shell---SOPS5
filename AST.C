/*-----------------------------------------------------------------
C file: ast.c

Deze file bevat procedures voor bewerkingen met abstract syntax
trees (AST).

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


#include <malloc.h>
#include <string.h>
#include <stdio.h>

#include "decl.h"
#include "ast.h"
#include "list.h"


/* creeer een abstract syntax tree (AST) */
astnode_t *create_ast(char asttag, char *token, float number,
								astnode_t *left, astnode_t *middle,
								astnode_t *right)
{
	astnode_t *new;
	int 	stringlen;

	if ((new = MALLOC(astnode_t)) != NULL)
	{
		new->asttag = asttag;
		if (token)
		{
			stringlen = strlen(token);
			new->token = (char *) malloc(stringlen+1);
			strcpy(new->token,token);
		}
		else
			new->token = NULL;
		new->number = number;
		new->delete = ast_delete;
		new->left = left;
		new->middle = middle;
		new->right = right;
	}
	return new;
}


/* creeer een AST zonder takken */
astnode_t *leaf_ast(char asttag, char *token, float number)
{
	astnode_t *ast;

	ast = create_ast(asttag, token, number, NULL, NULL, NULL);
	return ast;
}


/* creeer een AST met 1 tak */
astnode_t *unary_ast(char asttag, astnode_t *left)
{
	astnode_t *ast;

	ast = create_ast(asttag, '\0', 0, left, NULL, NULL);
	return ast;
}


/* creeer een AST met 2 takken */
astnode_t *binary_ast(char asttag, astnode_t *left, astnode_t *middle)
{
	astnode_t *ast;

	ast = create_ast(asttag, '\0', 0, left, middle, NULL);
	return ast;
}


/* creeer een AST met 3 takken */
astnode_t *ternary_ast(asttag, left, middle, right)
char asttag;
astnode_t *left, *middle, *right;
{
	astnode_t *ast;

	ast = create_ast(asttag, '\0', 0, left, middle, right);
	return ast;
}


/* copieer een AST */
astnode_t *copy_astnode(astnode_t *ast)
{
	astnode_t *astcopy;

	astcopy = create_ast(ast->asttag, ast->token, ast->number,
								ast->left, ast->middle, ast->right);
	return astcopy;
}


/* zet de strings van een AST in een lijst */
s_head_t *ast2slist(astnode_t *ast, s_head_t *slist)
{
	if (ast == NULL)
		return slist;
	slist = ast2slist(ast->left, slist);
	slist = ast2slist(ast->middle, slist);
	if (ast->token)
		append_slist(ast->token, slist);
	return slist;
}


/* verwijder een AST */
void delete_ast(astnode_t *ast)
{
	if (!ast)
		return;

	delete_ast(ast->left);
	delete_ast(ast->middle);
	delete_ast(ast->right);
	if (ast->token)
		free(ast->token);
	free((char *)ast);
	ast = NULL;
}


/* creeer het hoofd van een lijst met ASTs */
ast_head_t *create_ast_list()
{
	ast_head_t *new;

	if ((new = MALLOC(ast_head_t)) != NULL)
	{
		new->length = 0;
		new->first = new->last = NULL;
	}
	return(new);
}


/* creeer een element van een lijst met ASTs */
ast_el_t *create_ast_el(astnode_t *ast, ast_el_t *next)
{
	ast_el_t *new;

	if ((new = MALLOC(ast_el_t)) != NULL)
	{
		new->ast = ast;
		new->next = next;
	}
	return(new);
}


/* voeg een element toe aan een lijst met ASTs */
BOOLEAN append_ast_list(astnode_t *ast, ast_head_t *ast_list)
{
	ast_el_t *new;

	if ((new = create_ast_el(ast, NULL)) != NULL)
	{
		if (ast_list->length)
			ast_list->last->next = new;
		else
			ast_list->first = new;
		ast_list->last = new;
		ast_list->length++;
		return TRUE;
	}
	else
		return FALSE;
}


/* verwijder een lijst met ASTs */
void delete_ast_list(ast_head_t *list)
{
	if (list->length != 0)
		delete_ast_el(list->first, list->length);
	free((char *)list);
}


/* verwijder een aantal elementen (bepaald door het argument counter) van
	een lijst met ASTs */
void delete_ast_el(ast_el_t *el, int counter)
{
	if (counter != 0)
		delete_ast_el(el->next, counter-1);
	free((char *)el);
}


/* creeer het hoofd van een lijst met ASTs en informatie van variabelen uit
	een produktie regel */
ast_var_head_t *create_ast_var_list()
{
	ast_var_head_t *new;

	if ((new = MALLOC(ast_var_head_t)) != NULL)
	{
		new->length = 0;
		new->first = new->last = NULL;
	}
	return(new);
}


/* creeer een element van een lijst met ASTs en informatie van variabelen
	uit een produktie regel */
ast_var_el_t *create_ast_var_el(astnode_t *ast, int number_of_tests,
											varbind_head_t *varbindings,
											elvarbind_head_t *elvarbindings,
											ast_var_el_t *next)
{
	ast_var_el_t *new;

	if ((new = MALLOC(ast_var_el_t)) != NULL)
	{
		new->ast = ast;
		new->number_of_tests = number_of_tests;
		new->varbindings = varbindings;
		new->elvarbindings = elvarbindings;
		new->next = next;
	}
	return(new);
}


/* voeg een element toe aan een lijst met ASTs en informatie van variabelen
	uit een produktie regel */
BOOLEAN append_ast_var_list(astnode_t *ast, int number_of_tests,
										varbind_head_t *varbindings,
										elvarbind_head_t *elvarbindings,
										ast_var_head_t *ast_var_list)
{
	ast_var_el_t *new;

	if ((new = create_ast_var_el(ast, number_of_tests, varbindings,
											elvarbindings, NULL)) != NULL)
	{
		if (ast_var_list->length)
			ast_var_list->last->next = new;
		else
			ast_var_list->first = new;
		ast_var_list->last = new;
		ast_var_list->length++;
		return TRUE;
	}
	else
		return FALSE;
}


/* deze procedure bekijkt of een naam voor een produktie regel al is
	gedefinieerd */
BOOLEAN prodrulename_exists(char *prodrulename)
{
	register int i;
	ast_var_el_t *prod_mem_el;
	BOOLEAN prod_name_found;

	i = 1;
	prod_mem_el = production_mem->first;
	prod_name_found = FALSE;
	while (!prod_name_found && (i<=production_mem->length))
	{
		if (!strcmp(prod_mem_el->ast->left->token, prodrulename))
			prod_name_found = TRUE;
		else
		{
			prod_mem_el = prod_mem_el->next;
			i++;
		}
	}
	return (prod_name_found);
}


/* vind het aantal tests van een produktie regel (voor de specificiteit stap
	bij de conflict resolutie */
int find_number_of_tests(char *prodrulename)
{
	register int i;
	ast_var_el_t *prod_mem_el;
	BOOLEAN number_found;

	i = 1;
	prod_mem_el = production_mem->first;
	number_found = FALSE;
	while (!number_found && (i<=production_mem->length))
	{
		if (!strcmp(prod_mem_el->ast->left->token, prodrulename))
			number_found = TRUE;
		else
		{
			prod_mem_el = prod_mem_el->next;
			i++;
		}
	}
	if (number_found)
		return prod_mem_el->number_of_tests;
	else
		return 0;
}


/* verwijder een lijst met ASTs en informatie van variabelen
	uit een produktie regel */
void delete_ast_var_list(ast_var_head_t *list)
{
	if (list->length != 0)
		delete_ast_var_el(list->first, list->length);
	free((char *)list);
}


/* verwijder een aantal elementen (bepaald door het argument counter) van
	een lijst met ASTs en informatie van variabelen uit een produktie regel */
void delete_ast_var_el(ast_var_el_t *el, int counter)
{
	if (counter != 0)
		delete_ast_var_el(el->next, counter-1);
	free(el->varbindings);
	free(el->elvarbindings);
	free((char *)el);
}


