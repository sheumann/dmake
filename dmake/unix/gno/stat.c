#ifdef __CCFRONT__
#include <14:pragma.h>
#endif
/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/RCS/stat.c,v 1.1 1992/01/24 03:27:58 dvadura Exp $
-- SYNOPSIS -- bind a target name to a file.
-- 
-- DESCRIPTION
--	This file contains the code to go and stat a target.  The stat rules
--	follow a predefined order defined in the comment for Stat_target.
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
--     $Log: stat.c,v $
 * Revision 1.1  1992/01/24  03:27:58  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"


static	int	_check_dir_list ANSI((CELLPTR, CELLPTR, int));

#ifdef DBUG
   /* Just a little ditty for debugging this thing */
   static time_t
   _do_stat( char *name, char *lib, char **sym )
   {
      time_t res;
      DB_ENTER( "_do_stat" );

      res = Do_stat(name, lib, sym);
      DB_PRINT( "stat", ("Statted [%s,%s,%d,%ld]", name, lib, sym, res) );

      DB_RETURN( res );
   }   
#define DO_STAT(A,B,C)  _do_stat(A,B,C)
#else
#define DO_STAT(A,B,C)  Do_stat(A,B,C)
#endif

static char *_first;	/* local storage of first attempted path */

PUBLIC void
Stat_target( CELLPTR cp, int setfname )/*
=============================
	Stat a target.  When doing so follow the following rules, suppose
	that cp->CE_NAME points at a target called fred.o:

		0.      If A_SYMBOL attribute set look into the library
			then do the steps 1 thru 4 on the resulting name.
		1.	Try path's obtained by prepending any dirs found as
			prerequisites for .SOURCE.o.
		2.	If not found, do same as 2 but use .SOURCE
		3.	If not found and .LIBRARYM attribute for the target is
			set then look for it in the corresponding library.
	   4.	If found in step 0 thru 3, then ce_fname points at
			file name associate with target, else ce_fname points
			at a file name built by the first .SOURCE* dir that
			applied. */

{
   register HASHPTR hp;
   static   HASHPTR srchp = NIL(HASH);
   char		    *name;
   char		    *tmp;
   int		    res = 0;

   DB_ENTER( "Stat_target" );

   name = cp->CE_NAME;
   if( srchp == NIL(HASH) ) srchp = Get_name(".SOURCE",Defs,FALSE);

   /* Look for a symbol of the form lib((symbol)) the name of the symbol
    * as entered in the hash table is (symbol) so pull out symbol and try
    * to find it's module.  If successful DO_STAT will return the module
    * as well as the archive member name (pointed at by tmp).  We then
    * replace the symbol name with the archive member name so that we
    * have the proper name for any future refrences. */

   if( cp->ce_attr & A_SYMBOL ) {
      DB_PRINT( "stat", ("Binding lib symbol [%s]", name) );

      cp->ce_time = DO_STAT( name, cp->ce_lib, &tmp );

      if( cp->ce_time != (time_t) 0L ) {
	 /* stat the new member name below  note tmp must point at a string
	  * returned by MALLOC... ie. the Do_stat code should use _strdup */

	 if( Verbose & V_MAKE )
	    printf( "%s:  Mapped ((%s)) to %s(%s)\n", Pname,
		     name, cp->ce_lib, tmp );

         FREE( name );		
	 name = cp->CE_NAME = tmp;		
	 cp->ce_attr &= ~(A_FFNAME | A_SYMBOL);
      }
      else
         { DB_VOID_RETURN; }
   }

   _first = NIL(char);
   tmp = _strjoin( ".SOURCE", Get_suffix( name ), -1, FALSE);

   /* Check .SOURCE.xxx target */
   if( (hp = Get_name(tmp, Defs, FALSE)) != NIL(HASH) )
      res = _check_dir_list( cp, hp->CP_OWNR, setfname );

   /* Check just .SOURCE */
   if( !res && (srchp != NIL(HASH)) )
      res = _check_dir_list( cp, srchp->CP_OWNR, setfname );

   /* If libmember and we haven't found it check the library */
   if( !res && (cp->ce_attr & A_LIBRARYM) ) {
      cp->ce_time = DO_STAT(name, cp->ce_lib, NIL(char *));

      if( !cp->ce_time && Tmd && *Tmd && cp->ce_lib ) {
	 cp->ce_lib=Build_path(Tmd,cp->ce_lib);
	 cp->ce_time = DO_STAT(name, cp->ce_lib, NIL(char *));
      }

      if( Verbose & V_MAKE )
	 printf( "%s:  Checking library '%s' for member [%s], time %ld\n",
		 Pname, cp->ce_lib, name, cp->ce_time );
   }

   FREE( tmp );

   if( setfname == 1 || (setfname == -1 && cp->ce_time != (time_t)0L) ) {
      if( (cp->ce_attr & A_FFNAME) && (cp->ce_fname != NIL(char)) )
	 FREE( cp->ce_fname );

      if( _first != NIL(char) ) {
	 cp->ce_fname = _first;
	 cp->ce_attr |= A_FFNAME;
      }
      else {
	 cp->ce_fname = cp->CE_NAME;
	 cp->ce_attr &= ~A_FFNAME;
      }
   }
   else if( _first )
      FREE( _first );

   /* set it as stated only if successful, this way, we shall try again
    * later. */
   if( cp->ce_time != (time_t)0L ) cp->ce_flag |= F_STAT;

   DB_VOID_RETURN;
}



