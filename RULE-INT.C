/*-----------------------------------------------------------------
C file: rule-int.c

Deze file bevat procedures voor het bewerkstelligen van de
recognize-act cycle.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "decl.h"
#include "rule-int.h"
#include "workmem.h"
#include "list.h"
#include "execute.h"
#include "error.h"
#include "ast.h"


/* voer de recognize-act cycle uit */
void start_rule_interpreter()
{
	instantiation_t *selected_instantiation;

	selected_instantiation = NULL;
	while (conflict_set->length > 0)
	{
		selected_instantiation = select_instantiation(selected_instantiation);
		if (selected_instantiation != NULL)
			exec_instantiation(selected_instantiation);
	}
	delete_conflict_set(conflict_set);
}

/* voer LEX strategie uit om een instantiatie van de conflict set te
	selecteren */
instantiation_t *select_instantiation(instantiation_t *selected_instantiation)
{
	instantiation_ptr_head_t *selected_instantiations;
	instantiation_t *new_instantiation;
	workingmem_ptr_head_t *wm_ptr_list;

	/* refractie */
	if (selected_instantiation != NULL)
	{
		remove_instantiation(selected_instantiation->prodrule,
									selected_instantiation->workingmem_list);
		delete_instantiation(selected_instantiation, 1);
	}
	if (conflict_set->length > 0)
	{
		if (conflict_set->length == 1)
			selected_instantiation = conflict_set->first;
		else
		{
			selected_instantiations = create_instantiation_ptr_head();
			copy_confl_set_to_instant_ptr_list(selected_instantiations);
			/* recentie */
			selected_instantiations = recency(selected_instantiations,
															wm_el_counter+1);
			/* specificiteit */
			selected_instantiations = specificity(selected_instantiations);
			/* arbitrary choice */
			selected_instantiations = arbitrary_choice(selected_instantiations);
			selected_instantiation =
										selected_instantiations->first->instantiation;
			delete_instantiation_ptr_head(selected_instantiations);
		}
		new_instantiation = selected_instantiation;
		wm_ptr_list = copy_wm_ptr_list(new_instantiation->workingmem_list);
		selected_instantiation = create_instantiation(
			new_instantiation->prodrule, wm_ptr_list, NULL);
	}
	else
		selected_instantiation = NULL;
	return selected_instantiation;
}


/* voer de recentie stap uit van de conflict resolutie */
instantiation_ptr_head_t *recency(
						instantiation_ptr_head_t *selected_instantiations, int max)
{
	register int i;
	instantiation_ptr_el_t *selected_instantiation;
	instantiation_ptr_head_t *new_selected_instantiations;
	int max_wm_el_number_of_inst, max_wm_el_number;

	max_wm_el_number = 0;
	new_selected_instantiations = create_instantiation_ptr_head();
	selected_instantiation = selected_instantiations->first;
	for (i=1;i<=selected_instantiations->length;i++)
	{
		max_wm_el_number_of_inst = get_next_highest_wm_el_number(
							selected_instantiation->instantiation->workingmem_list,
							max);
		if (max_wm_el_number_of_inst > max_wm_el_number)
		{
			max_wm_el_number = max_wm_el_number_of_inst;
			if (new_selected_instantiations->length > 0)
			{
				delete_instantiation_ptr_head(new_selected_instantiations);
				new_selected_instantiations = create_instantiation_ptr_head();
			}
			add_instantiation_ptr_el(selected_instantiation->instantiation,
												new_selected_instantiations);
		}
		else
		{
			if (max_wm_el_number_of_inst == max_wm_el_number)
				add_instantiation_ptr_el(selected_instantiation->instantiation,
													new_selected_instantiations);
		}
		selected_instantiation = selected_instantiation->next;
	}
	if (new_selected_instantiations->length == 0)
	{
		delete_instantiation_ptr_head(new_selected_instantiations);
		new_selected_instantiations = selected_instantiations;
	}
	else
	{
		delete_instantiation_ptr_head(selected_instantiations);
		if (new_selected_instantiations->length > 1)
			new_selected_instantiations = recency(new_selected_instantiations,
																max_wm_el_number);
	}
	return new_selected_instantiations;
}


