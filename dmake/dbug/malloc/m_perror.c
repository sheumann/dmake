/*
 * (c) Copyright 1990 Conor P. Cahill (uunet!virtech!cpcahil).  
 * You may copy, distribute, and use this software as long as this
 * copyright statement is not removed.
 */

#ifndef lint
static
char rcsid[] = "$Id: m_perror.c,v 1.1 1992/01/24 03:29:04 dvadura Exp $";
#endif

/*
 * malloc errno error strings...
 */

char *malloc_err_strings[] = 
{
	"No errors",
	"Malloc chain is corrupted, pointers out of order",
	"Malloc chain is corrupted, end before end pointer",
	"Pointer is not within malloc area",
	"Malloc region does not have valid magic number in header",
	"Pointers between this segment and ajoining segments are invalid",
	"Data has overrun beyond requested number of bytes",
	"Data in free'd area has been modified",
	"Data are is not in use (can't be freed or realloced)",
	"Unable to get additional memory from the system",
	"Pointer within malloc region, but outside of malloc data bounds",
	(char *) 0
};

/*
 * Function:	malloc_perror()
 *
 * Purpose:	to print malloc_errno error message
 *
 * Arguments:	str	- string to print with error message
 *
 * Returns:	nothing of any value
 *
 * Narrative:
 */
void
malloc_perror(str)
	char	* str;
{
	extern int	  malloc_errno;
	register char 	* s;
	register char 	* t;

	if( str && *str)
	{
		for(s=str; *s; s++)
		{
			/* do nothing */;
		}

		(void) write(2,str,(unsigned)(s-str));
		(void) write(2,": ",(unsigned)2);
	}

	t = malloc_err_strings[malloc_errno];

	for(s=t; *s; s++)
	{
		/* do nothing */;
	}

	(void) write(2,t,(unsigned)(s-t));

	(void) write(2,"\n",(unsigned)1);
}

/*
 * $Log: m_perror.c,v $
 * Revision 1.1  1992/01/24  03:29:04  dvadura
 * dmake Version 3.8, Initial revision
 *
 * Revision 1.5  90/08/29  21:25:08  cpcahil
 * added additional error message that was missing (and 
 * caused a core dump)
 * 
 * Revision 1.4  90/05/11  00:13:08  cpcahil
 * added copyright statment
 * 
 * Revision 1.3  90/02/24  21:50:21  cpcahil
 * lots of lint fixes
 * 
 * Revision 1.2  90/02/24  17:39:55  cpcahil
 * 1. added function header
 * 2. added rcs id and log strings.
 * 
 */
