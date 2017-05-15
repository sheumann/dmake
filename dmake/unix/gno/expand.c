#ifdef __CCFRONT__
#include <14:pragma.h>
#endif
/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/RCS/expand.c,v 1.1 1992/01/24 03:27:03 dvadura Exp $
-- SYNOPSIS -- macro expansion code.
-- 
-- DESCRIPTION
--
--	This routine handles all the necessary junk that deals with macro
--	expansion.  It understands the following syntax.  If a macro is
--	not defined it expands to NULL, and {} are synonyms for ().
--
--		$$      - expands to $
--		{{      - expands to {
--	        }}      - expands to }
--		$A      - expands to whatever the macro A is defined as
--		$(AA)   - expands to whatever the macro AA is defined as
--		$($(A)) - represents macro indirection
--		<+...+> - get mapped to $(mktmp ...)
--	
--        following macro is recognized
--        
--                string1{ token_list }string2
--                
--        and expands to string1 prepended to each element of token_list and
--        string2 appended to each of the resulting tokens from the first
--        operation.  If string2 is of the form above then the result is
--        the cross product of the specified (possibly modified) token_lists.
--        
--        The folowing macro modifiers are defined and expanded:
--        
--               $(macro:modifier_list:modifier_list:...)
--               
--        where modifier_list a combination of:
--        
--               D or d      - Directory portion of token including separator
--               F or f      - File portion of token including suffix
--               B or b      - basename portion of token not including suffix
--		 T or t      - for tokenization
--
--	  or a single
--               S or s      - pattern substitution (simple)
--               
--        NOTE:  Modifiers are applied once the macro value has been found.
--               Thus the construct $($(test):s/joe/mary/) is defined and
--               modifies the value of $($(test))
--
--	       Also the construct $(m:d:f) is not the same as $(m:df)
--	       the first applies d to the value of $(m) and then
--	       applies f to the value of that whereas the second form
--	       applies df to the value of $(m).
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
--     $Log: expand.c,v $
 * Revision 1.1  1992/01/24  03:27:03  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"

/* Microsoft BRAINDAMAGE ALERT!!!!
 * This #ifdef is here only to satisfy stupid bugs in MSC5.0 and MSC5.1
 * it isn't needed for anything else.  It turns loop optimization off. */
#if defined(_MSC_VER)
#include "optoff.h"
#endif

static	char*	_scan_token ANSI((char*, char**));
static	char*	_scan_macro ANSI((char*, char**));
static	char*	_scan_brace ANSI((char*, char**, int*));
static	char*	_cross_prod ANSI((char*, char*));


PUBLIC char *
Expand( char *src )		/* pointer to source string */

/*
===============
      This is the driver routine for the expansion, it identifies non-white
      space tokens and gets the _scan_token routine to figure out if they should
      be treated in a special way. */

{
   char  *tmp;		      /* pointer to temporary str  */
   char  *res;                /* pointer to result string  */
   char  *start;              /* pointer to start of token */
   
   DB_ENTER( "Expand" );
   DB_PRINT( "exp", ("Expanding [%s]", src) );

   res = _strdup( "" );
   if( src == NIL(char) ) DB_RETURN( res );

   while( *src ) {
      char *ks, *ke;

      /* Here we find the next non white space token in the string
       * and find it's end, with respect to non-significant white space. */
      
      start = _strspn( src, " \t\n" );
      res   = _strjoin( res, src, start-src, TRUE );
      if( !(*start) ) break;

      /* START <+...+> KLUDGE */
      if( (ks=_strstr(start,"<+")) && (ke=_strstr(ks,"+>")) ){
	 char *t1, *t2;

	 res = _strjoin( res, t2=Expand(t1=_substr(start,ks)), -1, TRUE);
	 FREE(t1); FREE(t2);

	 t1 = _substr(ks+2, ke+1); t1[ke-ks-2] = ')';
	 t2 = _strjoin( "$(mktmp ", t1, -1,FALSE);
	 FREE(t1);
	 res = _strjoin( res, t2=Expand(t2), -1, TRUE);
	 FREE(t2);
	 src = ke+2;
      }
      /* END <+...+> KLUDGE */
      else {
	 res   = _strjoin( res, tmp = _scan_token( start, &src ), -1, TRUE );
	 FREE( tmp );
      }
   }
   
   DB_PRINT( "exp", ("Returning [%s]", res) );
   DB_RETURN( res );
}


