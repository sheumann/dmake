# Generic UNIX DMAKE startup file.  Customize to suit your needs.
# Should work for both SYSV, and BSD 4.3
# See the documentation for a description of internally defined macros.
#
# Disable warnings for macros redefined here that were given
# on the command line.
__.SILENT := $(.SILENT)
.SILENT   := yes

# Configuration parameters for DMAKE startup.mk file
# Set these to NON-NULL if you wish to turn the parameter on.
_HAVE_RCS	:= yes		# yes => RCS  is installed.
_HAVE_SCCS	:= yes		# yes => SCCS is installed.

# Applicable suffix definitions
A := .a		# Libraries
E :=		# Executables
F := .f		# Fortran
O := .o		# Objects
P := .p		# Pascal
S := .s		# Assembler sources
V := ,v		# RCS suffix

# Recipe execution configurations
SHELL		:= /bin/sh
SHELLFLAGS	:= -ce
GROUPSHELL	:= $(SHELL)
GROUPFLAGS	:= 
SHELLMETAS	:= |();&<>?*][$$:\\#`'"
GROUPSUFFIX	:=
DIVFILE		 = $(TMPFILE)

# Standard C-language command names and flags
   CPP	   := /lib/cpp		# C-preprocessor
   CC      := cc		# C-compiler and flags
   CFLAGS  +=

   AS      := as		# Assembler and flags
   ASFLAGS += 

   LD       = $(CC)		# Loader and flags
   LDFLAGS +=
   LDLIBS   =

# Definition of $(MAKE) macro for recursive makes.
   MAKE = $(MAKECMD) $(MFLAGS)

# Definition of Print command for this system.
   PRINT = lpr

# Language and Parser generation Tools and their flags
   YACC	  := yacc		# standard yacc
   YFLAGS +=
   YTAB	  := y.tab		# yacc output files name stem.

   LEX	  := lex		# standard lex
   LFLAGS +=
   LEXYY  := lex.yy		# lex output file

# Other Compilers, Tools and their flags
   PC	:= pc			# pascal compiler
   RC	:= f77			# ratfor compiler
   FC	:= f77			# fortran compiler

   CO	   := co		# check out for RCS
   COFLAGS += -q

   AR     := ar			# archiver
   ARFLAGS+= ruv

   RM	   := /bin/rm		# remove a file command
   RMFLAGS +=

# Implicit generation rules for making inferences.
# We don't provide .yr or .ye rules here.  They're obsolete.
# Rules for making *$O
   %$O : %.c ; $(CC) -o $@ $(CFLAGS) -c $<
   %$O : %$P ; $(PC) -o $@ $(PFLAGS) -c $<
   %$O : %$S ; $(AS) -o $@ $(ASFLAGS) $<
   %$O : %.cl ; class -c $<
   %$O : %.e %.r %.F %$F
	$(FC) $(RFLAGS) $(EFLAGS) $(FFLAGS) -c $<

# Executables
   %$E : %$O ; $(LD) $(LDFLAGS) -o $@ $< $(LDLIBS)

# lex and yacc rules
   %.c : %.y ; $(YACC)  $(YFLAGS) $<; mv $(YTAB).c $@
   %.c : %.l ; $(LEX)   $(LFLAGS) $<; mv $(LEXYY).c $@

# This rule tells how to make *.out from it's immediate list of prerequisites
# UNIX only.
   %.out :; $(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# RCS support
.IF $(_HAVE_RCS)
   % : %$V $$(@:d)RCS/$$(@:f)$V;- $(CO) $(COFLAGS) $@
   .NOINFER : %$V $$(@:d)RCS/$$(@:f)$V
.END

# SCCS support
.IF $(_HAVE_SCCS)
   % : s.% ; get $<
   .NOINFER : s.%
.END

# Recipe to make archive files.
%$A :
[
   $(AR) $(ARFLAGS) $@ $?
   $(RM) $(RMFLAGS) $?
   ranlib $@
]

# DMAKE uses this recipe to remove intermediate targets
.REMOVE :; $(RM) -f $<

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

# Local startup file if any
.INCLUDE .IGNORE: "_startup.mk"
