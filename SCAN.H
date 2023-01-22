/*-----------------------------------------------------------------
Header file: scan.h

Header file van de file scan.c.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


extern void get_token(void);
extern void get_any_atom(void);
extern void get_number_or_any_atom(void);
extern char look_up(char *s);
extern BOOLEAN isdelim(char c);
extern BOOLEAN iswhite(char c);
extern BOOLEAN ispredicate(void);
