/*
 * (c) Copyright 1990 Conor P. Cahill (uunet!virtech!cpcahil).  
 * You may copy, distribute, and use this software as long as this
 * copyright statement is not removed.
 */
#include <stdio.h>
#include <fcntl.h>
#include "malloc.h"

/*
 * Function:	mallopt()
 *
 * Purpose:	to set options for the malloc debugging library
 *
 * Arguments:	none
 *
 * Returns:	nothing of any value
 *
 * Narrative:	
 *
 */

#ifndef lint
static
char rcs_hdr[] = "$Id: mallopt.c,v 1.1 1992/01/24 03:29:07 dvadura Exp $";
#endif

int
mallopt(cmd,value)
	int			  cmd;
	union malloptarg	  value;
{
	int			  i;
	extern int		  malloc_checking;
	extern char		* malloc_data_start;
	extern int		  malloc_errfd;
	extern int		  malloc_fatal_level;
	void			  malloc_init();
	extern int		  malloc_warn_level;
	register char		* s;

	/*
 	 * If not initialized...
	 */
	if( malloc_data_start == (char *) 0)
	{
		malloc_init();
	}


	switch(cmd)
	{
		case MALLOC_WARN:
			malloc_warn_level = value.i;
			break;

		case MALLOC_FATAL:
			malloc_fatal_level = value.i;
			break;

		case MALLOC_CKCHAIN:
			malloc_checking = value.i;
			break;

		case MALLOC_ERRFILE:
			
			i = open(value.str,O_CREAT|O_APPEND|O_WRONLY,0666);
			if( i == -1 )
			{
				(void) write(2,
					  "Unable to open malloc error file: ",
					  (unsigned) 34);
				for(s=value.str; *s; s++)
				{
					/* do nothing */;
				}
				(void) write(2,value.str,
					     (unsigned)(s-value.str));
				(void) write(2,"\n",(unsigned)1);
			}
			else
			{
				if( malloc_errfd != 2 )
				{
					(void) close(malloc_errfd);
				}
				malloc_errfd = i;
			}
			
			break;

		default:
			return(1);
	}

	return(0);
}

/*
 * $Log: mallopt.c,v $
 * Revision 1.1  1992/01/24  03:29:07  dvadura
 * dmake Version 3.8, Initial revision
 *
 * Revision 1.6  90/08/29  22:23:36  cpcahil
 * fixed mallopt to use a union as an argument.
 * 
 * Revision 1.5  90/08/29  21:22:51  cpcahil
 * miscellaneous lint fixes
 * 
 * Revision 1.4  90/05/11  00:13:10  cpcahil
 * added copyright statment
 * 
 * Revision 1.3  90/02/25  11:03:26  cpcahil
 * changed to return int so that it agrees with l libmalloc.a's mallopt()
 * 
 * Revision 1.2  90/02/25  11:01:21  cpcahil
 * added support for malloc chain checking.
 * 
 * Revision 1.1  90/02/24  21:50:24  cpcahil
 * Initial revision
 * 
 * Revision 1.1  90/02/24  17:10:53  cpcahil
 * Initial revision
 * 
 */
