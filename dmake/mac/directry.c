/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/mac/directry.c,v 1.1 1992/01/24 03:29:45 dvadura Exp $
-- SYNOPSIS -- Fake directory and file functions for the Mac
--
-- DESCRIPTION
--  This file contains implementations for some ANSI standard routines dmake
--  uses which are not otherwise available for the mac.
--
--  Assume we are using at least 128K ROMS.
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
--     $Log: directry.c,v $
 * Revision 1.1  1992/01/24  03:29:45  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include <Errors.h>
#include <Files.h>
#include <OSUtils.h>
#include <StdLib.h>
#include <Strings.h>
#include <SysEqu.h>
#include "extern.h"



/*
 * Implementation of stat function for dmake on the mac.
 *
 * Many fields aren't filled in, and the times are seconds from 1/1//1904,
 * but it should be enough for dmake (I think we only need st_mtime and
 * st_mode's S_IFDIR set correctly).
 */
PUBLIC int stat(pPath, pStat)
char *pPath;
struct stat *pStat;
{
    CInfoPBRec infoPB;
    OSErr err;
    int retVal;

    pPath = Unix2MacFName (pPath);

    infoPB.hFileInfo.ioCompletion = NULL;
    infoPB.hFileInfo.ioNamePtr = c2pstr (pPath);
    infoPB.hFileInfo.ioVRefNum = 0;
    infoPB.hFileInfo.ioFDirIndex = 0;
    infoPB.hFileInfo.ioDirID = 0;
    err = PBGetCatInfo(&infoPB, FALSE);
    p2cstr ((StringPtr) pPath);

    if (err == noErr) {
        pStat->st_mtime = (time_t) infoPB.hFileInfo.ioFlMdDat;
        pStat->st_ctime = (time_t) infoPB.hFileInfo.ioFlCrDat;
        pStat->st_mode = S_IREAD | S_IEXEC;

        /* If it is a directory ... */
        if (infoPB.hFileInfo.ioFlAttrib & 0x10) {
            pStat->st_size = infoPB.dirInfo.ioDrNmFls;
            pStat->st_mode |= S_IFDIR;
        } else {
            pStat->st_size = infoPB.hFileInfo.ioFlLgLen;
            pStat->st_mode |= S_IFREG;
        } /* if ... else */

        /* If it is writeable */
        if ((infoPB.hFileInfo.ioFlAttrib & 0x1) == 0) {
            pStat->st_mode |= S_IWRITE;
        } /* if */
        
        retVal = 0;

    } else {
        retVal = -1;
    } /* if ... else */

    return (retVal);
} /* PUBLIC int stat () */



/*
 * Return the current working directory, or NULL if there is an error.
 */
PUBLIC char *getcwd (char *pPath, size_t pathSize) {
    DirInfo dirInfo;
    OSErr err;
    Str255 dirName;
    char *pBeginName;
    char *pC;
    size_t len;
    size_t spaceForColon;

    pPath = Unix2MacFName (pPath);

    /* Set up the info for the PBGetCatInfo() calls */
    dirInfo.ioCompletion = NULL;
    dirInfo.ioNamePtr = dirName;
    dirInfo.ioVRefNum = 0;
    dirInfo.ioFDirIndex = -1;
    dirInfo.ioDrDirID = 0;
    pBeginName = pPath + pathSize - 1;
    spaceForColon = 0;  /* Make sure we don't have an end colon on the name */

    /*
     * Keep going up the directory path until the end is reached or an error
     * occurs.  Ideally, we would check for errors at every level and stop
     * when we received an fnfErr (File Not Found), but it appears that there
     * are some problems with network volumes.  (During testing, I received
     * a paramErr (No Default Volume) beyond the top level.)  Thus, to keep it
     * simple, I assume any error past the first directory indicates we have
     * seen all directories.
     */
    while (TRUE) {
        err = PBGetCatInfo ((CInfoPBPtr) &dirInfo, FALSE);
        len = ((size_t)(unsigned char) dirName[0]);
        if ((err == noErr) && (len < pBeginName - pPath)) {
            p2cstr (dirName);
            pBeginName -= len + spaceForColon;
            strcpy (pBeginName, dirName);
            /* Note that strcpy() adds the '\0' at the end of
               the first directory for us */
            if (spaceForColon == 1) {
                pBeginName[len] = ':';
            } else {
                /* The end of the string shouldn't have a ':' */
                spaceForColon = 1;
            } /* if */

            /* Set up for the next call to PBGetCatInfo() with
               the parent's directory ID */
            dirInfo.ioDrDirID = dirInfo.ioDrParID;

        } else if (spaceForColon == 1) {
            /* We got past the top-level directory */
            break;

        } else {
            /* We either have an error when looking at the first directory
               or have run out of room. */
            return (NULL);
        } /* if ... elses */
    } /* while */

    /* Now copy the directory string to the beginning of the path string.
       (It's possible the directory already starts at the beginning of the
       string, but this is unlikely and doesn't hurt anything if it does,
       so we don't bother to check for it.) */
    pC = pPath;
    while ((*(pC++) = *(pBeginName++)) != '\0')
        ;

    return (pPath);
} /* PUBLIC char *getcwd () */



