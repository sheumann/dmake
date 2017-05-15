/* RCS      -- $Header: /u/dvadura/src/generic/dmake/src/RCS/extern.h,v 1.2 1992/04/07 04:42:46 dvadura Exp $
-- SYNOPSIS -- external declarations for dmake functions.
-- 
-- DESCRIPTION
--	ANSI is a macro that allows the proper handling of ANSI style
--	function declarations.
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
--     $Log: extern.h,v $
 * Revision 1.2  1992/04/07  04:42:46  dvadura
 * Changed namemax.h to posix.h.
 *
 * Revision 1.1  1992/01/24  03:26:52  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#ifndef EXTERN_h
#define EXTERN_h

/* Define this for the RS/6000 if it breaks something then we have to put a
 * #ifdef around it. */
#if defined(rs6000)
#define _POSIX_SOURCE
#endif

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#if defined (_MPW)
# include <types.h>
# include <time.h>
#else
# include <sys/types.h>
# include <sys/stat.h>
#endif
#include <signal.h>
#include "itypes.h"
#include "stdmacs.h"
#include "alloc.h"
#include "db.h"
#include "dmake.h"
#include "struct.h"
#include "vextern.h"
#include "public.h"

/* Include this last as it invalidates some functions that are defined
 * externally above and turns them into no-ops.  Have to do this after
 * the extern declarations however. */
#include "config.h"
#include "posix.h"

#endif