PUBLIC char *
Apply_edit(

/*
===========================================
   Take the src string and apply the pattern substitution.  ie. look for
   occurrences of pat in src and replace each occurrence with subst.  This is
   NOT a regular expressions pattern substitution, it's just not worth it.
   
   if anchor == TRUE then the src pattern match must be at the end of a token.
   ie. this is for SYSV compatibility and is only used for substitutions of
   the caused by $(macro:pat=sub).  So if src = "fre.o.k june.o" then
   $(src:.o=.a) results in "fre.o.k june.a", and $(src:s/.o/.a) results in
   "fre.a.k june.a" */

	char *src,			/* the source string */
	char *pat,			/* pattern to find   */
	char *subst,		/* substitute string */
	int   fr,			/* if TRUE free src  */
	int   anchor)		/* if TRUE anchor    */
{
   char *res;
   char *p;
   char *s;
   int   l;

   DB_ENTER( "Apply_edit" );
   
   if( !*pat ) DB_RETURN( src );		/* do nothing if pat is NULL */

   DB_PRINT( "mod", ("Source str:  [%s]", src) );
   DB_PRINT( "mod", ("Replacing [%s], with [%s]", pat, subst) );

   s   = src;
   l   = strlen( pat );
   if( (p = _strstr( s, pat )) != NIL(char) ) {
      res = _strdup( "" );
      do {
	 if( anchor )
	    if( !*(p+l) || (strchr(" \t", *(p+l)) != NIL(char)) )
	       res = _strjoin( _strjoin( res, s, p-s, TRUE ), subst, -1, TRUE );
	    else
	       res = _strjoin( res, s, p+l-s, TRUE );
	 else
	    res = _strjoin( _strjoin( res, s, p-s, TRUE ), subst, -1, TRUE );

	 s   = p + l;
      }
      while( (p = _strstr( s, pat )) != NIL(char) );

      res = _strjoin( res, s, -1, TRUE );
      if( fr ) FREE( src );
   }
   else
      res = src;


   DB_PRINT( "mod", ("Result [%s]", res) );
   DB_RETURN( res );
}


PUBLIC void
Map_esc( char *tok )/*
================
   Map an escape sequence and replace it by it's corresponding character
   value.  It is assumed that tok points at the initial \, the esc
   sequence in the original string is replaced and the value of tok
   is not modified. */
{
   if( strchr( "\"\\vantbrf01234567", tok[1] ) ) {
      switch( tok[1] ) {
	 case 'a' : *tok = 0x07; break;
	 case 'b' : *tok = '\b'; break;
	 case 'f' : *tok = '\f'; break;
	 case 'n' : *tok = '\n'; break;
	 case 'r' : *tok = '\r'; break;
	 case 't' : *tok = '\t'; break;
	 case 'v' : *tok = 0x0b; break;
	 case '\\': *tok = '\\'; break;
	 case '\"': *tok = '\"'; break;

	 default: {
	    register int i = 0;
	    register int j = 0;
	    for( ; i<2 && isdigit(tok[2]); i++ ) {
	       j = (j << 3) + (tok[1] - '0');
	       strcpy( tok+1, tok+2 );
	    }
	    j = (j << 3) + (tok[1] - '0');
	    *tok = j;
	 }
      }
      strcpy( tok+1, tok+2 );
   }
}


