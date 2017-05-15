# Default makefile for the various versions of dmake that we
# have available.  This is a bootstrap version and uses /bin/sh to
# execute a script which compiles dmake.
#
# Note the DOS commands actually invoke command.com to run the .bat file
# to make the script.

all :
	@-echo ""; clear; exit 0
	@echo "INDEX:  You must specify 'make target' where target is one of:"
	@echo "-------------"
	@echo "   make bsd43         - Generic BSD 4.3 System"
	@echo "   make bsd43uw       - Generic BSD 4.3 at U of Waterloo"
	@echo "   make bsd43vf       - Generic BSD 4.3 that needs vfprintf"
	@echo "   make sysvr4        - Generic SysV R4 UNIX System"
	@echo "   make sysvr3        - Generic SysV R3 UNIX System"
	@echo "   make sysvr3pwd     - Generic SysV R3 UNIX System, our PWD, (Any HP System)"
	@echo "   make sysvr1        - Generic SysV R1 UNIX System"
	@echo "   make dynix         - Sequent DYNIX System"
	@echo "   make ultrix        - Ultrix 3.0 System"
	@echo "   make mips          - Any MIPS System"
	@echo "   make coherent      - Any Coherent System"
	@echo "   make 386ix         - 386/ix (SysV R3) System"
	@echo "   make xenix         - 386 Xenix System"
	@echo "   make xenixpwd      - 386 Xenix System, our PWD"
	@echo "   make aix           - IBM RS6000/AIX System"
	@echo "   make os2msc40      - OS/2 using MSC 4.0 compiler"
	@echo "   make os2msc50      - OS/2 using MSC 5.0 compiler"
	@echo "   make os2msc51      - OS/2 using MSC 5.1 compiler"
	@echo "   make os2msc60      - OS/2 using MSC 6.0 compiler"
	@echo "   make os2ibm        - OS/2 using IBM C 2.0 compiler"
	@echo "   make tos           - Atari-ST TOS using GCC as compiler"
	@echo "   make tcc           - DOS with Turbo C 2.0"
	@echo "   make bcc           - DOS with Borland C++ 3.0"
	@echo "   make tccswp        - swapping DOS version with Turbo C 2.0"
	@echo "   make bccswp        - swapping DOS version with Borland C++ 3.0"
	@echo "   make msc40         - DOS with MSC 4.0"
	@echo "   make msc50         - DOS with MSC 5.0"
	@echo "   make msc51         - DOS with MSC 5.1"
	@echo "   make msc60         - DOS with MSC 6.0"
	@echo "   make msc40swp      - swapping DOS version with MSC 4.0"
	@echo "   make msc50swp      - swapping DOS version with MSC 5.0"
	@echo "   make msc51swp      - swapping DOS version with MSC 5.1"
	@echo "   make msc60swp      - swapping DOS version with MSC 6.0"
	@echo "   make ztc           - DOS with Zortech C++ 2.1"
	@echo "   make ztcswp        - swapping DOS version with Zortech C++ 2.1"
	@echo "   make mpwmac        - Macintosh under MPW"

ultrix aix: sysvr3;

sysvr1 sysvr3 sysvr4 bsd43 386ix :; /bin/sh -x < unix/$@/make.sh

bsd43uw 	     :; /bin/sh -x < unix/bsd43/uw/make.sh
bsd43vf dynix mips   :; /bin/sh -x < unix/bsd43/vf/make.sh
coherent 	     :; /bin/sh -x < unix/coherent/make.sh
xenix 		     :; /bin/sh -x < unix/xenix/make.sh
xenixpwd 	     :; /bin/sh -x < unix/xenix/pwd/make.sh
sysvr3pwd 	     :; /bin/sh -x < unix/sysvr3/pwd/make.sh
tos                  :; sh -x tos/make.sh
mpwmac               :; :mac:make.sh

# Various OS/2 targets.
OS2_VER = os2msc40 os2msc50 os2msc51 os2msc60 os2ibm
$(OS2_VER) :; make.cmd $(@:s/os2//)

# DOS with some form of make and sh
# Note if you do not have a 'make and/or sh' program under MSDOS then
# typing 'make' in the dmake distribution directory will invoke the make.bat
# batch file which will issue the appropriate instructions.
DOS_VER = tcc    msc40    msc50    msc51    msc60 \
	  tccswp msc40swp msc50swp msc51swp msc60swp \
	  bcc    bccswp   ztc      ztcswp
$(DOS_VER) :; make.bat $@