/* deze procedure retourneert de hoogste time tag van een instantiatie */
int get_next_highest_wm_el_number(workingmem_ptr_head_t *wm_list, int max)
{
	register int i;
	workingmem_ptr_el_t *wm_el;
	int wm_list_max;

	wm_el = wm_list->first;
	wm_list_max = -1;
	for (i=1;i<=wm_list->length;i++)
	{
		if ((wm_el->wm_el_ptr->wm_el_number > wm_list_max) &&
				(wm_el->wm_el_ptr->wm_el_number < max))
			wm_list_max = wm_el->wm_el_ptr->wm_el_number;
		wm_el = wm_el->next;
	}
	return wm_list_max;
}


/* voer de specificiteit stap uit van de conflict resolutie */
instantiation_ptr_head_t *specificity(
									instantiation_ptr_head_t *selected_instantiations)
{
	register int i;
	instantiation_ptr_el_t *selected_instantiation;
	instantiation_ptr_head_t *new_selected_instantiations;
	int max_number_of_tests, number_of_tests;

	if (selected_instantiations->length > 1)
	{
		max_number_of_tests = 0;
		new_selected_instantiations = create_instantiation_ptr_head();
		selected_instantiation = selected_instantiations->first;
		for (i=1;i<=selected_instantiations->length;i++)
		{
			number_of_tests = find_number_of_tests(
					selected_instantiation->instantiation->prodrule->left->token);
			if (number_of_tests > max_number_of_tests)
			{
				max_number_of_tests = number_of_tests;
				if (new_selected_instantiations->length > 0)
				{
					delete_instantiation_ptr_head(new_selected_instantiations);
					new_selected_instantiations = create_instantiation_ptr_head();
				}
				add_instantiation_ptr_el(selected_instantiation->instantiation,
													new_selected_instantiations);
			}
			else if (number_of_tests == max_number_of_tests)
				add_instantiation_ptr_el(selected_instantiation->instantiation,
													new_selected_instantiations);
			selected_instantiation = selected_instantiation->next;
		}
		delete_instantiation_ptr_head(selected_instantiations);
		return new_selected_instantiations;
	}
	else
		return selected_instantiations;
}


/* voer de willekeurige keuze stap uit van de conflict resolutie */
instantiation_ptr_head_t *arbitrary_choice(
									instantiation_ptr_head_t *selected_instantiations)
{
	register int i;
	instantiation_ptr_el_t *selected_instantiation;
	instantiation_ptr_head_t *new_selected_instantiations;
	int random_choice;

	if (selected_instantiations->length > 1)
	{
		new_selected_instantiations = create_instantiation_ptr_head();
		randomize();
		random_choice = random(selected_instantiations->length);
		random_choice++;
		selected_instantiation = selected_instantiations->first;
		for (i=2;i<=random_choice;i++)
			selected_instantiation = selected_instantiation->next;
		add_instantiation_ptr_el(selected_instantiation->instantiation,
													new_selected_instantiations);
		delete_instantiation_ptr_head(selected_instantiations);
		return new_selected_instantiations;
	}
	else
		return selected_instantiations;
}


/* creeer een token (symbool) dat door het rete netwerk gestuurd wordt
	(een rete netwerk token) */
retenet_token_t *create_retenet_token()
{
	retenet_token_t *new;

	if ((new = MALLOC(retenet_token_t)) != NULL)
		new->workingmem_list = NULL;
	else
		print_error(OUT_OF_MEM, "");
	return new;
}


/* verwijder een rete netwerk token */
void delete_retenet_token(retenet_token_t *retenet_token)
{
	free((char *) retenet_token);
}


/* creeer het hoofd van een lijst met instantiaties (de conflict set) */
conflict_set_t *create_conflict_set()
{
	conflict_set_t *new;

	if ((new = MALLOC(conflict_set_t)) != NULL)
	{
		new->length = 0;
		new->first = new->last = NULL;
	}
	else
		print_error(OUT_OF_MEM,"");
	return new;
}


/* creeer een element (een instantiatie) van een lijst met instantiaties
	(de conflict set) */
instantiation_t *create_instantiation(astnode_t *prodrule,
													workingmem_ptr_head_t *workingmem_list,
													instantiation_t *next)
{
	instantiation_t *new;

	if ((new = MALLOC(instantiation_t)) != NULL)
	{
		new->prodrule = prodrule;
		new->workingmem_list = workingmem_list;
		new->next = next;
	}
	else
		print_error(OUT_OF_MEM,"");
	return(new);
}


