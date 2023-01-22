/*-----------------------------------------------------------------
C file: main.c

Deze file is het hoofd programma.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


#include <stdio.h>
#include <malloc.h>

#include "decl.h"
#include "load.h"
#include "scan.h"
#include "parse.h"
#include "execute.h"
#include "list.h"
#include "ast.h"
#include "retenet.h"
#include "workmem.h"
#include "rule-int.h"
#include "error.h"


main_t *decl_list;
workingmem_head_t *working_mem;
ast_var_head_t	*production_mem;
root_head_t *retenet;
conflict_set_t *conflict_set;
var_head_t *var_list1;
var_head_t *var_list2;
varbind_head_t *varbindings;
elvarbind_head_t *elvarbindings;
astnode_t *numberast, *nilast;

node_rec_t *lastnode1;
node_rec_t *lastnode2;
node_rec_t *node_found;
node_rec_t *node_found_help;
node_rec_t *node_found_help_prev;
node_rec_t *two_node_found;
workingmem_ptr_head_t *instantiation_wm_list;
inter_restriction_head_t *interrestrictions_temp;
elvarelclass_head_t *elvarelclass_list;
s_head_t *varlist;

BOOLEAN same_1_input_nodes;
BOOLEAN all_nodes_match;
BOOLEAN build_two_in_node;
BOOLEAN halt;
BOOLEAN varlists_processed;
BOOLEAN pred_atomic_val;
BOOLEAN parse_righths;
BOOLEAN top_level;
BOOLEAN write;
char *i_buf;
char *input;
char *input_filename;
char token[80];
char token_class;
int  wm_el_counter;
int condition_el_counter;
int test_counter;
int max_node_count;
char ast_delete;


void init(void);
void load_parse_and_exec_input(char *filename);


/* hoofd procedure */
void main(argc, argv)
int argc;
char *argv[];
{
	if (argc!=3)
		print_error(SPEC_FILE, "");
	init();
	printf("\nSOPS5> Loading SOPS5 program from file \"%s\".\n",argv[1]);
	load_parse_and_exec_input(argv[1]);
	ast_delete = DELETE;
	printf("SOPS5> Loading SOPS5 data from file \"%s\".\n",argv[2]);
	load_parse_and_exec_input(argv[2]);
	free(i_buf);
	printf("SOPS5> Starting rule interpreter.\n\n");
	start_rule_interpreter();
	printf("\n\n\nSOPS5> SOPS5 program has ended.\n");
}

/* laad, parse en voer uit invoer */
void load_parse_and_exec_input(char *filename)
{
	FILE *fp;
	astnode_t *ast;

	if (!(fp=fopen(filename,"r")))
	{
		print_error(OPEN_FILE, filename);
	}
	input_filename = filename;
	load_input(fp,i_buf);
	input = i_buf;
	get_token();
	while (token[0] != '\0')
	{
		ast = parse_input();
		exec_command(ast);
	}
	fclose(fp);
}


/* initialiseer globale variabelen */
void init()
{
	char *nilstring;

	nilstring = (char *) malloc(4);
	nilstring = "nil";
	if (!(i_buf=(char *) malloc(INPUT_SIZE)))
		print_error(OUT_OF_MEM, "");
	decl_list = create_lol();
	working_mem = create_wm_head();
	production_mem = create_ast_var_list();
	retenet = create_retenet();
	conflict_set = create_conflict_set();
	var_list1 = NULL;
	var_list2 = create_varlist();
	lastnode1 = create_node_rec();
	lastnode2 = create_node_rec();
	two_node_found = create_node_rec();
	node_found = create_node_rec();
	node_found_help = create_node_rec();
	node_found_help_prev = create_node_rec();
	elvarelclass_list = NULL;
	varlist = NULL;
	varlists_processed = FALSE;
	same_1_input_nodes = TRUE;
	all_nodes_match = FALSE;
	build_two_in_node = TRUE;
	interrestrictions_temp = NULL;
	varbindings = NULL;
	elvarbindings = NULL;
	instantiation_wm_list = NULL;
	input_filename = NULL;
	wm_el_counter = 0;
	condition_el_counter = 0;
	test_counter = 0;
	halt = FALSE;
	numberast = leaf_ast(NUMBER, NULL, 0);
	nilast = leaf_ast(NIL, nilstring, 0);
	ast_delete = NOT_DELETE;
	pred_atomic_val = FALSE;
	parse_righths = FALSE;
	top_level = FALSE;
	write = FALSE;
}

