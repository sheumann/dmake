/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/mac/tomacfil.c,v 1.1 1992/01/24 03:29:53 dvadura Exp $
-- SYNOPSIS -- Routines to change unix file names to mac file names
--
-- DESCRIPTION
--  Dmake sometimes assumes that files have '/' as a directory parameter in some makefiles.
--  This works, even on DOS, but not on the Mac.  In fact, on the Mac, you can't even do a
--  a simple switch of ':' for '/' because all other the Mac has decided to reverse the use
--  of a first-character directory delimiter to mean a relative path rather than absolute path.
--  (i.e., ':test:b' means directory test is relative to the current directory, rather than
--  a top-level directory.  Thus, this file attempts to do the directory conversion behind
--  the back of the rest of the program.
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
--     $Log: tomacfil.c,v $
 * Revision 1.1  1992/01/24  03:29:53  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"
int IsUnixFName (char *pFileName);



/*
 * Convert a file that may be in unix format to mac format.
 *
 * The returned name is either the untouched pUnixName, or a static buffer.
 */
PUBLIC char *Unix2MacFName(char *pUnixName) {
    static char MacName[256];
    char *pCurMac;

    if (IsUnixFName (pUnixName)) {
        if (strlen (pUnixName) + (*pUnixName == '/' ? -1 : 1) >= sizeof (MacName)) {
            Fatal ("File name '%s' is too long and will cause a buffer overflow", pUnixName);
        } /* if */

        /* Set up relative or absolute path */
        pCurMac = MacName;
        if (*pUnixName == '/') {
            ++pUnixName;
        } else {
            *(pCurMac++) = ':';
        } /* if ... else */

        /* Convert the rest of the name */
        while (*pUnixName != '\0') {
            if (*pUnixName == '/') {
                *(pCurMac++) = ':';
                pUnixName++;

            } else {
                *(pCurMac++) = *(pUnixName++);
            } /* if ... else */
        } /* while */

        *pCurMac = '\0';
        return (MacName);

    } else {
        return (pUnixName);
    } /* if ... else */
} /* PUBLIC char *Unix2MacFName() */



/*
 * Is this file name in UNIX format?
 * (We assume it is if there are any slashes in its name.)
 */
int IsUnixFName (char *pFileName) {

    for ( ; *pFileName != '\0'; ++pFileName) {
        if (*pFileName == '/') {
            return (TRUE);
        } /* if */
    } /* while */

    return (FALSE);
} /* int IsUnixFName () */



/*
 * Call the real fopen() from this override of the function
 * that the rest of the program uses.
 */
#undef fopen
PUBLIC FILE *MacFOpen (char *pFName, char *pMode) {
    return (fopen (Unix2MacFName (pFName), pMode));
} /* PUBLIC FILE *MacFOpen () */
