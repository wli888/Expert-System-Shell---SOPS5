/*-----------------------------------------------------------------
Header file: decl.h

Deze file bevat declaraties van datastructuren voor het programma.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/

#define FALSE 0
#define TRUE  1
#define BOOLEAN int
#define INPUT_SIZE 10000
#define MALLOC(x)	((x *) malloc(sizeof(x)))


enum token_class {ZERO_TK, L_PAREN_TK, R_PAREN_TK, L_CURLY_BRACKET_TK,
						R_CURLY_BRACKET_TK,
						TWO_L_ANGLE_BRACKET_TK, TWO_R_ANGLE_BRACKET_TK,
						ARROW_TK, UPARROW_TK, TAKE_LITERALLY_TK, CSA_TK,
						NUMBER_TK, LITERALIZE_TK, P_TK, MAKE_TK, REMOVE_TK,
						MODIFY_TK, WRITE_TK, HALT_TK, CRLF_TK, COMPUTE_TK,
						TEMP, EOT_TK,
						PLUS_TK, NEG_TK, MUL_TK,
						EQ_TK, NE_TK, LT_TK, LE_TK, GT_TK, GE_TK};

enum error {OUT_OF_MEM, SPEC_FILE, OPEN_FILE};

enum syntax_error {SYNTAX_ERROR, L_PAREN, R_PAREN, COMMAND, NOT_CSA,
						ARROW, L_CURLY_BRACKET, R_CURLY_BRACKET,
						UPARROW, TWO_L_ANGLE_BRACKET, TWO_R_ANGLE_BRACKET, LT, GT,
						NEG, OPERATOR, NOT_EL_CLASS, NOT_ATTRIBUTE, NOT_NUMBER,
						NOT_EXPRESSION, ELVAR_EXISTS, ELVAR_NOT_DEFINED,
						EL_CLASS_EXISTS, TAKE_LITERALLY, FUNCTION, FIRST_VAR_LHS,
						FIRST_VAR_RHS, ATTRIBUTE_EXISTS, PRODRULENAME_EXISTS,
						NO_VAR_TOP_LEVEL, NO_FUNCTION_TOP_LEVEL, NO_CRLF_WRITE,
						NOT_TOP_LEVEL_COMMAND, NOT_ACTION_COMMAND};

enum run_time_error {VAR_NOT_NUMBER, DIVBYZERO, ILLEGAL_OPERATOR};

enum asttag {NIL, CSA, CSALIST_CSALIST, LITERALIZE, P, POSCE, POSCE_CE, RHS,
				 CE_CE, FORM, VAR_FORM, FORM_VAR, NEGFORM, CSA_LHSTERM,
				 CSA_LHSVAL, LT_LT, LV_LV, DISJ, PRED_ATOMICVALUE, ATOMICVALUE,
				 PRED, VOC, ANYATOM, NUMBER, VAR, DISJ_DISJ, MAKE, REMOVE,
				 MODIFY, HALT, WRITE, CRLF, COMPUTE, ACTION_ACTION, ELD_ELD,
				 RV_RV, RHSPATTERN, RT_RT, CSA_RHSVAL, FUNC, E_O_E, OPLUS,
				 OMIN, OMUL, ODIV, OMOD};

/* de verschillende knooppunten van het Rete netwerk */
enum nodetype {NONE, ONE_INPUT, VAR_ONE_INPUT, TWO_INPUT, NOT_TWO_INPUT,
					PROD};

/* richting voor de linker of rechter ingang van een two-input of Not
	two-input knooppunt van het Rete netwerk*/
enum direction {NODIR, LEFT, RIGHT};

enum retenet_token_tag {PLUS, MIN};

/* het type geheugen van een two-input of Not two-input knooppunt van het
	Rete netwerk */
enum retenet_mem_type {LEFTMEM, RIGHTMEM, NOT_LEFTMEM};

/* kan een AST verwijderd worden of niet */
enum ast_delete {NOT_DELETE, DELETE};


/* een commando tabel met commando naam en token klasse */
struct commands
{
	char command[20];
	char tclass;
};

typedef struct commands commands_t;


/* een element van een lijst van strings */
struct s_element
{
	char *string;
	struct s_element *next;
};

typedef struct s_element s_element_t;


/* het hoofd van een lijst van strings */
struct s_head
{
	int	length;
	s_element_t	*first, *last;
};

typedef struct s_head s_head_t;


