/*LINTLIBRARY*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "alloc.h"

/* ZTC++ doesn't have environ, so we have to create one. */

extern char *_envptr;
char **environ = { NULL };

void
make_env()
{
	int		i;
	char	*cp;

	for (i = 0, cp = _envptr; *cp; i++, cp += strlen(cp)+1)
		;

	TALLOC(environ, i+1, char*);

	for (i = 0, cp = _envptr; *cp; i++, cp += strlen(cp)+1)
		environ[i] = cp;

	return;
}

void
free_env()
{
	FREE(environ);

	return;
}
