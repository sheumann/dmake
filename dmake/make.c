/* RCS      -- $Header: /u/dvadura/src/generic/dmake/src/RCS/make.c,v 1.2 1992/04/07 04:42:46 dvadura Exp $
-- SYNOPSIS -- perform the update of all outdated targets.
-- 
-- DESCRIPTION
--	This is where we traverse the make graph looking for targets that
--	are out of date, and we try to infer how to make them if we can.
--	The usual make macros are understood, as well as some new ones:
--
--		$$	- expands to $
--		$@      - full target name
--		$*      - target name with no suffix, same as $(@:db)
--			  or, the value of % in % meta rule recipes
--		$?      - list of out of date prerequisites
--		$<      - all prerequisites associated with rules line
--		$&	- all prerequisites associated with target
--		$>      - library name for target (if any)
--		$^	- out of date prerequisites taken from value of $<
--		{{	- expands to {
--		}}	- expands to }
--		\#	- expands to #
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
--     $Log: make.c,v $
 * Revision 1.2  1992/04/07  04:42:46  dvadura
 * Minor change to variable name to remove "redefined warning from compiler".
 *
 * Revision 1.1  1992/01/24  03:26:51  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"

static	void	_drop_mac ANSI((HASHPTR));
static	void	_set_recipe ANSI((char*, int));
static	void	_set_tmd ANSI(());
static	void	_append_file ANSI((STRINGPTR, FILE*, char*, int));
static  LINKPTR _dup_prq ANSI((LINKPTR));
static  char*   _prefix ANSI((char *, char *));
static  char*   _pool_lookup ANSI((char *));

#define RP_GPPROLOG	0
#define RP_RECIPE	1
#define RP_GPEPILOG	2
#define NUM_RECIPES	3

static STRINGPTR _recipes[ NUM_RECIPES ];


PUBLIC int
Make_targets()/*
================
   Actually go and make the targets on the target list */
{
   LINKPTR lp;
   int     done = 0;

   DB_ENTER( "Make_targets" );

   Read_state();
   _set_recipe( ".GROUPPROLOG", RP_GPPROLOG );
   _set_recipe( ".GROUPEPILOG", RP_GPEPILOG );

   Root->ce_flag  |= F_RULES|F_TARGET|F_STAT;
   Root->ce_attr  |= A_NOSTATE;
   Root->ce_time   = Do_time();
   TALLOC( Root->ce_recipe, 1, STRING );
   Root->ce_recipe->st_string = "";

   for( lp = Root->ce_prq; lp != NIL(LINK); lp = lp->cl_next )
      lp->cl_prq->ce_attr |= A_ROOT;

   while( !done ) {
      int rval;

      if( (rval = Make(Root, NIL(CELL))) == -1 )
	 DB_RETURN(1);
      else
	 done = Root->ce_flag & F_MADE;

      if( !rval && !done ) Wait_for_child( FALSE, -1 );
   }

   for( lp = Root->ce_prq; lp != NIL(LINK); lp = lp->cl_next ) {
      CELLPTR root = lp->cl_prq;
      if( !(root->ce_attr & A_UPDATED) )
	 printf( "`%s' is up to date\n", root->CE_NAME );
   }

   DB_RETURN( 0 );
}



