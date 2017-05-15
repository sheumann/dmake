/* RCS      -- $Header: /u/dvadura/src/generic/dmake/src/RCS/vextern.h,v 1.2 1992/04/07 04:42:46 dvadura Exp $
-- SYNOPSIS -- global variable declarations.
-- 
-- DESCRIPTION
-- 	Leave _DEFINE_GLOBALS_ undefined and the following declarations
--	will be defined as global variables, otherwise you get the
--	external declarations to the same global variables.
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
--     $Log: vextern.h,v $
 * Revision 1.2  1992/04/07  04:42:46  dvadura
 * Added the No_exec variable for -X flag.
 *
 * Revision 1.1  1992/01/24  03:27:19  dvadura
 * dmake Version 3.8, Initial revision
 *
*/
 
/* These two are defined in dir/ruletab.c and dir/dirbrk.c, and are always
 * imported as externals by the other code.  Their defining modules do not
 * #include this file. */
extern	char*	DirBrkStr;	/* pointer to value of macro DIRBRKSTR    */
extern	char**	Rule_tab;	/* Builtin rules */
 
#ifndef _DEFINE_GLOBALS_
#define EXTERN	extern
#else
#define EXTERN
#endif
 
EXTERN	int	Line_number;	/* Current line number in make file parse */
EXTERN	t_attr	Glob_attr;	/* Global attrs to control global ops     */
EXTERN	char*	Prep;		/* Value of macro PREP			  */
EXTERN	char*	Makedir;	/* pointer to macro value for MAKEDIR     */
EXTERN	char* 	Start_dir;	/* cell for   macro value for .SETDIR	  */
EXTERN	char*	Shell;		/* pointer to macro value for SHELL       */
EXTERN	char*	Shell_flags;	/* pointer to macro value for SHELLFLAGS  */
EXTERN	char*	GShell;		/* pointer to macro value for GROUPSHELL  */
EXTERN	char*	GShell_flags;	/* pointer to macro value for GROUPFLAGS  */
EXTERN	char*	Shell_metas;	/* pointer to macro value for SHELLMETAS  */
EXTERN	char*	Grp_suff;	/* pointer to macro value for GROUPSUFFIX */
EXTERN  char*   DirSepStr;	/* pointer to macro value for DIRSEPSTR   */
EXTERN	char*	Pname;		/* dmake process invoke name              */
EXTERN	char*	Pwd;		/* current working dir, value of PWD	  */
EXTERN	char*	Tmd;		/* path to directory where dmake started  */
EXTERN	char*	Keep_state;	/* current .KEEP_STATE file		  */
EXTERN  char*   Escape_char;	/* Current escape character               */
EXTERN	int	Target;		/* TRUE if target found in makefile       */
EXTERN	int	If_expand;	/* TRUE if calling Expand from getinp.c   */
EXTERN  int	If_multi;	/* M_MULTI for expand of single MULTI macs*/
EXTERN  int	Suppress_temp_file;/* TRUE if doing a test in _exec_recipe   */
EXTERN	int	Readenv;	/* TRUE if defining macro from environment*/
EXTERN	int	Makemkf;	/* TRUE if making makefile(s)		  */
EXTERN	int	Nest_level;	/* Nesting level for .IF .ELSE .END ...   */
EXTERN	int	Def_targets;	/* TRUE if defining targets		  */
 
EXTERN  CELLPTR Root;		/* Root of the make graph		  */
 
EXTERN  CELLPTR Current_target; /* cell of current target being made      */
EXTERN  int	Wait_for_completion;
EXTERN  int	Doing_bang;
EXTERN  int	Packed_shell;	/* TRUE if packed args to use a shell	  */
EXTERN  int	Swap_on_exec;	/* TRUE if going to swap on exec call     */
EXTERN  int	State;		/* parser state				  */
EXTERN  int	Group;		/* parsing a group recipe ==> TRUE        */
 
/* Command line option flags are defined here.  They correspond one-for one
 * with the flags defined in dmake.c */
 
EXTERN	char    *Augmake;	/* -A */
EXTERN  char    Comment;	/* -c */
EXTERN	char	Get_env;	/* -e or -E */
EXTERN  char*   Notabs;		/* -B */
EXTERN	int	Continue;	/* -k */
EXTERN	int	Force;		/* -u */
EXTERN	int	Listing;	/* -p */
EXTERN	int	Rules;		/* -r */
EXTERN	int	Trace;		/* -n */
EXTERN	int	Touch;		/* -t */
EXTERN	int	Check;		/* -q */
EXTERN	uint16	Verbose;	/* -v */
EXTERN	int	Microsoft;	/* -M */
EXTERN	int	Transitive;	/* -T */
EXTERN  int     No_exec;        /* -X */
 
EXTERN	HASHPTR	Macs[HASH_TABLE_SIZE];	/* hash table for macro defs    */
EXTERN	HASHPTR	Defs[HASH_TABLE_SIZE];	/* hash table for other defs    */
 
EXTERN	char	*Buffer;		/* a general purpose buffer     */
EXTERN  int	Buffer_size;
EXTERN  int	Max_proclmt;		/* limit of max # of conc procs */
EXTERN	int	Max_proc;		/* max # of conc procs		*/
