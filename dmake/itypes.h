/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/RCS/itypes.h,v 1.1 1992/01/24 03:29:40 dvadura Exp $
-- SYNOPSIS -- type declarations for common types
-- 
-- DESCRIPTION
-- 	portable type declarations.
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
--     $Log: itypes.h,v $
 * Revision 1.1  1992/01/24  03:29:40  dvadura
 * dmake Version 3.8, Initial revision
 *
*/


#ifndef ITYPES_h
#define	ITYPES_h

#if defined(M_I86) || defined(MC68000)
typedef char  int8;               /* typedefs for right size ints */
typedef int   int16;
typedef long  int32;
typedef unsigned char  uint8;
typedef unsigned int   uint16;
typedef unsigned long  uint32;
#else
typedef char  int8;               /* typedefs for right size ints */
typedef short int16;
typedef long  int32;
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned long  uint32;
#endif

#endif