/*
 * Change the directory to a new default directory.
 *
 * Return 0 if successful, or -1 if there is an error.
 */
PUBLIC int chdir (char *pPath) {
    WDPBRec WDPB;
    VolumeParam vParam;
    OSErr err;
    int result;
    char *pC;
    char c;

    pPath = Unix2MacFName (pPath);

    /* Set up the directory */
    c2pstr (pPath);
    WDPB.ioCompletion = NULL;
    WDPB.ioNamePtr = pPath;
    WDPB.ioVRefNum = 0;
    WDPB.ioWDProcID = 0;
    WDPB.ioWDDirID = 0;
    err = PBOpenWD (&WDPB, FALSE);
    /* Restore path to a C-type string in case the caller wants
       to use it after this call. */
    p2cstr (pPath);
    if (err != noErr) {
        return (-1);
    } /* if */

    /* Set up the volume if necessary */
    if (*pPath != ':') {
        for (pC = pPath + 1; (*pC != ':') && (*pC != '\0'); ++pC)
            ;
        c = *pC;
        *pC = '\0';
        vParam.ioCompletion = NULL;
        vParam.ioNamePtr = c2pstr (pPath);
        vParam.ioVRefNum = WDPB.ioVRefNum;
        err = PBSetVol ((ParmBlkPtr) &vParam, FALSE);
        p2cstr (pPath);
        *pC = c;
        result = ((err == noErr) ? 0 : -1);

    } else {
        result = 0;
    } /* if ... else */
    
    return (result);
} /* PUBLIC int chdir () */



/*
 * Change the modification time for the file to the current time.
 *
 * The normal version of utime can set the modification time to any
 * time, this function aborts the function if this is tried.
 *
 * We return 0 if the modification time was updated and -1 if there
 * was an error.
 */
PUBLIC int utime (char *pPath, time_t *pTimes) {
    CInfoPBRec infoPB;
    OSErr err;

    pPath = Unix2MacFName (pPath);

    if (pTimes != NULL) {
        Fatal ("SUBROUTINE SHORTCOMING: utime cannot take a utimbuf struct");
    } /* if */

    /* Get the old info */
    infoPB.hFileInfo.ioCompletion = NULL;
    infoPB.hFileInfo.ioNamePtr = c2pstr (pPath);
    infoPB.hFileInfo.ioVRefNum = 0;
    infoPB.hFileInfo.ioFDirIndex = 0;
    infoPB.hFileInfo.ioDirID = 0;
    err = PBGetCatInfo (&infoPB, FALSE);
    if (err != noErr) {
        p2cstr ((StringPtr) pPath);
        return (-1);
    } /* if */

    /* Change the modification time and set the new info */
    GetDateTime (&(infoPB.hFileInfo.ioFlMdDat));
    infoPB.hFileInfo.ioDirID = 0;
    err = PBSetCatInfo (&infoPB, FALSE);
    p2cstr ((StringPtr) pPath);
    return ((err == noErr) ? 0 : -1);
} /* PUBLIC int utime () */
