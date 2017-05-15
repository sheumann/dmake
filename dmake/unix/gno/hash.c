#ifdef __CCFRONT__
#include <14:pragma.h>
#endif
/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/RCS/hash.c,v 1.1 1992/01/24 03:27:06 dvadura Exp $
-- SYNOPSIS -- hashing function for hash tables.
-- 
-- DESCRIPTION
--      Hash an identifier.  The hashing function works by computing the sum
--      of each char and the previous hash value multiplied by 129.  Finally the
--      length of the identifier is added in.  This way the hash depends on the
--      chars as well as the length, and appears to be sufficiently unique,
--      and is FAST to COMPUTE, unlike the previous hash function...
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
--     $Log: hash.c,v $
 * Revision 1.1  1992/01/24  03:27:06  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"

PUBLIC uint16
Hash( char *id, uint32 *phv )/*
=================
      This function computes the identifier's hash value and returns the hash
      value modulo the key size as well as the full hash value.  The reason
      for returning both is so that hash table searches can be sped up.  You
      compare hash keys instead and compare strings only for those whose 32-bit
      hash keys match. (not many) */

{
   register char   *p    = id;
   register uint32 hash  = (uint32) 0;

   while( *p ) hash = (hash << 7) + hash + (uint32) (*p++);
   *phv = hash = hash + (uint32) (p-id);

   return( (uint16) (hash % HASH_TABLE_SIZE) );
}