int
Make( cp, setdirroot )/*
========================  Make a specified target */
CELLPTR cp;
CELLPTR setdirroot;
{
   register LINKPTR dp;
   register CELLPTR tcp;
   CELLPTR          nsetdirroot;
   char		    *name, *lib;
   HASHPTR	    m_at, m_q, m_b, m_g, m_l, m_bb, m_up;
   char             *all    = NIL(char);
   char             *inf    = NIL(char);
   char             *outall = NIL(char);
   char             *imm    = NIL(char);
   int              rval    = 0;
   int		    push    = 0;
   int 		    made    = F_MADE;
   int		    ignore;
   time_t           otime   = (time_t) 1L;
   time_t	    ttime   = (time_t) 1L;
   int		    mark_made = FALSE;

   DB_ENTER( "Make" );
   DB_PRINT( "mem", ("%s:-> mem %ld", cp->CE_NAME, (long) coreleft()) );

   m_q = m_b = m_g = m_l = m_bb = m_up = m_at = NIL(HASH);

   /* If we are supposed to change directories for this target then do so.
    * If we do change dir, then modify the setdirroot variable to reflect
    * that fact for all of the prerequisites that we will be making. */

   nsetdirroot = setdirroot;
   ignore = (((cp->ce_attr|Glob_attr)&A_IGNORE) != 0);

   if( cp->ce_attr & A_SETDIR ) {
      /* Change directory only if the previous .SETDIR is a different
       * directory from the current one.  ie. all cells with the same .SETDIR
       * attribute are assumed to come from the same directory. */

      if( (setdirroot == NIL(CELL) || setdirroot->ce_dir != cp->ce_dir) &&
          (push = Push_dir(cp->ce_dir,cp->CE_NAME,ignore)) )
	 setdirroot = cp;
   }

   DB_PRINT( "mem", ("%s:-A mem %ld", cp->CE_NAME, (long) coreleft()) );
   if( cp->ce_recipe == NIL(STRING) ) {
      char *dir = cp->ce_dir;
      
      if( Verbose & V_MAKE )
	 printf( "%s:  Infering prerequisite(s) and recipe for [%s]\n", Pname,
	 	 cp->CE_NAME );

      Infer_recipe( cp, setdirroot );

      /* See if the directory has changed, if it has then make sure we
       * push it. */
      if( dir != cp->ce_dir ) {
	 if( push ) Pop_dir(FALSE);
         push = Push_dir( cp->ce_dir, cp->CE_NAME, ignore );
	 setdirroot = cp;
      }
   }

   tcp = cp;
   do {
      if( push ) {
	 if( tcp->ce_dir ) FREE( tcp->ce_dir );
	 tcp->ce_dir   = _pool_lookup(Pwd);
	 tcp->ce_attr |= A_SETDIR;
      }
      tcp->ce_setdir = nsetdirroot;
      tcp = tcp->ce_all;
   }
   while( tcp != NIL(CELL) && tcp != cp );

   DB_PRINT( "mem", ("%s:-A mem %ld", cp->CE_NAME, (long) coreleft()) );
   /* If we have not yet statted the target then do so. */
   if( !(cp->ce_flag & F_STAT) && !(cp->ce_attr&A_PHONY) ) {
      time_t itime = cp->ce_time;
      Stat_target( cp, TRUE );

      if( cp->ce_time == (time_t)0L ) {
         if( cp->ce_flag & F_INFER )
	    cp->ce_time = itime;
      }
      else
         cp->ce_attr |= A_PRECIOUS;  /* File exists so don't remove it later. */

      if( Verbose & V_MAKE )
	 printf("%s:  Time stamp of [%s] is %ld\n",Pname,cp->CE_NAME,
	        cp->ce_time);
   }

   DB_PRINT( "make", ("(%s, %ld, 0x%08x, 0x%04x)", cp->CE_NAME,
			cp->ce_time, cp->ce_attr, cp->ce_flag) );

   if( !(cp->ce_flag & F_TARGET) && (cp->ce_time == (time_t) 0L) )
      if( Makemkf ) {
	 rval = -1;
	 goto stop_making_it;
      }
      else if(cp->ce_prq != NIL(LINK)||(Augmake && (cp->ce_flag&F_EXPLICIT)))
	 /* Assume an empty recipe for a target that we have run inference on
	  * but do not have a set of rules for but for which we have inferred
	  * a list of prerequisites. */
	 cp->ce_flag |= F_RULES;
      else
	 Fatal( "`%s' not found, and can't be made", cp->CE_NAME );

   DB_PRINT( "mem", ("%s:-A mem %ld", cp->CE_NAME, (long) coreleft()) );

   /* set value of $* if we have not infered a recipe, in this case $* is
    * the same as $(@:db), this allows us to be compatible with BSD make */
   if( cp->ce_per == NIL(char) ) cp->ce_per = "$(@:db)";

   for( dp = cp->ce_prq; dp != NIL(LINK); dp = dp->cl_next ) {
      int seq;

      /* Make the prerequisite, note that if the current target has the
       * .LIBRARY attribute set we pass on to the prerequisite the .LIBRARYM
       * attribute and pass on the name of the current target as the library
       * name, and we take it away when we are done.  */

      tcp = dp->cl_prq;
      seq = (((cp->ce_attr | Glob_attr) & A_SEQ) != 0);

      if( tcp->ce_flag & F_VISITED )
	 if( _explode_graph(tcp, dp, setdirroot) == 0 ) {
	    /* didn't blow it up so see if we need to wait for it. */
	    if( tcp->ce_flag & F_MADE ) {
	       if( tcp->ce_time > ttime ) ttime = tcp->ce_time;
	       continue;
	    }
	    else
	       goto stop_making_it;
	 }
	 else
	    tcp = dp->cl_prq;

      if( seq && !made ) goto stop_making_it;

      if( strchr(tcp->CE_NAME, '$') ) {
	 /* Make this prerequisite link point at the real prerequisite we
	  * are after, ie figure out what the dynamic one is and point at it. */

	 m_at = Def_macro( "@", cp->ce_fname, M_MULTI );
	 name = Expand( tcp->CE_NAME );
	 if( strcmp(name,cp->CE_NAME) == 0 )
	    Fatal( "Circular dynamic dependency generated '%s'", name );

	 tcp = dp->cl_prq = Def_cell( name );
	 FREE( name );
      }

      if( cp->ce_attr & A_LIBRARY ) {
         tcp->ce_attr |= A_LIBRARYM;
	 tcp->ce_lib   = cp->ce_fname;
      }

      if( (tcp->ce_flag & (F_INFER|F_STAT))==F_INFER && cp->ce_time >= ttime )
	 tcp->ce_time = cp->ce_time;

      /* Propagate parents F_REMOVE and F_INFER attribute to children.
       * Make certain you do this AFTER propagating the time, since the
       * time propagation test above uses the F_INFER flag to decide if
       * it should do so. */
      tcp->ce_flag |= cp->ce_flag & (F_REMOVE|F_INFER);

      rval |= Make(tcp, setdirroot);

      if( cp->ce_attr & A_LIBRARY )
         tcp->ce_attr ^= A_LIBRARYM;

      if( rval == -1 || (seq && (rval==1)) )
	 goto stop_making_it;

      if( tcp->ce_time > ttime ) ttime = tcp->ce_time;
      made &= tcp->ce_flag & F_MADE;
   }


   /* Do the loop again.  We are most definitely going to make the current
    * cell now.  NOTE:  doing this loop here also results in a reduction
    * in peak memory usage by the algorithm. */

   for( dp = cp->ce_prq; dp != NIL(LINK); dp = dp->cl_next ) {
      int  tgflg;
      tcp  = dp->cl_prq;
      name = tcp->ce_fname;

      /* make certain that all prerequisites are made prior to advancing. */
      if( !(tcp->ce_flag & F_MADE) ) goto stop_making_it;

      /* If the target is a library, then check to make certain that a member
       * is newer than an object file sitting on disk.  If the disk version
       * is newer then set the time stamps so that the archived member is
       * replaced. */
      if( cp->ce_attr & A_LIBRARY )
	 if( tcp->ce_time < cp->ce_time ) {
	    time_t mtime = Do_stat( name, tcp->ce_lib, NIL(char *) );
	    if( mtime < tcp->ce_time ) tcp->ce_time = cp->ce_time+1L;
	 }

      if( tcp->ce_time > otime ) otime = tcp->ce_time;

      all = _strapp( all, name );
      if( tgflg = (dp->cl_flag & F_TARGET) ) inf = _strapp( inf, name );

      if((cp->ce_time<tcp->ce_time) || ((tcp->ce_flag & F_TARGET) && Force)) {
         outall = _strapp( outall, name );
         if( tgflg ) imm = _strapp( imm, name );
      }
   }

   DB_PRINT( "mem", ("%s:-C mem %ld", cp->CE_NAME, (long) coreleft()) );
   DB_PRINT( "make", ("I make '%s' if %ld > %ld", cp->CE_NAME, otime,
	      cp->ce_time) );

   if( Verbose & V_MAKE && !(cp->ce_flag & F_MULTI) ) {
      printf( "%s:  >>>> Making ", Pname );
      if( cp->ce_count != 0 )
	 printf( "[%s::{%d}]\n", cp->CE_NAME, cp->ce_count );
      else
	 printf( "[%s]\n", cp->CE_NAME );
   }

   m_at = Def_macro( "@", cp->ce_fname, M_MULTI|M_EXPANDED );
   m_g  = Def_macro( ">", cp->ce_lib,   M_MULTI|M_EXPANDED );
   m_q  = Def_macro( "?", outall,       M_MULTI|M_EXPANDED );
   m_b  = Def_macro( "<", inf,          M_MULTI|M_EXPANDED );
   m_l  = Def_macro( "&", all,          M_MULTI|M_EXPANDED );
   m_up = Def_macro( "^", imm,          M_MULTI|M_EXPANDED );
   m_bb = Def_macro( "*", cp->ce_per,   M_MULTI );

   _recipes[ RP_RECIPE ] = cp->ce_recipe;

   /* We attempt to make the target if
    *   1. it has a newer prerequisite
    *   2. It is a target and Force is set
    *   3. It's set of recipe lines has changed.
    */
   if(    Check_state(cp, _recipes, NUM_RECIPES )
      || (cp->ce_time < otime)
      || ((cp->ce_flag & F_TARGET) && Force)
     ) {

      /* Only checking so stop as soon as we determine we will make something */
      if( Check ) {
	 rval = -1;
	 goto stop_making_it;
      }

      if( Verbose & V_MAKE )
	 printf( "%s:  Updating [%s], (%ld > %ld)\n", Pname,
		 cp->CE_NAME, otime, cp->ce_time );

      if( Touch ) {
	 name = cp->ce_fname;
	 lib  = cp->ce_lib;

	 if( !(Glob_attr & A_SILENT) || !Trace )
	    if( lib == NIL(char) )
	       printf("touch(%s)", name );
	    else if( cp->ce_attr & A_SYMBOL )
	       printf("touch(%s((%s)))", lib, name );
	    else
	       printf("touch(%s(%s))", lib, name );

	 if( !Trace )
	    if( Do_touch( name, lib,
		(cp->ce_attr & A_SYMBOL) ? &name : NIL(char *) ) != 0 )
	       printf( "  not touched - non-existant" );

	 printf( "\n" );
	 Update_time_stamp( cp );
      }
      else if( cp->ce_recipe != NIL(STRING) ) {
	 if( !(cp->ce_flag & F_SINGLE) )
	       rval = Exec_commands( cp );
	 else {
	    TKSTR tk;

	    _drop_mac( m_q );

	    if( outall && *outall ) {
	       SET_TOKEN( &tk, outall );

	       Doing_bang = TRUE;
	       name = Get_token( &tk, "", FALSE );
	       do {
		  m_q->ht_value = name;

		  Wait_for_completion = TRUE;	/* Reset in Exec_commands */
		  rval = Exec_commands( cp );
		  Unlink_temp_files(cp);
	       }
	       while( *(name = Get_token( &tk, "", FALSE )) != '\0' );
	       Doing_bang = FALSE;
	    }

	    Update_time_stamp( cp );
	    m_q->ht_value = NIL(char);
	 }
      }
      else if( !(cp->ce_flag & F_RULES) && !(cp->ce_flag & F_STAT) &&
	       (!(cp->ce_attr & A_ROOT) || !(cp->ce_flag & F_EXPLICIT)) )
	 Fatal( "Don't know how to make `%s'",cp->CE_NAME );
      else {
         /* Empty recipe, set the flag as MADE and update the time stamp */
	 Update_time_stamp( cp );
      }
   }
   else
      mark_made = TRUE;

   /* Make sure everyone gets remade if Force is set */
   tcp = cp;
   do {
      if( !(tcp->ce_flag & F_TARGET) && Force ) tcp->ce_time = Do_time();
      if( mark_made ) {
	 tcp->ce_flag |= F_MADE;
	 if( tcp->ce_flag & F_MULTI ) {
	    LINKPTR tdp;
	    for( tdp = tcp->ce_prq; tdp != NIL(LINK); tdp = tdp->cl_next )
	       tcp->ce_attr |= tdp->cl_prq->ce_attr & A_UPDATED;
	 }
      }

      tcp->ce_flag |= F_VISITED;

      /* Note:  If the prerequisite was made using a .SETDIR= attribute
       * 	directory then we will include the directory in the fname
       *        of the target.  */
      if( push ) {
	 char *dir   = nsetdirroot ? nsetdirroot->ce_dir : Makedir;
	 char *nname = Build_path(_prefix(dir,tcp->ce_dir), tcp->ce_fname);

	 if( (tcp->ce_attr & A_FFNAME) && (tcp->ce_fname != NIL(char)) )
	    FREE( tcp->ce_fname );

	 tcp->ce_fname = _strdup(nname);
	 tcp->ce_attr |= A_FFNAME;
      }

      tcp = tcp->ce_all;
   }
   while( tcp != NIL(CELL) && tcp != cp );

stop_making_it:
   _drop_mac( m_g  );
   _drop_mac( m_q  );
   _drop_mac( m_b  );
   _drop_mac( m_l  );
   _drop_mac( m_bb );
   _drop_mac( m_up );
   _drop_mac( m_at );

   while( push-- )  Pop_dir(FALSE);

   if( inf    != NIL(char) ) FREE( inf    );
   if( all    != NIL(char) ) FREE( all    );
   if( imm    != NIL(char) ) FREE( imm    );
   if( outall != NIL(char) ) FREE( outall );

   DB_PRINT( "mem", ("%s:-< mem %ld", cp->CE_NAME, (long) coreleft()) );
   DB_RETURN( rval );
}


