# This is the MSC 4.0 and higher DOS configuration file for DMAKE
#	It simply modifies the values of SRC, and checks to see if
#	OSENVIRONMENT is defined.  If so it includes the appropriate
#	config.mk file.
#
# It also sets the values of .SOURCE.c and .SOURCE.h to include the local
# directory.
#
osrdir := $(OS)$(DIRSEPSTR)$(OSRELEASE)

TMPDIR :=
.EXPORT : TMPDIR

# Definition of macros for library, and C startup code.

# The following sources are required for MSC
OSR_SRC = tempnam.c
.SETDIR=$(osrdir) : $(OSR_SRC)

SRC += $(OSR_SRC)
.SOURCE.h : $(osrdir)

SET_STACK = /stack:4096
NDB_LDFLAGS += $(SET_STACK)

# Local configuration modifications for CFLAGS 
# If you have a 286 or better, you can uncomment the following line.
#HAVE_286 = y

.IF $(HAVE_286)
  CFLAGS  += -G2
  ASFLAGS += -Dhave286
.END

ASFLAGS += -t -mx $(S_$(MODEL))

# Microsoft C doesn't need tail but needs head
LDTAIL = ;
LDHEAD = $(LDFLAGS)

# Debugging libraries
DB_LDFLAGS += /co /li /map $(SET_STACK)
DB_LDLIBS  +=

# NO Debug MSC flags:
# Set the environment variable MSC_VER to be one of 4.0, 5.0, 5.1, or 6.0
# to get these by default when you make dmake using 'dmake'.
#
# Setting MSC_VER to one of the above sets the variable _MSC_VER appropriately
# and sets the flags appropriately.

.IMPORT .IGNORE : MSC_VER
MSC_VER *= 6.0			  # If unset, assume 6.0 by default.

.IF $(MSC_VER) == 4.0
   CFLAGS      += -I$(osrdir) $(C_$(MODEL):s/A/m/)
   CFLAGS      += -DM_I86=1	  # 5.0+ define this automatically
#   CFLAGS      += -D__STDC__=1    # 5.0, 5.1, but not 6.0 do this automatically
   NDB_CFLAGS  +=
   DB_CFLAGS   += -Zi
.ELSE
   DB_CFLAGS   += -Zi
   CFLAGS      += -I$(osrdir) $(C_$(MODEL))
   .IF $(MSC_VER) != 6.0
      # For 5.0 and 5.1, we define _MSC_VER=500 or 510
      CFLAGS      += -D_MSC_VER=$(MSC_VER:s,.,,)0
      NDB_CFLAGS  += -Oscl -Gs
   .ELSE
      # Microsoft C 6.0 auto defines _MSC_VER=600, but not __STDC__
      CFLAGS      += -D__STDC__=1 # incredibly not auto done by 6.0
      NDB_CFLAGS  += -Osecgl -Gs

      # Redefine rule for making our objects, we don't need mv
      %$O : %.c ;% $(CC) -c $(CFLAGS) -Fo$@ $<
   .END
   NDB_LDFLAGS += /exe /packc /batch
   NDB_LDLIBS  +=
.END

# See if we modify anything in the lower levels.
.IF $(OSENVIRONMENT) != $(NULL)
   .INCLUDE .IGNORE : $(osrdir)$(DIRSEPSTR)$(OSENVIRONMENT)$(DIRSEPSTR)config.mk
.END

C_s =
C_m = -AM
C_c = -AC
C_l = -AL

S_s = -Dmsmall
S_m = -Dmmedium
S_c = -Dmcompact
S_l = -Dmlarge
