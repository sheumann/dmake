#ifdef __CCFRONT__
#include <14:pragma.h>
#endif
/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/RCS/parse.c,v 1.1 1992/01/24 03:26:53 dvadura Exp $
-- SYNOPSIS -- parse the input, and perform semantic analysis
-- 
-- DESCRIPTION
-- 	This file contains the routines that parse the input makefile and
--	call the appropriate routines to perform the semantic analysis and
--	build the internal dag.
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
--     $Log: parse.c,v $
 * Revision 1.1  1992/01/24  03:26:53  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"


PUBLIC void
Parse( FILE *fil )/*
==============  Parse the makefile input */
{
   int  rule  = FALSE;                 /* have seen a recipe line        */
   char *p;			       /* termporary pointer into Buffer */

   DB_ENTER( "Parse" );

   State = NORMAL_SCAN;
   Group = FALSE;                 /* true if scanning a group rcpe  */
   while( TRUE ) {
      if( Get_line( Buffer, fil ) ) {
	 		if( fil != NIL( FILE ) )               /* end of parsable input */
	    		Closefile();

	 		Bind_rules_to_targets( F_DEFAULT );
         if( Group )  Fatal( "Incomplete rule recipe group detected" );

	 		DB_VOID_RETURN;
      }
      else {
      	switch( State ) {
	    	case RULE_SCAN:

	      	/* Check for the `[' that starts off a group rule definition.  It
	       	 * must appear as the first non-white space
				 * character in the line. */

	       	p = _strspn( Buffer, " \t\r\n" );
            if( Set_group_attributes( p ) ) {
            	if( rule && Group )
               	Fatal( "Cannot mix single and group recipe lines" );
               else
               	Group = TRUE;

               rule = TRUE;

               break;                     /* ignore the group start  */
           	}

            if( Group ) {
            	if( *p != ']' ) {
               	Add_recipe_to_list( Buffer, TRUE, TRUE );
                 	rule = TRUE;
               }
               else
              		State = NORMAL_SCAN;
            }
            else {
            	if(    *Buffer == '\t' || (Notabs && *Buffer == ' ') ) {
               	Add_recipe_to_list( Buffer, FALSE, FALSE );
                  rule = TRUE;
               }
               else if( *p == ']' )
               	Fatal( "Found unmatched ']'" );
               else if( (*Buffer && *p) || (Notabs && !*Buffer && !*p))
		     			State = NORMAL_SCAN;
           	}
 
            if( State == RULE_SCAN ) break;     /* ie. keep going    */
               
	       	Bind_rules_to_targets( (Group) ? F_GROUP: F_DEFAULT );

            rule = FALSE;
            if( Group ) {
            	Group = FALSE;
               break;
            }
	       	/*FALLTRHOUGH*/

         	/* In this case we broke out of the rule scan because we do not
             * have a recipe line that begins with a <TAB>, so lets
				 * try to scan the thing as a macro or rule definition. */

	    	case NORMAL_SCAN:
	      	if( !*Buffer ) continue;         /* we have null input line */

	       	/* STUPID AUGMAKE uses "include" at the start of a line as
	        	 * a signal to include a new file, so let's look for it.
				 * if we see it replace it by .INCLUDE: and stick this back
				 * into the buffer. */
	       	if( !strncmp( "include", Buffer, 7 ) &&
		   		(Buffer[7] == ' ' || Buffer[7] == '\t') )
	       		{
		  				char *tmp;

		  				tmp = _strjoin( ".INCLUDE:", Buffer+7, -1, FALSE );
		  				strcpy( Buffer, tmp );
		  				FREE( tmp );
	       	}

            /* look for a macro definition, they all contain an = sign
	        	 * if we fail to recognize it as a legal macro op then try to
				 * parse the same line as a rule definition, it's one or the
				 * other */
		
	       	if( Parse_macro(Buffer, M_DEFAULT) ) break;/* it's a macro def */
	       	if( Parse_rule_def( &State ) ) 	    break;/* it's a rule def  */

	       	/* if just blank line then ignore it */
	       	if( *_strspn( Buffer, " \t\r\n" ) == '\0' ) break;
	       
	       	/* otherwise assume it was a line of unrecognized input, or a
	        	 * recipe line out of place so print a message */
		
	       	Fatal( "Expecting macro or rule defn, found neither" );
	       	break;

	    	default:
	      	Fatal( "Internal -- UNKNOWN Parser state %d", State );
	 		}
     	}
   }
}
