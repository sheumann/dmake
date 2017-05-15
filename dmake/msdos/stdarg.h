/*
 * stdarg.h
 *
 * defines ANSI style macros for accessing arguments of a function which takes
 * a variable number of arguments
 *
 */

#if !defined(__STDARG)
#define __STDARG

#if !defined(ZTC)
#define __ss
#endif

typedef char __ss *va_list;

#define va_dcl int va_alist
#define va_start(ap,v)  ap = (va_list)&va_alist
#define va_arg(ap,t)    ((t*)(ap += sizeof(t)))[-1]
#define va_end(ap)      ap = NULL
#endif
