/*-----------------------------------------------------------------
C file: list.c

Deze file bevat procedures voor bewerkingen van lijsten.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


#include <malloc.h>
#include <string.h>

#include "decl.h"
#include "list.h"
#include "error.h"



/* creeer het hoofd van een lijst van lijsten van strings */
main_t *create_lol()
{
	main_t *new;
	if ((new = MALLOC(main_t)) != NULL)
	{
		new->length = 0;
		new->first = new->last = NULL;
	}
	return(new);
}


/* creeer een element van een lijst van lijsten van strings */
link_t *create_link(int length, s_element_t *first, s_element_t *last,
							link_t *next)
{
	link_t *new;
	if ((new = MALLOC(link_t)) != NULL)
	{
		new->length = length;
		new->first = first;
		new->last = last;
		new->next = next;
	}
	return new;
}


/* voeg een lijst van strings toe aan een lijst van lijsten van strings */
BOOLEAN add_sublist(s_head_t *sub, main_t *lol)
{
	link_t *new;
	if ((new = create_link(sub->length, sub->first, sub->last, NULL))
		 != NULL)
	{
		if (lol->length)
			lol->last->next = new;
		else
			lol->first = new;
		lol->last = new;
		lol->length++;
		return TRUE;
	}
	else
		return FALSE;
}


/* creeer het hoofd van een lijst van strings */
s_head_t *create_slist()
{
	s_head_t *new;

	if ((new = MALLOC(s_head_t)) != NULL)
	{
		new->length = 0;
		new->first = new->last = NULL;
	}
	return(new);
}


/* creeer een element van lijst van strings */
s_element_t *create_selement(char *s, s_element_t *ptr)
{
	s_element_t *new;
	int 	stringlen;

	stringlen = strlen(s);
	if ((new = MALLOC(s_element_t)) != NULL)
	{
		new->string = (char *) malloc(stringlen+1);
		strcpy(new->string,s);
		new->next = ptr;
	}
	return(new);
}


/* voeg een string toe aan een lijst van strings */
BOOLEAN append_slist(char *s, s_head_t *list)
{
	s_element_t *new;

	if ((new = create_selement(s,NULL)) != NULL)
	{
		if (list->length)
			list->last->next = new;
		else
			list->first = new;
		list->last = new;
		list->length++;
		return TRUE;
	}
	else
		return FALSE;
}


/* creeer een element van lijst van pointers naar strings */
s_element_t *create_ptrtostrelement(char *s, s_element_t *ptr)
{
	s_element_t *new;

	if ((new = MALLOC(s_element_t)) != NULL)
	{
		new->string = s;
		new->next = ptr;
	}
	return(new);
}


/* voeg een string toe aan een lijst van pointers naar strings */
void append_ptrtostrlist(char *s, s_head_t *list)
{
	s_element_t *new;

	if ((new = create_ptrtostrelement(s,NULL)) != NULL)
	{
		if (list->length)
			list->last->next = new;
		else
			list->first = new;
		list->last = new;
		list->length++;
	}
	else
		print_error(OUT_OF_MEM, "");
}


/* verwijder de lijst met pointers naar variabelen */
void delete_ptrtostrlist(s_head_t *list)
{
	if (list->length != 0)
		delete_ptrtostrel(list->first, list->length);
	free((char *)list);
}


/* verwijder een aantal elementen (bepaald door het argument counter) van de
	lijst met pointers naar variabelen */
void delete_ptrtostrel(s_element_t *el, int counter)
{
	if (counter > 1)
		delete_ptrtostrel(el->next, counter-1);
	free((char *)el);
}


/* deze procedure "bekijkt" of er in een lijst met pointers naar strings
	een string al bestaat */
BOOLEAN string_exists(char *string, s_head_t *string_list)
{
	register int i;
	BOOLEAN string_found;
	s_element_t *s;

	i = 1;
	string_found = FALSE;
	s = string_list->first;
	while (!string_found && (i<=string_list->length))
	{
		if (!strcmp(string, s->string))
			string_found = TRUE;
		else
		{
			i++;
			s = s->next;
		}
	}
	return (string_found);
}


