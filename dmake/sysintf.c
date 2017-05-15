/* RCS      -- $Header: /u/dvadura/src/generic/dmake/src/RCS/sysintf.c,v 1.2 1992/04/07 04:42:46 dvadura Exp $
-- SYNOPSIS -- system independent interface
-- 
-- DESCRIPTION
--	These are the routines constituting the system interface.
--	The system is taken to be essentially POSIX conformant.
--	The original code was extensively revised by T J Thompson at MKS,
--	and the library cacheing was added by Eric Gisin at MKS.  I then
--	revised the code yet again, to improve the lib cacheing, and to
--	make it more portable.
--
--	The following is a list of routines that are required by this file
--	in order to work.  These routines are provided as functions by the
--	standard C lib of the target system or as #defines in system/sysintf.h
--	or via appropriate C code in the system/ directory for the given
--	system.
--
--	The first group must be provided by a file in the system/ directory
--	the second group is ideally provided by the C lib.  However, there
--	are instances where the C lib implementation of the specified routine
--	does not exist, or is incorrect.  In these instances the routine
--	must be provided by the the user in the system/ directory of dmake.
--	(For example, the bsd/ dir contains code for putenv(), and tempnam())
--
--	DMAKE SPECIFIC:
--		seek_arch()
--		touch_arch()
--		void_lcache()
--		runargv()
--		STAT()
--		Remove_prq()
--
--	C-LIB SPECIFIC:  (should be present in your C-lib)
--		utime()
--		time()
--		getenv()
--		putenv()
--		getcwd()
--		signal()
--		chdir()
--		tempnam()
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
--     $Log: sysintf.c,v $
 * Revision 1.2  1992/04/07  04:42:46  dvadura
 * Replaced the use of _POSIX_NAME_MAX and MAX_PATH_LEN by NAME_MAX and
 * PATH_MAX respectively.
 *
 * Revision 1.1  1992/01/24  03:27:08  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"
#include "sysintf.h"

/*
** Tries to stat the file name.  Returns 0 if the file
** does not exist.  Note that if lib is not null it tries to stat
** the name found inside lib.
**
** If member is NOT nil then look for the library object which defines the
** symbol given by name.  If found _strdup the name and return make the
** pointer pointed at by sym point at it.  Not handled for now!
*/
PUBLIC time_t
Do_stat(name, lib, member)
char *name;
char *lib;
char **member;
{
   struct stat buf;
   time_t seek_arch();

   if( member != NIL(char *) )
      Fatal("Library symbol names not supported");

   buf.st_mtime = (time_t)0L;
   if( lib != NIL(char) )
      return( seek_arch(basename(name), lib) );
   else if( strlen(basename(name)) > NAME_MAX )
      return((time_t)0L);
   else
      return( (STAT(name,&buf)==-1 || (Augmake && (buf.st_mode & S_IFDIR)))
	      ? (time_t)0L
	      : (time_t) buf.st_mtime
	    );
}



/* Touch existing file to force modify time to present.
 */
PUBLIC int
Do_touch(name, lib, member)
char *name;
char *lib;
char **member;
{
   if( member != NIL(char *) )
      Fatal("Library symbol names not supported");

   if (lib != NIL(char))
      return( touch_arch(basename(name), lib) );
   else
      return( utime(name, NIL(time_t)) );
}



PUBLIC void
Void_lib_cache( lib_name, member_name )/*
=========================================
   Void the library cache for lib lib_name, and member member_name. */
char *lib_name;
char *member_name;
{
   VOID_LCACHE( lib_name, member_name );
}



/*
** return the current time
*/
PUBLIC time_t
Do_time()
{
   extern time_t time();
   return (time((time_t*)0));
}



/*
** Execute the string passed in as a command and return
** the return code. The command line arguments are
** assumed to be separated by spaces or tabs.  The first
** such argument is assumed to be the command.
**
** If group is true then this is a group of commands to be fed to the
** the shell as a single unit.  In this case cmd is of the form
** "file" indicating the file that should be read by the shell
** in order to execute the command group.
*/
PUBLIC int
Do_cmnd(cmd, group, do_it, target, ignore, shell, last)
char   *cmd;
int     group;
int	do_it;
CELLPTR target;
int     ignore;
int     shell;
int	last;
{
   int  i;

   if( !do_it ) {
      if( last && !Doing_bang ) {
         Update_time_stamp( target );
      }
      return(0);
   }

   if( Max_proc == 1 ) Wait_for_completion = TRUE;

   if( (i = runargv(target, ignore, group, last, shell, cmd)) == -1 )
      Quit();

   /* NOTE:  runargv must return either 0 or 1, 0 ==> command executed, and
    * we waited for it to return, 1 ==> command started and is running
    * concurrently with make process. */
   return(i);
}


