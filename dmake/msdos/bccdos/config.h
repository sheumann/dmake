/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/msdos/bccdos/config.h,v 1.1 1992/01/24 03:27:46 dvadura Exp $
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
 * Revision 1.1  1992/01/24  03:27:46  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

/* define this for configurations that don't have the coreleft function
 * so that the code compiles.  To my knowledge coreleft exists only on
 * Turbo C, but it is needed here since the function is used in many debug
 * macros. */
/*#define coreleft() 0L*/
extern unsigned int coreleft();

#define SIGQUIT	SIGTERM		/* turbo C doesn't understand SIGQUIT */

/* Turbo-C understands const declarations. */
#define CONST const

#ifndef MSDOS
#   define MSDOS 1
#endif

/* a small problem with pointer to voids on some unix machines needs this */
#define PVOID void *
