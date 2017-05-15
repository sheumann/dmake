#ifdef __CCFRONT__
#include <14:pragma.h>
#endif
/* RCS      -- $Header: /u/dvadura/src/generic/dmake/src/RCS/getinp.c,v 1.2 1992/04/07 04:42:46 dvadura Exp $
-- SYNOPSIS -- handle reading of input.
-- 
-- DESCRIPTION
--	The code in this file reads the input from the specified stream
--	into the provided buffer of size Buffer_size.  In doing so it deletes
--	comments.  Comments are delimited by the #, and
--	<nl> character sequences.  An exception is \# which
--	is replaced by # in the input.  Line continuations are signalled
--	at the end of a line and are recognized inside comments.
--	The line continuation is always  <\><nl>.
--
--	If the file to read is NIL(FILE) then the Get_line routine returns the
--	next rule from the builtin rule table if there is one.
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
--     $Log: getinp.c,v $
 * Revision 1.2  1992/04/07  04:42:46  dvadura
 * Minor clean up and a check for -X flag on the execution of #! lines.
 *
 * Revision 1.1  1992/01/24  03:27:56  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"

#define IS_WHITE(A)  ((A == ' ') || (A == '\t') || (A == '\n') || (A == '\r'))
#define SCAN_WHITE(A) \
    while( IS_WHITE(*A) ) A++;

static	int	_is_conditional ANSI((char*));
static	int	_handle_conditional ANSI((int, TKSTRPTR));

static int  rule_ind = 0;	/* index of rule when reading Rule_tab	 */
static int  skip = FALSE;	/* if true the skip input		 */

PUBLIC int
Get_line( char *buf, FILE *fil )/*
======================
		Read a line of input from the file stripping
		off comments.  The routine returns TRUE if EOF */
{
   register char *p;
   register char *c;
   char 	 *q;
   char	    	 *buf_org;
   static   int	 ignore = FALSE;
   int 		 cont   = FALSE;
   int 		 pos    = 0;
   int		 res;

   DB_ENTER( "Get_line" );

   if( fil == NIL(FILE) ) {
      /* Reading the internal rule table.  Set the rule_index to zero.
       * This way ReadEnvironment works as expected every time. */

      while( (p = Rule_tab[ rule_ind++ ]) != NIL(char) )
	 /* The last test in this if '*p != '~', handles the environment
	  * passing conventions used by MKS to pass arguments.  We want to
	  * skip those environment entries. */
	 if( !Readenv || (Readenv && (strchr(p,'=') != NIL(char)) && *p!='~')){
	    strcpy( buf, p );

	    DB_PRINT( "io", ("Returning [%s]", buf) );
	    DB_RETURN( FALSE );
	 }

      rule_ind = 0;

      DB_PRINT( "io", ("Done Ruletab") );
      DB_RETURN( TRUE );
   }

   buf_org = buf;

do_again:
   do {
      p = buf+pos;
      if(feof( fil ) || (fgets( p, Buffer_size-pos, fil ) == NIL(char)))
	 DB_RETURN( TRUE );

      Line_number++;

      /* ignore input if ignore flag set and line ends in a continuation
	 character. */
      q = p+strlen(p)-2;
      if( q<p ) q=p;

      /* ignore each RETURN at the end of a line before any further
       * processing */
      if( q[0] == '\r' && q[1] == '\n' ) {
	 q[0] = '\n';
	 q[1] = '\0';
	 q--;
      }
      /* you also have to deal with END_OF_FILE chars to process raw
       * DOS-Files. Normally they are the last chars in file, but after
       * working on these file with vi, there is an additional NEWLINE
       * after the last END_OF_FILE. So if the second last char in the
       * actual line is END_OF_FILE, you can skip the last char. Then
       * you can search the line back until you find no more END_OF_FILE
       * and nuke each you found by string termination. */
      if( q[0] == '\032' )
	 q--;
      while( q[1] == '\032' ) {
	 q[1] = '\0';
	 q--;
      }

      if( ignore ) {
	 if( q[0] != CONTINUATION_CHAR || q[1] != '\n' )  ignore = FALSE;
	 *p = '\0';
	 continue;
      }

      c = Do_comment(p, &q, Group || (*buf == '\t'));
      
      /* Does the end of the line end in a continuation sequence? */
      
      if( (q[0] == CONTINUATION_CHAR) && (q[1] == '\n')) {
	 /* If the continuation was at the end of a comment then ignore the
	  * next input line, (or lines until we get one ending in just <nl>)
	  * else it's a continuation, so build the input line from several
	  * text lines on input.  The maximum size of this is governened by
	  * Buffer_size */
	 if( q != p && q[-1] == CONTINUATION_CHAR ) {
	    strcpy( q, q+1 );
	    q--;
	    cont = FALSE;
	 }
	 else if( c != NIL(char) )
	    ignore = TRUE;
	 else
	    cont   = TRUE;
      }
      else {
	 cont = FALSE;
      }

      q    = ( c == NIL(char) ) ? q+2 : c;
      pos += q-p;
   }
   while( (cont || !*buf) && (pos <= Buffer_size) );

   if( buf[ pos-1 ] == '\n' )
      buf[ --pos ] = '\0';
   else
      if( pos == Buffer_size-1 )
	 Fatal( "Input line too long, increase MAXLINELENGTH" );


   /* Now that we have the next line of input to make, we should check to
    * see if it is a conditional expression.  If it is then process it,
    * otherwise pass it on to the parser. */

   if( *(p = _strspn(buf, " \t\r\n")) == CONDSTART ) {
      TKSTR token;

      SET_TOKEN( &token, p );

      p = Get_token( &token, "", FALSE );

      if( (res = _is_conditional( p )) )	/* ignore non control special */
      {						/* targets 		      */
	 res  = _handle_conditional( res, &token );
	 skip = TRUE;
      }
      else {
	 CLEAR_TOKEN( &token );
	 res  = TRUE;
      }
   }

   if( skip ) {
      buf  = buf_org;		/* ignore line just read in */
      pos  = 0;
      skip = res;
      goto do_again;
   }

   DB_PRINT( "io", ("Returning [%s]", buf) );
   DB_RETURN( FALSE );
}