#define MINARGV 64
/* Take a command and pack it into an argument vector to be executed. */
PUBLIC char **
Pack_argv( group, shell, cmd )
int    group;
int    shell;
char  *cmd;
{
   static char **av = NIL(char *);
   static int   avs = 0;
   int i = 0;

   if( av == NIL(char *) ) {
      TALLOC(av, MINARGV, char*);
      avs = MINARGV;
   }

   if( (Packed_shell = shell||group||(*_strpbrk(cmd, Shell_metas)!='\0')) ) {
      char* sh = group ? GShell : Shell;

      if( sh != NIL(char) ) {
         av[i++] = sh;
         if( (av[i] = (group?GShell_flags:Shell_flags)) != NIL(char) ) i++;

	 av[i++] = cmd;
	 av[i]   = NIL(char);
      }
      else
	 Fatal("%sSHELL macro not defined", group?"GROUP":"");
   }
   else {
      do {
         while( iswhite(*cmd) ) ++cmd;
         if( *cmd ) av[i++] = cmd;

         while( *cmd != '\0' && !iswhite(*cmd) ) ++cmd;
         if( *cmd ) *cmd++ = '\0';

	 if( i == avs ) {
	    avs += MINARGV;
	    av = (char **) realloc( av, avs*sizeof(char *) );
	 }
      } while( *cmd );

      av[i] = NIL(char);
   }

   return(av);
}


/*
** Return the value of ename from the environment
** if ename is not defined in the environment then
** NIL(char) should be returned
*/
PUBLIC char *
Read_env_string(ename)
char *ename;
{
#if !defined(_MSC_VER) || _MSC_VER < 600
   extern char *getenv();
#endif
   return( getenv(ename) );
}



/*
** Set the value of the environment string ename to value.
**  Returns 0 if success, non-zero if failure
*/
PUBLIC int
Write_env_string(ename, value)
char *ename;
char *value;
{
   extern int putenv();
   char*   p;
   char*   envstr = _stradd(ename, value, FALSE);

   p = envstr+strlen(ename);	/* Don't change this code, _stradd does not */
   *p++ = '=';			/* add the space if *value is 0, it does    */
   if( !*value ) *p = '\0';	/* allocate enough memory for one though.   */

   return( putenv(envstr) );
}



PUBLIC void
ReadEnvironment()
{
   extern char **Rule_tab;
#if !defined(_MSC_VER)
   extern char **environ;
#endif
   char **rsave;

#if !defined(__ZTC__) && !defined(_MPW)
# define make_env()
# define free_env()
#else
   void make_env();
   void free_env();
#endif

   make_env();

   rsave    = Rule_tab;
   Rule_tab = environ;
   Readenv  = TRUE;

   Parse( NIL(FILE) );

   Readenv  = FALSE;
   Rule_tab = rsave;

   free_env();
}



/*
** All we have to catch is SIG_INT
*/
PUBLIC void
Catch_signals(fn)
void (*fn)();
{
   if( signal(SIGINT, SIG_IGN) != SIG_IGN )
      signal( SIGINT, fn );
   if( signal(SIGQUIT, SIG_IGN) != SIG_IGN )
      signal( SIGQUIT, fn );
}



/*
** Clear any previously set signals
*/
PUBLIC void
Clear_signals()
{
   if( signal(SIGINT, SIG_IGN) != SIG_IGN )
      signal( SIGINT, SIG_DFL );
   if( signal(SIGQUIT, SIG_IGN) != SIG_IGN )
      signal( SIGQUIT, SIG_DFL );
}



/*
** Set program name
*/
PUBLIC void
Prolog(argc, argv)
int   argc;
char* argv[];
{
   char buf[50];

   Pname = (argc == 0) ? DEF_MAKE_PNAME : argv[0];
   sprintf( buf, "dmake-%d-root", GETPID );
   Root = Def_cell( buf );
   tzset();
}



