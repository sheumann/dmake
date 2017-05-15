/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/unix/ruletab.c,v 1.1 1992/01/24 03:28:29 dvadura Exp $
-- SYNOPSIS -- Default initial configuration of dmake.
-- 
-- DESCRIPTION
-- 	Define here the initial set of rules that are defined before
--	dmake performs any processing.
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
--     $Log: ruletab.c,v $
 * Revision 1.1  1992/01/24  03:28:29  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

/* These are control macros for dmake that MUST be defined at some point
 * if they are NOT dmake will not work!  These are default definitions.  They
 * may be overridden inside the .STARTUP makefile, they are here
 * strictly so that dmake can parse the STARTUP makefile */

static char *_rules[] = {
	"MAXPROCESSLIMIT := 10",
	"MAXLINELENGTH := 8190",
	".IMPORT .IGNORE: ROOTDIR",
#ifdef NO_CASE							/* for case insensitive filesystems */
	".MAKEFILES : makefile.mk Makefile",
#else
	".MAKEFILES : makefile.mk Makefile makefile",
#endif
	".SOURCE    : .NULL",
#include "startup.h"
	0 };

char **Rule_tab = _rules; /* for sundry reasons in Get_environment() */
