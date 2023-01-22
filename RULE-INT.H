/*-----------------------------------------------------------------
Header file: rule-int.h

Header file van de file rule-int.c.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


extern void start_rule_interpreter(void);
extern instantiation_t *select_instantiation(
												instantiation_t *selected_instantiation);
extern instantiation_ptr_head_t *recency(
						instantiation_ptr_head_t *selected_instantiations, int max);
extern int get_next_highest_wm_el_number(workingmem_ptr_head_t *wm_list,
														int max);
extern instantiation_ptr_head_t *specificity(
									instantiation_ptr_head_t *selected_instantiations);
extern instantiation_ptr_head_t *arbitrary_choice(
									instantiation_ptr_head_t *selected_instantiations);


extern retenet_token_t *create_retenet_token(void);
extern void delete_retenet_token(retenet_token_t *retenet_token);
extern conflict_set_t *create_conflict_set(void);
extern instantiation_t *create_instantiation(astnode_t *prodrule,
													workingmem_ptr_head_t *workingmem_list,
													instantiation_t *next);
extern void add_instantiation(astnode_t *prodrule,
								workingmem_ptr_head_t *workingmem_list,
								conflict_set_t *conflict_set);
extern void remove_instantiation(astnode_t *prodrule,
									workingmem_ptr_head_t *workingmem_list);
extern void delete_instantiation(instantiation_t *instantiation, int length);
extern void delete_conflict_set(conflict_set_t *conflict_set);
extern instantiation_ptr_head_t *create_instantiation_ptr_head(void);
extern void copy_confl_set_to_instant_ptr_list(
									instantiation_ptr_head_t *selected_instantiations);
extern instantiation_ptr_el_t *create_instantiation_ptr_el(
															instantiation_t *instantiation,
															instantiation_ptr_el_t *next);
extern void add_instantiation_ptr_el(instantiation_t *instantiation,
									instantiation_ptr_head_t *instantiation_ptr_list);
extern void delete_instantiation_ptr_head(
									instantiation_ptr_head_t *instantiation_ptr_list);


extern void wm_el_through_retenet(char retenet_token_tag,
												workingmem_el_t *wm_el);
extern void match_wm_el(retenet_token_t *retenet_token);
extern void wm_el_through_retenet_el(retenet_token_t *retenet_token,
											root_el_t *retenet_el);
extern void test_node_constraints(retenet_token_t *retenet_token,
												char direction,
												node_rec_t *node);
extern BOOLEAN test_elclass_number(workingmem_el_t *wm_el,
													one_node_t *one_node);
extern BOOLEAN test_1_in_node(workingmem_el_t *wm_el, one_node_t *one_node);
extern BOOLEAN test_var_1_in_node(workingmem_el_t *wm_el,
									var_one_node_t *var_one_node);
extern BOOLEAN test_2_in_node(inter_restriction_head_t *inter_restrictions,
								workingmem_ptr_el_t *left_wm_ptr,
								workingmem_ptr_el_t *right_wm_ptr);
extern BOOLEAN satisfy_restriction(astnode_t *ast, char predicate,
										astnode_t *restriction);
extern BOOLEAN evaluate_string(char *string1, char predicate, char *string2);
extern BOOLEAN evaluate_number(float number1, char predicate, float number2);
