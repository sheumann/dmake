/* This file contains the default value of the DMMAKESTARTUP variable.
 * You must set the quoted string below to the default path to the startup
 * variable, so that it gets compiled in.  LEAVE DMSTARTUPDIR in
 * the path.  This allows the user to customize his environment for dmake
 * by setting up a new DMSTARTUPDIR environment variable. */

"MAKESTARTUP := $(MPW)tools:$(DMSTARTUPDIR)startup.mk",