PUBLIC char*
Apply_modifiers( int mod, char *src )/*
=============================
   This routine applies the appropriate modifiers to the string src
   and returns the proper result string */

{
   char	   *s;
   char    *e;
   TKSTR   str;

   DB_ENTER( "Apply_modifiers" );

   if( mod == (SUFFIX_FLAG | DIRECTORY_FLAG | FILE_FLAG) )
      DB_RETURN( src );

   SET_TOKEN( &str, src );
   DB_PRINT( "mod", ("Source string [%s]", src) );

   while( *(s = Get_token( &str, "", FALSE )) != '\0' ) {
      /* search for the directory portion of the filename.  If the
       * DIRECTORY_FLAG is set, then we want to keep the directory portion
       * othewise throw it away and blank out to the end of the token */

      if( (e = basename(s)) != s)
	 if( !(mod & DIRECTORY_FLAG) ) {
	    strcpy(s, e);
	    e = s+(str.tk_str-e);
	    for(; e != str.tk_str; e++)
               *e = ' ';
	 }
	 else
	    s = e;

      /* search for the suffix, if there is none, treat it as a NULL suffix.
       * if no file name treat it as a NULL file name.  same copy op as
       * for directory case above */

      e = strrchr( s, '.' );			/* NULL suffix if e=0 */
      if( e == NIL(char) ) e = s+strlen(s);

      if( !(mod & FILE_FLAG) ) {
	 strcpy( s, e );
	 e = s+(str.tk_str-e);
	 for( ; e != str.tk_str; e++ ) *e = ' ';
      }
      else
	 s = e;

      /* The last and final part.  This is the suffix case, if we don't want
       * it then just erase to the end of the token. */

      if( s != NIL(char) )
	 if( !(mod & SUFFIX_FLAG) )
	    for( ; s != str.tk_str; s++ ) *s = ' ';
   }

   /* delete the extra white space, it looks ugly */
   for( s = src, e = NIL(char); *s; s++ )
      if( *s == ' ' || *s == '\t' || *s == '\n' ) {
	 if( e == NIL(char) )
	    e = s;
      }
      else {
	 if( e != NIL(char) ) {
	    if( e+1 < s ) {
	       strcpy( e+1, s );
	       s = e+1;
	       *e = ' ';
	    }
	    e = NIL(char);
	 }
      }

   if( e != NIL(char) )
      if( e < s )
	 strcpy( e, s );

   DB_PRINT( "mod", ("Result string [%s]", src) );
   DB_RETURN( src );
}


PUBLIC char*
Tokenize( char *src, char *separator )/*
============================
	Tokenize the input of src and join each token found together with
	the next token separated by the separator string.

	When doing the tokenization, <sp>, <tab>, <nl>, and \<nl> all
	constitute white space. */

{
   TKSTR	tokens;
   char		*tok;
   char		*res;
   int		first = TRUE;

   DB_ENTER( "Tokenize" );

   SET_TOKEN( &tokens, src );


   /* map the escape codes in the separator string first */

   for(tok=separator; (tok = strchr(tok,ESCAPE_CHAR)) != NIL(char); tok++)
      Map_esc( tok );

   DB_PRINT( "exp", ("Separator [%s]", separator) );

   /* Build the token list */
   res = _strdup( "" );
   while( *(tok = Get_token( &tokens, "", FALSE )) != '\0' ) {
      DB_PRINT( "exp", ("Tokenizing [%s]", tok) );

      if( first ) {
	 FREE( res );
	 res   = _strdup( tok );
	 first = FALSE;
      }
      else {
      	 char *x;
	 res = _strjoin(res, x =_strjoin(separator, tok, -1, FALSE), -1, TRUE);
	 FREE( x );
      }
   }

   FREE( src );
   DB_RETURN( res );
}


