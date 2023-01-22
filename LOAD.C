/*-----------------------------------------------------------------
C file: load.c

Deze file bevat procedures voor het inlezen van data uit een invoer
file. De ingelezen data komt in een string te staan.

auteur : W.L. Li
datum  : januari 1998
-------------------------------------------------------------------*/


#include <stdio.h>
#include <process.h>

#include "decl.h"
#include "load.h"
#include "scan.h"



/* laad een file in */
void load_input(FILE *fp, char *p)
{
	register int   i;
	register char  ch;

	i = 0;
	while (((ch=fgetc(fp))!=EOF) && (i<INPUT_SIZE))
	{
		*p = ch;
		p++; i++;
	}
	*p = '\0';
}
