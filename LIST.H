/*-----------------------------------------------------------------
Header file: list.h

Header file van de file list.c.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


extern main_t *create_lol(void);
extern link_t *create_link(int length, s_element_t *first, s_element_t *last,
									link_t *next);
extern BOOLEAN add_sublist(s_head_t *sub, main_t *lol);
extern s_head_t *create_slist(void);
extern s_element_t *create_selement(char *s,	s_element_t *ptr);
extern BOOLEAN append_slist(char *s, s_head_t *list);
extern s_element_t *create_ptrtostrelement(char *s, s_element_t *ptr);
extern void append_ptrtostrlist(char *s, s_head_t *list);
extern void delete_ptrtostrlist(s_head_t *list);
extern void delete_ptrtostrel(s_element_t *el, int counter);
extern BOOLEAN string_exists(char *string, s_head_t *string_list);

extern int search_decl(char *decl_name, link_t *decl);
extern int search_decllist(char *decl_name, int elclass_number);
extern int find_elclass_no(char *decl_name);
extern int get_elclass_length(int elclass_number);

extern var_head_t *create_varlist(void);
extern var_el_t *create_varel(int elclass_number, char predicate, char *var,
										int pos_number, BOOLEAN var_matched,
										var_el_t *next);
extern BOOLEAN add_varlist(int elclass_number, char predicate, char *var,
									int pos_number, BOOLEAN var_matched,
									var_head_t *list);
extern void delete_varlist(var_head_t *list);
extern void delete_varel(var_el_t *el, int counter);
extern int double_var(char *var, var_head_t *var_list);

extern elvarelclass_head_t *create_elvarelclass_head(void);
extern elvarelclass_el_t *create_elvarelclassel(char *elvar, int elclass_no,
															elvarelclass_el_t *next);
extern void add_elvarelclasslist(char *elvar, int elclass_no,
										elvarelclass_head_t *list);
extern void delete_elvarelclasslist(elvarelclass_head_t *list);
extern void delete_elvarelclassel(elvarelclass_el_t *el, int counter);
extern int get_elvarelclassno(char *elvar, elvarelclass_head_t *elvar_list);

extern varbind_head_t *create_varbindings(void);
extern BOOLEAN var_already_exists(char *var, varbind_head_t *varbindings);
extern varbind_el_t *create_varbinding(char *varname,	int condel_no,
													int value_pos, astnode_t *varvalue,
												  varbind_el_t *next);
extern void add_varbinding(char *varname,	int condel_no, int value_pos,
									astnode_t *varvalue, varbind_head_t *list);
extern void delete_varbindings(varbind_head_t *list);
extern void delete_varbinding(varbind_el_t *el);
extern elvarbind_head_t *create_elvarbindings(void);
extern BOOLEAN elvar_already_exists(char *var,
												elvarbind_head_t *elvarbindings);
extern elvarbind_el_t *create_elvarbinding(char *varname, astnode_t *prodrule,
												int cond_number, elvarbind_el_t *next);
extern void add_elvarbinding(char *varname, astnode_t *prodrule,
										int cond_number, elvarbind_head_t *list);
extern void delete_elvarbindings(elvarbind_head_t *list);
extern void delete_elvarbinding(elvarbind_el_t *el);