/* voeg een element (een instantiatie) toe aan een lijst met instantiaties
	(de conflict set) */
void add_instantiation(astnode_t *prodrule,
								workingmem_ptr_head_t *workingmem_list,
								conflict_set_t *conflict_set)
{
	instantiation_t *new;

	new = create_instantiation(prodrule, workingmem_list, NULL);
	if (conflict_set->length)
		conflict_set->last->next = new;
	else
		conflict_set->first = new;
	conflict_set->last = new;
	conflict_set->length++;
}


/* zoek en verwijder een instantiatie uit de conflict set */
void remove_instantiation(astnode_t *prodrule,
									workingmem_ptr_head_t *workingmem_list)
{
	instantiation_t *instantiation, *instantiation_prev;
	register int i;
	BOOLEAN instantiation_found;

	instantiation = conflict_set->first;
	instantiation_prev = NULL;
	i = 1;
	instantiation_found = FALSE;
	while ((!instantiation_found) && (i<=conflict_set->length))
	{
		if (!strcmp(prodrule->left->token,
						instantiation->prodrule->left->token))
		{
			if (compare_wm_ptrs_list(workingmem_list,
												instantiation->workingmem_list->first))
				instantiation_found = TRUE;
			else
			{
				i++;
				instantiation_prev = instantiation;
				instantiation = instantiation->next;
			}
		}
		else
		{
			i++;
			instantiation_prev = instantiation;
			instantiation = instantiation->next;
		}
	}
	if (instantiation_found)
	{
		if (i==1)
			conflict_set->first = instantiation->next;
		else if (i == conflict_set->length)
		{
			conflict_set->last = instantiation_prev;
			conflict_set->last->next = NULL;
		}
		else
			instantiation_prev->next = instantiation->next;
		conflict_set->length--;
		delete_instantiation(instantiation, 1);
	}
}


/* verwijder een aantal instantiaties (bepaald door het argument length)
	uit de conflict set */
void delete_instantiation(instantiation_t *instantiation, int length)
{
	if (length > 1)
		delete_instantiation(instantiation->next, length-1);
	delete_wm_ptr_list(instantiation->workingmem_list);
	free((char *) instantiation);
	instantiation = NULL;
}


/* verwijder de conflict set */
void delete_conflict_set(conflict_set_t *conflict_set)
{
	if (conflict_set->length != 0)
		delete_instantiation(conflict_set->first, conflict_set->length);
	free((char *) conflict_set);
}


/* creeer het hoofd van een lijst met pointers naar instantiaties */
instantiation_ptr_head_t *create_instantiation_ptr_head()
{
	instantiation_ptr_head_t *new;

	if ((new = MALLOC(instantiation_ptr_head_t)) != NULL)
	{
		new->length = 0;
		new->first = new->last = NULL;
	}
	else
		print_error(OUT_OF_MEM, "");
	return new;
}


/* copieer de instantiaties van de conflict set naar een lijst met pointers
	naar instantiaties */
void copy_confl_set_to_instant_ptr_list(
									instantiation_ptr_head_t *selected_instantiations)
{
	register int i;
	instantiation_t *instantiation;

	instantiation = conflict_set->first;
	for (i=1; i<=conflict_set->length;i++)
	{
		add_instantiation_ptr_el(instantiation, selected_instantiations);
		instantiation = instantiation->next;
	}
}


/* creeer een element van een lijst met pointers naar instantiaties */
instantiation_ptr_el_t *create_instantiation_ptr_el(
															instantiation_t *instantiation,
															instantiation_ptr_el_t *next)
{
	instantiation_ptr_el_t *new;

	if ((new = MALLOC(instantiation_ptr_el_t)) != NULL)
	{
		new->instantiation = instantiation;
		new->next = next;
	}
	else
		print_error(OUT_OF_MEM, "");
	return (new);
}


