mkdir objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O infer.c
mv infer.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O make.c
mv make.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O stat.c
mv stat.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O expand.c
mv expand.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O dmstring.c
mv dmstring.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O hash.c
mv hash.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O dag.c
mv dag.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O dmake.c
mv dmake.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O path.c
mv path.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O imacs.c
mv imacs.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O sysintf.c
mv sysintf.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O parse.c
mv parse.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O getinp.c
mv getinp.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O quit.c
mv quit.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O state.c
mv state.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O basename.c
mv basename.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O dmdump.c
mv dmdump.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O macparse.c
mv macparse.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O rulparse.c
mv rulparse.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O percent.c
mv percent.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O function.c
mv function.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O unix/arlib.c
mv arlib.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O unix/dirbrk.c
mv dirbrk.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O unix/rmprq.c
mv rmprq.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O unix/ruletab.c
mv ruletab.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O unix/runargv.c
mv runargv.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O unix/bsd43/putenv.c
mv putenv.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O unix/bsd43/tempnam.c
mv tempnam.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O unix/bsd43/utime.c
mv utime.o objects
cc -c -I. -Iunix -Iunix/bsd43 -Iunix/bsd43/uw -O unix/bsd43/setvbuf.c
mv setvbuf.o objects
cc  -o dmake  objects/infer.o objects/make.o objects/stat.o objects/expand.o \
objects/dmstring.o objects/hash.o objects/dag.o objects/dmake.o objects/path.o \
objects/imacs.o objects/sysintf.o objects/parse.o objects/getinp.o \
objects/quit.o objects/state.o objects/basename.o objects/dmdump.o \
objects/macparse.o objects/rulparse.o objects/percent.o objects/function.o \
objects/arlib.o objects/dirbrk.o objects/rmprq.o objects/ruletab.o \
objects/runargv.o objects/putenv.o objects/tempnam.o objects/utime.o objects/setvbuf.o -luw 
cp unix/bsd43/uw/startup.mk startup.mk