PUBLIC char *
Do_comment( char *str, char **pend, int keep)/*
=============================
   Search the input string looking for comment chars.  If it contains
   comment chars then NUKE the remainder of the line, if the comment
   char is preceeded by \ then shift the remainder of the line left
   by one char. */
{
   char *c = str;

   while( (c = strchr(c, COMMENT_CHAR)) != NIL(char) ) {
      if( Comment || State == NORMAL_SCAN )
	 if( c != str && c[-1] == ESCAPE_CHAR ) {
	    strcpy( c-1, c );        /* copy it left, due to \# */
	    if( pend ) (*pend)--;    /* shift tail pointer left */
	 }
	 else {
	    if(    !No_exec
		&& c == str
	        && c[1] == '!'
	        && Line_number == 1
		&& Nestlevel() == 1 ) {
	       char *cmnd;
	     
	       cmnd = Expand(c+2);
	       cmnd[strlen(cmnd)-1] = '\0';	/* strip last newline */
	       Current_target = Root;
	       Swap_on_exec = TRUE;
	       Wait_for_completion = TRUE;
	       Do_cmnd(cmnd, FALSE, TRUE, Current_target, FALSE, FALSE, TRUE);
	    }

	    *c = '\0';               /* a true comment so break */
	    break;
	 }
      else {
         if( keep )
	    c = NIL(char);
	 else
	    *c = '\0';

	 break;
      }
   }

   return(c);
}
      

PUBLIC char *
Get_token( TKSTRPTR string, char *brk, int anchor) /*
==================================
	Return the next token in string.
	Returns empty string when no more tokens in string.
	brk is a list of chars that also cause breaks in addition to space and
	tab, but are themselves returned as tokens.  if brk is NULL then the
	remainder of the line is returned as a single token.
	
	anchor if 1, says break on chars in the brk list, but only if
	the entire token begins with the first char of the brk list, if
	0 then any char of brk will cause a break to occurr.
	
	If anchor is 2, then break only seeing the first char in the break
	list allowing only chars in the break list to form the prefix. */