/* een element van een lijst van lijsten van strings */
struct link
{
	int length;
	s_element_t	*first, *last;
	struct link	*next;
};

typedef struct link link_t;


/* het hoofd van een lijst van lijsten van strings */
struct main
{
	int length;
	link_t	*first, *last;
};

typedef struct main main_t;


/* een element van een lijst om element variabelen bij te houden */
struct elvarelclass_el
{
	char *elvar;
	int elclass_no;
	struct elvarelclass_el *next;
};

typedef struct elvarelclass_el elvarelclass_el_t;


/* het hoofd van een lijst om element variabelen bij te houden */
struct elvarelclass_head
{
	int	length;
	elvarelclass_el_t	*first, *last;
};

typedef struct elvarelclass_head elvarelclass_head_t;


/* een abstract syntax tree (AST) knoop */
struct astnode
{
	char	asttag;
	char *token;
	float	number;
	char 	delete;
	struct astnode	*left;
	struct astnode	*middle;
	struct astnode	*right;
};

typedef struct astnode astnode_t;


typedef struct node_type_head node_type_head_t;
typedef struct node_rec node_rec_t;


/* een "gewone" one-input knooppunt van het Rete netwerk */
struct one_node
{
	int	position;
	astnode_t *restriction;
	node_type_head_t *next_nodes;
};

typedef struct one_node one_node_t;


/* een one-input knooppunt van het Rete netwerk die test of dezelfde
	variabelen in een conditie element consistent gebonden zijn */
struct var_one_node
{
	int	position;  /* de positie van de tweede variabele */
	char predicate;
	int position_prev_var; /* de positie van de eerste variabele */
	node_type_head_t *next_nodes;
};

typedef struct var_one_node var_one_node_t;


/* een element van een lijst die de inter-conditie tests bevat */
struct inter_restriction_el
{
	int	left_pos;
	int	left_condel_no; /* conditie element nummer voor links */
	char	predicate;
	int	right_pos;
	struct inter_restriction_el *next;
};

typedef struct inter_restriction_el inter_restriction_el_t;


/* het hoofd van een lijst die de inter-conditie tests bevat */
struct inter_restriction_head
{
	int length;
	inter_restriction_el_t	*first, *last;
};

typedef struct inter_restriction_head inter_restriction_head_t;


/* een element van een lijst die variabele informatie bevat voor gebruik
	tijdens de uitvoering van (de RHS van) een regel */
struct varbind_el
{
	char 	*varname;
	int 	condel_no;  /* conditie element nummer */
	int 	value_pos;  /* positie van de attribuut waarde voor de variabele */
	astnode_t *varvalue;
	struct varbind_el *next;
};

typedef struct varbind_el varbind_el_t;


/* het hoofd van een lijst die variabele informatie bevat voor gebruik
	tijdens de uitvoering van (de RHS van) een regel */
struct varbind_head
{
	int	length;
	varbind_el_t	*first, *last;
};

typedef struct varbind_head varbind_head_t;


/* een element van een lijst die element variabele informatie bevat voor
	gebruik tijdens de uitvoering van (de RHS van) een regel */
struct elvarbind_el
{
	char 	*varname;
	astnode_t *prodrule;  /* een pointer naar de AST die de produktie regel
										representeert */
	int 	cond_number; /* conditie element nummer */
	struct elvarbind_el *next;
};

typedef struct elvarbind_el elvarbind_el_t;


/* het hoofd van een lijst die element variabele informatie bevat voor
	gebruik tijdens de uitvoering van (de RHS van) een regel */
struct elvarbind_head
{
	int	length;
	elvarbind_el_t	*first, *last;
};

typedef struct elvarbind_head elvarbind_head_t;


/* een element van een lijst van pointers naar ASTs */
struct ast_el
{
	astnode_t	*ast;
	struct ast_el	*next;
};

typedef struct ast_el ast_el_t;


/* het hoofd van een lijst van pointers naar ASTs */
struct ast_head
{
	int length;
	ast_el_t	*first, *last;
};

typedef struct ast_head ast_head_t;


/* een element van de lijst die in SOPS5 de produktie memory voorstelt */
struct ast_var_el
{
	astnode_t	*ast; /* een pointer naar een AST die de regel representeert*/
	int	number_of_tests; /* het aantal tests van een regel */
	varbind_head_t *varbindings; /* de lijst met variabele informatie */
	elvarbind_head_t *elvarbindings; /* de lijst met element variabele
													informatie */
	struct ast_var_el	*next;
};

