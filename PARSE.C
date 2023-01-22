/*------------------------------------------------------------------
C file: parse.c

Deze file bevat procedures voor het parsen van een string (ingelezen
uit een invoer file) om een abstract syntax tree (AST) op te bouwen.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


#include <string.h>
#include <math.h>

#include "decl.h"
#include "parse.h"
#include "scan.h"
#include "ast.h"
#include "error.h"
#include "list.h"


/* parse ingelezen invoerstring */
astnode_t *parse_input()
{
	astnode_t *ast;

	accept(L_PAREN_TK, L_PAREN);
	switch(token_class)
	{
		case LITERALIZE_TK:
			get_token();
			ast = parse_literalize();
			break;
		case P_TK:
			top_level = FALSE;
			get_token();
			ast = parse_p();
			break;
		case MAKE_TK:
			top_level = TRUE;
			get_token();
			ast = parse_make();
			break;
		case REMOVE_TK:
			print_syntax_error(NOT_TOP_LEVEL_COMMAND, token);
			break;
		case MODIFY_TK:
			print_syntax_error(NOT_TOP_LEVEL_COMMAND, token);
			break;
		case WRITE_TK:
			print_syntax_error(NOT_TOP_LEVEL_COMMAND, token);
			break;
		case HALT_TK:
			print_syntax_error(NOT_TOP_LEVEL_COMMAND, token);
			break;
		default:
			print_syntax_error(COMMAND, token);
	}
	accept(R_PAREN_TK, R_PAREN);
	return ast;
}


/* accepteer een token */
void accept(char tclass, char error)
{
	if (token_class == tclass)
		get_token();
	else
		print_syntax_error(error, "\0");
}


/* parse literalize commando */
astnode_t *parse_literalize()
{
	s_head_t *attribute_list;
	astnode_t *i1ast, *i2ast;

	i1ast = parse_csa();
	if (find_elclass_no(i1ast->token) != 0)
		print_syntax_error(EL_CLASS_EXISTS, i1ast->token);
	attribute_list = create_slist();
	while (token_class != R_PAREN_TK)
	{
		i2ast = parse_csa();
		if (string_exists(i2ast->token, attribute_list))
			print_syntax_error(ATTRIBUTE_EXISTS, i2ast->token);
		else
			append_ptrtostrlist(i2ast->token, attribute_list);
		i1ast = binary_ast(CSALIST_CSALIST, i1ast, i2ast);
	}
	delete_ptrtostrlist(attribute_list);
	i1ast = unary_ast(LITERALIZE, i1ast);
	return i1ast;
}


/* parse een produktieregel */
astnode_t *parse_p()
{
	astnode_t *iast, *lhsast, *rhsast, *past;

	elvarelclass_list = create_elvarelclass_head();
	varlist = create_slist();
	iast = parse_csa();
	if (prodrulename_exists(iast->token))
		print_syntax_error(PRODRULENAME_EXISTS, iast->token);
	parse_righths = FALSE;
	lhsast = parse_lhs();
	accept(ARROW_TK, ARROW);
	parse_righths = TRUE;
	rhsast = parse_rhs();
	past = ternary_ast(P,iast,lhsast,rhsast);
	delete_elvarelclasslist(elvarelclass_list);
	delete_ptrtostrlist(varlist);
	return past;
}


/* parse lefthand side  */
astnode_t *parse_lhs()
{
	astnode_t *pcast, *cesast;

	pcast = parse_pos_ce();
	if (token_class != ARROW_TK)
	{
		cesast = parse_ces();
		pcast = binary_ast(POSCE_CE, pcast, cesast);
	}
	else
		pcast = unary_ast(POSCE, pcast);
	return pcast;
}


/* parse conditie elementen */
astnode_t *parse_ces()
{
	astnode_t *ce1ast, *ce2ast;

	if (token_class == NEG_TK)
		ce1ast = parse_neg_ce();
	else
		ce1ast = parse_pos_ce();
	while (token_class != ARROW_TK)
	{
		if (token_class == NEG_TK)
			ce2ast = parse_neg_ce();
		else
			ce2ast = parse_pos_ce();
		ce1ast = binary_ast(CE_CE, ce1ast, ce2ast);
	}
	return ce1ast;
}