{
   register char *s;
   register char *curp;
   register char *t;
   int           done = FALSE;
   char          space[10];

   DB_ENTER( "Get_token" );

   s  = string->tk_str;			  /* Get string parameters	*/
   *s = string->tk_cchar;		  /* ... and strip leading w/s	*/

   SCAN_WHITE( s );

   DB_PRINT( "tok", ("What's left [%s]", s) );

   if( !*s ) {
      DB_PRINT( "tok", ("Returning NULL token") );
      DB_RETURN( "" );
   }


   /* Build the space list.  space contains all those chars that may possibly
    * cause breaks.  This includes the brk list as well as white space. */

   if( brk != NIL(char) ) {
      strcpy( space, " \t\r\n" );
      strcat( space, brk   );
   }
   else {
      space[0] = (char) 0xff;      /* a char we know will not show up      */
      space[1] = 0;
   }


   /* Handle processing of quoted tokens.  Note that this is disabled if
    * brk is equal to NIL */

   while( *s == '\"' && ((brk != NIL(char)) || !string->tk_quote) ) {
      s++;
      if( string->tk_quote ) {
	 curp = s-1;
	 do { curp = strchr( curp+1, '\"' ); }
	 while( (curp != NIL(char)) && (*(curp+1) == '\"'));

         if( curp == NIL(char) ) Fatal( "Unmatched quote in token" );
	 string->tk_quote = !string->tk_quote;

	 /* Check for "" case, and if found ignore it */
	 if( curp == s ) continue;
	 goto found_token;
      }
      else
	 SCAN_WHITE( s );

      string->tk_quote = !string->tk_quote;
   }
   

   /* Check for a token break character at the beginning of the token.
    * If found return the next set of break chars as a token. */

   if( anchor == 2 && brk != NIL(char) ) {
      curp = s;
      while( *curp && (strchr(brk,*curp)!=NIL(char)) && (*curp!=*brk) ) curp++;
      done = (*brk == *curp++);
   }
   else if( (brk != NIL(char)) && (strchr( brk, *s ) != NIL(char)) ) {
      curp = _strspn( s, brk );
      done = (anchor == 0) ? TRUE :
	     ((anchor == 1)?(*s == *brk) : (*brk == curp[-1]));
   }


   /* Scan for the next token in the list and return it less the break char
    * that was used to terminate the token.  It will possibly be returned in
    * the next call to Get_token */

   if( !done ) {
      SCAN_WHITE( s );

      t = s;
      do {
	 done = TRUE;
	 curp = _strpbrk(t, space);
	 
	 if( anchor && *curp && !IS_WHITE( *curp ) )
	    if( ((anchor == 1)?*curp:_strspn(curp,brk)[-1]) != *brk ) {
	       t++;
	       done = FALSE;
	    }
      }
      while( !done );

      if( (curp == s) && (strchr(brk, *curp) != NIL(char)) ) curp++;
   }

found_token:
   string->tk_str   = curp;
   string->tk_cchar = *curp;
   *curp = '\0';

   DB_PRINT( "tok", ("Returning [%s]", s) );
   DB_RETURN( s );
}


static int
_is_conditional( char *tg )/*
=======================
	Look at tg and return it's value if it is a conditional identifier
	otherwise return 0. */
{
   DB_ENTER( "_is_conditional" );
   
   tg++;
   switch( *tg ) {
      case 'I': if( !strcmp( tg, "IF" )) DB_RETURN( ST_IF   ); break;
      
      case 'E':
         if( !strcmp( tg, "END" ))	 DB_RETURN( ST_END  );
         else if( !strcmp( tg, "ENDIF")) DB_RETURN( ST_END  );
         else if( !strcmp( tg, "ELSE" )) DB_RETURN( ST_ELSE );
         else if( !strcmp( tg, "ELIF" )) DB_RETURN( ST_ELIF );
	 break;
   }
   
   DB_RETURN( 0 );
}



#define SEEN_END  0x00
#define SEEN_IF   0x01
#define SEEN_ELSE 0x02
#define SEEN_ELIF 0x04