/*
** Do any clean up for exit.
*/
PUBLIC void
Epilog(ret_code)
int ret_code;
{
   Write_state();
   Unlink_temp_files(Root);
   Hook_std_writes(NIL(char));		/* For MSDOS tee (-F option) */
   exit( ret_code );
}



/*
** Use the built-in functions of the operating system to get the current
** working directory.
*/
PUBLIC char *
Get_current_dir()
{
   static char buf[PATH_MAX+2];
   return(getcwd(buf, sizeof(buf)));
}



/*
** change working directory
*/
PUBLIC int
Set_dir(path)
char*   path;
{
   return( chdir(path) );
}



/*
** return switch char
*/
PUBLIC char
Get_switch_char()
{
   return( getswitchar() );
}



/*
** Generate a temporary file name and open the file for writing.
** If a name cannot be generated or the file cannot be opened
** return -1, else return the fileno of the open file.
** and update the source file pointer to point at the new file name.
** Note that the new name should be freed when the file is removed.
*/
PUBLIC FILE*
Get_temp(path, suff, op)
char **path;
char *suff;
int  op;
{
   extern char *tempnam();

   *path = _strjoin( tempnam(NIL(char), "mk"), suff, -1, TRUE );
   Def_macro( "TMPFILE", *path, M_MULTI|M_EXPANDED );

   return( op?fopen(*path, "w"):NIL(FILE) );
}


/*
** Open a new temporary file and set it up for writing.
*/
PUBLIC FILE *
Start_temp( suffix, cp, fname )
char     *suffix;
CELLPTR   cp;
char    **fname;
{
   FILE	       *fp;
   char        *tmpname;
   char	       *name;

   name = (cp != NIL(CELL))?cp->CE_NAME:"makefile text";

   if( (fp = Get_temp(&tmpname, suffix, TRUE)) == NIL(FILE) )
      Open_temp_error( tmpname, name );

   Link_temp( cp, fp, tmpname );
   *fname = tmpname;

   return( fp );
}


/*
** Issue an error on failing to open a temporary file
*/
PUBLIC void
Open_temp_error( tmpname, name )
char *tmpname;
char *name;
{
   Fatal("Cannot open temp file `%s' while processing `%s'", tmpname, name );
}


/*
** Link a temp file onto the list of files.
*/
PUBLIC void
Link_temp( cp, fp, fname )
CELLPTR cp;
FILE   *fp;
char   *fname;
{
   FILELISTPTR new;

   if( cp == NIL(CELL) ) cp = Root;

   TALLOC( new, 1, FILELIST );

   new->fl_next = cp->ce_files;
   new->fl_name = fname;
   new->fl_file = fp;		/* indicates temp file is open */

   cp->ce_files = new;
}


/*
** Close a previously used temporary file.
*/
PUBLIC void
Close_temp(cp, file)
CELLPTR cp;
FILE    *file;
{
   FILELISTPTR fl;
   if( cp == NIL(CELL) ) cp = Root;

   for( fl=cp->ce_files; fl && fl->fl_file != file; fl=fl->fl_next );
   if( fl ) {
      fl->fl_file = NIL(FILE);
      fclose(file);
   }
}


/*
** Clean-up, and close all temporary files associated with a target.
*/
PUBLIC void
Unlink_temp_files( cp )/*
==========================
   Unlink the tempfiles if any exist.  Make sure you close the files first
   though.  This ensures that under DOS there is no disk space lost. */
CELLPTR cp;
{
   FILELISTPTR cur, next;

   if( cp == NIL(CELL) || cp->ce_files == NIL(FILELIST) ) return;

   for( cur=cp->ce_files; cur != NIL(FILELIST); cur=next ) {
      next = cur->fl_next;

      if( cur->fl_file ) fclose( cur->fl_file );

      if( Verbose & V_LEAVE_TMP )
         printf( "%s:  Left temp file [%s]\n", Pname, cur->fl_name );
      else
         (void) Remove_file( cur->fl_name );

      FREE(cur->fl_name);
      FREE(cur);
   }

   cp->ce_files = NIL(FILELIST);
}