/* voeg een element toe aan een lijst met pointers naar instantiaties */
void add_instantiation_ptr_el(instantiation_t *instantiation,
									instantiation_ptr_head_t *instantiation_ptr_list)
{
	instantiation_ptr_el_t *new;

	new = create_instantiation_ptr_el(instantiation, NULL);
	if (instantiation_ptr_list->length)
		instantiation_ptr_list->last->next = new;
	else
		instantiation_ptr_list->first = new;
	instantiation_ptr_list->last = new;
	instantiation_ptr_list->length++;
}


/* verwijder een lijst met pointers naar instantiaties */
void delete_instantiation_ptr_head(
									instantiation_ptr_head_t *instantiation_ptr_list)
{
	instantiation_ptr_el_t *instantiation, *instantiation_help;
	register int i;

	if (instantiation_ptr_list->length > 0)
	{
		instantiation = instantiation_ptr_list->first;
		for (i=1; i<=instantiation_ptr_list->length; i++)
		{
			instantiation_help = instantiation->next;
			free((char *) instantiation);
			instantiation = instantiation_help;
		}
	}
	free((char *) instantiation_ptr_list);
}


/* maak een rete netwerk token en stuur het door het rete netwerk */
void wm_el_through_retenet(char retenet_token_tag, workingmem_el_t *wm_el)
{
	retenet_token_t *retenet_token;

	retenet_token = create_retenet_token();
	retenet_token->tag = retenet_token_tag;
	retenet_token->workingmem_list = create_wm_ptr_head();
	add_wm_ptr_el(wm_el, retenet_token->workingmem_list);
	match_wm_el(retenet_token);
	delete_wm_ptr_list(retenet_token->workingmem_list);
	delete_retenet_token(retenet_token);
}


/* stuur een rete netwerk token door rete netwerk */
void match_wm_el(retenet_token_t *retenet_token)
{
	root_el_t *retenet_el_help;
	register int root_el_count;

	if (retenet->length > 0)
	{
		root_el_count = 1;
		retenet_el_help = retenet->first;
		while (root_el_count<=retenet->length)
		{
			wm_el_through_retenet_el(retenet_token, retenet_el_help);
			retenet_el_help = retenet_el_help->next;
			root_el_count++;
		}
	}
}


/* stuur een rete netwerk token door een element van het rete netwerk */
void wm_el_through_retenet_el(retenet_token_t *retenet_token,
											root_el_t *retenet_el)
{
	workingmem_el_t *wm_el_temp;
	register int no_next_nodes, i;
	node_type_el_t *next_node;

	wm_el_temp = retenet_token->workingmem_list->first->wm_el_ptr;
	if (test_elclass_number(wm_el_temp, retenet_el->element_class))
	{
		no_next_nodes = retenet_el->element_class->next_nodes->length;
		if (no_next_nodes > 0)
		{
			next_node = retenet_el->element_class->next_nodes->first;
			test_node_constraints(retenet_token, next_node->direction,
											next_node->next_node);
			for (i=2;i<=no_next_nodes;i++)
			{
				next_node = next_node->next;
				test_node_constraints(retenet_token, next_node->direction,
												next_node->next_node);
			}
		}
	}
}


