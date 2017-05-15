/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/msdos/mscdos/config.h,v 1.1 1992/01/24 03:27:25 dvadura Exp $
-- SYNOPSIS -- Configurarion include file.
-- 
-- DESCRIPTION
-- 	There is one of these for each specific machine configuration.
--	It can be used to further tweek the machine specific sources
--	so that they compile.
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
--     $Log: config.h,v $
 * Revision 1.1  1992/01/24  03:27:25  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#if defined (_MSC_VER)
# if _MSC_VER < 500
	Force a compile-time blowup.
	Do not define define _MSC_VER for MSC compilers ealier than 5.0.
# endif
#endif

/* define this for configurations that don't have the coreleft function
 * so that the code compiles.  To my knowledge coreleft exists only on
 * Turbo C, but it is needed here since the function is used in many debug
 * macros. */
#define coreleft() 0L

/* MSC Version 4.0 doesn't understand SIGTERM, later versions do. */
#ifndef SIGTERM
#   define SIGTERM SIGINT
#endif

/* Fixes unimplemented line buffering for MSC 5.x and 6.0.
 * MSC _IOLBF is the same as _IOFBF
 */
#if defined(MSDOS) && defined (_MSC_VER)
#   undef  _IOLBF
#   define _IOLBF   _IONBF
#endif

/* in alloc.h: size_t is redefined
 * defined in stdio.h which is included by alloc.h
 */
#if defined(MSDOS) && defined (_MSC_VER)
#   define _TYPES_
#endif

/* in sysintf.c: SIGQUIT is used, this is not defined in MSC */
#ifndef SIGQUIT
#   define SIGQUIT SIGTERM
#endif

/* MSC doesn't seem to care about CONST */
#define CONST

#ifndef MSDOS
#   define MSDOS 1
#endif

/* a small problem with pointer to voids on some unix machines needs this */
#define PVOID void *
