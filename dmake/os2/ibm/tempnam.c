/*LINTLIBRARY*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>

#if defined(max)
#   undef  max
#endif
#define max(A,B) (((A)<(B))?(B):(A))

extern int access();
int _access();

/* MSC stdio.h defines P_tmpdir, so let's undo it here */
/* Under DOS leave the default tmpdir pointing here!		*/
#ifdef P_tmpdir
#undef P_tmpdir
#endif
static char *P_tmpdir = "";

char *
tempnam(dir, prefix)
char *dir;		/* use this directory please (if non-NULL) */
char *prefix;		/* use this (if non-NULL) as filename prefix */
{
   static         int count = 0;
   register char *p, *q, *tmpdir;
   int            tl=0, dl=0, pl;
   char		  buf[30];

   pl = strlen(P_tmpdir);

   if( (tmpdir = getenv("TMPDIR")) != NULL )
      tl = strlen(tmpdir);
   else if( (tmpdir = getenv("TMP")) != NULL )
      tl = strlen(tmpdir);
   if( dir != NULL ) dl = strlen(dir);

   if( (p = malloc((unsigned)(max(max(dl,tl),pl)+13))) == NULL )
     return(NULL);

   *p = '\0';

   if( (tl == 0) || (_access( strcpy(p, tmpdir), 0) != 0) )
     if( (dl == 0) || (_access( strcpy(p, dir), 0) != 0) )
	if( _access( strcpy(p, P_tmpdir), 0) != 0 )
	   if( !prefix )
	      prefix = "tp";

   if(prefix)
   {
      *(p+strlen(p)+2) = '\0';
      (void)strncat(p, prefix, 2);
   }

#ifdef OS2
   sprintf( buf, "%08x", getpid() );
#else
   sprintf( buf, "%08x", _psp );
#endif
   buf[6]='\0';
   (void)strcat(p, buf );
   sprintf( buf, "%04d", count++ );
   q=p+strlen(p)-6;
   *q++ = buf[0]; *q++ = buf[1];
   *q++ = buf[2]; *q++ = buf[3];

   if( (q = strrchr(p,'.')) != NULL ) *q = '\0';

   return strlwr(p);
}



_access( name, flag )
char *name;
int  flag;
{
   char *p;
   int r;

   if( name == NULL || !*name ) return(1);  /* NULL dir means current dir */
   p = name+strlen(name)-1;
   if(*p == ':' ) strcat( p++, "\\" );
   r = access( name, flag );
   if(*p != '/' && *p != '\\') strcat( p, "\\" );

   return( r );
}
