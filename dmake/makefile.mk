# 			//// Makefile for DMAKE. \\\\
# The target system is characterized by the following macros imported from
# the environment.
#
#	OS	      - gives the class of operating system
#	OSRELEASE     - optionally gives the particular release of the OS above.
#	OSENVIRONMENT - optionally gives the environment under which the above
#			OS is in use.
#
# For valid values for the above macros consult the readme/* files or type
# 'make' by itself to get a summary of what is available.

# First target in the makefile, do this so that targets declared in the
# included files are never marked as being the first *default* target.
first : all ;

#Enable keeping of state for future compiles
.KEEP_STATE := _state.mk

# Pull in the configuration macros, from the environment.  OS is required,
# OSRELEASE, and OSENVIRONMENT are optional.
.IF $(OS) == $(NULL)
   .IMPORT : OS
.END
.IMPORT .IGNORE : OSRELEASE OSENVIRONMENT TMPDIR

# Define $(PUBLIC)
_osenv  := $(OSENVIRONMENT)$(DIRSEPSTR)
_osre   := $(OSRELEASE)$(DIRSEPSTR)$(!null,$(OSENVIRONMENT) $(_osenv))
ENVDIR   = $(OS)$(DIRSEPSTR)$(!null,$(OSRELEASE) $(_osre))
PUBLIC   = $(ENVDIR)public.h
STARTUP := startup.mk

# Define the source files
SRC =\
	infer.c make.c stat.c expand.c dmstring.c hash.c dag.c dmake.c\
	path.c imacs.c sysintf.c parse.c getinp.c quit.c state.c\
	basename.c dmdump.c macparse.c rulparse.c percent.c function.c

# Common Include files.
HDR = dmake.h extern.h struct.h vextern.h patchlvl.h version.h

# Define the TARGET we are making, and where the OBJECT files go.
OBJDIR := objects
TARGET  = dmake$E
CFLAGS += -I.

# Meta rule for making .o's from .c's (give our own so we can move object
# to objects directory in a portable, compiler independent way)
# Define it before the .INCLUDE so that different OS combinations can redefine
# it.
%$O : %.c
.IF $(SHELL) == mpw
	%$(CC) $(CFLAGS) -o :$(OBJDIR:s,/,:,):$@ $<
.ELSE
	%$(CC) -c $(CFLAGS) $<
.IF $(SHELL) != $(COMSPEC)
	mv $(@:f) $(OBJDIR)
.ELSE
	+copy $(@:f) $(OBJDIR)
	+del $(@:f)
.ENDIF
.ENDIF

# Pull in the proper configuration files, based on the value of OS.
.INCLUDE : $(OS)/config.mk
.INCLUDE : dbug/dbug.mk

# Set the .SOURCE targets so that we look for things in the right place.
.SOURCE.c :^ .NULL
.SOURCE.h :^ .NULL
.SOURCE$O :^ $(OBJDIR)
.PRECIOUS : $(HDR)

# Must come after the above INCLUDE so that it gets ALL objects.
OBJECTS	:= {$(ASRC:b) $(SRC:b)}$O

# The main target, make sure the objects directory exists first.
# LDARGS is defined in config.mk file of each OS/OSRELEASE combination.
all : $(TARGET) $(STARTUP);
$(TARGET)  : $(OBJDIR)
$(TARGET)  : $(OBJECTS);$(LD) $(LDARGS)
.IF $(SHELL) == mpw
$(STARTUP) : $(ENVDIR)$(STARTUP); duplicate :$(<:s,/,:,) $@
.ELSE
$(STARTUP) : $(ENVDIR)$(STARTUP); $(eq,$(SHELL),$(COMSPEC) +copy cp) $< $@
.ENDIF

# how to make public.h
public .PHONY : $(PUBLIC);
$(PUBLIC) .SHELL .NOSTATE: $(SRC)
	genpub -n DMAKE $< >$@
	rcsclean $@ > /dev/null

# Other obvious targets...
.IF $(SHELL) == mpw
$(OBJDIR):;-newfolder $@
.ELSE
$(OBJDIR):;-$(eq,$(SHELL),$(COMSPEC) +md mkdir) $@
.ENDIF

# remaining dependencies should be automatically generated
sysintf$O  : $(OS)/sysintf.h
ruletab$O  : $(OS)/startup.h  #khc 01NOV90 - dependency was missing
$(OBJECTS) : $(HDR)

clean:;+- $(RM) -rf dmake$E dbdmake$E objects* $(STARTUP) _*state*.mk

# Rules for making the manual pages.
man .SETDIR=man : dmake.nc dmake.uue ;
dmake.nc : dmake.p ; scriptfix < $< > $@
dmake.p  : dmake.tf; typeset -man -Tdumb $< > $@
dmake.uue : dmake.p
	compress -b 12 dmake.p
	mv dmake.p.Z dmake.Z
	uuencode dmake.Z dmake.Z >dmake.uue
	/bin/rm -f dmake.Z

