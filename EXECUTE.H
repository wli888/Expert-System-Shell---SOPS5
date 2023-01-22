/*-----------------------------------------------------------------
Header file: execute.h

Header file van de file execute.c.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


extern void exec_command(astnode_t *ast);
extern void exec_p(astnode_t *ast);
extern void exec_make(astnode_t *ast);
extern void exec_remove(astnode_t *ast);
extern void exec_modify(astnode_t *ast);
extern void exec_write(astnode_t *ast);

extern void exec_instantiation(instantiation_t *instantiation);
extern void set_variables(void);
extern void delete_varvalues(void);
extern void process_rhs_term(astnode_t *ast, ast_head_t *wm_element,
										int elclass_number);
extern astnode_t *process_rhs_value(astnode_t *ast);
extern float evaluate_expression(astnode_t *ast);
extern float apply_operator(float number1, astnode_t *opast, float number2);
extern astnode_t *get_var_value(char *varname);
extern int get_elvar_value(char *varname);
extern workingmem_el_t *get_wm_el(int cond_number);


