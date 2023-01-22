/*-----------------------------------------------------------------
Header file: parse.h

Header file van de file parse.c.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


extern astnode_t *parse_input(void);
extern void accept(char tclass, char error);
extern astnode_t *parse_literalize(void);
extern astnode_t *parse_p(void);
extern astnode_t *parse_make(void);
extern astnode_t *parse_lhs(void);
extern astnode_t *parse_ces(void);
extern astnode_t *parse_pos_ce(void);
extern astnode_t *parse_neg_ce(void);
extern astnode_t *parse_form(void);
extern astnode_t *parse_lhs_terms(int elclass_no);
extern astnode_t *parse_lhs_value(void);
extern astnode_t *parse_restriction(void);
extern astnode_t *parse_predicate(void);
extern astnode_t *parse_atomic_value(void);
extern astnode_t *parse_any_atom(void);
extern astnode_t *parse_disj_any_atom(void);
extern astnode_t *parse_var_or_const(void);
extern astnode_t *parse_number(void);
extern astnode_t *parse_disj_number(void);
extern astnode_t *parse_var(void);
extern astnode_t *parse_csa(void);
extern astnode_t *parse_disjunction(void);

extern astnode_t *parse_rhs(void);
extern astnode_t *parse_action(void);
extern astnode_t *parse_remove(void);
extern astnode_t *parse_modify(void);
extern astnode_t *parse_write(void);
extern astnode_t *parse_halt(void);
extern astnode_t *parse_rhs_pattern(void);
extern astnode_t *parse_rhs_terms(int elclass_no);
extern astnode_t *parse_rhs_term(int elclass_no);
extern astnode_t *parse_rhs_values(void);
extern astnode_t *parse_rhs_value(void);
extern astnode_t *parse_function(void);
extern astnode_t *parse_crlf(void);
extern astnode_t *parse_expression(void);
extern astnode_t *parse_primary_expression(void);
extern astnode_t *parse_operator(void);
extern astnode_t *parse_el_designator(void);

