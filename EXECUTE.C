/*-----------------------------------------------------------------
C file: execute.c

Deze file bevat procedures voor het uitvoeren van SOPS5 commando's.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decl.h"
#include "execute.h"
#include "list.h"
#include "ast.h"
#include "retenet.h"
#include "workmem.h"
#include "error.h"
#include "rule-int.h"


/* voer een commando, gerepresenteerd door een AST, uit */
void exec_command(astnode_t *ast)
{
	s_head_t *decl;

	switch(ast->asttag)
	{
		case ACTION_ACTION:
			exec_command(ast->left);
			exec_command(ast->middle);
			break;
		case LITERALIZE:
			decl = create_slist();
			decl = ast2slist(ast->left, decl);
			if (!add_sublist(decl, decl_list))
				print_error(OUT_OF_MEM, "");
			break;
		case P:
			exec_p(ast);
			break;
		case MAKE:
			exec_make(ast->left);
			break;
		case REMOVE:
			exec_remove(ast->left);
			break;
		case MODIFY:
			exec_modify(ast);
			break;
		case WRITE:
			exec_write(ast->left);
			break;
		case HALT:
			halt = TRUE;
			break;
	}
}


/* verwerk een produktie regel, gerepresenteerd door een AST,
	(bouw retenet) */
void exec_p(astnode_t *ast)
{
	varbindings = create_varbindings();
	elvarbindings = create_elvarbindings();
	build_retenet(ast);
	append_ast_var_list(ast, test_counter, varbindings, elvarbindings,
								production_mem);
}


/* voer het "make" commando uit */
void exec_make(astnode_t *ast)
{
	workingmem_el_t *wm_element;
	ast_head_t *attribute_values;
	int elclass_number, elclass_length;

	if (ast->asttag == RHSPATTERN)
		elclass_number = find_elclass_no(ast->left->token);
	else
		elclass_number = find_elclass_no(ast->token);
	elclass_length = get_elclass_length(elclass_number);
	if (elclass_length > 0)
	{
		wm_el_counter++;
		if (ast->asttag == RHSPATTERN)
		{
			attribute_values = create_attribute_values(elclass_length);
			process_rhs_term(ast->middle, attribute_values, elclass_number);
		}
		else
			attribute_values = NULL;
		add_wm_el(wm_el_counter, elclass_number, attribute_values,
						working_mem);
		wm_element = find_working_element(wm_el_counter);
		if (wm_element == NULL)
			;
		else
			wm_el_through_retenet(PLUS, wm_element);
	}
	else
		;
}


/* voer het "remove" commando uit */
void exec_remove(astnode_t *ast)
{
	int cond_number;
	workingmem_el_t *wm_element;

	switch (ast->asttag)
	{
		case ELD_ELD:
			exec_remove(ast->left);
			exec_remove(ast->middle);
			break;
		case VAR:
			cond_number = get_elvar_value(ast->token);
			wm_element = get_wm_el(cond_number);
			if (wm_element == NULL)
				;
			else
			{
				wm_el_through_retenet(MIN, wm_element);
				remove_wm_el(wm_element->wm_el_number);
			}
			break;
	}
}


/* voer het "modify" commando uit */
void exec_modify(astnode_t *ast)
{
	int cond_number;
	workingmem_el_t *wm_element;
	ast_head_t *attribute_values;

	cond_number = get_elvar_value(ast->left->token);
	wm_element = get_wm_el(cond_number);
	wm_el_counter++;
	attribute_values = copy_attribute_values(wm_element->attribute_values);
	wm_el_through_retenet(MIN, wm_element);
	process_rhs_term(ast->middle, attribute_values,wm_element->elclass_number);
	add_wm_el(wm_el_counter, wm_element->elclass_number, attribute_values,
					working_mem);
	remove_wm_el(wm_element->wm_el_number);
	wm_element = find_working_element(wm_el_counter);
	if (wm_element == NULL)
		;
	else
		wm_el_through_retenet(PLUS, wm_element);
}