#define ACCEPT_IF   0x10
#define ACCEPT_ELIF 0x20

static int
_handle_conditional( int opcode, TKSTRPTR tg )/*
===================================
	Perform the necessary processing for .IF conditinal targets.
	Someday this should be modified to do bracketted expressions ala
	CPP... sigh */
{
   static short	action[MAX_COND_DEPTH];
   static char  ifcntl[MAX_COND_DEPTH];
   char 	*tok, *lhs, *rhs, *op, *expr;
   int  	result;

   DB_ENTER( "_handle_conditional" );

   switch( opcode ) {
      case ST_ELIF:
         if( !(ifcntl[Nest_level] & SEEN_IF) || (ifcntl[Nest_level]&SEEN_ELSE) )
	    Fatal(".ELIF without a preceeding .IF" );
	 /* FALLTHROUGH */

      case ST_IF:
	 if( opcode == ST_IF && (Nest_level+1) == MAX_COND_DEPTH )
	    Fatal( ".IF .ELSE ... .END nesting too deep" );

	 If_expand = TRUE;
	 expr = Expand( Get_token( tg, NIL(char), FALSE ));
	 If_expand = FALSE;
	 lhs = _strspn( expr, " \t" );
	 if( !*lhs ) lhs = NIL(char);

	 if( (op = _strstr( lhs, "==" )) == NIL(char) )
	    op = _strstr( lhs, "!=" );

	 if( op == NIL(char) )
	    result = (lhs != NIL(char));
	 else {
	    op[1] = op[0];
	    if( lhs != op ) {
	       for( tok = op-1; (tok != lhs) && ((*tok == ' ')||(*tok == '\t'));
	            tok-- );
	       tok[1] = '\0';
	    }
	    else
	       lhs = NIL(char);

	    op++;
	    rhs = _strspn( op+1, " \t" );
	    if( !*rhs ) rhs = NIL(char);

	    if( (rhs == NIL(char)) || (lhs == NIL(char)) )
	       result = (rhs == lhs) ? TRUE : FALSE;
	    else {
	       tok = rhs + strlen( rhs );
	       for( tok=tok-1; (tok != lhs) && ((*tok == ' ')||(*tok == '\t'));
		    tok--);
	       tok[1] = '\0';

	       result = (strcmp( lhs, rhs ) == 0) ? TRUE : FALSE;
	    }

	    if( *op == '!' ) result = !result;
	 }

	 if( expr != NIL(char) ) FREE( expr );

	 if( opcode == ST_IF ) {
	    Nest_level++;
	    action[Nest_level] = 1;
	 }
	 ifcntl[Nest_level] |= (opcode==ST_IF)?SEEN_IF:SEEN_ELIF;

	 if( result ) {
	    if( !(ifcntl[Nest_level] & (ACCEPT_IF|ACCEPT_ELIF)) ) {
	       action[ Nest_level ] = action[ Nest_level-1 ];
	       ifcntl[Nest_level] |= (opcode==ST_IF)?ACCEPT_IF:ACCEPT_ELIF;
	    }
	    else
	       action[Nest_level] = 1;
	 }
	 else
	    action[Nest_level] = 1;
	 break;

      case ST_ELSE:
	 if( Nest_level <= 0 ) Fatal( ".ELSE without .IF" );
	 if( ifcntl[Nest_level] & SEEN_ELSE )
	    Fatal( "Missing .IF or .ELIF before .ELSE" );

	 if( ifcntl[Nest_level] & (ACCEPT_IF|ACCEPT_ELIF) )
	    action[Nest_level] = 1;
	 else if( action[ Nest_level-1 ] != 1 )
	    action[ Nest_level ] ^= 0x1;	/* flip between 0 and 1	*/

	 ifcntl[Nest_level] |= SEEN_ELSE;
	 break;

      case ST_END:
	 ifcntl[Nest_level] = SEEN_END;
	 Nest_level--;
	 if( Nest_level < 0 ) Fatal( "Unmatched .END[IF]" );
	 break;
   }

   DB_RETURN( action[ Nest_level ] );
}