/* parse een positieve conditie element */
astnode_t *parse_pos_ce()
{
	astnode_t *fast, *evast;
	int elclass_no;

	if (token_class == L_PAREN_TK)
	{
		fast = parse_form();
		fast = unary_ast(FORM, fast);
	}
	else
	{
		accept(L_CURLY_BRACKET_TK, L_CURLY_BRACKET);
		if (token_class == LT_TK)
		{
			get_token();
			evast = parse_var();
			elclass_no = get_elvarelclassno(evast->token, elvarelclass_list);
			if (elclass_no != 0)
				print_syntax_error(ELVAR_EXISTS, evast->token);
			accept(GT_TK, GT);
			fast = parse_form();
			elclass_no = find_elclass_no(fast->left->token);
			add_elvarelclasslist(evast->token, elclass_no, elvarelclass_list);
			fast = binary_ast(VAR_FORM,evast,fast);
		}
		else
		{
			fast = parse_form();
			get_token();
			evast = parse_var();
			accept(GT_TK, GT);
			elclass_no = get_elvarelclassno(evast->token, elvarelclass_list);
			if (elclass_no != 0)
				print_syntax_error(ELVAR_EXISTS, evast->token);
			elclass_no = find_elclass_no(fast->left->token);
			add_elvarelclasslist(evast->token, elclass_no, elvarelclass_list);
			fast = binary_ast(FORM_VAR,fast,evast);
		}
		accept(R_CURLY_BRACKET_TK, R_CURLY_BRACKET);
	}
	return fast;
}


/* parse een negatieve conditie element */
astnode_t *parse_neg_ce()
{
	astnode_t *nfast;

	accept(NEG_TK, NEG);
	nfast = parse_form();
	nfast = unary_ast(NEGFORM, nfast);
	return nfast;
}


/* parse form */
astnode_t *parse_form()
{
	astnode_t *iast, *ltast;
	int elclass_no;

	accept(L_PAREN_TK, L_PAREN);
	iast = parse_csa();
	elclass_no = find_elclass_no(iast->token);
	if (elclass_no == 0)
		print_syntax_error(NOT_EL_CLASS, iast->token);
	if (token_class != R_PAREN_TK)
	{
		ltast = parse_lhs_terms(elclass_no);
		iast = binary_ast(CSA_LHSTERM, iast, ltast);
	}
	accept(R_PAREN_TK, R_PAREN);
	return iast;
}


/* parse lhs terms */
astnode_t *parse_lhs_terms(int elclass_no)
{
	astnode_t *lt1ast, *lt2ast, *iast, *lvast;

	accept(UPARROW_TK, UPARROW);
	iast = parse_csa();
	if (search_decllist(iast->token, elclass_no) == 0)
		print_syntax_error(NOT_ATTRIBUTE, iast->token);
	lvast = parse_lhs_value();
	lt1ast = binary_ast(CSA_LHSVAL, iast, lvast);
	while (token_class != R_PAREN_TK)
	{
		accept(UPARROW_TK, UPARROW);
		iast = parse_csa();
		if (search_decllist(iast->token, elclass_no) == 0)
			print_syntax_error(NOT_ATTRIBUTE, iast->token);
		lvast = parse_lhs_value();
		lt2ast = binary_ast(CSA_LHSVAL, iast, lvast);
		lt1ast = binary_ast(LT_LT, lt1ast, lt2ast);
	}
	return lt1ast;
}


/* parse lhs value*/
astnode_t *parse_lhs_value()
{
	astnode_t *r1ast, *r2ast;

	r1ast = NULL;
	if (token_class == L_CURLY_BRACKET_TK)
	{
		get_token();
		if (token_class != R_CURLY_BRACKET_TK)
			r1ast = parse_restriction();
		while (token_class != R_CURLY_BRACKET_TK)
		{
			r2ast = parse_restriction();
			r1ast = binary_ast(LV_LV, r1ast, r2ast);
		}
		accept(R_CURLY_BRACKET_TK, R_CURLY_BRACKET);
	}
	else
		r1ast = parse_restriction();
	return r1ast;
}


/* parse restriction*/
astnode_t *parse_restriction()
{
	astnode_t *restrast, *dast, *predast, *avast;

	if (token_class == TWO_L_ANGLE_BRACKET_TK)
	{
		get_number_or_any_atom();
		dast = parse_disjunction();
		accept(TWO_R_ANGLE_BRACKET_TK, TWO_R_ANGLE_BRACKET);
		restrast = unary_ast(DISJ, dast);
	}
	else
	{
		if (ispredicate())
		{
			if (token[0] == EQ_TK)
				pred_atomic_val = FALSE;
			else
				pred_atomic_val = TRUE;
			predast = parse_predicate();
			avast = parse_atomic_value();
			restrast = binary_ast(PRED_ATOMICVALUE, predast, avast);
		}
		else
		{
			pred_atomic_val = FALSE;
			avast = parse_atomic_value();
			restrast = unary_ast(ATOMICVALUE, avast);
		}
	}
	return restrast;
}


