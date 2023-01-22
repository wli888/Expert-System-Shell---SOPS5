/*-----------------------------------------------------------------
C file: workmem.c

Deze file bevat procedures voor bewerkingen van het working memory.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


#include <malloc.h>
#include <string.h>
#include <stdio.h>

#include "decl.h"
#include "workmem.h"
#include "ast.h"
#include "list.h"
#include "error.h"
#include "rule-int.h"


/* creeer het hoofd van een lijst met working memory elementen, de working
	memory */
workingmem_head_t *create_wm_head()
{
	workingmem_head_t *new;

	if ((new = MALLOC(workingmem_head_t)) != NULL)
	{
		new->length = 0;
		new->first = new->last = NULL;
	}
	else
		print_error(OUT_OF_MEM, "");
	return(new);
}


/* creeer een working memory element */
workingmem_el_t *create_wm_el(int wm_el_number, int elclass_number,
										ast_head_t *attribute_values,
										workingmem_el_t *next)
{
	workingmem_el_t *new;

	if ((new = MALLOC(workingmem_el_t)) != NULL)
	{
		new->wm_el_number = wm_el_number;
		new->elclass_number = elclass_number;
		new->attribute_values = attribute_values;
		new->next = next;
	}
	else
		print_error(OUT_OF_MEM, "");
	return(new);
}


/* voeg een working memory element toe aan de working memory lijst */
void add_wm_el(int wm_el_number, int elclass_number,
					ast_head_t *attribute_values, workingmem_head_t *list)
{
	workingmem_el_t *new;

	new = create_wm_el(wm_el_number, elclass_number, attribute_values, NULL);
	if (list->length)
		list->last->next = new;
	else
		list->first = new;
	list->last = new;
	list->length++;
}


/* zoek en verwijder een working memory element van de working memory
	lijst */
void remove_wm_el(int wm_el_number)
{
	workingmem_el_t *wm_element, *wm_element_prev;
	register int i;
	BOOLEAN wm_el_found;

	wm_element = working_mem->first;
	wm_element_prev = NULL;
	i = 1;
	wm_el_found = FALSE;
	while ((!wm_el_found) && (i<=working_mem->length))
	{
		if (wm_element->wm_el_number == wm_el_number)
			wm_el_found = TRUE;
		else
		{
			i++;
			wm_element_prev = wm_element;
			wm_element = wm_element->next;
		}
	}
	if (wm_el_found)
	{
		if (i==1)
			working_mem->first = wm_element->next;
		else if (i == working_mem->length)
		{
			working_mem->last = wm_element_prev;
			working_mem->last->next = NULL;
		}
		else
			wm_element_prev->next = wm_element->next;
		working_mem->length--;
		delete_wm_el(wm_element);
	}
}


/* verwijder een working memory element van de working memory lijst */
void delete_wm_el(workingmem_el_t *wm_element)
{
	delete_ast_list(wm_element->attribute_values);
	free((char *) wm_element);
}


/* creeer het hoofd van een lijst van lijsten met pointers naar working
	memory elementen */
lol_wm_ptrs_head_t *create_lol_wm_ptrs_head()
{
	lol_wm_ptrs_head_t *new;

	if ((new = MALLOC(lol_wm_ptrs_head_t)) != NULL)
	{
		new->length = 0;
		new->first = new->last = NULL;
	}
	else
		print_error(OUT_OF_MEM, "");
	return(new);
}


/* creeer een element van een lijst van lijsten met pointers naar working
	memory elementen */
lol_wm_ptrs_el_t *create_lol_wm_ptrs_el(int length,
														workingmem_ptr_el_t *first,
														workingmem_ptr_el_t *last,
														lol_wm_ptrs_el_t *next)
{
	lol_wm_ptrs_el_t *new;

	if ((new = MALLOC(lol_wm_ptrs_el_t)) != NULL)
	{
		new->length = length;
		new->first = first;
		new->last = last;
		new->next = next;
	}
	else
		print_error(OUT_OF_MEM, "");
	return new;
}


/* voeg een element toe aan een lijst van lijsten met pointers naar working
	memory elementen */