static char*
_scan_token(

/*
======================
      This routine scans the token characters one at a time and identifies
      macros starting with $( and ${ and calls _scan_macro to expand their
      value.   the string1{ token_list }string2 expansion is also handled.
      In this case a temporary result is maintained so that we can take it's
      cross product with any other token_lists that may possibly appear. */
      
	char *s,			/* pointer to start of src string */
	char **ps)		/* pointer to start pointer	  */
{
   char *res;                 /* pointer to result          */
   char *start;               /* pointer to start of prefix */
   int  crossproduct = 0;     /* if 1 then computing X-prod */

   start = s;
   res   = _strdup( "" );
   while( 1 )
      switch( *s ) {
         /* Termination, We halt at seeing a space or a tab or end of string.
          * We return the value of the result with any new macro's we scanned
          * or if we were computing cross_products then we return the new
          * cross_product.
          * NOTE:  Once we start computing cross products it is impossible to
          *        stop.  ie. the semantics are such that once a {} pair is
          *        seen we compute cross products until termination. */

         case ' ':
         case '\t':
	 case '\n':
         case '\0': 
	 {
	    char *tmp;

	    *ps = s;
	    if( !crossproduct )
	       tmp = _strjoin( res, start, (s-start), TRUE );
	    else
	    {
	       tmp = _substr( start, s );
	       tmp = _cross_prod( res, tmp );
	    }
	    return( tmp );
	 }
         
         case '$':
         case '{':
	 {
	    /* Handle if it's a macro or if it's a {} construct.
	     * The results of a macro expansion are handled differently based
	     * on whether we have seen a {} beforehand. */
	    
	    char *tmp;
	    tmp = _substr( start, s );          /* save the prefix */

	    if( *s == '$' ) {
	       start = _scan_macro( s+1, &s );

	       if( crossproduct )
		  res = _cross_prod( res, _strjoin( tmp, start, -1, TRUE ) );
	       else {
		  res = _strjoin(res,tmp = _strjoin(tmp,start,-1,TRUE),-1,TRUE);
		  FREE( tmp );
	       }
	       FREE( start );
	    }
	    else if( strchr("{ \t",s[1]) == NIL(char) ){
	       int ok;
	       start = _scan_brace( s+1, &s, &ok );
		  
	       if( ok ) {
		  res = _cross_prod( res, _cross_prod(tmp, start) );
		  crossproduct = TRUE;
	       }
	       else {
		  res =_strjoin(res,tmp=_strjoin(tmp,start,-1,TRUE),-1,TRUE);
		  FREE( start );
		  FREE( tmp   );
	       }
	    }
	    else {    /* handle the {{ case */
	       res = _strjoin( res, start, (s-start+1), TRUE );
	       s  += (s[1]=='{')?2:1;
	       FREE( tmp );
	    }

	    start = s;
	 }
	 break;

	 case '}':
	    if( s[1] != '}' ) {
	       /* error malformed macro expansion */
	       s++;
	    }
	    else {    /* handle the }} case */
	       res = _strjoin( res, start, (s-start+1), TRUE );
	       s += 2;
	       start = s;
	    }
	    break;
         
         default: s++;
      }
}