static char *
_prefix( pfx, pat )
char *pfx;
char *pat;
{
   char *opat = pat;
   while( *pfx && *pat && *pfx++ == *pat++ );

   return( !*pfx ? _strspn(pat,DirBrkStr) : opat );
}


static LINKPTR
_dup_prq( lp )
LINKPTR lp;
{
   LINKPTR tlp;

   if( lp == NIL(LINK) ) return(lp);

   TALLOC(tlp, 1, LINK);
   *tlp = *lp;
   tlp->cl_next = _dup_prq( lp->cl_next );

   return(tlp);
}


static void
_drop_mac( hp )/*
================ set a macro value to zero. */
HASHPTR hp;
{
   if( hp && hp->ht_value != NIL(char) ) {
      FREE( hp->ht_value );
      hp->ht_value = NIL(char);
   }
}



int
_explode_graph( cp, parent, setdirroot )/*
==========================================
   Check to see if we have made the node already.  If so then don't do
   it again, except if the cell's ce_setdir field is set to something other
   than the value of setdirroot.  If they differ then, and we have made it
   already, then make it again and set the cell's stat bit to off so that
   we do the stat again.  */
CELLPTR cp;
LINKPTR parent;
CELLPTR setdirroot;
{
   /* we may return if we made it already from the same setdir location,
    * or if it is not a library member whose lib field is non NULL.  (if
    * it is such a member then we have a line of the form:
    *	lib1 lib2 .LIBRARY : member_list...
    * and we have to make sure all members are up to date in both libs. */

   if( cp->ce_setdir == setdirroot &&
       !((cp->ce_attr & A_LIBRARYM) && (cp->ce_lib != NIL(char))) )
      return( 0 );

   /* We check to make sure that we are comming from a truly different
    * directory, ie. ".SETDIR=joe : a.c b.c d.c" are all assumed to come
    * from the same directory, even though setdirroot is different when
    * making dependents of each of these targets. */

   if( cp->ce_setdir != NIL(CELL) &&
       setdirroot != NIL(CELL) &&
       cp->ce_dir &&
       setdirroot->ce_dir &&
       !strcmp(cp->ce_dir, setdirroot->ce_dir) )
      return( 0 );

   if( Max_proc > 1 ) {
      if( parent == NIL(LINK) )
	 Fatal( "Internal Error:  NIL parent in Make()" );

      TALLOC(parent->cl_prq, 1, CELL);
      *parent->cl_prq = *cp;
      cp = parent->cl_prq;
      cp->ce_prq = _dup_prq(cp->ce_prq);
   }
   cp->ce_flag  &= ~(F_STAT|F_VISITED|F_MADE);

   /* Indicate that we exploded the graph and that the current node should
    * be made. */
   return(1);
}



