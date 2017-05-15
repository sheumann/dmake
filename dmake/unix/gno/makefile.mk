# Makefile.mk generated by makedmake (Gno) v1.1.1 for Dennis Vadura's dmake

#
# Configuration Information.  Use '/', not ':'.
#

BINDIR = /usr/local/bin
LIBDIR = /usr/local/lib
MANDIR = /usr/man

#
# YOU SHOULD NOT HAVE TO CHANGE ANYTHING BELOW THIS LINE
#

#
# BUILD should either be "distribution", "development", or "debug"
#

BUILD = distribution

#
# Define all the flags, definitions, and libraries
#

STACK = -s4096

.IF $(BUILD) == distribution

DEFINES = -DGNO
CFLAGS    = $(__OFLAG) -w -i -r -O   $(DEFINES) $(STACK)
# Orca/C has a problem with loop-invariant optimization on getinp.c
GETINPFLG = $(__OFLAG) -w -i -r -O31 $(DEFINES) $(STACK)
FORKFLAGS = $(__OFLAG) -w -i -r -O   $(DEFINES) $(STACK)
VAFLAGS   = $(__OFLAG) -w -i -r -O   $(DEFINES) $(STACK)
MAINFLAGS = $(__OFLAG) -w -i    -O   $(DEFINES) $(STACK)
LDFLAGS   = 
LDLIBS    = 

.ELSE

DEFINES   = -DGNO -DDEBUG
CFLAGS    = $(__OFLAG) -w -i -r -G25 $(DEFINES) $(STACK)
GETINPFLG = $(__OFLAG) -w -i -r -G25 $(DEFINES) $(STACK)
FORKFLAGS = $(__OFLAG) -w -i -r -O25 $(DEFINES) $(STACK)
VAFLAGS   = $(__OFLAG) -w -i -r -O8  $(DEFINES) $(STACK)
MAINFLAGS = $(__OFLAG) -w -i    -G25 $(DEFINES) $(STACK)
LDFLAGS   = -v
LDLIBS    = 

.END

#
# These are the segment names required for Orca/C's segment directive.
# We use the small memory model with segmentation.  These have been assigned
# at random and should be reassigned, and with more informative names.
#

SEGMENT1 = -SMain______
SEGMENT2 = -SIO________
SEGMENT3 = -SString____
SEGMENT4 = -SMake______
SEGMENT5 = -SGSSpecific
SEGMENT6 = -SMacro_____
SEGMENT7 = -SLibrary___
SEGMENT8 = -SDAG_______

.IF $(BUILD) != debug
SPARESEG1 = $(SEGMENT4)
SPARESEG2 = $(SEGMENT4)
SPARESEG3 = $(SEGMENT4)
SPARESEG4 = $(SEGMENT4)
.ELSE
SPARESEG1 = -SSpare1____
SPARESEG2 = -SSpare2____
SPARESEG3 = -SSpare3____
SPARESEG4 = -SSpare4____
.END     

OBJS  = dmake.o arlib.o basename.o dag.o dirbrk.o dmdump.o dmstring.o\
	expand.o function.o getinp.o hash.o imacs.o infer.o macparse.o make.o\
	parse.o path.o percent.o quit.o rmprq.o ruletab.o rulparse.o\
	runargv.o stat.o state.o sysintf.o tempnam.o utime.o va.o

#
# Targets
#

all: dmake ../../../rmroot/rmroot
                                                
dmake: $(OBJS)
	$(CC) $(LDFLAGS) -o dmake $(OBJS) $(LDLIBS)

../../../rmroot/rmroot: ../../../rmroot/rmroot.c
	$(CC) -O -o $@ $<

arlib.o: arlib.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h sysintf.h
	$(CC) -c $(CFLAGS) $(SEGMENT7) arlib.c

basename.o: basename.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h\
	struct.h vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT3) basename.c

dag.o: dag.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT8) dag.c

dirbrk.o: dirbrk.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT5) dirbrk.c

dmake.o: dmake.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h patchlvl.h version.h
	$(CC) -c $(MAINFLAGS) $(SEGMENT1) dmake.c

dmdump.o: dmdump.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT2) dmdump.c

dmstring.o: dmstring.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h\
	struct.h vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT3) dmstring.c

expand.o: expand.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT6) expand.c

function.o: function.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h\
	struct.h vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT6) function.c

getinp.o: getinp.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(GETINPFLG) $(SEGMENT2) getinp.c

hash.o: hash.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SPARESEG1) hash.c

imacs.o: imacs.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT6) imacs.c

infer.o: infer.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SPARESEG2) infer.c

macparse.o: macparse.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h\
	struct.h vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT6) macparse.c

make.o: make.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SPARESEG3) make.c

parse.o: parse.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT2) parse.c

path.o: path.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT3) path.c

percent.o: percent.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SPARESEG4) percent.c

quit.o: quit.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT1) quit.c

rmprq.o: rmprq.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT1) rmprq.c

ruletab.o: ruletab.c startup.h
	$(CC) -c $(CFLAGS) -DNO_CASE $(SEGMENT1) ruletab.c

rulparse.o: rulparse.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h\
	struct.h vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT8) rulparse.c

runargv.o: runargv.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h sysintf.h
	$(CC) -c $(FORKFLAGS) $(SEGMENT5) runargv.c

stat.o: stat.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT2) stat.c

state.o: state.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h
	$(CC) -c $(CFLAGS) $(SEGMENT2) state.c

sysintf.o: sysintf.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h\
	vextern.h public.h config.h posix.h sysintf.h
	$(CC) -c $(CFLAGS) $(SEGMENT5) sysintf.c

tempnam.o: tempnam.c
	$(CC) -c $(CFLAGS) $(SEGMENT5) tempnam.c

utime.o: utime.c
	$(CC) -c $(CFLAGS) $(SEGMENT5) utime.c

va.o: va.c extern.h itypes.h stdmacs.h alloc.h db.h dmake.h struct.h vextern.h\
	public.h config.h posix.h patchlvl.h version.h
	$(CC) -c $(VAFLAGS) $(SEGMENT1) va.c

#
# Housekeeping
#

clean:
	$(RM) $(OBJS) dmake.root

install:
#	dump old files
#	install the new ones
	$(CP) dmake $(BINDIR)
	$(CP) GS.startup.mk $(LIBDIR)/startup.mk
	$(CP) ../../man/dmake.nc $(MANDIR)/cat1/dmake.1
	$(CP) ../../../rmroot/rmroot.1 $(MANDIR)/man1/rmroot.1
	$(CP) ../../../rmroot/rmroot $(BINDIR)
	@echo Ensure you delete any dmake and startup.mk file you may have in
	@echo /usr/bin from a previous install.
