# MPW Mac startup file.  Customize to suit your needs.
# Assumes MPW C.  Change as req'd.
# See the documentation for a description of internally defined macros.
#
# Disable warnings for macros redefined here that were given
# on the command line.
__.SILENT := $(.SILENT)
.SILENT   := yes

# Configuration parameters for DMAKE startup.mk file
# Set these to NON-NULL if you wish to turn the parameter on.
_HAVE_RCS	:= yes		# yes => RCS  is installed.
_HAVE_SCCS	:= 		# yes => SCCS is installed.

# Applicable suffix definitions
A := .lib	# Libraries
C := .c		# C
E := 		# Executables
F := .f		# Fortran
O := .c.o	# Object files: Assume they are created from C files
P := .p		# Pascal
S := .a		# Assembler sources
V := v		# RCS suffix

# See if these are defined
TMPDIR := $(MPW)/tmp
.IMPORT .IGNORE : TMPDIR
.IMPORT .IGNORE : CLibraries Libraries

# Set arguments for the SHELL.  Since we can't execute sub-processes,
# these variables are not important, except for some makefiles that check
# for some values to determine the platform.
SHELL := mpw
SHELLFLAGS  :=
GROUPFLAGS  :=
SHELLMETAS  :=
GROUPSUFFIX :=
DIVFILE      = $(TMPFILE)

# Standard C-language command names and flags
   CC      := c			# C-compiler and flags
   CFLAGS  +=

   AS      := asm		# Assembler and flags
   ASFLAGS +=

   LD       = link		# Loader and flags
   LDFLAGS +=
   LDLIBS   = "$(CLibraries)CSANELib.o" "$(CLibraries)Math.o" \
              "$(CLibraries)StdCLib.o" "$(Libraries)Runtime.o" \
              "$(Libraries)Interface.o" "$(Libraries)Toollibs.o"

# Definition of $(MAKE) macro for recursive makes.
   MAKE = $(MAKECMD) $(MFLAGS)

# Language and Parser generation Tools and their flags
   YACC	  := yacc		# standard yacc
   YFLAGS +=
   YTAB	  := ytab		# yacc output files name stem.

   LEX	  := lex		# standard lex
   LFLAGS +=
   LEXYY  := lex_yy		# lex output file

# Other Compilers, Tools and their flags
   PC	:= any_pc		# pascal compiler
   RC	:= anyf77		# ratfor compiler
   FC	:= anyf77		# fortran compiler

   CO	   := co		# check out for RCS
   COFLAGS += -q

   RM	   := delete	# remove a file command
   RMFLAGS +=

# Implicit generation rules for making inferences.
# We don't provide .yr or .ye rules here.  They're obsolete.
# Rules for making *$O
   %$C.o : %$C ; $(CC) $(CFLAGS) -c $<
   %$P.o : %$P ; $(PC) $(PFLAGS) -c $<
   %$S.o : %$S ; $(AS) $(ASFLAGS) $(<:s,/,\);
   %$F.o : %$F ; $(FC) $(RFLAGS) $(EFLAGS) $(FFLAGS) -c $<

# Executables rules should go here.  However, on the Mac, there is not
# executable suffix (such as ".exe") and thus we end up with rules like
# "% : %.c.o", which cause circular-dependency errors.  Thus, it's easier
# just to avoid specifiying any default rule for executables.

# lex and yacc rules
   %.c : %.y ; $(YACC)  $(YFLAGS) $<; mv $(YTAB).c $@
   %.c : %.l ; $(LEX)   $(LFLAGS) $<; mv $(LEXYY).c $@

# RCS support
.IF $(_HAVE_RCS)
   % : $$(@:d)RCS$$(DIRSEPSTR)$$(@:f)$V;- $(CO) $(COFLAGS) $@
   .NOINFER : $$(@:d)RCS$$(DIRSEPSTR)$$(@:f)$V
.END

# SCCS support
.IF $(_HAVE_SCCS)
   % : s.% ; get $@
   .NOINFER : s.%
.END

# DMAKE uses this recipe to remove intermediate targets
.REMOVE :; $(RM) $<

# AUGMAKE extensions for SYSV compatibility
@B = $(@:b)
@D = $(@:d)
@F = $(@:f)
"*B" = $(*:b)
"*D" = $(*:d)
"*F" = $(*:f)
<B = $(<:b)
<D = $(<:d)
<F = $(<:f)
?B = $(?:b)
?F = $(?:f)
?D = $(?:d)

# Turn warnings back to previous setting.
.SILENT := $(__.SILENT)

# Local init file if any, gets parsed before user makefile
.INCLUDE .IGNORE: "_startup.mk"