PUBLIC int
Exec_commands( cp )/*
=====================
  Execute the commands one at a time that are pointed to by the rules pointer
  of the target cp. If a group is indicated, then the ce_attr determines
  .IGNORE and .SILENT treatment for the group.
  
  The function returns 0, if the command is executed and has successfully
  returned, and returns 1 if the command is executing but has not yet
  returned (for parallel makes).
  
  The F_MADE bit in the cell is guaranteed set when the command has
  successfully completed.  */
CELLPTR cp;
{
   static HASHPTR useshell = NIL(HASH);
   static HASHPTR command  = NIL(HASH);
   static         int   read_cmnd = 0;
   register STRINGPTR	rp;
   STRINGPTR            orp;
   char			*cmnd;
   char			*groupfile;
   FILE    		*tmpfile;
   int			do_it;
   t_attr		attr;
   int			group;
   int			trace;
   int			rval  = 0;

   DB_ENTER( "Exec_commands" );

   attr  = Glob_attr | cp->ce_attr;
   trace = Trace || !(attr & A_SILENT);
   group = cp->ce_flag & F_GROUP;

   /* Do it again here for those that call us from places other than Make()
    * above. */
   orp = _recipes[ RP_RECIPE ];
   _recipes[ RP_RECIPE ] = cp->ce_recipe;

   if( group ) {
      /* Leave this assignment of Current_target here.  It is needed just
       * incase the user hits ^C after the tempfile for the group recipe
       * has been opened. */
      Current_target = cp;
      trace  = Trace || !(attr & A_SILENT);

      if( !Trace ) tmpfile = Start_temp( Grp_suff, cp, &groupfile );
      if( trace )  fputs( "[\n", stdout );

      /* Emit group prolog */
      if( attr & A_PROLOG )
         _append_file( _recipes[RP_GPPROLOG], tmpfile, cp->CE_NAME, trace );
   }

   if( !useshell  ) useshell=Def_macro("USESHELL",NIL(char),M_MULTI|M_EXPANDED);
   if( !read_cmnd ) {command = GET_MACRO("COMMAND"); read_cmnd = 1;}

   /* Process commands in recipe. If in group, merely append to file.
    * Otherwise, run them.  */
   for( rp = _recipes[RP_RECIPE]; rp != NIL(STRING); rp=rp->st_next,FREE(cmnd)){
      t_attr a_attr = A_DEFAULT;
      t_attr l_attr;
      char   *p;
      int    new_attr = FALSE;
      int    shell;

      /* Reset it for each recipe line otherwise tempfiles don't get removed.
       * Since processing of $(mktmp ...) depends on Current_target being
       * correctly set. */
      Current_target = cp;

      /* Only check for +,-,%,@ if the recipe line begins with a '$' macro
       * expansion.  Otherwise there is no way it is going to find these
       * now. */
      if( *rp->st_string == '$' && !group ) {
         t_attr s_attr = Glob_attr;
	 Glob_attr |= A_SILENT;
	 Suppress_temp_file = TRUE;
	 cmnd = Expand(rp->st_string);
	 Suppress_temp_file = FALSE;
	 a_attr |= Rcp_attribute(cmnd);
	 FREE(cmnd);
	 ++new_attr;
	 Glob_attr = s_attr;
      }

      l_attr = attr|a_attr|rp->st_attr;
      shell  = ((l_attr & A_SHELL) != 0);
      useshell->ht_value = (group||shell)?"yes":"no";

      cmnd = Expand( rp->st_string );

      if( new_attr ) {
	 char *ecmnd = cmnd;
	 cmnd = _strdup(_strspn(cmnd, " \t\n+-@%"));
	 FREE(ecmnd);
      }

      /* COMMAND macro is set to "$(CMNDNAME) $(CMNDARGS)" by default, it is
       * possible for the user to reset it to, for example
       *	COMMAND = $(CMNDNAME) @$(mktmp $(CMNDARGS))
       * in order to get a different interface for his command execution. */
      if( command != NIL(HASH) && !group && *(p = _strpbrk(cmnd, " \t\n")) ) {
	 char *cname = cmnd;

	 *p = '\0';
	 (void) Def_macro("CMNDNAME",cname,M_MULTI|M_EXPANDED);
	 (void) Def_macro("CMNDARGS",p+1,M_MULTI|M_EXPANDED);

	 cmnd = Expand("$(COMMAND)");
	 FREE(cname); 			/* cname == cmnd at this point. */
      }

      Swap_on_exec = ((l_attr & A_SWAP) != 0);	  /* Swapping for DOS only */
      do_it = !Trace;

      if( !group && Trace && _strstr(rp->st_string,"$(MAKE)") ) {
	 Wait_for_completion |= Trace;
	 do_it = TRUE;
      }

      if( group )
         Append_line( cmnd, TRUE, tmpfile, cp->CE_NAME, trace, 0 );
      else {
	 if( *_strspn(cmnd, " \t") != '\0' )
	    Print_cmnd(cmnd, !(do_it && (l_attr & A_SILENT)), 0);
	 else
	    do_it = FALSE;

	 rval=Do_cmnd(cmnd,FALSE,do_it,cp,(l_attr&A_IGNORE)!=0, shell,
		      rp->st_next == NIL(STRING) );
      }
   }

   /* If it is a group then output the EPILOG if required and possibly
    * execute the command */
   if( group ) {
      if( attr & A_EPILOG )	/* emit epilog */
	 _append_file( _recipes[RP_GPEPILOG], tmpfile, cp->CE_NAME, trace );

      if( trace ) fputs("]\n", stdout);

      if( do_it = !Trace ) Close_temp( cp, tmpfile );
      rval = Do_cmnd(groupfile, TRUE, do_it, cp, (attr & A_IGNORE)!=0,
		     TRUE, TRUE);
   }

   Wait_for_completion = FALSE;
   _recipes[ RP_RECIPE ] = orp;
   DB_RETURN( rval );
}


