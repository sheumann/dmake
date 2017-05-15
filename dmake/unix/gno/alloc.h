/* RCS      -- $Header: /u/dvadura/src/generic/dmake/src/RCS/alloc.h,v 1.2 1992/04/07 04:42:46 dvadura Exp $
-- SYNOPSIS -- macros for allocating memory.
-- 
-- DESCRIPTION
--	A somewhat nicer interface to malloc and calloc.
--	Here we standardise the calling convention with a common macro
--	interface.
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
--     $Log: alloc.h,v $
 * Revision 1.2  1992/04/07  04:42:46  dvadura
 * Added a !defined(_SIZE_T_) to satisfy Silicon Graphics IRIX.
 *
 * Revision 1.1  1992/01/24  03:26:54  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#ifndef ALLOC_h
#define ALLOC_h

/* DO NOT CHANGE these!  These are the definitions that the make source
 * uses for allocating memory.  They must be defined for make to compile
 * properly.
 */

/* This is the only place that we define size_t now.  This should be more
 * than enough! */
#if __STDC__
#else
#   if !defined(_TYPES_) && !defined(M_XENIX) && !defined(atarist) && !defined(_MPW) && !defined(_SIZE_T) && !defined(_SIZE_T_)
#      if defined(MSDOS) || defined(__MSDOS__)
#         undef size_t
          typedef unsigned size_t;
#      else
          typedef long size_t;
#      endif
#   endif
#endif

#define	usizeof(t)	(size_t)sizeof(t)

#define FREE(p)         free((char*)(p))
#define MALLOC(n, t)    (t*) malloc((unsigned int)(n)*usizeof(t))
#define CALLOC(n, t)    (t*) calloc((unsigned int)(n), usizeof(t))

#define TALLOC(p, n, t)	if ((p = CALLOC(n, t)) == (t*)0) {No_ram();}

#endif