PUBLIC void
Handle_result(status, ignore, abort_flg, target)
int	status;
int	ignore;
int	abort_flg;
CELLPTR target;
{
   status = ((status&0xff)==0 ? status>>8
	    : (status & 0xff)==SIGTERM ? -1
	    : (status & 0x7f)+128);

   if( status )
      if( !abort_flg ) {
	 fprintf( stderr, "%s:  Error code %d, while making '%s'",
		  Pname, status, target->ce_fname );

	 if( ignore || Continue ) {
	    fputs( " (Ignored)\n", stderr );
	 }
	 else {
	    fputc( '\n', stderr );

	    if( !(target->ce_attr & A_PRECIOUS) )
	       if( Remove_file( target->ce_fname ) == 0 )
		  fprintf(stderr,"%s:  '%s' removed.\n",Pname,target->ce_fname);

	    Quit();
	 }
      }
      else if( !(target->ce_attr & A_PRECIOUS) )
	 Remove_file( target->ce_fname );
}


PUBLIC void
Update_time_stamp( cp )
CELLPTR cp;
{
   HASHPTR hp;
   CELLPTR tcp;
   int     phony = ((cp->ce_attr&A_PHONY) != 0);

   tcp = cp;
   do {
      if( tcp->ce_attr & A_LIBRARY )
	 Void_lib_cache( tcp->ce_fname, NIL(char) );
      else if( !Touch && (tcp->ce_attr & A_LIBRARYM) )
	 Void_lib_cache( tcp->ce_lib, tcp->ce_fname );

      if( !phony )
	 Stat_target(tcp, -1);

      if( tcp->ce_time == (time_t) 0L )
	 tcp->ce_time = Do_time();

      if( Trace )
	 tcp->ce_flag |= F_STAT;		/* pretend we stated ok */

      if( Verbose & V_MAKE )
	 printf( "%s:  <<<< Set [%s] time stamp to %ld\n",
		 Pname, tcp->CE_NAME, tcp->ce_time );

      Unlink_temp_files( tcp );
      tcp->ce_flag |= F_MADE;
      tcp->ce_attr |= A_UPDATED;
      tcp = tcp->ce_all;
   }
   while( tcp != NIL(CELL) && tcp != cp );


   /* Scan the list of prerequisites and if we find one that is
    * marked as being removable, (ie. an inferred intermediate node
    * then remove it.  We remove a prerequisite by running the recipe
    * associated with the special target .REMOVE, with $< set to
    * the list of prerequisites to remove. */

   /* Make sure we don't try to remove prerequisites for the .REMOVE
    * target. */
   if( strcmp(cp->CE_NAME,".REMOVE") != 0 &&
       (hp = Get_name( ".REMOVE", Defs, FALSE )) != NIL(HASH) ) {
      register LINKPTR dp;
      int flag = FALSE;
      int rem;
      t_attr attr;

      tcp = hp->CP_OWNR;

      tcp->ce_flag |= F_TARGET;
      Clear_prerequisites( tcp );

      for( dp = cp->ce_prq; dp != NIL(LINK); dp = dp->cl_next ) {
	 register CELLPTR prq = dp->cl_prq;

	 attr = Glob_attr | prq->ce_attr;
	 rem  = (prq->ce_flag & F_REMOVE) &&
		(prq->ce_flag & F_MADE  ) &&
		!(prq->ce_attr & A_PHONY) &&
		!(attr & A_PRECIOUS) &&
		!Force;

	 if( rem ) {
	    CELLPTR tmp = prq;
	    do {
	       (Add_prerequisite(tcp,prq,FALSE,FALSE))->cl_flag |= F_TARGET;
	       prq->ce_flag &= ~F_REMOVE;
	       prq = prq->ce_all;
	    }
	    while( prq != NIL(CELL) && prq != tmp );
	    flag = TRUE;
	 }
      }

      if( flag ) {
	 Remove_prq( tcp );

	 for( dp=tcp->ce_prq; dp != NIL(LINK); dp=dp->cl_next ) {
	    register CELLPTR prq = dp->cl_prq;

	    prq->ce_flag &= ~(F_MADE|F_VISITED|F_STAT);
	    prq->ce_flag |= F_REMOVE;
	    prq->ce_time  = (time_t)0L;
	 }
      }
   }
}


PUBLIC int
Remove_file( name )
char *name;
{
   struct stat buf;

   if( stat(name, &buf) != 0 )
      return 1;
   if( (buf.st_mode & S_IFMT) == S_IFDIR )
      return 1;
   return(unlink(name));
}
