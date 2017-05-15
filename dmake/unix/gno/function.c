#ifdef __CCFRONT__
#include <14:pragma.h>
#endif
/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/RCS/function.c,v 1.1 1992/01/24 03:27:00 dvadura Exp $
-- SYNOPSIS -- GNU style functions for dmake.
-- 
-- DESCRIPTION
-- 	All GNU stule functions understood by dmake are implemented in this
--	file.  Currently the only such function is $(mktmp ...) which is
--	not part of GNU-make is an extension provided by dmake.
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
--     $Log: function.c,v $
 * Revision 1.1  1992/01/24  03:27:00  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include <unistd.h>
#include "extern.h"

static char *_exec_mktmp  ANSI((char *, char *, char *));
static char *_exec_subst  ANSI((char *, char *, char *));
static char *_exec_iseq   ANSI((char *, char *, char *, int));
static char *_exec_sort   ANSI((char *));
static char *_exec_shell  ANSI((char *));
static int   _mystrcmp    ANSI((CONST PVOID, CONST PVOID));


PUBLIC char *
Exec_function( char *buf)/*
====================
   Execute the function given by the value of args.

   So far mktmp is the only valid function, anything else elicits and error
   message.  It is my hope to support the GNU style functions in this portion
   of the code at some time in the future. */
{
   char *fname;
   char *args;
   char *mod1;
   char *mod2 = NIL(char);
   char *res  = NIL(char);

   /* This must succeed since the presence of ' ', \t or \n is what
    * determines if this functions is called in the first place. */
   fname = _substr(buf, args=_strpbrk(buf," \t\n"));

   if( (mod1 = strchr(fname,',')) != NIL(char) ){
      *mod1 = '\0';
      mod1++;

      if( (mod2 = strchr(mod1,',')) != NIL(char) ){
	 		*mod2 = '\0';
	 		mod2++;
      }
   }

   switch( *fname ) {
   case 'e':
	 	if(strncmp(fname,"eq",2) == 0) res = _exec_iseq(mod1,mod2,args,TRUE);
		break;

   case 'm':
	 	if( strncmp(fname,"mktmp", 5) == 0 ) res = _exec_mktmp(mod1,mod2,args);
	 	break;

   case 'n':
		if( strncmp(fname,"null", 4) == 0 )
	   	res = _exec_iseq(mod1,NIL(char),args,TRUE);
	 	break;

   case '!':
		if(strncmp(fname,"!null",5) == 0)
	   	res = _exec_iseq(mod1,NIL(char),args,FALSE);
	 	if(strncmp(fname,"!eq",3) == 0) res = _exec_iseq(mod1,mod2,args,FALSE);
	 	break;

   case 's':
		if(strncmp(fname,"sort",4) == 0) res = _exec_sort(args);
	 	else if(strncmp(fname,"shell",5)==0) res = _exec_shell(args);
	 	else if(strncmp(fname,"strip",5)==0) res = Tokenize(Expand(args)," ");
	 	else if(strncmp(fname,"subst",5)==0) res = _exec_subst(mod1,mod2,args);
	 	break;

   default:
		Warning( "Function '%s' not implemented at this time", fname );
   }

   if( res == NIL(char) ) res = _strdup("");

   FREE(fname);
   return(res);
}


static char *
_exec_mktmp( char *file, char *text, char *data )
{
   register char *p;
   char *tmpname;
   char *name;
   FILE *tmpfile = NIL(FILE);

   /* This is only a test of the recipe line so prevent the tempfile side
    * effects. */
   if( Suppress_temp_file ) return(NIL(char));

   name = Current_target ? Current_target->CE_NAME:"makefile text";

   if( file && *file ) {
      char *newtmp;

      /* This call to Get_temp sets TMPFILE for subsequent expansion of file.
       * DO NOT DELETE IT! */
      Get_temp( &newtmp, "", FALSE ); FREE(newtmp);
      tmpname = Expand(file);

      if( *tmpname ) {
	 if( (tmpfile = fopen(tmpname, "w")) == NIL(FILE) )
	    Open_temp_error( tmpname, name );

	 Def_macro("TMPFILE", tmpname, M_EXPANDED|M_MULTI);
	 Link_temp( Current_target, tmpfile, tmpname );
      }
      else
	 FREE(tmpname);
   }

   if( !tmpfile )
      tmpfile = Start_temp( "", Current_target, &tmpname );

   if( !text || !*text ) text = tmpname;
   data = Expand(_strspn(data, " \t\n"));

   for(p=strchr(data,'\n'); p; p=strchr(p,'\n')) {
      char *q = _strspn(++p," \t");
      strcpy(p,q);
   }

   Append_line( data, FALSE, tmpfile, name, FALSE, TRUE );
   Close_temp( Current_target, tmpfile );
   FREE(data);

   return( Expand(text) );
}