/* voer het "write" commando uit */
void exec_write(astnode_t *ast)
{
	astnode_t *temp_ast;

	switch(ast->asttag)
	{
		case RV_RV:
			exec_write(ast->left);
			exec_write(ast->middle);
			break;
		case ATOMICVALUE:
			temp_ast = process_rhs_value(ast);
			if (temp_ast->asttag == NUMBER)
				printf("%g ",temp_ast->number);
			else
			{
				if (!strcmp(temp_ast->token,"\n"))
					printf("\n");
				else
					printf("%s ",temp_ast->token);
			}
			break;
		case FUNC:
			temp_ast = process_rhs_value(ast);
			if (temp_ast->asttag == NUMBER)
				printf("%g ",temp_ast->number);
			else
			{
				if (!strcmp(temp_ast->token,"\n"))
					printf("\n");
				else
					printf("%s ",temp_ast->token);
			}
			break;
	}
}


/* voer een instantiatie uit */
void exec_instantiation(instantiation_t *instantiation)
{
	astnode_t *prodrule;
	register int i;
	int prodmem_length;
	BOOLEAN rule_found;
	ast_var_el_t *prodmem_el;

	instantiation_wm_list = instantiation->workingmem_list;
	prodrule = instantiation->prodrule;
	i = 1;
	prodmem_length = production_mem->length;
	rule_found  = FALSE;
	prodmem_el = production_mem->first;
	while (!rule_found && (i<=prodmem_length))
	{
		if (!strcmp(prodrule->left->token, prodmem_el->ast->left->token))
			rule_found = TRUE;
		else
		{
			prodmem_el = prodmem_el->next;
			i++;
		}
	}
	varbindings = prodmem_el->varbindings;
	elvarbindings = prodmem_el->elvarbindings;
	set_variables();
	exec_command(prodrule->right);
	if (halt == TRUE)
		exit(0);
	delete_varvalues();
}


/* geef de variabelen in een voor uitvoering geselecteerde produktieregel
	hun waarden */
void set_variables(void)
{
	register int i, j;
	varbind_el_t *varbinding;
	workingmem_ptr_el_t *wm_ptr;
	ast_el_t *attribute_value;

	varbinding = varbindings->first;
	for (i=1;i<=varbindings->length;i++)
	{
		wm_ptr = instantiation_wm_list->first;
		for (j=2;j<=varbinding->condel_no;j++)
			wm_ptr = wm_ptr->next;
		attribute_value =  wm_ptr->wm_el_ptr->attribute_values->first;
		for (j=2;j<=(varbinding->value_pos-1);j++)
			attribute_value = attribute_value->next;
		varbinding->varvalue = copy_astnode(attribute_value->ast);
		varbinding = varbinding->next;
	}
}


/* verwijder de waarden van variabelen van een voor uitvoering
	geselecteerde produktieregel */
void delete_varvalues(void)
{
	register int i;
	varbind_el_t *varbinding;

	varbinding = varbindings->first;
	for (i=1;i<=varbindings->length;i++)
	{
		delete_ast(varbinding->varvalue);
		varbinding = varbinding->next;
	}
}


/* verwerk de right hand side termen */
void process_rhs_term(astnode_t *ast, ast_head_t *attribute_values,
								int elclass_number)
{
	int pos;
	astnode_t *temp_ast;

	switch(ast->asttag)
	{
		case RT_RT:
			process_rhs_term(ast->left,attribute_values, elclass_number);
			process_rhs_term(ast->middle,attribute_values, elclass_number);
			break;
		case CSA_RHSVAL:
			pos = search_decllist(ast->left->token, elclass_number);
			if (pos > 0)
			{
				ast = ast->middle;
				temp_ast = process_rhs_value(ast);
				set_attribute_values(pos-1, temp_ast, attribute_values);
			}
			else
				;
			break;
	}
}