static char*
_scan_macro(

/*
======================
	This routine scans a macro use and expands it to the value.  It
	returns the macro's expanded value and modifies the pointer into the
	src string to point at the first character after the macro use.
	The types of uses recognized are:

		$$		- expands to $
		$(name)		- expands to value of name
		${name}		- same as above
		$($(name))	- recurses on macro names (any level)
	and
		$(func[,args ...] [data])
	and 
	        $(name:modifier_list:modifier_list:...)
        
	see comment for Expand for description of valid modifiers.

	NOTE that once a macro name bounded by ( or { is found only
	the appropriate terminator (ie. ( or } is searched for. */

	char *s,			/* pointer to start of src string   */
	char **ps)		/* pointer to start pointer	    */
{
   char sdelim;         /* start of macro delimiter         */
   char edelim;         /* corresponding end macro delim    */
   char *start;         /* start of prefix                  */
   char *macro_name;    /* temporary macro name             */
   char *recurse_name;  /* recursive macro name             */
   char *result;	/* result for macro expansion	    */
   int  bflag = 0;      /* brace flag, ==0 => $A type macro */
   int  done  = 0;      /* != 0 => done macro search        */
   int  lev   = 0;      /* brace level                      */
   int  mflag = 0;      /* != 0 => modifiers present in mac */
   int  fflag = 0;	/* != 0 => GNU style function 	    */
   HASHPTR hp;		/* hash table pointer for macros    */
   
   DB_ENTER( "_scan_macro" );

   /* Check for the simple $ at end of line case */
   if( !*s ) {
      *ps = s;
      DB_RETURN( _strdup("") );
   }

   if( *s == '$' ) {    /* Take care of the simple $$ case. */
      *ps = s+1;
      DB_RETURN( _strdup("$") );
   }

   sdelim = *s;         /* set and remember start/end delim */
   if( sdelim == '(' )
      edelim = ')';
   else
      edelim = '}';

   start = s;           /* build up macro name, find its end*/
   while( !done ) {
      switch( *s ) {
         case '(':				/* open macro brace */
         case '{':
	    if( *s == sdelim ) {
	       lev++;
	       bflag++;
	    }
	    break;
         
         case ':':                              /* halt at modifier */
            if( lev == 1 ) {
               done = TRUE;
               mflag = 1;
            }
            break;

	 case ' ':
	 case '\t':
	 case '\n':
	    fflag = 1;
	    break;
            
	 case '\0':				/* check for null */
	    *ps = s;
	    if( lev ) {
	       done  = TRUE;
	       bflag = 0;
	       s     = start;
	    }
	    break;
         
         case ')':				/* close macro brace */
         case '}':
	    if( *s == edelim && lev ) --lev;
	    /*FALLTHROUGH*/

         default:
	    done = !lev;
      }
      s++;
   }

   /* Check if this is a $A type macro.  If so then we have to
    * handle it a little differently. */
   if( bflag )
      macro_name = _substr( start+1, s-1 );
   else
      macro_name = _substr( start, s );

   /* Check to see if the macro name contains spaces, if so then treat it
    * as a GNU style function invocation and call the function mapper to
    * deal with it. */
   if( fflag )
      result = Exec_function(macro_name);
   else {
      /* Check if the macro is a recursive macro name, if so then
       * EXPAND the name before expanding the value */
      if( strchr( macro_name, '$' ) != NIL(char) ) {
	 recurse_name = Expand( macro_name );
	 FREE( macro_name );
	 macro_name = recurse_name;
      }

      /* Code to do value expansion goes here, NOTE:  macros whose assign bit
	 is one have been evaluated and assigned, they contain no further
	 expansions and thus do not need their values expanded again. */

      if( (hp = GET_MACRO( macro_name )) != NIL(HASH) ) {
	 if( hp->ht_flag & M_MARK )
	    Fatal( "Detected circular macro [%s]", hp->ht_name );

	 /* for M_MULTI macro variable assignments */
	 If_multi = hp->ht_flag & M_MULTI;

	 if( !(hp->ht_flag & M_EXPANDED) ) {
	    hp->ht_flag |= M_MARK;
	    result = Expand( hp->ht_value );
	    hp->ht_flag ^= M_MARK;
	 }
	 else if( hp->ht_value != NIL(char) )
	    result = _strdup( hp->ht_value );
	 else
	    result = _strdup( "" );

	 /*
	  * Mark macros as used only if we are not expanding them for
	  * the purpose of a .IF test, so we can warn about redef after use*/

	 if( !If_expand ) hp->ht_flag |= M_USED;
      }
      else
	 result = _strdup( "" );
   }

   if( mflag ) {
      char separator;
      int  modifier_list = 0;
      int  aug_mod       = FALSE;
      char *pat1;
      char *pat2;
      char *p;

      /* Yet another brain damaged AUGMAKE kludge.  We should accept the 
       * AUGMAKE bullshit of $(f:pat=sub) form of macro expansion.  In
       * order to do this we will forgo the normal processing if the
       * AUGMAKE solution pans out, otherwise we will try to process the
       * modifiers ala dmake.
       *
       * So we look for = in modifier string.
       * If found we process it and not do the normal stuff */

      for( p=s; *p && *p != '=' && *p != edelim; p++ );

      if( *p == '=' ) {
	 pat1 = _substr( s, p );
	 for( s=p=p+1; (*p != edelim); p++ );

	 pat2 = _substr( s, p );

	 if( !Augmake ) {
	    char *tmp = pat2;
	    pat2 = Expand(pat2);
	    FREE(tmp);
	 }

	 result = Apply_edit( result, pat1, pat2, TRUE, TRUE );
	 FREE( pat1 );
	 FREE( pat2 );
	 s = p;
	 aug_mod = TRUE;
      }

      if( !aug_mod )
	 while( *s && *s != edelim ) {		/* while not at end of macro */
	    switch( *s++ ) {
	       case 'b':
	       case 'B': modifier_list |= FILE_FLAG; 		   break;

	       case 'd':
	       case 'D': modifier_list |= DIRECTORY_FLAG;  	   break;

	       case 'f':
	       case 'F': modifier_list |= FILE_FLAG | SUFFIX_FLAG; break;

	       case 'S':
	       case 's':
		  if( modifier_list ) {
		     Warning( "Edit modifier must appear alone, ignored");
		     modifier_list = 0;
		  }
		  else {
		     separator = *s++;
		     for( p=s; *p != separator && *p != edelim; p++ );

		     if( *p == edelim )
		        Warning("Syntax error in edit pattern, ignored");
		     else {
			char *t1, *t2;
			pat1 = _substr( s, p );
			for(s=p=p+1; (*p != separator) && (*p != edelim); p++ );
			pat2 = _substr( s, p );
			t1 = Expand(pat1); FREE(pat1);
			t2 = Expand(pat2); FREE(pat2);
			result = Apply_edit( result, t1, t2, TRUE, FALSE );
			FREE( t1 );
			FREE( t2 );
		     }
		     s = p;
		  }
		  /* find the end of the macro spec, or the start of a new
		   * modifier list for further processing of the result */

		  for( ; (*s != edelim) && (*s != ':'); s++ );
		  if( *s == ':' ) s++;
		  break;

	       case 'T':
	       case 't':
		  if( modifier_list ) {
		     Warning( "Tokenize modifier must appear alone, ignored");
		     modifier_list = 0;
		  }
		  else {
		     char *msg = "Separator string must be quoted";

		     separator = *s++;

		     if( separator != '\"' )
			Warning( msg );
		     else {
			/* we change the semantics to allow $(v:t")") */
			for (p = s; *p && *p != separator; p++)
			   if (*p == '\\')
			      if (p[1] == '\\' || p[1] == '"')
				 p++;
			if( *p == 0 )
			   Fatal( "Unterminated separator string" );
			else {
			   pat1 = _substr( s, p );
			   result = Tokenize( result, pat1 );
			   FREE( pat1 );
			}
			s = p;
		     }

		     /* find the end of the macro spec, or the start of a new
		      * modifier list for further processing of the result */

		     for( ; (*s != edelim) && (*s != ':'); s++ );
		     if( *s == ':' ) s++;
		  }
		  break;

	       case ':':
		  if( modifier_list ) {
		     result = Apply_modifiers( modifier_list, result );
		     modifier_list = 0;
		  }
		  break;

	       default:
		  Warning( "Illegal modifier in macro, ignored" );
		  break;
	    }
	 }

      if( modifier_list ) /* apply modifier */
         result = Apply_modifiers( modifier_list, result );
      
      s++;
   }

   *ps = s;
   FREE( macro_name );
   DB_RETURN( result );
}


