/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/msdos/rmprq.c,v 1.1 1992/01/24 03:27:30 dvadura Exp $
-- SYNOPSIS -- remove prerequisites code.
-- 
-- DESCRIPTION
--	This code is different for DOS and for UNIX and parallel make
--	architectures since the parallel case requires the rm's to be
--	run in parallel, whereas DOS guarantees to run them sequentially.
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
--     $Log: rmprq.c,v $
 * Revision 1.1  1992/01/24  03:27:30  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"

PUBLIC void
Remove_prq( tcp )
CELLPTR tcp;
{
   tcp->ce_flag &= ~(F_MADE|F_VISITED);
   tcp->ce_time  = 0L;

   Make( tcp, NIL(LINK), NIL(CELL) );
}
