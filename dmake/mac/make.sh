newfolder objects
c -I. -I :mac -d _MPW -s infer -sym off -o :objects:infer.c.o infer.c
c -I. -I :mac -d _MPW -s make -sym off -o :objects:make.c.o make.c
c -I. -I :mac -d _MPW -s stat -sym off -o :objects:stat.c.o stat.c
c -I. -I :mac -d _MPW -s expand -sym off -o :objects:expand.c.o expand.c
c -I. -I :mac -d _MPW -s dmstring -sym off -o :objects:dmstring.c.o dmstring.c
c -I. -I :mac -d _MPW -s hash -sym off -o :objects:hash.c.o hash.c
c -I. -I :mac -d _MPW -s dag -sym off -o :objects:dag.c.o dag.c
c -I. -I :mac -d _MPW -s dmake -sym off -o :objects:dmake.c.o dmake.c
c -I. -I :mac -d _MPW -s path -sym off -o :objects:path.c.o path.c
c -I. -I :mac -d _MPW -s imacs -sym off -o :objects:imacs.c.o imacs.c
c -I. -I :mac -d _MPW -s sysintf -sym off -o :objects:sysintf.c.o sysintf.c
c -I. -I :mac -d _MPW -s parse -sym off -o :objects:parse.c.o parse.c
c -I. -I :mac -d _MPW -s getinp -sym off -o :objects:getinp.c.o getinp.c
c -I. -I :mac -d _MPW -s quit -sym off -o :objects:quit.c.o quit.c
c -I. -I :mac -d _MPW -s state -sym off -o :objects:state.c.o state.c
c -I. -I :mac -d _MPW -s basename -sym off -o :objects:basename.c.o basename.c
c -I. -I :mac -d _MPW -s dmdump -sym off -o :objects:dmdump.c.o dmdump.c
c -I. -I :mac -d _MPW -s macparse -sym off -o :objects:macparse.c.o macparse.c
c -I. -I :mac -d _MPW -s rulparse -sym off -o :objects:rulparse.c.o rulparse.c
c -I. -I :mac -d _MPW -s percent -sym off -o :objects:percent.c.o percent.c
c -I. -I :mac -d _MPW -s function -sym off -o :objects:function.c.o function.c
c -I. -I :mac -d _MPW -s arlib -sym off -o :objects:arlib.c.o :mac:arlib.c
c -I. -I :mac -d _MPW -s bogus -sym off -o :objects:bogus.c.o :mac:bogus.c
c -I. -I :mac -d _MPW -s dirbrk -sym off -o :objects:dirbrk.c.o :mac:dirbrk.c
c -I. -I :mac -d _MPW -s directry -sym off -o :objects:directry.c.o :mac:directry.c
c -I. -I :mac -d _MPW -s environ -sym off -o :objects:environ.c.o :mac:environ.c
c -I. -I :mac -d _MPW -s main -sym off -o :objects:main.c.o :mac:main.c
c -I. -I :mac -d _MPW -s rmprq -sym off -o :objects:rmprq.c.o :mac:rmprq.c
c -I. -I :mac -d _MPW -s ruletab -sym off -o :objects:ruletab.c.o :mac:ruletab.c
c -I. -I :mac -d _MPW -s tempnam -sym off -o :objects:tempnam.c.o :mac:tempnam.c
c -I. -I :mac -d _MPW -s tomacfil -sym off -o :objects:tomacfil.c.o :mac:tomacfil.c
link -w -c 'MPS ' -t MPST -sym off -o dmake  :objects:infer.c.o \
:objects:make.c.o :objects:stat.c.o :objects:expand.c.o :objects:dmstring.c.o \
:objects:hash.c.o :objects:dag.c.o :objects:dmake.c.o :objects:path.c.o \
:objects:imacs.c.o :objects:sysintf.c.o :objects:parse.c.o :objects:getinp.c.o \
:objects:quit.c.o :objects:state.c.o :objects:basename.c.o :objects:dmdump.c.o \
:objects:macparse.c.o :objects:rulparse.c.o :objects:percent.c.o \
:objects:function.c.o :objects:arlib.c.o :objects:bogus.c.o :objects:dirbrk.c.o \
:objects:directry.c.o :objects:environ.c.o :objects:main.c.o :objects:rmprq.c.o \
:objects:ruletab.c.o :objects:tempnam.c.o :objects:tomacfil.c.o "CSANELib.o" \
"Math.o"               "StdCLib.o" "Runtime.o"               "Interface.o" "Toollibs.o" 
duplicate :mac:startup.mk startup.mk