#--------------------------------------------------------------------------
# Make the various archives for shipping the thing around.
#
archives : zoo tar shar;

zoo  .PHONY : dmake.zoo ;
shar .PHONY : dmake.shar;
tar  .PHONY : dmake.tar;

dmake.zoo  : dir-copy
[
	find dmake -type f -print | zoo aI $@
	$(RM) -rf src-list dmake
]

dmake.shar : dir-copy
[
	find dmake -type f -print >src-list
	xshar -vc -o$@ -L40 `cat src-list`
	$(RM) -rf src-list dmake
]

dmake.tar : dir-copy
[
	tar cf $@ dmake
	$(RM) -rf src-list dmake
]

dir-copy .PHONY : src-list
[
	echo 'tmp.tar .SILENT :$$(ALLSRC) ;tar -cf tmp.tar $$(ALLSRC)' >> $<
	$(MAKECMD) -f $< tmp.tar
	mkdir dmake
	cd dmake
	tar xf ../tmp.tar; chmod -R u+rw .
	cd ..
	/bin/rm -f tmp.tar
]

src-list : clean man
	echo 'ALLSRC = \' >$@
	find . -type f -print |\
	sed -e 's/RCS\///' -e 's/,v//' -e 's/$$/\\/' -e 's/^\.\// /'|\
	sort -u |\
	grep -v tst | grep -v $@ | grep -v LICENSE |\
	grep -v '\.zoo' | grep -v '\.tar'| grep -v '\.shar' >> $@
	echo ' LICENSE' >> $@

#--------------------------------------------------------------------------
# This section can be used to make the necessary script files so that dmake
# can be bootstrapped.
#
#	dmake scripts	-- makes all the script files at once.
#
SH_n = $(@:s/swp-/-/:s,-,/,:s/scripts/${SCRIPTFILE}/)
MS_n = MAKESTARTUP=$(@:s/swp-/-/:s,-,/,:s/scripts/startup.mk/)
SH = $(SH_n:s/c40d/cd/:s/c50d/cd/:s/c51d/cd/:s/c60d/cd/:s/ibmc2/ibm/)
MS = $(MS_n:s/c40d/cd/:s/c50d/cd/:s/c51d/cd/:s/c60d/cd/:s/ibmc2/ibm/)
FIX-SH = $(SH:s,fix/,,)

scripts: unix-scripts atari-tos-scripts msdos-scripts os2-scripts\
	 apple-mac-scripts

# To add a new environment for UNIX, simply create the appropriate entry
# in the style below for the macro which contains the OS, OSRELEASE and
# OSENVIRONMENT flags.  Then add the entry as a recipe line for the target
# unix-scripts.
#
unix-bsd43-scripts-flags   = OS=unix OSRELEASE=bsd43  OSENVIRONMENT=
unix-bsd43-uw-scripts-flags= OS=unix OSRELEASE=bsd43  OSENVIRONMENT=uw
unix-bsd43-vf-scripts-flags= OS=unix OSRELEASE=bsd43  OSENVIRONMENT=vf
unix-sysvr4-scripts-flags  = OS=unix OSRELEASE=sysvr4 OSENVIRONMENT=
unix-sysvr3-scripts-flags  = OS=unix OSRELEASE=sysvr3 OSENVIRONMENT=
unix-sysvr3-pwd-scripts-flags  = OS=unix OSRELEASE=sysvr3 OSENVIRONMENT=pwd
unix-xenix-scripts-flags  = OS=unix OSRELEASE=xenix OSENVIRONMENT=
unix-xenix-pwd-scripts-flags  = OS=unix OSRELEASE=xenix OSENVIRONMENT=pwd
unix-sysvr1-scripts-flags  = OS=unix OSRELEASE=sysvr1 OSENVIRONMENT=
unix-386ix-scripts-flags   = OS=unix OSRELEASE=386ix  OSENVIRONMENT=
unix-coherent-scripts-flags= OS=unix OSRELEASE=coherent OSENVIRONMENT=
unix-gno-scripts-flags     = OS=unix OSRELEASE=gno    OSENVIRONMENT=
tos--scripts-flags         = OS=tos  OSRELEASE=       OSENVIRONMENT=
mac--scripts-flags         = OS=mac  OSRELEASE=       OSENVIRONMENT=

