/*-----------------------------------------------------------------
Header file: ast.h

Header file van de file ast.c.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


extern astnode_t *create_ast(char asttag, char *token, float number,
										astnode_t *left, astnode_t *middle,
										astnode_t *right);
extern astnode_t *leaf_ast(char asttag, char *token, float number);
extern astnode_t *unary_ast(char asttag, astnode_t *left);
extern astnode_t *binary_ast(char asttag, astnode_t *left,
										astnode_t *middle);
extern astnode_t *ternary_ast(char asttag, astnode_t *left,
										astnode_t *middle, astnode_t *right);
extern astnode_t *copy_astnode(astnode_t *ast);
extern s_head_t *ast2slist(astnode_t *ast, s_head_t *slist);
extern void delete_ast(astnode_t *ast);

extern ast_head_t *create_ast_list(void);
extern ast_el_t *create_ast_el(astnode_t *ast, ast_el_t *next);
extern BOOLEAN append_ast_list(astnode_t *ast, 	ast_head_t *astlist);
extern void delete_ast_list(ast_head_t *list);
extern void delete_ast_el(ast_el_t *el, int counter);

extern ast_var_head_t *create_ast_var_list(void);
extern ast_var_el_t *create_ast_var_el(astnode_t *ast, int number_of_tests,
												varbind_head_t *varbindings,
												elvarbind_head_t *elvarbindings,
												ast_var_el_t *next);
extern BOOLEAN append_ast_var_list(astnode_t *ast, int number_of_tests,
												varbind_head_t *varbindings,
												elvarbind_head_t *elvarbindings,
												ast_var_head_t *astlist);
extern BOOLEAN prodrulename_exists(char *prodrulename);
extern int find_number_of_tests(char *prodrulename);
extern void delete_ast_var_list(ast_var_head_t *list);
extern void delete_ast_var_el(ast_var_el_t *el, int counter);