static char *
_exec_iseq( char *lhs, char *rhs, char *data, int eq )
{
	char *l, *r, *i, *e, *res;
   int val;

   l = Expand(lhs);
   r = Expand(rhs);
   i = _strspn(data, " \t\n");
   e = strchr(i, ' ');
   res = NIL(char);
   val = strcmp(l,r);

   if( (!val && eq) || (val && !eq) ) {
      if( e != NIL(char) ) *e = '\0';
      res = Expand(i);
   }
   else if( e != NIL(char) ) {
      e = _strspn(e," \t\n");
      if( *e ) res = Expand(e);
   }

   FREE(l);
   FREE(r);
   return(res);
}


static char *
_exec_sort( char *args )
{
   char *res  = NIL(char);
   char *data = Expand(args);
   char **tokens = NIL(char *);
   char *p;
   char *white = " \t\n";
   int  j;
   int  i = 0;

   for( i=0,p=_strspn(data,white); *p; p=_strspn(_strpbrk(p,white),white),i++); 

   if( i != 0 ) {
      TALLOC(tokens, i, char *);

      for( i=0,p=_strspn(data,white); *p; p=_strspn(p,white),i++){
	 tokens[i] = p;
	 p = _strpbrk(p,white);
	 if( *p ) *p++ = '\0';
      }

      qsort( tokens, i, sizeof(char *), _mystrcmp );

      for( j=0; j<i; j++ ) res = _strapp(res, tokens[j]);
      FREE(data);
      FREE(tokens);
   }

   return(res);
}


static int
_mystrcmp(
CONST PVOID p,
CONST PVOID q)
{
   return(strcmp(*((CONST char **)p),*((CONST char **)q)));
}


static char *
_exec_subst( char *pat, char *subst, char *data )
{
   char *res;

   pat = Expand(pat);
   subst = Expand(subst);
   res = Apply_edit( Expand(data), pat, subst, TRUE, FALSE );
   FREE(pat);
   FREE(subst);

   return(res);
}


static char *
_exec_shell( char *data )
{
#if !defined(GNO)
   extern char *tempnam();
#endif
   static int  nestlevel = 0;
   static int  org_out;
   static int  bsize;
   static char *buffer;
   static char *tmpnm;
   static FILE *tmp;

   int wait     = Wait_for_completion;
   uint16 vflag = Verbose;
   int tflag    = Trace;
   char *res    = NIL(char);
   CELL cell;
   STRING rcp;
   HASH   cname;

   if( Suppress_temp_file ) return(NIL(char));

   /* Set the temp CELL used for building prerequisite candidates to
    * all zero so that we don't have to keep initializing all the
    * fields. */
   {
      register char *s = (char *) &cell;
      register int   n = sizeof(CELL);
      while( n ) { *s++ = '\0'; n--; }
   }
   rcp.st_string  = _strspn(data, " \t+-%@");
   rcp.st_attr    = Rcp_attribute( data );
   rcp.st_next    = NIL(STRING);
   cname.ht_name  = "Shell escape";
   cell.ce_name   = &cname;
   cell.ce_fname  = cname.ht_name;
   cell.ce_recipe = &rcp;
   cell.ce_flag   = F_TARGET|F_RULES;
   cell.ce_attr   = A_PHONY|A_SILENT;

   if( nestlevel == 0 ) {
      tmpnm   = tempnam(NIL(char),"mk");
      org_out = dup(STDOUT_FILENO);

      if( (tmp = fopen(tmpnm, "w+")) == NIL(FILE) )
	 		Open_temp_error( tmpnm, cname.ht_name );

      close(STDOUT_FILENO);
      dup( fileno(tmp) );

      bsize  = (Buffer_size < BUFSIZ)?BUFSIZ:Buffer_size;
      buffer = MALLOC(bsize,char);
   }

   Wait_for_completion = TRUE;
   Verbose = V_NONE;
   Trace   = FALSE;
   nestlevel++;
   Exec_commands( &cell );
   nestlevel--;
   Trace   = tflag;
   Verbose = vflag;
   Wait_for_completion = wait;

   /* Now we have to read the temporary file, get the tokens and return them
    * as a string. */
   rewind(tmp);
   while( fgets(buffer, bsize, tmp) ) {
      char *p = strchr(buffer, '\n');

      if( p == NIL(char) )
	 		res = _strjoin(res,buffer,-1,TRUE);
      else {
	 		*p = '\0';
         res = _strapp(res,buffer);
      }
   }

   fclose(tmp);
   if( nestlevel == 0 ) {
      Remove_file(tmpnm);
      close(STDOUT_FILENO);
      dup(org_out);
      close(org_out);
      FREE(tmpnm);
      FREE(buffer);
   }
   else {
      if( (tmp = fopen(tmpnm, "w+")) == NIL(FILE) )
	 		Open_temp_error( tmpnm, cname.ht_name );

      close(STDOUT_FILENO);
      dup( fileno(tmp) );
   }

   return(res);
}