unix-scripts .SWAP : clean
	$(MAKE) SCRIPTFILE=make.sh unix-bsd43-scripts
	$(MAKE) SCRIPTFILE=make.sh unix-bsd43-uw-scripts
	$(MAKE) SCRIPTFILE=make.sh unix-bsd43-vf-scripts
	$(MAKE) SCRIPTFILE=make.sh unix-sysvr4-scripts
	$(MAKE) SCRIPTFILE=make.sh unix-sysvr3-scripts
	$(MAKE) SCRIPTFILE=make.sh unix-sysvr3-pwd-scripts
	$(MAKE) SCRIPTFILE=make.sh unix-xenix-scripts
	$(MAKE) SCRIPTFILE=make.sh unix-xenix-pwd-scripts
	$(MAKE) SCRIPTFILE=make.sh unix-sysvr1-scripts
	$(MAKE) SCRIPTFILE=make.sh unix-386ix-scripts
	$(MAKE) SCRIPTFILE=make.sh unix-coherent-scripts
	$(MAKE) SCRIPTFILE=make.sh unix-gno-scripts

unix-%-scripts .SWAP :
	$(MAKECMD) -s $($@-flags) .KEEP_STATE:= public
	$(MAKECMD) -ns .KEEP_STATE:= $(MS) $($@-flags) >/tmp/dmscr
	dfold </tmp/dmscr >$(SH)

tos-%-scripts .SWAP :
	$(MAKECMD) -s $($@-flags) .KEEP_STATE:= public
	$(MAKECMD) -ns .KEEP_STATE:= $(MS) $($@-flags) >/tmp/dmscr
	dfold </tmp/dmscr >$(SH)

atari-tos-scripts .SWAP : clean
	$(MAKE) SCRIPTFILE=make.sh tos--scripts

apple-mac-scripts .SWAP : clean
	$(MAKE) SCRIPTFILE=make.sh mac--scripts

mac-%-scripts .SWAP :
	$(MAKECMD) -s $($@-flags) .KEEP_STATE:= public
	$(MAKECMD) -ns .KEEP_STATE:= $(MS) $($@-flags) >$(SH)
	sed 's/ mac\/\(.*\)$$/ :mac:\1/' <$(SH) | dfold >/tmp/dmscr
	/bin/mv /tmp/dmscr $(SH)

# We make the standard dos scripts here, but we have to go and fix up the
# mkXX.bat file since it contains names of temporary files for the response
# files required by the linker.  We need to also construct the response file
# contents.  These two functions are performed by the fix-msdos-%-scripts
# meta-target.
#
# To add a new DOS environment just do what is described for adding a new
# unix environment, and then make certain that the fix-msdos-%-scripts target
# performs the correct function for the new environment.
msdos-cf = OS=msdos OSENVIRONMENT=
msdos-tccdos-scripts-flags = $(msdos-cf) OSRELEASE=tccdos SWAP=n
msdos-tccdosswp-scripts-flags = $(msdos-cf) OSRELEASE=tccdos
msdos-bccdos-scripts-flags = $(msdos-cf) OSRELEASE=bccdos SWAP=n
msdos-bccdosswp-scripts-flags = $(msdos-cf) OSRELEASE=bccdos
msdos-msc40dos-scripts-flags= $(msdos-cf) OSRELEASE=mscdos SWAP=n MSC_VER=4.0
msdos-msc40dosswp-scripts-flags = $(msdos-cf) OSRELEASE=mscdos MSC_VER=4.0
msdos-msc50dos-scripts-flags= $(msdos-cf) OSRELEASE=mscdos SWAP=n MSC_VER=5.0
msdos-msc50dosswp-scripts-flags = $(msdos-cf) OSRELEASE=mscdos MSC_VER=5.0
msdos-msc51dos-scripts-flags= $(msdos-cf) OSRELEASE=mscdos SWAP=n MSC_VER=5.1
msdos-msc51dosswp-scripts-flags = $(msdos-cf) OSRELEASE=mscdos MSC_VER=5.1
msdos-msc60dos-scripts-flags= $(msdos-cf) OSRELEASE=mscdos SWAP=n MSC_VER=6.0
msdos-msc60dosswp-scripts-flags = $(msdos-cf) OSRELEASE=mscdos MSC_VER=6.0
msdos-ztcdos-scripts-flags= $(msdos-cf) OSRELEASE=ztcdos SWAP=n
msdos-ztcdosswp-scripts-flags= $(msdos-cf) OSRELEASE=ztcdos

msdos-scripts: clean\
	       msdos-tcc-scripts msdos-bcc-scripts\
	       msdos-msc-scripts msdos-ztc-scripts;

msdos-tcc-scripts .SWAP :
	$(MAKE) SCRIPTFILE=mk.bat msdos-tccdos-scripts
	$(MAKE) SCRIPTFILE=mkswp.bat msdos-tccdosswp-scripts

msdos-bcc-scripts .SWAP :
	$(MAKE) SCRIPTFILE=mk.bat msdos-bccdos-scripts
	$(MAKE) SCRIPTFILE=mkswp.bat msdos-bccdosswp-scripts