typedef struct ast_var_el ast_var_el_t;


/* het hoofd van de lijst die in SOPS5 de produktie memory voorstelt */
struct ast_var_head
{
	int length;
	ast_var_el_t	*first, *last;
};

typedef struct ast_var_head ast_var_head_t;


/* een element van de lijst die in SOPS5 de working memory voorstelt */
struct workingmem_el
{
	int 	wm_el_number;  /* WME nummer */
	int	elclass_number; /* element klas nummer */
	ast_head_t *attribute_values; /* een lijst met pointers naar ASTs (die
												de attribuut waarden bevatten) */
	struct workingmem_el *next;
};

typedef struct workingmem_el workingmem_el_t;


/* het hoofd van de lijst die in SOPS5 de working memory voorstelt */
struct workingmem_head
{
	int length;
	workingmem_el_t	*first, *last;
};

typedef struct workingmem_head workingmem_head_t;


/* een element van een lijst van pointers naar WME's */
struct workingmem_ptr_el
{
	workingmem_el_t	*wm_el_ptr;
	struct workingmem_ptr_el	*next;
};

typedef struct workingmem_ptr_el workingmem_ptr_el_t;


/* het hoofd van een lijst van pointers naar WME's */
struct workingmem_ptr_head
{
	int	length;
	workingmem_ptr_el_t	*first, *last;
};

typedef struct workingmem_ptr_head workingmem_ptr_head_t;


/* een element van een lijst van lijsten van pointers naar WME's */
struct lol_wm_ptrs_el
{
	int length;
	workingmem_ptr_el_t	*first, *last;
	struct lol_wm_ptrs_el	*next;
};

typedef struct lol_wm_ptrs_el lol_wm_ptrs_el_t;


/* het hoofd van een lijst van lijsten van pointers naar WME's */
struct lol_wm_ptrs_head
{
	int length;
	lol_wm_ptrs_el_t	*first, *last;
};

typedef struct lol_wm_ptrs_head lol_wm_ptrs_head_t;


/* een element van een lijst van negatie tellers en de bij de tellers horende
	lijsten van pointers naar WME's */
struct lol_count_wm_ptrs_el
{
	int length;
	int count; /* negatie teller */
	workingmem_ptr_el_t	*first, *last; /* een pointer naar het eerste en het
														laatste element van een lijst van
														pointers naar WME's*/
	struct lol_count_wm_ptrs_el	*next;
};

typedef struct lol_count_wm_ptrs_el lol_count_wm_ptrs_el_t;


/* het hoofd van een lijst van negatie tellers en de bij de tellers horende
	lijsten van pointers naar WME's */
struct lol_count_wm_ptrs_head
{
	int length;
	lol_count_wm_ptrs_el_t	*first, *last;
};

typedef struct lol_count_wm_ptrs_head lol_count_wm_ptrs_head_t;


/* een symbool dat door het Rete netwerk wordt gestuurd */
struct retenet_token
{
	char tag; /* actie indicatie */
	workingmem_ptr_head_t *workingmem_list; /* lijst van pointers naar WME's*/
};

typedef struct retenet_token retenet_token_t;


/* linker geheugen van two-input knoooppunt */
struct left_mem
{
	lol_wm_ptrs_head_t *retenet_tokens; /* lijst van lijsten van pointers naar
														WME's */
};

typedef struct left_mem left_mem_t;


/* linker geheugen van Not two-input knoooppunt */
struct not_left_mem
{
	lol_count_wm_ptrs_head_t *retenet_tokens; /* lijst van negatie tellers en
																lijsten van pointers naar
																WME's */
};

typedef struct not_left_mem not_left_mem_t;


/* rechter geheugen van een two-input of Not two-input knoooppunt */
struct right_mem
{
	lol_wm_ptrs_head_t *retenet_tokens; /* lijst van lijsten van pointers naar
														WME's */
};

typedef struct right_mem right_mem_t;


/* een two-input knooppunt */
struct two_node
{
	inter_restriction_head_t *inter_restrictions;
	left_mem_t	*left_mem;
	right_mem_t 	*right_mem;
	node_type_head_t *next_nodes;
};

typedef struct two_node two_node_t;


/* een Not two-input knooppunt */
struct not_two_node
{
	inter_restriction_head_t *inter_restrictions;
	not_left_mem_t	*left_mem;
	right_mem_t 	*right_mem;
	node_type_head_t *next_nodes;
};

typedef struct not_two_node not_two_node_t;


