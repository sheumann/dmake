mkdir objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O infer.c
mv infer.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O make.c
mv make.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O stat.c
mv stat.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O expand.c
mv expand.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O dmstring.c
mv dmstring.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O hash.c
mv hash.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O dag.c
mv dag.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O dmake.c
mv dmake.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O path.c
mv path.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O imacs.c
mv imacs.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O sysintf.c
mv sysintf.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O parse.c
mv parse.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O getinp.c
mv getinp.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O quit.c
mv quit.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O state.c
mv state.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O basename.c
mv basename.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O dmdump.c
mv dmdump.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O macparse.c
mv macparse.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O rulparse.c
mv rulparse.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O percent.c
mv percent.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O function.c
mv function.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O unix/arlib.c
mv arlib.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O unix/dirbrk.c
mv dirbrk.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O unix/rmprq.c
mv rmprq.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O unix/ruletab.c
mv ruletab.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O unix/runargv.c
mv runargv.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O unix/coherent/vfprintf.c
mv vfprintf.o objects
cc -c -I. -Iunix -Iunix/coherent -Dvoid=int -O unix/coherent/getcwd.c
mv getcwd.o objects
cc  -o dmake  objects/infer.o objects/make.o objects/stat.o objects/expand.o \
objects/dmstring.o objects/hash.o objects/dag.o objects/dmake.o objects/path.o \
objects/imacs.o objects/sysintf.o objects/parse.o objects/getinp.o \
objects/quit.o objects/state.o objects/basename.o objects/dmdump.o \
objects/macparse.o objects/rulparse.o objects/percent.o objects/function.o \
objects/arlib.o objects/dirbrk.o objects/rmprq.o objects/ruletab.o objects/runargv.o objects/vfprintf.o objects/getcwd.o 
cp unix/coherent/startup.mk startup.mk