void add_lol_wm_ptrs_el(workingmem_ptr_head_t *sub, lol_wm_ptrs_head_t *lol)
{
	lol_wm_ptrs_el_t *new;

	new = create_lol_wm_ptrs_el(sub->length, sub->first, sub->last, NULL);
	if (lol->length)
		lol->last->next = new;
	else
		lol->first = new;
	lol->last = new;
	lol->length++;
}


/* vergelijk twee lijsten met pointers naar working memory elementen */
BOOLEAN compare_wm_ptrs_list(workingmem_ptr_head_t *wm_ptrs_list,
											workingmem_ptr_el_t *wm_ptr2)
{
	BOOLEAN match;
	register int i;
	workingmem_ptr_el_t *wm_ptr1;

	match = TRUE;
	i = 1;
	wm_ptr1 = wm_ptrs_list->first;
	while (match && (i<=wm_ptrs_list->length))
	{
		if (wm_ptr1->wm_el_ptr->wm_el_number !=
				wm_ptr2->wm_el_ptr->wm_el_number)
			match = FALSE;
		else
		{
			i++;
			wm_ptr1 = wm_ptr1->next;
			wm_ptr2 = wm_ptr2->next;
		}
	}
	return match;
}


/* zoek en verwijder een element van een lijst van lijsten met pointers
	naar working memory elementen */
void delete_lol_wm_ptrs_el(workingmem_ptr_head_t *sub,
									 lol_wm_ptrs_head_t *lol)
{
	lol_wm_ptrs_el_t *lol_el, *lol_el_prev;
	register int i;
	BOOLEAN lol_el_found;

	lol_el = lol->first;
	lol_el_prev = NULL;
	i = 1;
	lol_el_found = FALSE;
	while (!lol_el_found && (i<=lol->length))
	{
		if (compare_wm_ptrs_list(sub, lol_el->first))
			lol_el_found = TRUE;
		else
		{
			i++;
			lol_el_prev = lol_el;
			lol_el = lol_el->next;
		}
	}
	if (lol_el_found)
	{
		if (i==1)
			lol->first = lol_el->next;
		else if (i == lol->length)
		{
			lol->last = lol_el_prev;
			lol->last->next = NULL;
		}
		else
			lol_el_prev->next = lol_el->next;
		lol->length--;
		delete_wm_ptr_el(lol_el->first, lol_el->length);
		free(lol_el);
	}
}


/* Creeer het hoofd van een lijst van lijsten met pointers naar working
	memory elementen. Bij elke lijst met pointers naar working memory
	elementen hoort een teller */
lol_count_wm_ptrs_head_t *create_lol_count_wm_ptrs_head()
{
	lol_count_wm_ptrs_head_t *new;

	if ((new = MALLOC(lol_count_wm_ptrs_head_t)) != NULL)
	{
		new->length = 0;
		new->first = new->last = NULL;
	}
	else
		print_error(OUT_OF_MEM, "");
	return(new);
}


/* Creeer een element van een lijst van lijsten met pointers naar working
	memory elementen. Bij elke lijst met pointers naar working memory
	elementen hoort een teller */
lol_count_wm_ptrs_el_t *create_lol_count_wm_ptrs_el(int length, int count,
														workingmem_ptr_el_t *first,
														workingmem_ptr_el_t *last,
														lol_count_wm_ptrs_el_t *next)
{
	lol_count_wm_ptrs_el_t *new;

	if ((new = MALLOC(lol_count_wm_ptrs_el_t)) != NULL)
	{
		new->length = length;
		new->count = count;
		new->first = first;
		new->last = last;
		new->next = next;
	}
	else
		print_error(OUT_OF_MEM, "");
	return new;
}


/* voeg een element toe aan een lijst van lijsten met pointers naar working
	memory elementen. Bij elke lijst met pointers naar working memory
	elementen hoort een teller */
void add_lol_count_wm_ptrs_el(workingmem_ptr_head_t *sub, int count,
										lol_count_wm_ptrs_head_t *lol)
{
	lol_count_wm_ptrs_el_t *new;

	new = create_lol_count_wm_ptrs_el(sub->length, count, sub->first,
													sub->last, NULL);
	if (lol->length)
		lol->last->next = new;
	else
		lol->first = new;
	lol->last = new;
	lol->length++;
}


/* zoek en verwijder een element van een lijst van lijsten met pointers
	naar working memory elementen. Bij elke lijst met pointers naar working
	memory elementen hoort een teller */
