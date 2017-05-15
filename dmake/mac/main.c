/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/mac/main.c,v 1.1 1992/01/24 03:29:48 dvadura Exp $
-- SYNOPSIS -- The real main function
--
-- DESCRIPTION
--  In order to get the third argument to main(), which is a list of
--  environmental variables, we have #defined main to dmakemain,
--  and put the real main here.
--
--  The environmental variables are placed in the environ global variable
--  and set up for processing by dmake in make_env().
--
-- AUTHOR
--      Dennis Vadura, dvadura@watdragon.uwaterloo.ca
--      CS DEPT, University of Waterloo, Waterloo, Ont., Canada
--
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
--     $Log: main.c,v $
 * Revision 1.1  1992/01/24  03:29:48  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"



/*
 * Put envp in environ and call dmake's main().
 */
#undef main
void main (int argc, char *argv[], char *envp[]) {
    environ = envp;
    dmakemain (argc, argv);
} /* void main () */