static int
_check_dir_list( CELLPTR cp, CELLPTR sp, int setfname )/*
=====================================
	Check the list of dir's given by the prerequisite list of sp, for a
	file pointed at by cp.  Returns 0 if path not bound, else returns
	1 and replaces old name for cell with new cell name. */

{
   register LINKPTR lp;
   char *dir;
   char *path;
   char *name;
   int  res  = 0;
   int  fset = 0;

   DB_ENTER( "_check_dir_list" );
   DB_PRINT( "mem", ("%s:-> mem %ld", cp->CE_NAME, (long) coreleft()) );

	if( sp->ce_prq != NIL(LINK) )	/* check prerequisites if any */
   {
      /* Use the real name instead of basename, this prevents silly
       * loops in inference code, and is consistent with man page */
   	name = cp->CE_NAME;

      /* Here we loop through each directory on the list, and try to stat
       * the target.  We always save the first pathname we try and stat in
       * _first.  If we subsequently get a match we then replace the value of
       * _first by the matched path name.  */

      for( lp=sp->CE_PRQ; lp != NIL(LINK) && !res; lp=lp->cl_next ) {
	 		int  nodup = 0;
	 		dir  = lp->cl_prq->CE_NAME;

	 		if( strchr( dir, '$' ) ) dir = Expand(dir);
	 		if( strcmp( dir, ".NULL" ) == 0 ) {
	    		nodup = 1;
	    		path = cp->CE_NAME;
	 		}
	 		else
	    		path = Build_path( dir, name );

	 		res = ((cp->ce_time = DO_STAT(path,NIL(char),NIL(char *))) != (time_t)0L);
#if 0
I think this will break a lot of things!
	 		/* It didn't work and TMD macro has a value so try to stat it
	  		 * relative to the original MAKEDIR directory. */
	 		if( Tmd && !*Tmd && !res ) {
	    		char *p = _strdup(path);
	    		path = Build_path(Tmd,p); FREE(p);
	    		res = ((cp->ce_time = DO_STAT(path,NIL(char),NIL(char *))) != (time_t)0L);
	 		}
#endif

	 		/* Have to use _strdup to set _first since Build_path, builds it's
	  		 * path names inside a static buffer. */
	 		if( setfname )
	    	if( (_first == NIL(char) && !fset) || res ) {
	       	if( _first != NIL(char) ) FREE( _first );
	       	_first = nodup ? NIL(char) : _strdup(path);
	       	fset = 1;
	    	}

	 		DB_PRINT( "stat", ("_first [%s], path [%s]", _first, path) );
	 		if( dir != lp->cl_prq->CE_NAME )  FREE(dir);
      }
   }

   DB_PRINT( "mem", ("%s:-< mem %ld", cp->CE_NAME, (long) coreleft()) );
   DB_RETURN( res );
}