int delete_lol_count_wm_ptrs_el(workingmem_ptr_head_t *sub,
											lol_count_wm_ptrs_head_t *lol)
{
	lol_count_wm_ptrs_el_t *lol_el, *lol_el_prev;
	register int i;
	BOOLEAN lol_el_found;
	int count;

	lol_el = lol->first;
	lol_el_prev = NULL;
	i = 1;
	lol_el_found = FALSE;
	while (!lol_el_found && (i<=lol->length))
	{
		if (compare_wm_ptrs_list(sub, lol_el->first))
			lol_el_found = TRUE;
		else
		{
			i++;
			lol_el_prev = lol_el;
			lol_el = lol_el->next;
		}
	}
	if (lol_el_found)
	{
		count = lol_el->count;
		if (i==1)
			lol->first = lol_el->next;
		else if (i == lol->length)
		{
			lol->last = lol_el_prev;
			lol->last->next = NULL;
		}
		else
			lol_el_prev->next = lol_el->next;
		lol->length--;
		delete_wm_ptr_el(lol_el->first, lol_el->length);
		free(lol_el);
	}
	else
		count = -1;
	return count;
}


/* copieer een lijst met pointers naar working memory elementen */
workingmem_ptr_head_t *copy_wm_ptr_list(
										workingmem_ptr_head_t *retenet_token_wm_list)
{
	register int i;
	workingmem_ptr_el_t *retenet_token_wm_el;
	workingmem_ptr_head_t *wm_list_copy;

	wm_list_copy = create_wm_ptr_head();
	retenet_token_wm_el = retenet_token_wm_list->first;
	for (i=1; i<=retenet_token_wm_list->length; i++)
	{
		add_wm_ptr_el(retenet_token_wm_el->wm_el_ptr, wm_list_copy);
		retenet_token_wm_el = retenet_token_wm_el->next;
	}
	return wm_list_copy;
}


/* copieer een element van een lijst van lijsten met pointers naar
	working memory elementen */
workingmem_ptr_head_t *copy_mem_wm_ptr_list(lol_wm_ptrs_el_t *mem_wm_list)
{
	register int i;
	workingmem_ptr_el_t *mem_wm_el;
	workingmem_ptr_head_t *mem_wm_list_copy;

	mem_wm_list_copy = create_wm_ptr_head();
	mem_wm_el = mem_wm_list->first;
	for (i=1; i<=mem_wm_list->length; i++)
	{
		add_wm_ptr_el(mem_wm_el->wm_el_ptr,	mem_wm_list_copy);
		mem_wm_el = mem_wm_el->next;
	}
	return mem_wm_list_copy;
}


/* Copieer een element van een lijst van lijsten met pointers naar working
	memory elementen. Bij elke lijst met pointers naar working memory
	elementen hoort een teller */
workingmem_ptr_head_t *copy_mem_count_wm_ptr_list(
													lol_count_wm_ptrs_el_t *mem_wm_list)
{
	register int i;
	workingmem_ptr_el_t *mem_wm_el;
	workingmem_ptr_head_t *mem_wm_list_copy;

	mem_wm_list_copy = create_wm_ptr_head();
	mem_wm_el = mem_wm_list->first;
	for (i=1; i<=mem_wm_list->length; i++)
	{
		add_wm_ptr_el(mem_wm_el->wm_el_ptr, mem_wm_list_copy);
		mem_wm_el = mem_wm_el->next;
	}
	return mem_wm_list_copy;
}


/* creeer het hoofd van een lijst met pointers naar working memory
	elementen */
workingmem_ptr_head_t *create_wm_ptr_head()
{
	workingmem_ptr_head_t *new;

	if ((new = MALLOC(workingmem_ptr_head_t)) != NULL)
	{
		new->length = 0;
		new->first = new->last = NULL;
	}
	else
		print_error(OUT_OF_MEM, "");
	return(new);
}


/* creeer een pointer naar een working memory element */
workingmem_ptr_el_t *create_wm_ptr_el(workingmem_el_t *wm_el_ptr,
													workingmem_ptr_el_t *next)
{
	workingmem_ptr_el_t *new;

	if ((new = MALLOC(workingmem_ptr_el_t)) != NULL)
	{
		new->wm_el_ptr = wm_el_ptr;
		new->next = next;
	}
	else
		print_error(OUT_OF_MEM, "");
	return(new);
}


