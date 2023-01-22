/*-----------------------------------------------------------------
Header file: retenet.h

Header file van de file retenet.c.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


extern root_head_t *create_retenet(void);
extern root_el_t *create_rootel(one_node_t *element_class, root_el_t *next);
extern node_type_head_t *create_node_type_head(void);
extern node_type_el_t *create_node_type_el(char direction, node_rec_t *node,
												 node_type_el_t *next);
extern BOOLEAN add_node_type_el(char direction, node_rec_t *node,
											node_type_head_t *next_nodes);
extern void delete_node_type_head(node_type_head_t *next_nodes);
extern void delete_node_type_el(node_type_el_t *el);

extern BOOLEAN is_same_restriction(astnode_t *new_restriction,
										astnode_t *node_restriction);
extern BOOLEAN build_1_in_node(int pos_number, astnode_t *restriction);

extern inter_restriction_head_t *create_interconditiontests(void);
extern inter_restriction_el_t *create_interconditiontest(var_el_t *varptr1,
								var_el_t *varptr2, inter_restriction_el_t *next);
extern BOOLEAN add_interconditiontest(var_el_t *varptr1, var_el_t *varptr2,
										inter_restriction_head_t *interconditiontests);
extern void delete_interconditiontests(inter_restriction_head_t *list);
extern void delete_interconditiontest(inter_restriction_el_t *el, int counter);
extern inter_restriction_head_t *process_varlists(void);
extern left_mem_t *create_leftmem(void);
extern not_left_mem_t *create_not_leftmem(void);
extern right_mem_t *create_rightmem(void);
extern BOOLEAN build_2_in_node(void);
extern BOOLEAN build_not_2_in_node(void);
extern BOOLEAN build_p_node(astnode_t *ast);
extern node_rec_t *create_node_rec(void);
extern void copy_node_rec(node_rec_t *node_rec1, node_rec_t *node_rec2);
extern void build_retenet(astnode_t *ast);
extern BOOLEAN use_2_in_node(void);
extern BOOLEAN use_not_2_in_node(void);
extern void ce2retenet(astnode_t *ast, astnode_t *prodrule);

extern void add_nodes_to_retenet(one_node_t *new_nodes_ptr);
extern void delete_not_added_nodes(one_node_t *new_nodes_ptr);
extern void delete_not_added_nodes_help(node_rec_t *node, int count);
extern void reuse_nodes(one_node_t *new_node);
extern void reuse_nodes_help(node_rec_t *node, node_rec_t *new_node,
										int node_count);
extern BOOLEAN same_nodes(node_rec_t *node1, node_rec_t *node2);
extern BOOLEAN match_elclass_number(one_node_t *one_in_node, int pos_number);
extern BOOLEAN match_1_in_node(node_rec_t *node, node_rec_t *new_node);
extern BOOLEAN match_var_1_in_node(node_rec_t *node, node_rec_t *new_node);

extern one_node_t *form2retenet(astnode_t *ast);
extern void attribute2retenet(astnode_t *ast, int elclass_number);
extern void attribute2retenet_help(astnode_t *ast, int elclass_number,
										int pos_number);
extern char *has_var(astnode_t *ast);
