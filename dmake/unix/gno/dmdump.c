#ifdef __CCFRONT__
#include <14:pragma.h>
#endif
/* RCS      -- $Header: /u/dvadura/src/generic/dmake/src/RCS/dmdump.c,v 1.2 1992/04/07 04:42:46 dvadura Exp $
-- SYNOPSIS -- dump the internal dag to stdout.
-- 
-- DESCRIPTION
--	This file contains the routine that is called to dump a version of
--	the digested makefile to the standard output.  May be useful perhaps
--	to the ordinary user, and invaluable for debugging make.
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
--     $Log: dmdump.c,v $
 * Revision 1.2  1992/04/07  04:42:46  dvadura
 * Deleted define of unused variable.
 *
 * Revision 1.1  1992/01/24  03:28:51  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"

#define M_TEST	(M_PRECIOUS | M_VAR_MASK)

static	void	dump_name ANSI((HASHPTR, int));
static	void	dump_normal_target ANSI((CELLPTR, int));
static  void    dump_prerequisites ANSI((LINKPTR, int, int, int));


PUBLIC void
Dump(void)/*
========  Dump onto standard output the digested makefile.  Note that
	  the form of the dump is not representative of the contents
	  of the original makefile contents at all */
{
   HASHPTR      hp;
   int          i;

   DB_ENTER( "Dump" );

   puts( "# Dump of dmake macro variables:" );
   for( i=0; i<HASH_TABLE_SIZE; i++)
      for( hp=Macs[i]; hp != NIL(HASH); hp = hp->ht_next ) {
	 int flag = hp->ht_flag;

	 printf( "%s ", hp->ht_name );
	 if( flag & M_EXPANDED ) putchar( ':' );
	 printf( "= " );
	 if( hp->ht_value != NIL(char) ) printf( "%s",hp->ht_value );
	 if( flag & M_PRECIOUS )
	    printf( "\t # PRECIOUS " );
	 putchar( '\n' );
      }

   puts( "\n#====================================" );
   puts( "# Dump of targets:\n" );

   for( i=0; i<HASH_TABLE_SIZE; i++ )
      for( hp = Defs[i]; hp != NIL(HASH); hp = hp->ht_next )
         if( !(hp->CP_OWNR->ce_flag & F_PERCENT) ) {
	    if( Root->ce_prq && hp->CP_OWNR == Root->ce_prq->cl_prq )
	       puts( "# ******* FIRST TARGET ********" );
	    dump_normal_target( hp->CP_OWNR, hp->CP_OWNR->ce_flag );
	 }

   puts( "\n#====================================" );
   puts( "# Dump of inference graph\n" );

   for( i=0; i<HASH_TABLE_SIZE; i++ )
      for( hp = Defs[i]; hp != NIL(HASH); hp = hp->ht_next )
         if( (hp->CP_OWNR->ce_flag & F_PERCENT) &&
	    !(hp->CP_OWNR->ce_flag & F_MAGIC) )
	    dump_normal_target( hp->CP_OWNR, hp->CP_OWNR->ce_flag );

   DB_VOID_RETURN;
}



PUBLIC void
Dump_recipe( STRINGPTR sp )/*
===================
   Given a string pointer print the recipe line out */
{
   char *st;
   char *nl;

   if( sp == NIL(STRING) ) return;

   putchar( '\t' );
   if( sp->st_attr & A_SILENT ) putchar( '@' );
   if( sp->st_attr & A_IGNORE ) putchar( '-' );
   if( sp->st_attr & A_SHELL  ) putchar( '+' );
   if( sp->st_attr & A_SWAP   ) putchar( '%' );

   st = sp->st_string;
   for( nl=strchr(st,'\n'); nl != NIL( char); nl=strchr(st,'\n') ) {
      *nl = '\0';
      printf( "%s\\\n", st );
      *nl = '\n';
      st  = nl+1;
   }
   printf( "%s\n", st );
}


static char *_attrs[] = { ".PRECIOUS", ".SILENT", ".LIBRARY",
   ".EPILOG", ".PROLOG", ".IGNORE", ".SYMBOL", ".NOINFER",
   ".UPDATEALL", ".SEQUENTIAL", ".SETDIR=", ".USESHELL", ".SWAP", ".MKSARGS",
   ".PHONY", ".NOSTATE" };

static void
dump_normal_target( CELLPTR cp, int flag )/*
================================
	Dump in makefile like format the dag information */
{
   register STRINGPTR sp;
   t_attr	      attr;
   unsigned int	      k;

   DB_ENTER( "dump_normal_target" );

   if( !(cp->ce_flag & F_TARGET) && !cp->ce_attr ) { DB_VOID_RETURN; }

   if( cp->ce_flag & F_MULTI ) {
      int tflag = cp->ce_prq->cl_prq->ce_flag;
      if( !(cp->ce_flag & F_PERCENT) ) tflag |= F_MULTI;
      dump_prerequisites(cp->ce_prq, FALSE, TRUE, tflag);
   }
   else {
      dump_name( cp->ce_name, FALSE );

      for( k=0, attr=1; attr <= MAX_ATTR; attr <<= 1, k++ )
	 if( cp->ce_attr & attr ) {
	    printf( "%s%s ", _attrs[k],
		    (attr != A_SETDIR) ? "" : (cp->ce_dir?cp->ce_dir:"") );
	 }
	    
      putchar( ':' );

      if( flag & F_MULTI )  putchar( ':' );
      if( flag & F_SINGLE ) putchar( '!' );
      putchar( ' ' );

      dump_prerequisites( cp->ce_prq, FALSE, FALSE, F_DEFAULT);
      dump_prerequisites( cp->ce_indprq, TRUE, FALSE, F_DEFAULT);

      putchar( '\n' );
      if( cp->ce_flag & F_GROUP ) puts( "[" );
      for( sp = cp->ce_recipe; sp != NIL(STRING); sp = sp->st_next )
	 Dump_recipe( sp );
      if( cp->ce_flag & F_GROUP ) puts( "]" );

      putchar( '\n' );
   }

   DB_VOID_RETURN;
}


static void
dump_prerequisites(
LINKPTR lp,
int     quote,
int     recurse,
int     flag)
{
   for( ; lp; lp=lp->cl_next )
      if( recurse )
	 dump_normal_target(lp->cl_prq, flag);
      else if( lp->cl_prq )
	 dump_name(lp->cl_prq->ce_name, quote);
}


static void
dump_name( HASHPTR hp, int quote )/*
========================
	print out a name */
{
   if( quote ) putchar('\'');
   printf( "%s", hp->ht_name );
   if( quote ) putchar('\'');
   putchar(' ');
}