/* voeg een element toe aan een lijst met pointers naar working memory
	elementen */
void add_wm_ptr_el(workingmem_el_t *wm_el_ptr, workingmem_ptr_head_t *list)
{
	workingmem_ptr_el_t *new;

	new = create_wm_ptr_el(wm_el_ptr, NULL);
	if (list->length)
		list->last->next = new;
	else
		list->first = new;
	list->last = new;
	list->length++;
}


/* voeg een element toe aan het begin van een lijst met pointers naar
	working memory elementen */
void insert_wm_ptr_el(workingmem_el_t *wm_el_ptr,
							workingmem_ptr_head_t *list)
{
	workingmem_ptr_el_t *new;

	new = create_wm_ptr_el(wm_el_ptr, list->first);
	list->first = new;
	if (list->length == 0)
		list->last = new;
	list->length++;
}


/* voeg een aantal elementen (bepaald door het argument length) van een
	lijst met pointers naar working memory elementen toe aan het begin van
	een lijst met pointers naar working memory elementen. */
void insert_wm_ptr_list_help(workingmem_ptr_el_t *wm_ptr_el, int length,
								workingmem_ptr_head_t *list)
{
	if (length > 1)
		insert_wm_ptr_list_help(wm_ptr_el->next, length-1, list);
	insert_wm_ptr_el(wm_ptr_el->wm_el_ptr, list);
}


/* voeg een lijst met pointers naar working memory elementen toe aan het
	begin van een lijst met pointers naar working memory elementen. */
void insert_wm_ptr_list(workingmem_ptr_head_t *wm_ptr_list,
								workingmem_ptr_head_t *list)
{
	if (wm_ptr_list->length > 0)
		insert_wm_ptr_list_help(wm_ptr_list->first, wm_ptr_list->length, list);
}


/* verwijder een lijst met pointers naar working memory elementen */
void delete_wm_ptr_list(workingmem_ptr_head_t *list)
{
	if (list->length > 0)
		delete_wm_ptr_el(list->first, list->length);
	list->length = 0;
	list->first = list->last = NULL;
	free((char *)list);
}


/* verwijder een aantal elementen (bepaald door het argument length) van een
	lijst met pointers naar working memory elementen */
void delete_wm_ptr_el(workingmem_ptr_el_t *el, int length)
{
	if (length > 1)
		delete_wm_ptr_el(el->next, length-1);
	free((char *)el);
}


/* geef een attribuut een waarde */
void set_attribute_values(int pos, astnode_t *ast,
									ast_head_t *attribute_values)
{
	ast_el_t *wm_el_help;
	register int i;

	wm_el_help = attribute_values->first;
	for (i=2; i<= pos; i++)
		wm_el_help = wm_el_help->next;
	if ((wm_el_help->ast != nilast) && (wm_el_help->ast->delete == DELETE))
		delete_ast(wm_el_help->ast);
	wm_el_help->ast = ast;
}


/* creeer attributen voor een nieuw working memory element en initialiseer
	ze met "nil" */
ast_head_t *create_attribute_values(int elclass_length)
{
	ast_head_t *attribute_values;
	register int i;

	attribute_values = create_ast_list();
	for (i=2; i<=elclass_length; i++)
		append_ast_list(nilast, attribute_values);
	return attribute_values;
}


/* copieer de waarde van attributen van een working memory element */
ast_head_t *copy_attribute_values(ast_head_t *attribute_values)
{
	ast_head_t *attribute_values_copy;
	ast_el_t *attribute_value;
	register int i;

	attribute_values_copy = create_ast_list();
	attribute_value = attribute_values->first;
	for (i=1; i<=attribute_values->length; i++)
	{
		append_ast_list(attribute_value->ast, attribute_values_copy);
		attribute_value = attribute_value->next;
	}
	return attribute_values_copy;
}


/* zoek een working memory elememt */
workingmem_el_t *find_working_element(int wm_el_number)
{
	register int i;
	workingmem_el_t *wm_el;

	wm_el = working_mem->first;
	for (i=1;i<=working_mem->length;i++)
	{
		if (wm_el->wm_el_number == wm_el_number)
			return wm_el;
		else
			wm_el = wm_el->next;
	}
	return NULL;
}
