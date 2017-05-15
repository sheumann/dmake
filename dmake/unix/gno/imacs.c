/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/RCS/imacs.c,v 1.1 1992/01/24 03:29:55 dvadura Exp $
-- SYNOPSIS -- define default internal macros.
-- 
-- DESCRIPTION
--	This file adds to the internal macro tables the set of default
--	internal macros, and for those that are accessible internally via
--	variables creates these variables, and initializes them to point
--	at the default values of these macros.
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
--     $Log: imacs.c,v $
 * Revision 1.1  1992/01/24  03:29:55  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"

static	void	_set_int_var ANSI((char *, char *, int, int *));
static	void	_set_string_var ANSI((char *, char *, int, char **));
static	void	_set_bit_var ANSI((char *, char *, int));

/*
** Arrange to parse the strings stored in Rules[]
*/
PUBLIC void
Make_rules(void)
{
   Parse(NIL(FILE));
}


#define M_FLAG   M_DEFAULT | M_EXPANDED

/*
** Add to the macro table all of the internal macro variables plus
** create secondary variables which will give access to their values
** easily, both when needed and when the macro value is modified.
** The latter is accomplished by providing a flag in the macro and a field
** which gives a pointer to the value if it is a char or string macro value
** and a mask representing the bit of the global flag register that is affected
** by this macro's value.
*/
PUBLIC void
Create_macro_vars(void)
{
   static char* switchar;
   char   swchar[2];

   swchar[0] = Get_switch_char(), swchar[1] = '\0';
   _set_string_var("SWITCHAR", swchar, M_PRECIOUS, &switchar);
   _set_string_var("DIRSEPSTR", (*swchar=='/')?"\\":"/",M_PRECIOUS,&DirSepStr);
   _set_string_var("DIRBRKSTR", DirBrkStr, M_PRECIOUS, &DirBrkStr);
   swchar[0] = DEF_ESCAPE_CHAR, swchar[1] = '\0';
   _set_string_var(".ESCAPE_PREFIX", swchar, M_FLAG, &Escape_char);

   _set_bit_var(".SILENT",   "", A_SILENT  );
   _set_bit_var(".IGNORE",   "", A_IGNORE  );
   _set_bit_var(".PRECIOUS", "", A_PRECIOUS);
   _set_bit_var(".EPILOG",   "", A_EPILOG  );
   _set_bit_var(".PROLOG",   "", A_PROLOG  );
   _set_bit_var(".NOINFER",  "", A_NOINFER );
   _set_bit_var(".SEQUENTIAL","",A_SEQ     );
   _set_bit_var(".USESHELL", "", A_SHELL   );
   _set_bit_var(".SWAP",     "", A_SWAP    );
   _set_bit_var(".MKSARGS",  "", A_MKSARGS );

   Glob_attr = A_DEFAULT;        /* set all flags to NULL   */

   _set_string_var("SHELL",        "",  M_DEFAULT, &Shell       );
   _set_string_var("SHELLFLAGS",   " ", M_DEFAULT, &Shell_flags );
   _set_string_var("GROUPSHELL",   "",  M_DEFAULT, &GShell      );
   _set_string_var("GROUPFLAGS",   " ", M_DEFAULT, &GShell_flags);
   _set_string_var("SHELLMETAS",   "",  M_DEFAULT, &Shell_metas );
   _set_string_var("GROUPSUFFIX",  "",  M_DEFAULT, &Grp_suff    );
   _set_string_var("PREP",         "0", M_DEFAULT, &Prep        );
   _set_string_var("AUGMAKE",NIL(char), M_DEFAULT, &Augmake     );
   _set_string_var(".KEEP_STATE",  "",  M_DEFAULT, &Keep_state  );
   _set_string_var(".NOTABS",      "",  M_MULTI,   &Notabs      );

   _set_string_var(".SETDIR", "", M_DEFAULT|M_NOEXPORT, &Start_dir );
   _set_string_var("MAKEDIR",Get_current_dir(),M_PRECIOUS|M_NOEXPORT,&Makedir);
   _set_string_var("PWD",  Makedir,  M_DEFAULT|M_NOEXPORT, &Pwd);
   _set_string_var("TMD",  "",       M_DEFAULT|M_NOEXPORT, &Tmd);

   Def_macro("NULL", "", M_PRECIOUS|M_NOEXPORT|M_FLAG);
 
   _set_int_var( "MAXLINELENGTH", "0", M_DEFAULT|M_NOEXPORT, &Buffer_size );
   (void) Def_macro("MAXLINELENGTH", "0", M_FLAG | M_DEFAULT);

   /* set MAXPROCESSLIMIT high initially so that it allows MAXPROCESS to
    * change from command line. */
   _set_int_var( "MAXPROCESSLIMIT", "100", M_DEFAULT|M_NOEXPORT, &Max_proclmt );
   _set_int_var( "MAXPROCESS", "1", M_DEFAULT|M_NOEXPORT, &Max_proc );
}


/*
** Define an integer variable value, and set up the macro.
*/
static void
_set_int_var(
	char *name,
	char *val,
	int  flag,
	int  *var)
{
   HASHPTR hp;

   hp = Def_macro(name, val, M_FLAG | flag);
   hp->ht_flag |= M_VAR_INT | M_MULTI;
   hp->MV_IVAR  = var;
   *var         = atoi(val);
}


/*
** Define a string variables value, and set up the macro.
*/
static void
_set_string_var(
	char *name,
	char *val,
	int  flag,
	char **var)
{            
   HASHPTR hp;

   hp = Def_macro(name, val, M_FLAG | flag);
   hp->ht_flag |= M_VAR_STRING | M_MULTI;
   hp->MV_SVAR  = var;
   *var         = hp->ht_value;
}


/*
** Define a bit variable value, and set up the macro.
*/
static void
_set_bit_var(char *name, char *val, int mask)
{
   HASHPTR hp;

   hp           = Def_macro(name, val, M_FLAG);
   hp->ht_flag |= M_VAR_BIT | M_MULTI;
   hp->MV_MASK  = mask;
   hp->MV_BVAR  = &Glob_attr;
}