/* parse predicate */
astnode_t *parse_predicate()
{
	astnode_t *predast;

	predast = leaf_ast(PRED, token, 0);
	get_token();
	return predast;
}


/* parse atomic value*/
astnode_t *parse_atomic_value()
{
	astnode_t *avast, *aatast, *vocast;

	if (token_class == TAKE_LITERALLY_TK)
	{
		get_any_atom();
		aatast = parse_any_atom();
		avast = aatast;
	}
	else
	{
		vocast = parse_var_or_const();
		avast = unary_ast(VOC, vocast);
	}
	return avast;
}


/* parse any atom*/
astnode_t *parse_any_atom()
{
	astnode_t *aatast;

	aatast = leaf_ast(ANYATOM,token,0);
	get_token();
	return aatast;
}


/* parse any atom voor een disjunctie */
astnode_t *parse_disj_any_atom()
{
	astnode_t *aatast;

	aatast = leaf_ast(ANYATOM,token,0);
	get_number_or_any_atom();
	return aatast;
}


/* parse var or const*/
astnode_t *parse_var_or_const()
{
	astnode_t *vocast, *nast, *vast, *csaast;

	if (token_class == NUMBER_TK)
	{
		nast = parse_number();
		vocast = nast;
	}
	else
	{
		if (token_class == LT_TK)
		{
			if (top_level == FALSE)
			{
				get_token();
				vast = parse_var();
				accept(GT_TK, GT);
				if (parse_righths == TRUE)
				{
					if (!string_exists(vast->token, varlist))
						print_syntax_error(FIRST_VAR_RHS, vast->token);
				}
				else if (pred_atomic_val == TRUE)
				{
					if (!string_exists(vast->token, varlist))
						print_syntax_error(FIRST_VAR_LHS, vast->token);
				}
				else
				{
					if (!string_exists(vast->token, varlist))
						append_ptrtostrlist(vast->token, varlist);
				}
				vocast = vast;
			}
			else
				print_syntax_error(NO_VAR_TOP_LEVEL ,"");
		}
		else
		{
			csaast = parse_csa();
			vocast = csaast;
		}
	}
	return vocast;
}


/* parse number */
astnode_t *parse_number()
{
	astnode_t *nast;

	if (token_class == NUMBER_TK)
	{
		nast = leaf_ast(NUMBER, token, atof(token));
		get_token();
	}
	else
		print_syntax_error(NOT_NUMBER, token);
	return nast;
}


/* parse number voor disjunctie*/
astnode_t *parse_disj_number()
{
	astnode_t *nast;

	nast = leaf_ast(NUMBER, token, atof(token));
	get_number_or_any_atom();
	return nast;
}


/* parse var */
astnode_t *parse_var()
{
	astnode_t *vast;

	if (token_class == CSA_TK)
	{
		vast = leaf_ast(VAR,token,0);
		get_token();
	}
	else
		print_syntax_error(NOT_CSA, token);
	return vast;
}


/* parse een constant symbolic atom */
astnode_t *parse_csa()
{
	astnode_t *csaast;

	if (token_class == CSA_TK)
	{
		csaast = leaf_ast(CSA,token,0);
		get_token();
	}
	else
		print_syntax_error(NOT_CSA, token);
	return csaast;
}


/* parse disjunction */
astnode_t *parse_disjunction()
{
	astnode_t *d1ast, *d2ast;

	if (token_class == NUMBER_TK)
		d1ast = parse_disj_number();
	else
		d1ast = parse_disj_any_atom();
	while ((token_class != TWO_R_ANGLE_BRACKET_TK) && (token_class != EOT_TK))
	{
		if (token_class == NUMBER_TK)
			d2ast = parse_disj_number();
		else
			d2ast = parse_disj_any_atom();
		d1ast = binary_ast(DISJ_DISJ, d1ast, d2ast);
	}
	return d1ast;
}