PUBLIC void
Print_cmnd( cmnd, echo, map )/*
================================
   This routine is called to print out the command to stdout.  If echo is
   false the printing to stdout is supressed, but the new lines in the command
   are still deleted. */
char *cmnd;
int  echo;
int  map;
{
   register char *p;
   register char *n;
   char tmp[3];

   DB_ENTER( "Print_cmnd" );

   if( echo ) {
      printf( "%s\n", cmnd  );
      fflush(stdout);
   }

   tmp[0] = ESCAPE_CHAR;
   tmp[1] = CONTINUATION_CHAR;
   tmp[2] = '\0';

   for( p=cmnd; *(n = _strpbrk(p,tmp)) != '\0'; )
      if(*n == CONTINUATION_CHAR && n[1] == '\n') {
	 DB_PRINT( "make", ("fixing [%s]", p) );
	 strcpy( n, n+2 );
	 p = n;
      }
      else {
         if( *n == ESCAPE_CHAR && map ) Map_esc( n );
	 p = n+1;
      }

   DB_VOID_RETURN;
}



/* These routines are used to maintain a stack of directories when making
 * the targets.  If a target cd's to the directory then it is assumed that
 * it will undo it when it is finished making itself. */

static STRINGPTR dir_stack = NIL(STRING);

int
Push_dir( dir, name, ignore )/*
===============================
   Change the current working directory to dir and save the current
   working directory on the stack so that we can come back.
   
   If ignore is TRUE then do not complain about _ch_dir if not possible.*/