/* zoek de positie op van een string in een lijst van strings */
int search_decl(char *decl_name, link_t *decl)
{
	register int i;
	s_element_t *decl_el;

	decl_el = decl->first;
	for (i=1; i<=decl->length; i++)
	{
		if (!strcmp(decl_name, decl_el->string))
			return i;
		else
			decl_el = decl_el->next;
	}
	return 0;
}


/* zoek de positie van een string op in een lijst van strings met element
	class nummer elclass_number */
int search_decllist(char *decl_name, int elclass_number)
{
	register int i, position;
	main_t *decl_list_help;
	link_t *decl;

	decl_list_help = decl_list;
	decl = decl_list_help->first;
	for (i=2;i<=elclass_number;i++)
		decl = decl->next;
	position = search_decl(decl_name, decl);
	return position;
}


/* zoek de element class nummer op van een element class naam (string) */
int find_elclass_no(char *decl_name)
{
	register int elclass_number;
	main_t *decl_list_help;
	link_t *decl;

	elclass_number = 1;
	decl_list_help = decl_list;
	decl = decl_list_help->first;
	while (elclass_number <= decl_list_help->length)
	{
		if (!strcmp(decl_name,decl->first->string))
			return elclass_number;
		else
		{
			decl = decl->next;
			elclass_number++;
		}
	}
	return 0;
}


/* zoek het aantal elementen van een declaratie (element class naam en het
	aantal attributen) op aan de hand van een element class nummer */
int get_elclass_length(int elclass_number)
{
	register int i;
	main_t *decl_list_help;
	link_t *decl;

	decl_list_help = decl_list;
	decl = decl_list_help->first;
	for (i=2;i<=elclass_number;i++)
		decl = decl->next;
	return decl->length;
}


/* creeer het hoofd van een lijst voor informatie over variabelen in een
	produktie regel */
var_head_t *create_varlist()
{
	var_head_t *new;

	if ((new = MALLOC(var_head_t)) != NULL)
	{
		new->length = 0;
		new->first = new->last = NULL;
	}
	return(new);
}


/* creeer een element van een lijst dat informatie bevat over variabelen
	in een produktieregel */
var_el_t *create_varel(condel_number,predicate,var,pos_number,var_matched,
								next)
	int	condel_number;
	char	predicate;
	char	*var;
	int	pos_number;
	BOOLEAN var_matched;
	var_el_t	*next;
{
	var_el_t *new;
	int 	stringlen;

	if ((new = MALLOC(var_el_t)) != NULL)
	{
		new->condel_no = condel_number;
		new->predicate = predicate;
		stringlen = strlen(var);
		new->var_name = (char *) malloc(stringlen+1);
		strcpy(new->var_name,var);
		new->position = pos_number;
		new->var_matched = var_matched;
		new->next = next;
	}
	return(new);
}


/* voeg informatie van een variabele in een produktieregel toe aan de
	ervoor gecreerde lijst  */
BOOLEAN add_varlist(condel_number,predicate,var,pos_number,var_matched,list)
	int	condel_number;
	char	predicate;
	char	*var;
	int	pos_number;
	BOOLEAN var_matched;
	var_head_t *list;
{
	var_el_t *new;

	if ((new = create_varel(condel_number,predicate,var,pos_number,
									var_matched,NULL)) != NULL)
	{
		if (list->length)
			list->last->next = new;
		else
			list->first = new;
		list->last = new;
		list->length++;
		return TRUE;
	}
	else
		return FALSE;
}


/* verwijder de lijst met informatie over variabelen in een produktieregel */
void delete_varlist(var_head_t *list)
{
	if (list->length != 0)
		delete_varel(list->first, list->length);
	free((char *)list);
}


/* verwijder een aantal elementen (bepaald door het argument counter) van de
	lijst met informatie over variabelen in een produktie regel */
void delete_varel(var_el_t *el, int counter)
{
	if (counter > 1)
		delete_varel(el->next, counter-1);
	free((char *)el);
}


/* deze procedure "bekijkt" of er in een lijst met informatie over
	variabelen een variabele twee maal voorkomt */