msdos-msc-scripts .SWAP :! 40 50 51 60
	$(MAKE) SCRIPTFILE=mk$?.bat msdos-msc$?dos-scripts
	$(MAKE) SCRIPTFILE=mk$?swp.bat msdos-msc$?dosswp-scripts

msdos-ztc-scripts .SWAP :
	$(MAKE) SCRIPTFILE=mk.bat msdos-ztcdos-scripts
	$(MAKE) SCRIPTFILE=mkswp.bat msdos-ztcdosswp-scripts

msdos-%-scripts .SWAP .SILENT:
	$(MAKE) -s $($@-flags) .KEEP_STATE:= public
	$(MAKE) -ns SHELL=command.com COMSPEC=command.com .KEEP_STATE:= $(MS) $($@-flags) >$(SH)
	$(MAKE) -s $(MAKEMACROS) $(MS) $($@-flags) fix-msdos-$*-scripts


# We make the standard OS/2 scripts here, but we have to go and fix up the
# mkXX.cmd file since it contains names of temporary files for the response
# files required by the linker.  We need to also construct the response file
# contents.  These two functions are performed by the fix-msdos-%-scripts
# meta-target.
#
# To add a new OS/2 environment just do what is described for adding a new
# unix environment, and then make certain that the fix-msdos-%-scripts target
# performs the correct function for the new environment.
os2-cf = OS=os2 OSENVIRONMENT=
os2-msc40dos-scripts-flags= $(os2-cf) OSRELEASE=mscdos SWAP=n MSC_VER=4.0
os2-msc50dos-scripts-flags= $(os2-cf) OSRELEASE=mscdos SWAP=n MSC_VER=5.0
os2-msc51dos-scripts-flags= $(os2-cf) OSRELEASE=mscdos SWAP=n MSC_VER=5.1
os2-msc60dos-scripts-flags= $(os2-cf) OSRELEASE=mscdos SWAP=n MSC_VER=6.0
os2-ibmc2-scripts-flags=    $(os2-cf) OSRELEASE=ibm SWAP=n MSC_VER=5.1

os2-scripts: clean os2-msc-scripts os2-ibm-scripts;

os2-msc-scripts .SWAP :! 40 50 51 60
	$(MAKE) SCRIPTFILE=mk$?.cmd os2-msc$?dos-scripts

os2-ibm-scripts .SWAP :! c2
	$(MAKE) SCRIPTFILE=mk$?.cmd os2-ibm$?-scripts

os2-%-scripts .SWAP :
	$(MAKE) -s $($@-flags) .KEEP_STATE:= public
	$(MAKE) -ns SHELL=cmd.exe COMSPEC=cmd.exe .KEEP_STATE:= $(MS) $($@-flags) >$(SH)
	$(MAKE) -s $(MAKEMACROS) $(MS) $($@-flags) fix-os2-$*-scripts

# Signify NULL targets for the various MSC compiler versions.
c2 40 50 51 60:;

# Go over the created script file and make sure all the '/' that are in
# filenames are '\', and make sure the final link command line looks
# reasonable.
MAPOBJ = obj$(SWAP:s/y/swp/:s/n//).rsp
MAPLIB = lib$(SWAP:s/y/swp/:s/n//).rsp
OBJRSP = $(SH:s,fix/,,:s,${SCRIPTFILE},${MAPOBJ},)
LIBRSP = $(SH:s,fix/,,:s,${SCRIPTFILE},${MAPLIB},)
DOSOBJ = $(CSTARTUP) $(OBJDIR)/{$(OBJECTS)}
fix-%-scripts:
	tail -r $(FIX-SH) >tmp-sh-r
	tail +3 tmp-sh-r | sed -e 's,/,\\,g' >tmp-out
	tail -r tmp-out >$(FIX-SH)
	head -2 tmp-sh-r |\
	sed -e 's,\\usr\\tmp\\mkA[a-zA-Z]*[0-9]*,$(OBJRSP),'\
	    -e 's,\\usr\\tmp\\mkB[a-zA-Z]*[0-9]*,$(LIBRSP),' |\
	sed -e 's,$(OS)/,$(OS)\\,g'\
	    -e 's,$(OS)/$(OSRELEASE)/,$(OS)\\$(OSRELEASE)\\,g'\
	    -e 's,$(OS)\\$(OSRELEASE)/,$(OS)\\$(OSRELEASE)\\,g'\
	    -e 's,$(OS)/$(OSRELEASE)\\,$(OS)\\$(OSRELEASE)\\,g' >>$(FIX-SH)
	rm -f tmp-sh-r tmp-out
	mv <+$(DOSOBJ:s,/,\\,:t"+\n")\n+> $(OBJRSP)
	mv <+$(LDLIBS:s,/,\\,:t"+\n")\n+> $(LIBRSP)