char *dir;
char *name;
int  ignore;
{
   STRINGPTR   new_dir;

   DB_ENTER( "Push_dir" );

   if( dir == NIL(char)  || *dir == '\0' ) dir = Pwd;
   if( *dir == '\'' && dir[strlen(dir)-1] == '\'' ) {
      dir = _strdup(dir+1);
      dir[strlen(dir)-1]='\0';
   }
   else
      dir = Expand(dir);

   if( Set_dir(dir) ) {
      if( !ignore )
         Fatal( "Unable to change to directory `%s', target is [%s]",
	        dir, name );
      FREE(dir);
      DB_RETURN( 0 );
   }

   DB_PRINT( "dir", ("Push: [%s]", dir) );
   if( Verbose & V_PRINT_DIR )
      printf( "%s:  Changed to directory [%s]\n", Pname, dir  );

   FREE( dir );
   TALLOC( new_dir, 1, STRING );
   new_dir->st_next   = dir_stack;
   dir_stack          = new_dir;
   new_dir->st_string = _strdup( Pwd );

   Def_macro( "PWD", Get_current_dir(), M_MULTI | M_EXPANDED );
   _set_tmd();

   DB_RETURN( 1 );
}



PUBLIC void
Pop_dir(ignore)/*
=================
   Change the current working directory to the previous saved dir. */
