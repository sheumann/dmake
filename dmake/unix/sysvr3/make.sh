mkdir objects
cc -c -I. -Iunix -Iunix/sysvr3 -O infer.c
mv infer.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O make.c
mv make.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O stat.c
mv stat.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O expand.c
mv expand.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O dmstring.c
mv dmstring.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O hash.c
mv hash.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O dag.c
mv dag.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O dmake.c
mv dmake.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O path.c
mv path.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O imacs.c
mv imacs.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O sysintf.c
mv sysintf.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O parse.c
mv parse.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O getinp.c
mv getinp.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O quit.c
mv quit.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O state.c
mv state.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O basename.c
mv basename.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O dmdump.c
mv dmdump.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O macparse.c
mv macparse.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O rulparse.c
mv rulparse.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O percent.c
mv percent.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O function.c
mv function.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O unix/arlib.c
mv arlib.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O unix/dirbrk.c
mv dirbrk.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O unix/rmprq.c
mv rmprq.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O unix/ruletab.c
mv ruletab.o objects
cc -c -I. -Iunix -Iunix/sysvr3 -O unix/runargv.c
mv runargv.o objects
cc  -o dmake  objects/infer.o objects/make.o objects/stat.o objects/expand.o \
objects/dmstring.o objects/hash.o objects/dag.o objects/dmake.o objects/path.o \
objects/imacs.o objects/sysintf.o objects/parse.o objects/getinp.o \
objects/quit.o objects/state.o objects/basename.o objects/dmdump.o \
objects/macparse.o objects/rulparse.o objects/percent.o objects/function.o \
objects/arlib.o objects/dirbrk.o objects/rmprq.o objects/ruletab.o objects/runargv.o 
cp unix/sysvr3/startup.mk startup.mk
