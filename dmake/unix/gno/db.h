/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/RCS/db.h,v 1.1 1992/01/24 03:29:35 dvadura Exp $
-- SYNOPSIS -- front end to DBUG macros.
-- 
-- DESCRIPTION
--	This is a front end to Fred Fish's DBUG macros.  The intent was
--	to provide an interface so that if you don't have the DBUG code
--	you can still compile dmake, by undefining DBUG, if you do have
--	the code then you can use Fred Fish's DBUG package.  Originally
--	the DBUG stuff was copyrighted, it is now in the public domain
--	so the need for this is not as apparent.
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
--     $Log: db.h,v $
 * Revision 1.1  1992/01/24  03:29:35  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#ifndef	DB_h
#define	DB_h

#if defined(DBUG)

#  include "dbug.h"

#  define DB_ENTER(a1)                DBUG_ENTER(a1)
#  define DB_RETURN(a1)               DBUG_RETURN(a1)
#  define DB_VOID_RETURN              DBUG_VOID_RETURN
#  define DB_EXECUTE(keyword, a1)     DBUG_EXECUTE(keyword,a1)
#  define DB_PRINT(keyword,arglist)   DBUG_PRINT(keyword,arglist)
#  define DB_PUSH(a1)                 DBUG_PUSH(a1)
#  define DB_POP()                    DBUG_POP()
#  define DB_PROCESS(a1)              DBUG_PROCESS(a1)
#  define DB_FILE(file)               DBUG_FILE(file)
#  define DB_SETJMP                   DBUG_SETJMP
#  define DB_LONGJMP                  DBUG_LONGJMP

#else

#  define DB_ENTER(a1)
#  define DB_RETURN(a1)               return (a1)
#  define DB_VOID_RETURN              return
#  define DB_EXECUTE(keyword, a1)
#  define DB_PRINT(keyword,arglist)
#  define DB_PUSH(a1)
#  define DB_POP()
#  define DB_PROCESS(a1)
#  define DB_FILE(file)
#  define DB_SETJMP                   setjmp
#  define DB_LONGJMP                  longjmp

#endif
#endif