/* parse right hand side */
astnode_t *parse_rhs()
{
	astnode_t *act1ast, *act2ast, *rhsast;

	act1ast = parse_action();
	while (token_class != R_PAREN_TK)
	{
		act2ast = parse_action();
		act1ast = binary_ast(ACTION_ACTION, act1ast, act2ast);
	}
	rhsast = act1ast;
	return rhsast;
}


/* parse action */
astnode_t *parse_action()
{
	astnode_t *actast;

	accept(L_PAREN_TK, L_PAREN);
	switch(token_class)
	{
		case MAKE_TK:
			get_token();
			actast = parse_make();
			break;
		case REMOVE_TK:
			get_token();
			actast = parse_remove();
			break;
		case MODIFY_TK:
			get_token();
			actast = parse_modify();
			break;
		case WRITE_TK:
			get_token();
			actast = parse_write();
			break;
		case HALT_TK:
			get_token();
			actast = parse_halt();
			break;
		default:
			print_syntax_error(NOT_ACTION_COMMAND, token);
	}
	accept(R_PAREN_TK, R_PAREN);
	return actast;
}


/* parse make commando */
astnode_t *parse_make()
{
	astnode_t *makeast, *rhspatternast;

	rhspatternast = parse_rhs_pattern();
	makeast = unary_ast(MAKE, rhspatternast);
	return makeast;
}


/* parse remove command */
astnode_t *parse_remove()
{
	astnode_t *removeast, *eldesignatorast;

	eldesignatorast = parse_el_designator();
	removeast = unary_ast(REMOVE, eldesignatorast);
	return removeast;
}


/* parse modify command */
astnode_t *parse_modify()
{
	astnode_t *modifyast, *varast, *rhstermsast;
	int elclass_no;

	accept(LT_TK, LT);
	varast = parse_var();
	accept(GT_TK, GT);
	elclass_no = get_elvarelclassno(varast->token, elvarelclass_list);
	if (elclass_no == 0)
		print_syntax_error(ELVAR_NOT_DEFINED, varast->token);
	rhstermsast = parse_rhs_terms(elclass_no);
	modifyast = binary_ast(MODIFY, varast, rhstermsast);
	return modifyast;
}


/* parse write command */
astnode_t *parse_write()
{
	astnode_t *writeast, *rhsvaluesast;

	write = TRUE;
	rhsvaluesast = parse_rhs_values();
	writeast = unary_ast(WRITE, rhsvaluesast);
	write = FALSE;
	return writeast;
}


/* parse halt command */
astnode_t *parse_halt()
{
	astnode_t *haltast;

	haltast = leaf_ast(HALT, NULL,0);
	return haltast;
}


/* parse rhs pattern */
astnode_t *parse_rhs_pattern()
{
	astnode_t *iast, *rhstermsast, *rhspatternast;
	int elclass_no;

	iast = parse_csa();
	elclass_no = find_elclass_no(iast->token);
	if (elclass_no == 0)
		print_syntax_error(NOT_EL_CLASS, iast->token);
	if (token_class != R_PAREN_TK)
	{
		rhstermsast = parse_rhs_terms(elclass_no);
		rhspatternast = binary_ast(RHSPATTERN, iast, rhstermsast);
	}
	else
		rhspatternast = iast;
	return rhspatternast;
}


/* parse rhs terms */
astnode_t *parse_rhs_terms(int elclass_no)
{
	astnode_t *rhsterm1ast, *rhsterm2ast;

	rhsterm1ast = parse_rhs_term(elclass_no);
	while (token_class != R_PAREN_TK)
	{
		rhsterm2ast = parse_rhs_term(elclass_no);
		rhsterm1ast = binary_ast(RT_RT, rhsterm1ast, rhsterm2ast);
	}
	return rhsterm1ast;
}


/* parse rhs term */
astnode_t *parse_rhs_term(int elclass_no)
{
	astnode_t *rhstermast, *iast, *rhsvast;

	accept(UPARROW_TK, UPARROW);
	iast = parse_csa();
	if (search_decllist(iast->token, elclass_no) == 0)
		print_syntax_error(NOT_ATTRIBUTE, iast->token);
	rhsvast = parse_rhs_value();
	rhstermast = binary_ast(CSA_RHSVAL, iast, rhsvast);
	return rhstermast;
}