static char*
_scan_brace(

/*
============================
      This routine scans for { token_list } pairs.  It expands the value of
      token_list by calling Expand on it.  Token_list may be anything at all.
      Note that the routine count's ballanced parentheses.  This means you
      cannot have something like { fred { joe }, if that is what you really
      need the write it as { fred {{ joe }, flag is set to 1 if all ok
      and to 0 if the braces were unballanced. */
      
	char *s,
	char **ps,
	int  *flag)
{
   char *t;
   char *start;
   char *res;
   int  lev  = 1;
   int  done = 0;
   
   DB_ENTER( "_scan_brace" );

   start = s;
   while( !done )
      switch( *s++ ) {
         case '{': 
            if( *s == '{' ) break;              /* ignore {{ */
            lev++;
            break;
            
         case '}': 
            if( *s == '}' ) break;              /* ignore }} */
	    if( lev )
	       if( --lev == 0 ) done = TRUE;
	    break;

	 case '$':
	    if( *s == '{' || *s == '}' ) {
	      if( (t = strchr(s,'}')) != NIL(char) )
	         s = t;
	      s++;
	    }
	    break;
         
         case '\0':
	    if( lev ) {
	       done = TRUE;
	       s--;
	       /* error malformed macro expansion */
	    }
	    break;
      }

   start = _substr( start, (lev) ? s : s-1 );

   if( lev ) {
      /* Braces were not ballanced so just return the string.
       * Do not expand it. */
       
      res   = _strjoin( "{", start, -1, FALSE );
      *flag = 0;
   }
   else {
      *flag = 1;
      res   = Expand( start );

      if( (t = _strspn( res, " \t" )) != res ) strcpy( res, t );
   }

   FREE( start );       /* this is ok! start is assigned a _substr above */
   *ps = s;

   DB_RETURN( res );
}


