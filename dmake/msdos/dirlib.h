/* DIRLIB.H by M. J. Weinstein   Released to public domain 1-Jan-89 */

#ifndef _DIRLIB_h_
#define _DIRLIB_h_

#include <stdio.h>
#include "stdmacs.h"

#define MAXNAMLEN   15

struct direct {
   long              d_ino;
   unsigned short    d_reclen;
   unsigned short    d_namlen;
   char              d_name[MAXNAMLEN+1];
};

typedef struct {
   char   fcb[21];
   char   attr;
   short  time;
   short  date;
   long   size;
   char   name[13];
} DTA;

typedef struct {
   DTA    dd_dta;       /* disk transfer area for this dir.        */
   short  dd_stat;      /* status return from last lookup          */
   char   dd_name[1];   /* full name of file -- struct is extended */
} DIR;

extern DIR           *opendir   ANSI((char *));
extern struct direct *readdir   ANSI((DIR *));
extern long          telldir    ANSI((DIR *));
extern void          seekdir    ANSI((DIR *, long));
extern void          closedir   ANSI((DIR *));
extern DTA           *findfirst ANSI((char *, DTA *));
extern DTA           *findnext  ANSI((DTA *));

#define rewinddir(dirp)   seekdir(dirp,0L)
#endif
