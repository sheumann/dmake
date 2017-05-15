/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/mac/config.h,v 1.1 1992/01/24 03:29:43 dvadura Exp $
-- SYNOPSIS -- Configurarion include file for the Macintosh.
-- 
-- DESCRIPTION
--  There is one of these for each specific machine configuration.
--  It can be used to further tweek the machine specific sources
--  so that they compile.
--
-- AUTHOR
--      Dennis Vadura, dvadura@watdragon.uwaterloo.ca
--      CS DEPT, University of Waterloo, Waterloo, Ont., Canada
--
-- COPYRIGHT
--      Copyright (c) 1990 by Dennis Vadura.  All rights reserved.
-- 
--      This program is free software; you can redistribute it and/or
--      modify it under the terms of the GNU General Public License
--      (version 1), as published by the Free Software Foundation, and
--      found in the file 'LICENSE' included with this distribution.
-- 
--      This program is distributed in the hope that it will be useful,
--      but WITHOUT ANY WARRANTY; without even the implied warrant of
--      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--      GNU General Public License for more details.
-- 
--      You should have received a copy of the GNU General Public License
--      along with this program;  if not, write to the Free Software
--      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
--
-- LOG
--     $Log: config.h,v $
 * Revision 1.1  1992/01/24  03:29:43  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

/* Definitions left out of StdArg.h */
#define va_alist     ...
#define va_dcl

#define CONST const

#define FALSE 0
#define TRUE 1

/* signal extensions */
#define SIGQUIT SIGTERM


/* Mac doesn't have a stat function or structure so we have to add one in. */
typedef long off_t;
struct stat {
    unsigned short st_mode;
    off_t st_size;
    time_t st_mtime;
    time_t st_ctime;
    /* struct stat has lots of other fields, but we don't need them for dmake */
}; /* struct stat */
#define S_IFDIR  0040000    /* directory */
#define S_IFREG  0100000    /* regular */
#define S_IFMT   (S_IFDIR | S_IFREG)    /* Format */
#define S_IREAD  0000400    /* read owner permission */
#define S_IWRITE 0000200    /* write owner permission */
#define S_IEXEC  0000100    /* execute owner permission */


/* Global for environmental variables */
extern char **environ;


/* We really want main to be in the mac directory
   so that we get the envp argument */
#define main(argc, argv)    dmakemain(argc, argv)

/* Directory/file info. and directory moving */
int stat (char *pPath, struct stat *pStat);
char *getcwd (char *pPath, size_t pathSize);
int chdir (char *pPath);
int utime (char *pPath, time_t *pTimes);

/* Routines to handle conversion from Unix file names to Mac file names */
char *Unix2MacFName(char *pUnixName);
FILE *MacFOpen (char *pName, char *pMode);
#define fopen(pFName, pMode)    MacFOpen(pFName, pMode)

/* a small problem with pointer to voids on some unix machines needs this */
#define PVOID void *