int ignore;
{
   STRINGPTR old_dir;
   char      *dir;

   DB_ENTER( "Pop_dir" );

   if( dir_stack == NIL(STRING) )
      if( ignore ) {
         DB_VOID_RETURN;
      }
      else
	 Error( "Directory stack empty for return from .SETDIR" );

   if( Set_dir(dir = dir_stack->st_string) )
      Fatal( "Could not change to directory `%s'", dir );

   Def_macro( "PWD", dir, M_MULTI | M_EXPANDED );
   DB_PRINT( "dir", ("Pop: [%s]", dir) );
   if( Verbose & V_PRINT_DIR )
      printf( "%s:  Changed back to directory [%s]\n", Pname, dir);

   old_dir   = dir_stack;
   dir_stack = dir_stack->st_next;

   FREE( old_dir->st_string );
   FREE( old_dir );
   _set_tmd();

   DB_VOID_RETURN;
}



static void
_set_tmd()/*
============
   Set the TWD Macro */
{
   TKSTR md, pd;
   char  *m, *p;
   char  *tmd;
   int   is_sep;
   int   first = 1;

   SET_TOKEN( &md, Makedir );
   SET_TOKEN( &pd, Pwd );

   m = Get_token( &md, DirBrkStr, FALSE );
   (void) Get_token( &pd, DirBrkStr, FALSE );
   is_sep = (strchr(DirBrkStr, *m) != NIL(char));
   tmd = _strdup( "" );

   do {
      m = Get_token( &md, DirBrkStr, FALSE );
      p = Get_token( &pd, DirBrkStr, FALSE );

      if( !is_sep && strcmp(m, p) ) {	/* they differ */
	 char *tmp;
	 if( first ) {		/* They differ in the first component	*/
	    tmd = Makedir;	/* In this case use the full path	*/
	    break;
	 }

	 if( *p ) tmp = Build_path( "..", tmd );
	 if( *m ) tmp = Build_path( tmd, m );
	 FREE( tmd );
	 tmd = _strdup( tmp );
      }

      is_sep = 1-is_sep;
      first  = 0;
   } while (*m || *p);

   CLEAR_TOKEN( &md );
   CLEAR_TOKEN( &pd );

   Def_macro( "TMD", tmd, M_MULTI | M_EXPANDED );
   if( tmd != Makedir ) FREE( tmd );
}