static char*
_cross_prod( char *x, char *y )/*
=====================
      Given two strings x and y compute the cross-product of the tokens found
      in each string.  ie. if x = "a b" and y = "c d" return "ac ad bc bd".

	     NOTE:  buf will continue to grow until it is big enough to handle
	            all cross product requests.  It is never freed!  (maybe I
		    will fix this someday) */
      
{
   static char *buf;
   static int  buf_siz = 0;
   char *brkx;
   char *brky;
   char *cy;
   char *cx;
   char *res;
   int  i;

   if( *x && *y ) {
      res = _strdup( "" ); cx = x;
      while( *cx ) {
	 cy = y;
         brkx = _strpbrk( cx, " \t\n" );
	 if( (brkx-cx == 2) && *cx == '\"' && *(cx+1) == '\"' ) cx = brkx;

	 while( *cy ) {
	    brky = _strpbrk( cy, " \t\n" );
	    if( (brky-cy == 2) && *cy == '\"' && *(cy+1) == '\"' ) cy = brky;
	    i    = brkx-cx + brky-cy + 2;

	    if( i > buf_siz ) {		/* grow buf to the correct size */
	       if( buf != NIL(char) ) FREE( buf );
	       if( (buf = MALLOC( i, char )) == NIL(char))  No_ram();
	       buf_siz = i;
	    }

	    strncpy( buf, cx, (i = brkx-cx) );
	    buf[i] = '\0';
	    if (brky-cy > 0) strncat( buf, cy, brky-cy );
	    buf[i+(brky-cy)] = '\0';
	    strcat( buf, " " );
	    res = _strjoin( res, buf, -1, TRUE );
	    cy = _strspn( brky, " \t\n" );
	 }
	 cx = _strspn( brkx, " \t\n" );
      }

      FREE( x );
      res[ strlen(res)-1 ] = '\0';
   }
   else
      res = _strjoin( x, y, -1, TRUE );

   FREE( y );
   return( res );
}
