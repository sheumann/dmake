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
# define va_arg(list,mode) ((mode *)(list += sizeof(mode)))[-1]