static void
_set_recipe( target, ind )/*
============================
   Set up the _recipes static variable so that the slot passed in points
   at the rules corresponding to the target supplied. */
char *target;
int  ind;
{
   CELLPTR cp;
   HASHPTR hp;

   if( (hp = Get_name(target, Defs, FALSE)) != NIL(HASH) ) {
      cp = hp->CP_OWNR;
      _recipes[ ind ] = cp->ce_recipe;
   }
   else
      _recipes[ ind ] = NIL(STRING);
}



PUBLIC void
Append_line( cmnd, newline, tmpfile, name, printit, map )
char *cmnd;
int  newline;
FILE *tmpfile;
char *name;
int  printit;
int  map;
{
   Print_cmnd( cmnd, printit, map );

   if( Trace ) return;

   fputs(cmnd, tmpfile);
   if( newline ) fputc('\n', tmpfile);
   fflush(tmpfile);

   if( ferror(tmpfile) )
      Fatal("Write error on temporary file, while processing `%s'", name);
}



static void
_append_file( rp, tmpfile, name, printit )
register STRINGPTR rp;
FILE 		   *tmpfile;
char 		   *name;
int 		   printit;
{
   char *cmnd;

   while( rp != NIL(STRING) ) {
      Append_line(cmnd = Expand(rp->st_string), TRUE, tmpfile, name, printit,0);
      FREE(cmnd);
      rp = rp->st_next;
   }
}


#define NUM_BUCKETS	20

typedef struct strpool {
   char 	  *string;	/* a pointer to the string value */
   uint32	  keyval;	/* the strings hash value	 */
   struct strpool *next;	/* hash table link pointer	 */
} POOL, *POOLPTR;

static POOLPTR strings[ NUM_BUCKETS ];

static char *
_pool_lookup( str )/*
=====================
   Scan down the list of chained strings and see if one of them matches
   the string we are looking for. */
char    *str;
{
   register POOLPTR key;
   uint32   keyval;
   uint16   hv;
   uint16   keyindex;
   char     *string;

   DB_ENTER( "_pool_lookup" );

   if( str == NIL(char) ) DB_RETURN("");

   hv  = Hash(str, &keyval);
   key = strings[ keyindex = (hv % NUM_BUCKETS) ];

   while( key != NIL(POOL) )
      if( (key->keyval != keyval) || strcmp(str, key->string) )
	 key = key->next;
      else
	 break;

   if( key == NIL(POOL) ) {
      DB_PRINT( "pool", ("Adding string [%s]", str) );
      TALLOC( key, 1, POOL );			/* not found so add string */
      
      key->string = string = _strdup(str);
      key->keyval = keyval;

      key->next           = strings[ keyindex ];
      strings[ keyindex ] = key;
   }
   else {
      DB_PRINT( "pool", ("Found string [%s], key->string") );
      string = key->string;
   }

   DB_RETURN( string );
}