/* bekijk of het rete netwerk token aan de tests van de node voldoen */
void test_node_constraints(retenet_token_t *retenet_token, char direction,
									node_rec_t *node)
{
	node_type_el_t *next_node;
	workingmem_el_t *wm_el;
	left_mem_t *left_mem;
	not_left_mem_t *not_left_mem;
	right_mem_t *right_mem;
	one_node_t *one_in_node;
	var_one_node_t *var_one_in_node;
	two_node_t *two_in_node;
	not_two_node_t *not_two_in_node;
	p_node_t *p_node;
	register int no_next_nodes, i, j, k, count;
	lol_wm_ptrs_head_t *left_mem_wm_lol;
	lol_wm_ptrs_el_t *left_mem_wm_list;
	lol_count_wm_ptrs_head_t *not_left_mem_wm_lol;
	lol_count_wm_ptrs_el_t *not_left_mem_wm_list;
	lol_wm_ptrs_head_t *right_mem_wm_lol;
	lol_wm_ptrs_el_t *right_mem_wm_list;
	workingmem_ptr_head_t *wm_list_copy;
	retenet_token_t *retenet_token_help;

	wm_el = retenet_token->workingmem_list->first->wm_el_ptr;
	switch(node->type)
	{
		case ONE_INPUT:
			one_in_node = node->one_node;
			if (test_1_in_node(wm_el, one_in_node))
			{
				no_next_nodes = one_in_node->next_nodes->length;
				if (no_next_nodes > 0)
				{
					next_node = one_in_node->next_nodes->first;
					test_node_constraints(retenet_token, next_node->direction,
													next_node->next_node);
					for (i=2;i<=no_next_nodes;i++)
					{
						next_node = next_node->next;
						test_node_constraints(retenet_token, next_node->direction,
														next_node->next_node);
					}
				}
			}
			break;
		case VAR_ONE_INPUT:
			var_one_in_node = node->var_one_node;
			if (test_var_1_in_node(wm_el, var_one_in_node))
			{
				no_next_nodes = var_one_in_node->next_nodes->length;
				if (no_next_nodes > 0)
				{
					next_node = var_one_in_node->next_nodes->first;
					test_node_constraints(retenet_token, next_node->direction,
													next_node->next_node);
					for (i=2;i<=no_next_nodes;i++)
					{
						next_node = next_node->next;
						test_node_constraints(retenet_token, next_node->direction,
														next_node->next_node);
					}
				}
			}
			break;
		case TWO_INPUT:
			two_in_node = node->two_node;
			left_mem = two_in_node->left_mem;
			right_mem = two_in_node->right_mem;
			if (direction == RIGHT)
			{  // rechts
				if (retenet_token->tag == PLUS)
				{
					wm_list_copy = copy_wm_ptr_list(retenet_token->workingmem_list);
					add_lol_wm_ptrs_el(wm_list_copy, right_mem->retenet_tokens);
				}
				else
					delete_lol_wm_ptrs_el(retenet_token->workingmem_list,
														 right_mem->retenet_tokens);
				left_mem_wm_lol = left_mem->retenet_tokens;
				left_mem_wm_list = left_mem->retenet_tokens->first;
				for (i=1; i<=left_mem_wm_lol->length; i++)
				{
					if (test_2_in_node(two_in_node->inter_restrictions,
												left_mem_wm_list->first,
												retenet_token->workingmem_list->first))
					{
						no_next_nodes = two_in_node->next_nodes->length;
						if (no_next_nodes > 0)
						{
							retenet_token_help = create_retenet_token();
							retenet_token_help->tag = retenet_token->tag;
							retenet_token_help->workingmem_list =
								copy_wm_ptr_list(retenet_token->workingmem_list);
							wm_list_copy = copy_mem_wm_ptr_list(left_mem_wm_list);
							insert_wm_ptr_list(wm_list_copy,
													retenet_token_help->workingmem_list);
							next_node = two_in_node->next_nodes->first;
							test_node_constraints(retenet_token_help,
															next_node->direction,
															next_node->next_node);
							for (j=2;j<=no_next_nodes;j++)
							{
								next_node = next_node->next;
								test_node_constraints(retenet_token_help,
																next_node->direction,
																next_node->next_node);
							}
							delete_wm_ptr_list(retenet_token_help->workingmem_list);
							delete_retenet_token(retenet_token_help);
						}
					}
					left_mem_wm_list = left_mem_wm_list->next;
				}
			}
			else
			{  // links
				if (retenet_token->tag == PLUS)
				{
					wm_list_copy = copy_wm_ptr_list(retenet_token->workingmem_list);
					add_lol_wm_ptrs_el(wm_list_copy, left_mem->retenet_tokens);
				}
				else
					delete_lol_wm_ptrs_el(retenet_token->workingmem_list,
													left_mem->retenet_tokens);
				right_mem_wm_lol = right_mem->retenet_tokens;
				right_mem_wm_list = right_mem->retenet_tokens->first;
				for (i=1; i<=right_mem_wm_lol->length; i++)
				{
					if (test_2_in_node(two_in_node->inter_restrictions,
												retenet_token->workingmem_list->first,
												right_mem_wm_list->first))
					{
						no_next_nodes = two_in_node->next_nodes->length;
						if (no_next_nodes > 0)
						{
							retenet_token_help = create_retenet_token();
							retenet_token_help->tag = retenet_token->tag;
							retenet_token_help->workingmem_list =
								copy_wm_ptr_list(retenet_token->workingmem_list);
							wm_list_copy = copy_mem_wm_ptr_list(right_mem_wm_list);
							add_wm_ptr_el(wm_list_copy->first->wm_el_ptr,
												retenet_token_help->workingmem_list);
							next_node = two_in_node->next_nodes->first;
							test_node_constraints(retenet_token_help,
															next_node->direction,
															next_node->next_node);
							for (j=2;j<=no_next_nodes;j++)
							{
								next_node = next_node->next;
								test_node_constraints(retenet_token_help,
																next_node->direction,
																next_node->next_node);
							}
							delete_wm_ptr_list(retenet_token_help->workingmem_list);
							delete_retenet_token(retenet_token_help);
						}
					}
					right_mem_wm_list = right_mem_wm_list->next;
				}
			}
			break;
		case NOT_TWO_INPUT:
			not_two_in_node = node->not_two_node;
			not_left_mem = not_two_in_node->left_mem;
			right_mem = not_two_in_node->right_mem;
			if (direction == RIGHT)
			{  // rechts
				if (retenet_token->tag == PLUS)
				{
					wm_list_copy = copy_wm_ptr_list(retenet_token->workingmem_list);
					add_lol_wm_ptrs_el(wm_list_copy, right_mem->retenet_tokens);
				}
				else
					delete_lol_wm_ptrs_el(retenet_token->workingmem_list,
														 right_mem->retenet_tokens);
				not_left_mem_wm_lol = not_left_mem->retenet_tokens;
				not_left_mem_wm_list = not_left_mem->retenet_tokens->first;
				for (i=1; i<=not_left_mem_wm_lol->length; i++)
				{
					if (test_2_in_node(not_two_in_node->inter_restrictions,
												not_left_mem_wm_list->first,
												retenet_token->workingmem_list->first))
					{
						if (retenet_token->tag == PLUS)
							not_left_mem_wm_list->count++;
						else
							if (not_left_mem_wm_list->count > 0)
								not_left_mem_wm_list->count--;
						if (((not_left_mem_wm_list->count == 1) &&
								(retenet_token->tag == PLUS)) 			||
							 ((not_left_mem_wm_list->count == 0) &&
								(retenet_token->tag == MIN)))
						{
							retenet_token_help = create_retenet_token();
							retenet_token_help->workingmem_list =
								copy_mem_count_wm_ptr_list(not_left_mem_wm_list);
							if ((not_left_mem_wm_list->count == 1) &&
								(retenet_token->tag == PLUS))
								retenet_token_help->tag = MIN;
							else
								retenet_token_help->tag = PLUS;
							no_next_nodes = not_two_in_node->next_nodes->length;
							if (no_next_nodes > 0)
							{
								next_node = not_two_in_node->next_nodes->first;
								test_node_constraints(retenet_token_help,
																next_node->direction,
																next_node->next_node);
								for (j=2;j<=no_next_nodes;j++)
								{
									next_node = next_node->next;
									test_node_constraints(retenet_token_help,
																	next_node->direction,
																	next_node->next_node);
								}
							}
							delete_wm_ptr_list(retenet_token_help->workingmem_list);
							delete_retenet_token(retenet_token_help);
						}
					}
					not_left_mem_wm_list = not_left_mem_wm_list->next;
				}
			}
			else
			{  // links
				count = 0;
				if (retenet_token->tag == PLUS)
				{
					right_mem_wm_lol = right_mem->retenet_tokens;
					right_mem_wm_list = right_mem->retenet_tokens->first;
					for (i=1; i<=right_mem_wm_lol->length; i++)
					{
						if (test_2_in_node(not_two_in_node->inter_restrictions,
													retenet_token->workingmem_list->first,
													right_mem_wm_list->first))
							count++;
						right_mem_wm_list = right_mem_wm_list->next;
					}
					wm_list_copy = copy_wm_ptr_list(retenet_token->workingmem_list);
					add_lol_count_wm_ptrs_el(wm_list_copy, count,
													not_left_mem->retenet_tokens);
				}
				else
					count = delete_lol_count_wm_ptrs_el(
															retenet_token->workingmem_list,
															not_left_mem->retenet_tokens);
				if (count == 0)
				{
					no_next_nodes = not_two_in_node->next_nodes->length;
					if (no_next_nodes > 0)
					{
						next_node = not_two_in_node->next_nodes->first;
						test_node_constraints(retenet_token, next_node->direction,
														next_node->next_node);
						for (j=2;j<=no_next_nodes;j++)
						{
							next_node = next_node->next;
							test_node_constraints(retenet_token,
															next_node->direction,
															next_node->next_node);
						}
					}
				}
			}
			break;
		case PROD:
			p_node = node->p_node;
			if (retenet_token->tag == PLUS)
			{
				wm_list_copy = copy_wm_ptr_list(retenet_token->workingmem_list);
				add_instantiation(p_node->prodrule, wm_list_copy, conflict_set);
			}
			else
			{
				remove_instantiation(p_node->prodrule,
											retenet_token->workingmem_list);
			}
			break;
	}
}