/* parse rhs values */
astnode_t *parse_rhs_values()
{
	astnode_t *rhsval1ast, *rhsval2ast;

	rhsval1ast = parse_rhs_value();
	while (token_class != R_PAREN_TK)
	{
		rhsval2ast = parse_rhs_value();
		rhsval1ast = binary_ast(RV_RV, rhsval1ast, rhsval2ast);
	}
	return rhsval1ast;
}


/* parse rhs value */
astnode_t *parse_rhs_value()
{
	astnode_t *rhsvast, *rhsfast, *avast;

	if (token_class == L_PAREN_TK)
	{
		get_token();
		rhsfast = parse_function();
		accept(R_PAREN_TK, R_PAREN);
		rhsvast = unary_ast(FUNC, rhsfast);
	}
	else
	{
		pred_atomic_val = FALSE;
		avast = parse_atomic_value();
		rhsvast = unary_ast(ATOMICVALUE, avast);
	}
	return rhsvast;
}


/* parse rhs function */
astnode_t *parse_function()
{
	astnode_t *rhsfast, *expast;

	if (top_level == FALSE)
	{
		switch(token_class)
		{
			case CRLF_TK:
				if (write == TRUE)
				{
					get_token();
					rhsfast = parse_crlf();
				}
				else
					print_syntax_error(NO_CRLF_WRITE, "");
				break;
			case COMPUTE_TK:
				get_token();
				expast = parse_expression();
				rhsfast = unary_ast(COMPUTE, expast);
				break;
			default:
				print_syntax_error(FUNCTION, token);
		}
	}
	else
		print_syntax_error(NO_FUNCTION_TOP_LEVEL, "");
	return rhsfast;
}


/* parse crlf */
astnode_t *parse_crlf()
{
	astnode_t *crlfast;

	crlfast = leaf_ast(CRLF, "\n",0);
	return crlfast;
}


/* parse expressie */
astnode_t *parse_expression()
{
	astnode_t *exp1ast, *exp2ast, *opast;

	exp1ast = parse_primary_expression();
	while ((token_class == PLUS_TK) || (token_class == NEG_TK) ||
			 (token_class == MUL_TK) || (token_class == TAKE_LITERALLY_TK))
	{
		opast = parse_operator();
		exp2ast = parse_expression();
		exp1ast = ternary_ast(E_O_E, exp1ast, opast, exp2ast);
	}
	return exp1ast;
}


/* parse primary expressie */
astnode_t *parse_primary_expression()
{
	astnode_t *expast;

	if (token_class == NUMBER_TK)
	{
		expast = parse_number();
	}
	else
	{
		if (token_class == LT_TK)
		{
			get_token();
			expast = parse_var();
			accept(GT_TK, GT);
			if (!string_exists(expast->token, varlist))
				print_syntax_error(FIRST_VAR_RHS, expast->token);
		}
		else
		{
			if (token_class == L_PAREN_TK)
			{
				get_token();
				expast = parse_expression();
				accept(R_PAREN_TK, R_PAREN);
			}
			else
				print_syntax_error(NOT_EXPRESSION, token);
		}
	}
	return expast;
}


/* parse operator */
astnode_t *parse_operator()
{
	astnode_t *opast;

	switch (token_class)
	{
		case PLUS_TK:
			opast = leaf_ast(OPLUS,token,0);
			get_token();
			break;
		case NEG_TK:
			opast = leaf_ast(OMIN,token,0);
			get_token();
			break;
		case MUL_TK:
			opast = leaf_ast(OMUL,token,0);
			get_token();
			break;
		case TAKE_LITERALLY_TK:
			opast = leaf_ast(ODIV,token,0);
			get_token();
			break;
		default:
			print_syntax_error(OPERATOR, token);
			break;
	}
	return opast;
}


/* parse element designator */
astnode_t *parse_el_designator()
{
	astnode_t *eld1ast, *eld2ast;

	accept(LT_TK, LT);
	eld1ast = parse_var();
	accept(GT_TK, GT);
	if (get_elvarelclassno(eld1ast->token, elvarelclass_list) == 0)
		print_syntax_error(ELVAR_NOT_DEFINED, eld1ast->token);
	while (token_class != R_PAREN_TK)
	{
		accept(LT_TK, LT);
		eld2ast = parse_var();
		accept(GT_TK, GT);
		if (get_elvarelclassno(eld2ast->token, elvarelclass_list) == 0)
			print_syntax_error(ELVAR_NOT_DEFINED, eld2ast->token);
		eld1ast = binary_ast(ELD_ELD, eld1ast, eld2ast);
	}
	return eld1ast;
}