int double_var(char *var, var_head_t *var_list)
{
	register int i;
	BOOLEAN var_found;
	var_el_t *var_el;

	i = 1;
	var_found = FALSE;
	var_el = var_list->first;
	while (!var_found && (i<=var_list->length))
	{
		if (!strcmp(var, var_el->var_name))
			var_found = TRUE;
		else
		{
			i++;
			var_el = var_el->next;
		}
	}
	if (var_found)
		return var_el->position;
	else
		return 0;
}


/* creeer het hoofd van een lijst voor element variabelen en de daarbij
	behorende element class nummers */
elvarelclass_head_t *create_elvarelclass_head()
{
	elvarelclass_head_t *new;

	if ((new = MALLOC(elvarelclass_head_t)) != NULL)
	{
		new->length = 0;
		new->first = new->last = NULL;
	}
	return(new);
}


/* creeer een element van een lijst dat informatie bevat over
	element variabelen en de daarbij behorende element class nummers */
elvarelclass_el_t *create_elvarelclassel(char *elvar, int elclass_no,
															elvarelclass_el_t *next)
{
	elvarelclass_el_t *new;

	if ((new = MALLOC(elvarelclass_el_t)) != NULL)
	{
		new->elvar = elvar;
		new->elclass_no = elclass_no;
		new->next = next;
	}
	return(new);
}


/* voeg een element toe aan een lijst met element variabelen en de
	daarbij behorende element class nummers */
void add_elvarelclasslist(char *elvar, int elclass_no,
										elvarelclass_head_t *list)
{
	elvarelclass_el_t *new;

	if ((new = create_elvarelclassel(elvar, elclass_no, NULL)) != NULL)
	{
		if (list->length)
			list->last->next = new;
		else
			list->first = new;
		list->last = new;
		list->length++;
	}
	else
		print_error(OUT_OF_MEM, "");
}


/* verwijder de lijst met element variabelen en de daarbij behorende
	element class nummers */
void delete_elvarelclasslist(elvarelclass_head_t *list)
{
	elvarelclass_el_t *temp;

	if (list->length != 0)
		delete_elvarelclassel(list->first, list->length);
	free((char *)list);
}


/* verwijder een aantal elementen (bepaald door het argument counter) van de
	lijst met element variabelen en de daarbij behorende element class
	nummers */
void delete_elvarelclassel(elvarelclass_el_t *el, int counter)
{
	if (counter > 1)
		delete_elvarelclassel(el->next, counter-1);
	free((char *)el);
}


/* deze procedure "bekijkt" of er in een lijst met element variabelen
	en de daarbij behorende element class nummers een element variabele
	bestaat en retourneert dan de bij behorende element class nummer*/
int get_elvarelclassno(char *elvar, elvarelclass_head_t *elvar_list)
{
	register int i;
	BOOLEAN elvar_found;
	elvarelclass_el_t *elvar_el;

	i = 1;
	elvar_found = FALSE;
	elvar_el = elvar_list->first;
	while (!elvar_found && (i<=elvar_list->length))
	{
		if (!strcmp(elvar, elvar_el->elvar))
			elvar_found = TRUE;
		else
		{
			i++;
			elvar_el = elvar_el->next;
		}
	}
	if (elvar_found)
		return elvar_el->elclass_no;
	else
		return 0;
}


/* creeer het hoofd van een lijst voor gebonden variabelen (van een
	produktie regel) */
varbind_head_t *create_varbindings()
{
	varbind_head_t *new;

	if ((new = MALLOC(varbind_head_t)) != NULL)
	{
		new->length = 0;
		new->first = new->last = NULL;
	}
	else
		print_error(OUT_OF_MEM, "");
	return(new);
}


/* deze procedure "bekijkt" of een variabele al bestaat in de lijst met
	gebonden variabelen  */
BOOLEAN var_already_exists(char *var, varbind_head_t *varbindings)
{
	register int i;
	BOOLEAN var_exists;
	varbind_el_t *varbinding;

	i = 1;
	var_exists = FALSE;
	varbinding = varbindings->first;
	while(!var_exists && (i<=varbindings->length))
	{
		if (!strcmp(var, varbinding->varname))
			var_exists = TRUE;
		else
		{
			i++;
			varbinding = varbinding->next;
		}
	}
	return var_exists;
}


