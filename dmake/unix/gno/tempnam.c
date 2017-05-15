/*LINTLIBRARY*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define max(A,B) (((A)<(B))?(B):(A))

#if !defined(GNO)
extern char *mktemp();
extern int access();
#endif

#if defined(__GNUC__)
char *mktemp(char *);
#endif

static char  seed[4]="AAA";

/* BSD stdio.h doesn't define P_tmpdir, so let's do it here */
#ifndef P_tmpdir
static char *P_tmpdir = "/tmp";
#endif


static char *
cpdir(char *buf, char *str)
{
   char *p;

   if(str != NULL)
   {
      (void) strcpy(buf, str);
      p = buf - 1 + strlen(buf);
      if(*p == '/') *p = '\0';
   }

   return(buf);
}


char *
tempnam(
const char *dir,	/* use this directory please (if non-NULL) */
const char *prefix)	/* use this (if non-NULL) as filename prefix */
{
   register char *p, *q, *tmpdir;
   int            tl=0, dl=0, pl;

   pl = strlen(P_tmpdir);

   if( (tmpdir = getenv("TMPDIR")) != NULL ) tl = strlen(tmpdir);
   if( dir != NULL ) dl = strlen(dir);

   if( (p = malloc((unsigned int)(max(max(dl,tl),pl)+16))) == NULL )
     return(NULL);

   *p = '\0';

#if defined (GNO)
   if( (tl == 0) || (access( cpdir(p, tmpdir), W_OK) != 0) )
   	if( (dl == 0) || (access( cpdir(p, dir), W_OK) != 0) )
			if( access( cpdir(p, P_tmpdir),   W_OK) != 0 )
	   		if( access( cpdir(p, "/tmp"),  W_OK) != 0 )
	      		return(NULL);

#else  /* not GNO */
   if( (tl == 0) || (access( cpdir(p, tmpdir), 3) != 0) )
     if( (dl == 0) || (access( cpdir(p, dir), 3) != 0) )
	if( access( cpdir(p, P_tmpdir),   3) != 0 )
	   if( access( cpdir(p, "/tmp"),  3) != 0 ) 
	      return(NULL);
#endif /* not GNO */

   (void) strcat(p, "/");
   if(prefix)
   {
      *(p+strlen(p)+5) = '\0';
      (void)strncat(p, prefix, 5);
   }

   (void)strcat(p, seed);
   (void)strcat(p, "XXXXXX");

   q = seed;
   while(*q == 'Z') *q++ = 'A';
   ++*q;

   if(*mktemp(p) == '\0') return(NULL);
   return(p);
}
