#!/bin/sh
#
# This is a shell script for making a new build of dmake
#
occ -c -a0 -w -i    -O   -DGNO -s4096 -SMain______ dmake.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SLibrary___ arlib.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SString____ basename.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SDAG_______ dag.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SGSSpecific dirbrk.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SIO________ dmdump.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SString____ dmstring.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SMacro_____ expand.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SMacro_____ function.c
occ -c -a0 -w -i -r -O31 -DGNO -s4096 -SIO________ getinp.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SMake______ hash.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SMacro_____ imacs.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SMake______ infer.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SMacro_____ macparse.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SMake______ make.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SIO________ parse.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SString____ path.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SMake______ percent.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SMain______ quit.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SMain______ rmprq.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -DNO_CASE -SMain______ ruletab.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SDAG_______ rulparse.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SGSSpecific runargv.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SIO________ stat.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SIO________ state.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SGSSpecific sysintf.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SGSSpecific tempnam.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SGSSpecific utime.c
occ -c -a0 -w -i -r -O   -DGNO -s4096 -SMain______ va.c
occ -o dmake dmake.o arlib.o basename.o dag.o dirbrk.o dmdump.o dmstring.o	expand.o function.o getinp.o hash.o imacs.o infer.o macparse.o make.o	parse.o path.o percent.o quit.o rmprq.o ruletab.o rulparse.o	runargv.o stat.o state.o sysintf.o tempnam.o utime.o va.o -l/usr/lib/stack -l/usr/lib/lenviron
