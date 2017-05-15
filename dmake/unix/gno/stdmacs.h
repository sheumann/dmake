/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/RCS/stdmacs.h,v 1.1 1992/01/24 03:29:36 dvadura Exp $
-- SYNOPSIS -- general use macros.
-- 
-- DESCRIPTION
-- 	ANSI macro relies on the fact that it can be replaced by (), or by
--	its value, where the value is one value due to the preprocessors
--	handling of arguments that are surrounded by ()'s as a single
--	argument.
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
--     $Log: stdmacs.h,v $
 * Revision 1.1  1992/01/24  03:29:36  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#ifndef	MACROS_h
#define	MACROS_h

/* stupid AIX and Mac MPW define __STDC__ as special, but defined(__STDC__) is false, and
 * it's value is nothing */
#if !defined(__STDC__) && !defined(_AIX) && !defined(_MPW)
#define __STDC__ 0
#endif

#if __STDC__ || defined(__TURBOC__)
#define	ANSI(x)	x
#else
#define	ANSI(x)	()
#endif

#ifdef __ORCAC__
# undef NIL
#endif

#define	NIL(p)	((p*)NULL)

#if !defined(atarist)
#define	offsetof(type,id) ((size_t)&((type*)NULL)->id)
#endif

#define	FALSE	0
#define	TRUE	1

#define PUBLIC

#endif
