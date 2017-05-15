/*
 * stdarg.h
 *
 * defines ANSI style macros for accessing arguments of a function which takes
 * a variable number of arguments
 *
 */

#if !defined(__STDARG_H__)
#define __STDARG_H__

#if !defined(_VA_LIST_)
#define _VA_LIST_
typedef char *va_list;
#endif

#define va_dcl int va_alist
#define va_start(ap,v)  ap = (va_list)&va_alist
#define va_arg(ap,t)    ((t*)(ap += sizeof(t)))[-1]
#define va_end(ap)      ap = NULL
#endif
