/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/mac/eold.c,v 1.1 1992/01/24 03:29:47 dvadura Exp $
-- SYNOPSIS -- Set up and free for environ
--
-- DESCRIPTION
--  This file contains routines that will fill in and dispose of the
--  list of environmental variables in the environ global variable.
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
--     $Log: eold.c,v $
 * Revision 1.1  1992/01/24  03:29:47  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"


/*
 * Keep track of any environmental variables that have '='s in their
 * name.
 */
struct EqualPos {
    char *fpPos;
    struct equalsign *fpNext;
} /* struct EqualPos */

struct EqualPos *gpEqualList;

/*
 * The character used to replae the equal signs.
 */
const char gEqualReplace = '_';



/*
 * Set up the environmental variables in a format used by
 * the environ global variable.
 *
 * environ has already been set to main's envp argument when
 * this suboroutine is called.
 */
void main_env () {
    char **ppCurEnv;
    char *pCurPos;
    struct equalpos *pNewEqual;

    gpEqualList = NULL;

    for (ppCurEnv = environ; *ppCurEnv != NULL; ++ppCurEnv) {
        for (pCurPos = *ppCurEnv; *pCurPos != '\0'; ++pCurPos) {
            if (*pCurPos == '=') {
                if ((pNewEqual =
                     (struct EqualPos *) malloc (sizeof (struct EqualPos))) ==
                    NULL) {
                    fputs ("Out of Memory", stderr);
                    exit (EXIT_FAILURE);
                } /* if */
                pNewEqual->fpPos = pCurPos;
                pNewEqual->fpNext = gpEqualList;
                gpEqualList = pNewEqual;
                
                *pCurPos = gEqualReplace;
            } /* if */
        } /* for */

        *pCurPos = '=';
    } /* for */
} /* void main_env () */



/*
 * Reset the environmental variables so they look like they did
 * before the main_env() call.
 *
 * environ has already been set to main's envp argument when
 * this suboroutine is called.
 */
void main_env () {
    char **ppCurEnv;
    char *pCurPos;
    struct equalpos *pNewEqual;

    gpEqualList = NULL;

    for (ppCurEnv = environ; *ppCurEnv != NULL; ++ppCurEnv) {
        for (pCurPos = *ppCurEnv; *pCurPos != '\0'; ++pCurPos) {
            if (*pCurPos == '=') {
                if ((pNewEqual =
                     (struct EqualPos *) malloc (sizeof (struct EqualPos))) ==
                    NULL) {
                    fputs ("Out of Memory", stderr);
                    exit (EXIT_FAILURE);
                } /* if */
                pNewEqual->fpPos = pCurPos;
                pNewEqual->fpNext = gpEqualList;
                gpEqualList = pNewEqual;
                
                *pCurPos = gEqualReplace;
            } /* if */
        } /* for */

        *pCurPos = '=';
    } /* for */
} /* void main_env () */