/* verwerk een right hand side value */
astnode_t *process_rhs_value(astnode_t *ast)
{
	astnode_t *temp_ast;

	switch(ast->asttag)
	{
		case ATOMICVALUE:
			ast = ast->left;
			switch(ast->asttag)
			{
				case VOC:
					ast = ast->left;
					temp_ast = ast;
					switch(ast->asttag)
					{
						case VAR:
							temp_ast = get_var_value(ast->token);
							break;
					}
					break;
				case ANYATOM:
					temp_ast = ast;
					break;
				default:
					break;
			}
			break;
		case FUNC:
			ast = ast->left;
			switch(ast->asttag)
			{
				case CRLF:
					temp_ast = ast;
					break;
				case COMPUTE:
					ast = ast->left;
					numberast->number = evaluate_expression(ast);
					temp_ast = copy_astnode(numberast);
					break;
			}
			break;
	}
	return temp_ast;
}


/* evalueer een expressie */
float evaluate_expression(astnode_t *ast)
{
	float number, number1, number2;
	astnode_t *temp_ast;

	switch(ast->asttag)
	{
		case NUMBER:
			number = ast->number;
			break;
		case VAR:
			temp_ast = get_var_value(ast->token);
			if (temp_ast->asttag == NUMBER)
				number = temp_ast->number;
			else
				print_run_time_error(VAR_NOT_NUMBER, temp_ast->token);
			break;
		case E_O_E:
			number1 = evaluate_expression(ast->left);
			number2 = evaluate_expression(ast->right);
			number = apply_operator(number1, ast->middle, number2);
			break;
	}
	return number;
}


/* pas een operator toe op twee getallen */
float apply_operator(float number1, astnode_t *opast, float number2)
{
	float number;

	switch(opast->asttag)
	{
		case OPLUS:
			number = number1 + number2;
			break;
		case OMIN:
			number = number1 - number2;
			break;
		case OMUL:
			number = number1 * number2;
			break;
		case ODIV:
			if (number2 != 0)
				number = number1 / number2;
			else
				print_run_time_error(DIVBYZERO, "");
			break;
	}
	return number;
}


/* zoek de waarde van een gebonden variabele op */
astnode_t *get_var_value(char *varname)
{
	varbind_el_t *varbinding;
	register int i;
	BOOLEAN varbinding_found;
	astnode_t *varvalue;

	i = 1;
	varbinding_found = FALSE;
	varbinding = varbindings->first;
	while (!varbinding_found && (i<=varbindings->length))
	{
		if (!strcmp(varname, varbinding->varname))
			varbinding_found = TRUE;
		else
		{
			varbinding = varbinding->next;
			i++;
		}
	}
	varvalue = copy_astnode(varbinding->varvalue);
	return varvalue;
}


/* zoek de waarde van een gebonden element variabele op */
int get_elvar_value(char *varname)
{
	elvarbind_el_t *elvarbinding;
	register int i;
	BOOLEAN elvarbinding_found;

	i = 1;
	elvarbinding_found = FALSE;
	elvarbinding = elvarbindings->first;
	while (!elvarbinding_found && (i<=elvarbindings->length))
	{
		if (!strcmp(varname, elvarbinding->varname))
			elvarbinding_found = TRUE;
		else
		{
			elvarbinding = elvarbinding->next;
			i++;
		}
	}
	if (elvarbinding_found)
		return elvarbinding->cond_number;
	else
		return 0;
}


/* zoek een working memory element op */
workingmem_el_t *get_wm_el(int cond_number)
{
	register int i;
	workingmem_ptr_el_t *wm_ptr_el;

	wm_ptr_el = instantiation_wm_list->first;
	for (i=2;i<=cond_number;i++)
		wm_ptr_el = wm_ptr_el->next;
	return wm_ptr_el->wm_el_ptr;
}