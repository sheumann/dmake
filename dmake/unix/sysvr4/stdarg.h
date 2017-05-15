/* Some V.4 systems aren't ansi, just to make matters ugly.
   If the dmake sources used another name for this include file,
   then this wouldn't be quite so awful. */
#ifdef __STDC__
#include "/usr/include/stdarg.h"
#ifndef va_dcl
#define va_dcl int va_alist
#endif
#else
/*
 * stdarg.h
 *
 * defines ANSI style macros for accessing arguments of a function which takes
 * a variable number of arguments
 *
 */

typedef char *va_list;

#if defined(sparc)
# define va_alist __builtin_va_alist
#endif
# define va_dcl int va_alist
# define va_start(list,v) list = (char *)&va_alist
# define va_end(list)     list = NULL
#if defined(mips)
#define va_arg(list, mode) ((mode *)(list = \
	(char *) (sizeof(mode) > 4 ? ((int)list + 2*8 - 1) & -8 \
				   : ((int)list + 2*4 - 1) & -4)))[-1]
#else
# define va_arg(list,mode) ((mode *)(list += sizeof(mode)))[-1]
#endif
#endif