/* deze procedure test of de klasse naam van een working memory element
	identiek is aan de klasse naam van een node met 1 ingang
	(intra-conditie-test) */
BOOLEAN test_elclass_number(workingmem_el_t *wm_el, one_node_t *one_node)
{
	return (wm_el->elclass_number == one_node->position);
}


/* deze procedure test of een working memory element voldoet aan de
	beperkingen van een node met 1 ingang (intra-conditie-test) */
BOOLEAN test_1_in_node(workingmem_el_t *wm_el, one_node_t *one_node)
{
	ast_el_t *ast_el;
	register int i;

	if (wm_el->attribute_values->length > 0)
	{
		ast_el = wm_el->attribute_values->first;
		for (i=2; i<=(one_node->position-1); i++)
			ast_el = ast_el->next;
		return satisfy_restriction(ast_el->ast, EQ_TK, one_node->restriction);
	}
	else
		return FALSE;
}


/* deze procedure test of een working memory element voldoet aan de
	beperkingen van een node met 1 ingang: zijn twee dezelfde variabelen in
	een conditie element consistent gebonden? (intra-conditie-test) */
BOOLEAN test_var_1_in_node(workingmem_el_t *wm_el,
									var_one_node_t *var_one_node)
{
	ast_el_t *ast_el1, *ast_el2;
	register int i;

	if (wm_el->attribute_values->length > 0)
	{
		ast_el1 = wm_el->attribute_values->first;
		for (i=2; i<=(var_one_node->position-1); i++)
			ast_el1 = ast_el1->next;
		ast_el2 = wm_el->attribute_values->first;
		for (i=2; i<=(var_one_node->position_prev_var-1); i++)
			ast_el2 = ast_el2->next;
		return satisfy_restriction(ast_el1->ast,
											var_one_node->predicate, ast_el2->ast);
	}
	else
		return FALSE;
}