/* een P knooppunt */
struct p_node
{
	astnode_t *prodrule; /* een pointer naar de AST die de regel
									representeert */
};

typedef struct p_node p_node_t;


/* een structuur voor een knooppunt met type knooppunt en een pointer naar
	het knooppunt */
struct node_rec
{
	char	type;
	one_node_t	*one_node;
	var_one_node_t *var_one_node;
	two_node_t	*two_node;
	not_two_node_t *not_two_node;
	p_node_t		*p_node;
};


/* een element van de lijst van opvolgende knooppunten */
struct node_type_el
{
	char direction; /* richting voor linker of rechter ingang van een
								two-input of een Not two-input knooppunt */
	node_rec_t	*next_node; /* opvolgende knooppunt */
	struct node_type_el *next;
};

typedef struct node_type_el node_type_el_t;


/* het hoofd van de lijst van opvolgende knooppunten */
struct node_type_head
{
	int length;
	node_type_el_t	*first, *last;
};


/* een element van de lijst van pointers naar one-input knooppunten van het
	Rete netwerk */
struct root_el
{
	one_node_t	*element_class;
	struct root_el *next;
};

typedef struct root_el root_el_t;


/* het hoofd van de lijst van pointers naar one-input knooppunten van het
	Rete netwerk */
struct root_head
{
	int length;
	root_el_t	*first, *last;
};

typedef struct root_head root_head_t;


/* een element van de lijst met variabele informatie om een inter-conditie
	test te creeren */
struct var_el
{
	int	condel_no;  /* conditie element nummer */
	char	predicate;
	char	*var_name;
	int	position;   /* positie van de attribuut waarde */
	BOOLEAN var_matched;
	struct var_el	*next;
};

typedef struct var_el var_el_t;


/* het hoofd van de lijst met variabele informatie om een inter-conditie
	test te creeren */
struct var_head
{
	int length;
	var_el_t	*first, *last;
};

typedef struct var_head var_head_t;


/* een element van de lijst van instantiaties (conflict set) */
struct instantiation
{
	astnode_t *prodrule; /* een pointer naar de AST die de regel representeert */
	workingmem_ptr_head_t *workingmem_list; /* een lijst van pointers naar WME's */
	struct instantiation *next;
};

typedef struct instantiation instantiation_t;


/* het hoofd van de lijst van instantiaties (conflict set) */
struct conflict_set
{
	int length;
	instantiation_t	*first, *last;
};

typedef struct conflict_set conflict_set_t;


/* een element van de lijst van pointers naar instantiaties */
struct instantiation_ptr_el
{
	instantiation_t *instantiation;
	struct instantiation_ptr_el *next;
};

typedef struct instantiation_ptr_el instantiation_ptr_el_t;


/* het hoofd van de lijst van pointers naar instantiaties */
struct instantiation_ptr_head
{
	int length;
	instantiation_ptr_el_t	*first, *last;
};

typedef struct instantiation_ptr_head instantiation_ptr_head_t;


extern main_t *decl_list;
extern workingmem_head_t *working_mem;
extern ast_var_head_t	*production_mem;
extern root_head_t *retenet;
extern conflict_set_t *conflict_set;
extern var_head_t *var_list1;
extern var_head_t *var_list2;
extern varbind_head_t *varbindings;
extern elvarbind_head_t *elvarbindings;
extern astnode_t *numberast, *nilast;

extern node_rec_t *lastnode1;
extern node_rec_t *lastnode2;
extern node_rec_t *node_found;
extern node_rec_t *node_found_help;
extern node_rec_t *node_found_help_prev;
extern node_rec_t *two_node_found;
extern workingmem_ptr_head_t *instantiation_wm_list;
extern inter_restriction_head_t *interrestrictions_temp;
extern elvarelclass_head_t *elvarelclass_list;
extern s_head_t *varlist;

extern BOOLEAN same_1_input_nodes;
extern BOOLEAN all_nodes_match;
extern BOOLEAN build_two_in_node;
extern BOOLEAN halt;
extern BOOLEAN varlists_processed;
extern BOOLEAN pred_atomic_val;
extern BOOLEAN parse_righths;
extern BOOLEAN top_level;
extern BOOLEAN write;

extern char *i_buf;
extern char *input;
extern char *input_filename;
extern char token[80];
extern char token_class;
extern int 	wm_el_counter;
extern int 	condition_el_counter;
extern int test_counter;
extern int max_node_count;
extern char ast_delete;