/* creeer een element van een lijst met gebonden variabelen  */
varbind_el_t *create_varbinding(char *varname, int condel_no,
											int value_pos, astnode_t *varvalue,
											varbind_el_t *next)
{
	varbind_el_t *new;
	int stringlen;

	if ((new = MALLOC(varbind_el_t)) != NULL)
	{
		stringlen = strlen(varname);
		new->varname = (char *) malloc(stringlen+1);
		strcpy(new->varname,varname);
		new->condel_no = condel_no;
		new->value_pos = value_pos;
		new->varvalue = varvalue;
		new->next = next;
	}
	else
		print_error(OUT_OF_MEM, "");
	return(new);
}


/* voeg een element toe aan een lijst met gebonden variabelen  */
void add_varbinding(char *varname, int condel_no, int value_pos,
							astnode_t *varvalue, varbind_head_t *list)
{
	varbind_el_t *new;

	new = create_varbinding(varname, condel_no, value_pos, varvalue, NULL);
	if (list->length)
		list->last->next = new;
	else
		list->first = new;
	list->last = new;
	list->length++;
}


/* verwijder een lijst met gebonden variabelen */
void delete_varbindings(varbind_head_t *list)
{
	varbind_el_t *temp;

	if (list->length != 0)
		delete_varbinding(list->first);
	free((char *)list);
}


/* verwijder een element van een lijst met gebonden variabelen */
void delete_varbinding(varbind_el_t *el)
{
	if (el->next != NULL)
		delete_varbinding(el->next);
	free((char *)el);
}


/* creeer het hoofd van een lijst met gebonden element variabelen (van een
	produktieregel */
elvarbind_head_t *create_elvarbindings()
{
	elvarbind_head_t *new;

	if ((new = MALLOC(elvarbind_head_t)) != NULL)
	{
		new->length = 0;
		new->first = new->last = NULL;
	}
	else
		print_error(OUT_OF_MEM, "");
	return(new);
}


/* deze procedure "bekijkt" of een element variabele al bestaat in de lijst
	met gebonden element variabelen  */
BOOLEAN elvar_already_exists(char *var, elvarbind_head_t *elvarbindings)
{
	register int i;
	BOOLEAN var_exists;
	elvarbind_el_t *elvarbinding;

	i = 1;
	var_exists = FALSE;
	elvarbinding = elvarbindings->first;
	while(!var_exists && (i<=elvarbindings->length))
	{
		if (!strcmp(var, elvarbinding->varname))
			var_exists = TRUE;
		else
		{
			i++;
			elvarbinding = elvarbinding->next;
		}
	}
	return var_exists;
}


/* creeer een element van een lijst met gebonden element variabelen  */
elvarbind_el_t *create_elvarbinding(char *varname, astnode_t *prodrule,
												int cond_number, elvarbind_el_t *next)
{
	elvarbind_el_t *new;
	int stringlen;

	if ((new = MALLOC(elvarbind_el_t)) != NULL)
	{
		stringlen = strlen(varname);
		new->varname = (char *) malloc(stringlen+1);
		strcpy(new->varname,varname);
		new->prodrule = prodrule;
		new->cond_number = cond_number;
		new->next = next;
	}
	else
		print_error(OUT_OF_MEM, "");
	return(new);
}


/* voeg een element toe aan een lijst met gebonden element variabelen */
void add_elvarbinding(char *varname, astnode_t *prodrule, int cond_number,
							elvarbind_head_t *list)
{
	elvarbind_el_t *new;

	new = create_elvarbinding(varname, prodrule, cond_number, NULL);
	if (list->length)
		list->last->next = new;
	else
		list->first = new;
	list->last = new;
	list->length++;
}


/* verwijder een lijst met gebonden element variabelen */
void delete_elvarbindings(elvarbind_head_t *list)
{
	elvarbind_el_t *temp;

	if (list->length != 0)
		delete_elvarbinding(list->first);
	free((char *)list);
}


/* verwijder een element van een lijst met gebonden element variabelen */
void delete_elvarbinding(elvarbind_el_t *el)
{
	if (el->next != NULL)
		delete_elvarbinding(el->next);
	free((char *)el);
}



