/*-----------------------------------------------------------------
Header file: workmem.h

Header file van de file workmem.c.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


extern workingmem_head_t *create_wm_head(void);
extern workingmem_el_t *create_wm_el(int wm_el_number,int elclass_number,
													ast_head_t *attribute_values,
													workingmem_el_t *next);
extern void add_wm_el(int wm_el_number,int elclass_number,
								ast_head_t *attribute_values,
								workingmem_head_t *list);
extern void remove_wm_el(int wm_el_number);
extern void delete_wm_el(workingmem_el_t *wm_element);

extern lol_wm_ptrs_head_t *create_lol_wm_ptrs_head(void);
extern lol_wm_ptrs_el_t *create_lol_wm_ptrs_el(int length,
														workingmem_ptr_el_t *first,
														workingmem_ptr_el_t *last,
														lol_wm_ptrs_el_t *next);
extern void add_lol_wm_ptrs_el(workingmem_ptr_head_t *sub,
											lol_wm_ptrs_head_t *lol);
extern BOOLEAN compare_wm_ptrs_list(workingmem_ptr_head_t *wm_ptrs_list,
											workingmem_ptr_el_t *wm_ptr2);
extern void delete_lol_wm_ptrs_el(workingmem_ptr_head_t *sub,
												lol_wm_ptrs_head_t *lol);
extern lol_count_wm_ptrs_head_t *create_lol_count_wm_ptrs_head(void);
extern lol_count_wm_ptrs_el_t *create_lol_count_wm_ptrs_el(int length, int count,
														workingmem_ptr_el_t *first,
														workingmem_ptr_el_t *last,
														lol_count_wm_ptrs_el_t *next);
extern void add_lol_count_wm_ptrs_el(workingmem_ptr_head_t *sub, int count,
										lol_count_wm_ptrs_head_t *lol);
extern int delete_lol_count_wm_ptrs_el(workingmem_ptr_head_t *sub,
											lol_count_wm_ptrs_head_t *lol);

extern workingmem_ptr_head_t *copy_wm_ptr_list(
										workingmem_ptr_head_t *retenet_token_wm_list);
extern workingmem_ptr_head_t *copy_mem_wm_ptr_list(lol_wm_ptrs_el_t *mem_wm_list);
extern workingmem_ptr_head_t *copy_mem_count_wm_ptr_list(
													lol_count_wm_ptrs_el_t *mem_wm_list);
extern workingmem_ptr_head_t *create_wm_ptr_head(void);
extern workingmem_ptr_el_t *create_wm_ptr_el(workingmem_el_t *wm_el_ptr,
													workingmem_ptr_el_t *next);
extern void add_wm_ptr_el(workingmem_el_t *wm_el_ptr,
							workingmem_ptr_head_t *list);
extern void insert_wm_ptr_el(workingmem_el_t *wm_el_ptr,
										workingmem_ptr_head_t *list);
extern void insert_wm_ptr_list_help(workingmem_ptr_el_t *wm_ptr_el,
												int length, workingmem_ptr_head_t *list);
extern void insert_wm_ptr_list(workingmem_ptr_head_t *wm_ptr_list,
								workingmem_ptr_head_t *list);
extern void delete_wm_ptr_list(workingmem_ptr_head_t *list);
extern void delete_wm_ptr_el(workingmem_ptr_el_t *el, int length);

extern void set_attribute_values(int pos, astnode_t *ast,
									ast_head_t *attribute_values);
extern ast_head_t *create_attribute_values(int elclass_length);
extern ast_head_t *copy_attribute_values(ast_head_t *attribute_values);

extern workingmem_el_t *find_working_element(int wm_el_number);