/* deze procedure test of er aan de beperkingen van een node met twee
	ingangen wordt voldaan (inter-conditie-tests) */
BOOLEAN test_2_in_node(inter_restriction_head_t *inter_restrictions,
								workingmem_ptr_el_t *left_wm_ptr,
								workingmem_ptr_el_t *right_wm_ptr)
{
	inter_restriction_el_t *inter_restriction;
	workingmem_ptr_el_t *left_wm_ptr_temp;
	workingmem_ptr_el_t *right_wm_ptr_temp;
	ast_el_t *left_attribute_ast;
	ast_el_t *right_attribute_ast;
	register int i, j;
	BOOLEAN passed_test;

	passed_test = TRUE;
	inter_restriction = inter_restrictions->first;
	i = 1;
	while (passed_test && (i<=inter_restrictions->length))
	{
		left_wm_ptr_temp = left_wm_ptr;
		for (j=2; j<=inter_restriction->left_condel_no;j++)
			left_wm_ptr_temp = left_wm_ptr_temp->next;
		left_attribute_ast =
			left_wm_ptr_temp->wm_el_ptr->attribute_values->first;
		for (j=2; j<=inter_restriction->left_pos;j++)
			left_attribute_ast = left_attribute_ast->next;
		right_attribute_ast =
			right_wm_ptr->wm_el_ptr->attribute_values->first;
		for (j=2; j<=inter_restriction->right_pos;j++)
			right_attribute_ast = right_attribute_ast->next;
		switch(left_attribute_ast->ast->asttag)
		{
			case CSA:
				if ((right_attribute_ast->ast->asttag == CSA) ||
						(right_attribute_ast->ast->asttag == ANYATOM))
				{
					if (!evaluate_string(left_attribute_ast->ast->token,
												inter_restriction->predicate,
												right_attribute_ast->ast->token))
						passed_test = FALSE;
				}
				else
					passed_test = FALSE;
				break;
			case ANYATOM:
				if ((right_attribute_ast->ast->asttag == ANYATOM) ||
						(right_attribute_ast->ast->asttag == CSA))
				{
					if (!evaluate_string(left_attribute_ast->ast->token,
												inter_restriction->predicate,
												right_attribute_ast->ast->token))
						passed_test = FALSE;
				}
				else
					passed_test = FALSE;
				break;
			case NUMBER:
				if (right_attribute_ast->ast->asttag == NUMBER)
				{
					if (!evaluate_number(right_attribute_ast->ast->number,
												inter_restriction->predicate,
												left_attribute_ast->ast->number))
						passed_test = FALSE;
				}
				else
					passed_test = FALSE;
				break;
		}
		inter_restriction = inter_restriction->next;
		i++;
	}
	return passed_test;
}


