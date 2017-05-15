/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/RCS/macparse.c,v 1.1 1992/01/24 03:27:52 dvadura Exp $
-- SYNOPSIS -- parse a macro definition
-- 
-- DESCRIPTION
--	This file contains the code that parses a macro definition
--	stored in a buffer.  If the string in buffer is not a valid
--	macro definition the routie Parse_macro returns 0, otherwise it
--	returns 1 to indicate success.
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
--     $Log: macparse.c,v $
 * Revision 1.1  1992/01/24  03:27:52  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"

PUBLIC int
Parse_macro( buffer, flag )/*
=============================
   Parse the string in buffer and define it as a macro if it is a valid macro.
   Note especially the string .SETDIR= since it is an attribute, but looks a
   lot like a macro definition.  This would not be a problem if make used
   white space as token separators, since this is not the case we must do
   something about it. */
char *buffer;
int  flag;
{
   register char *tok1;		/* temporary place to keep a token */
   register char *tok2;		/* temporary place to keep a token */
   char		 *result;	/* temporary pointer for strings   */
   TKSTR          input;        /* place to scan the buffer from   */
   HASHPTR	  hv;		/* pointer to hash table value	   */
   int		  operator;	/* what macro operator do we have  */

   DB_ENTER( "Parse_macro" );

   SET_TOKEN( &input, buffer );
   tok1 = Get_token( &input, "=+:*", 0 );

   if( Macro_op( tok1 ) ) {
      Error( "No macro name" );
      CLEAR_TOKEN( &input );
      DB_RETURN( 1 );
   }

   tok1 = _strdup( tok1 );
   tok2 = Get_token( &input, "=+:*", 2 );
   if( !(operator = Macro_op(tok2)) || 
	  (!strcmp(tok1, ".SETDIR") &&
	     ((operator != M_OP_CL) || (operator != M_OP_PLCL) ||
	      (operator != M_OP_DFCL))) ) {
      CLEAR_TOKEN( &input );
      FREE( tok1 );
      DB_RETURN( 0 );
   }

   tok2 = Expand(tok1); FREE(tok1); tok1 = tok2;
   tok2 = Get_token( &input, NIL( char ), FALSE );

   switch( operator ) {
      case M_OP_PLCL:
	  tok2 = Expand( tok2 );
	  /* Fall thru */

      case M_OP_PL:
	 /* Add to an existing macro, if it is not defined, though, then
	  * just define a new macro */

	 if( (hv = GET_MACRO(tok1)) == NIL(HASH) || hv->ht_value == NIL(char) )
	    Def_macro( tok1, tok2, flag );
	 else {
	    result = _stradd( hv->ht_value, tok2, FALSE );
	    Def_macro( tok1, result, flag );
	    FREE( result );
	 }
	 if( operator == M_OP_PLCL ) FREE(tok2);
	 break;

      case M_OP_DF:
 	 if( (hv = GET_MACRO(tok1)) != NIL(HASH) )
	    break;
	 /* else FALLTHRU */

      case M_OP_EQ:
	 Def_macro( tok1, tok2, flag );
	 break;

      case M_OP_DFCL:
 	 if( (hv = GET_MACRO(tok1)) != NIL(HASH) )
	    break;
	 /* else FALLTHRU */

      case M_OP_CL:
	 /* If the macro we are assigning from is a single control
	  * macro with nothing else, then we propagate the M_MULTI
	  * flag to the macro we are assigning the value to so that
	  * the same macro can be used to do this over and over. */
	 If_multi = 0;
	 tok2 = Expand( tok2 );
	 Def_macro( tok1, tok2, M_EXPANDED | flag | If_multi );
	 FREE( tok2 );
	 break;
   }
	    
   FREE( tok1 );

   DB_RETURN( 1 );
}



PUBLIC int
Macro_op( op )/*
================
   Check the passed in op string and map it to one of the macro operators */
char *op;
{
   int ret = 0;

   DB_ENTER( "macro_op" );

   switch( *op ) {
      case '=': ret = M_OP_EQ; break;
      case ':': ret = M_OP_CL; op++; break;

      case '+':
         ret = M_OP_PL; op++;
         if( *op == ':' ) { ret = M_OP_PLCL; op++; }
         break;

      case '*':
         ret = M_OP_DF; op++;
         if( *op == ':' ) { ret = M_OP_DFCL; op++; }
         break;
   }

   if( *op++ != '=' )
      ret = 0;
   else if( *op != '\0' )
      ret = 0;

   DB_RETURN( ret );
}

