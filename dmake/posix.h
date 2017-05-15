#ifndef _POSIX_NAME_MAX
/* RCS      -- $Header: /u/dvadura/src/generic/dmake/src/RCS/posix.h,v 1.1 1992/04/07 04:42:46 dvadura Exp $
-- SYNOPSIS -- Definition for POSIX conforming defines in dmake.
-- 
-- DESCRIPTION
--	This file is intended to make certain that defines used within dmake
--	for file name lengths, and number of children processes are defined.
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
--     $Log: posix.h,v $
 * Revision 1.1  1992/04/07  04:42:46  dvadura
 * Initial revision
 *
*/

/* Define the minimum values that each system requires, and do so only if
 * we have not defined these includes elsewhere.  These should already be
 * defined in <limits.h> if you have a C compiler that is POSIX compliant.
 */
#ifndef _POSIX_NAME_MAX
#define _POSIX_NAME_MAX 14
#endif

#ifndef _POSIX_PATH_MAX
#define _POSIX_MATH_MAX 64
#endif

#ifndef _POSIX_CHILD_MAX
#define _POSIX_CHILD_MAX 1
#endif

/* Now define the actual manifests used in the code. */
#ifndef NAME_MAX
#define NAME_MAX _POSIX_NAME_MAX
#endif

#ifndef PATH_MAX
#define PATH_MAX _POSIX_PATH_MAX
#endif

#ifndef CHILD_MAX
#define CHILD_MAX _POSIX_CHILD_MAX
#endif

#endif