/* deze procedure test of een AST aan een beperking voldoet*/
BOOLEAN satisfy_restriction(astnode_t *ast, char predicate,
										astnode_t *restriction)
{
	if (restriction == NULL)
	{
		if (ast == NULL)
			return TRUE;
		else
			return FALSE;
	}
	else if (ast == NULL)
	{
		if (restriction == NULL)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		switch(restriction->asttag)
		{
			case CSA:
				if ((ast->asttag == CSA) || (ast->asttag == ANYATOM))
					return evaluate_string(ast->token, predicate, restriction->token);
				else
					return FALSE;
			case NUMBER:
				if (ast->asttag == NUMBER)
					return evaluate_number(ast->number, predicate, restriction->number);
				else
					return FALSE;
			case ANYATOM:
				if ((ast->asttag == CSA) || (ast->asttag == ANYATOM))
					return evaluate_string(ast->token, predicate, restriction->token);
				else
					return FALSE;
			case VOC:
				return satisfy_restriction(ast, predicate, restriction->left);
			case DISJ:
				return satisfy_restriction(ast, predicate, restriction->left);
			case DISJ_DISJ:
				if (satisfy_restriction(ast, predicate, restriction->left))
					return TRUE;
				else
					return satisfy_restriction(ast, predicate,
														restriction->middle);
			case PRED_ATOMICVALUE:
				return satisfy_restriction(ast, restriction->left->token[0],
													restriction->middle);
			case ATOMICVALUE:
				return satisfy_restriction(ast, predicate, restriction->left);
			case LV_LV:
				if (satisfy_restriction(ast, predicate, restriction->left))
					return satisfy_restriction(ast, predicate,
														restriction->middle);
				else
					return FALSE;
			default:
				return FALSE;
		}
	}
}


/* evalueer twee strings */
BOOLEAN evaluate_string(char *string1, char predicate, char *string2)
{
	switch(predicate)
	{
		case EQ_TK:
			if (!strcmp(string1, string2))
				return TRUE;
			else
				return FALSE;
		case NE_TK:
			if (!strcmp(string1, string2))
				return FALSE;
			else
				return TRUE;
		default:
			switch(predicate)
			{
				case LT_TK:
					print_run_time_error(ILLEGAL_OPERATOR, "<");
				case LE_TK:
					print_run_time_error(ILLEGAL_OPERATOR, "<=");
				case GT_TK:
					print_run_time_error(ILLEGAL_OPERATOR, ">");
				case GE_TK:
					print_run_time_error(ILLEGAL_OPERATOR, ">=");
			}
			return FALSE;
	}
}


/* evalueer twee (floating point) getallen */
BOOLEAN evaluate_number(float number1, char predicate, float number2)
{
	switch(predicate)
	{
		case EQ_TK:
			return (number1 == number2);
		case NE_TK:
			return (number1 != number2);
		case LT_TK:
			return (number1 < number2);
		case LE_TK:
			return (number1 <= number2);
		case GT_TK:
			return (number1 > number2);
		case GE_TK:
			return (number1 >= number2);
		default:
			return FALSE;
	}
}