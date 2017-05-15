/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/mac/tempnam.c,v 1.1 1992/01/24 03:29:52 dvadura Exp $
-- SYNOPSIS -- Fake tempnam function for the mac
--
-- DESCRIPTION
--  Get a temporary file name.
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
--     $Log: tempnam.c,v $
 * Revision 1.1  1992/01/24  03:29:52  dvadura
 * dmake Version 3.8, Initial revision
 *
*/


#include "extern.h"
#include <StdIO.h>
#include <String.h>



/*
 * Try to open a temporary file in the given directory (if non-NULL)
 * with the given prefix (if non-NULL).
 *
 * We ignore the directory argument.
 */
PUBLIC char *tempnam (char * /* pDir */, char * pPrefix) {
    char *pName;
    char *pFullName;

    pName = tmpnam ((char *) NULL);

    /* Assume that if the name returned by tmpnam is not being used,
       the name with the prefix is also not being used. */
    pFullName = MALLOC (((pPrefix != NULL) ? strlen (pPrefix) : 0) +
                        strlen (pName) + 1, char);

    /* Copy in the name if we successfully allocated space for it. */
    if (pFullName != NULL) {
        if (pPrefix != NULL) {
            strcpy (pFullName, pPrefix);
        } else {
            *pFullName = '\0';
        } /* if ... else */

        strcat (pFullName, pName);
    } /* if */
    
    return (pFullName);
} /* PUBLIC char *tempnam () */
