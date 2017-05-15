md objects
tasm -t -mx -dmlarge msdos\exec.asm;
mv exec.obj objects
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  infer.c
copy infer.obj objects
del infer.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  make.c
copy make.obj objects
del make.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  stat.c
copy stat.obj objects
del stat.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  expand.c
copy expand.obj objects
del expand.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  dmstring.c
copy dmstring.obj objects
del dmstring.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  hash.c
copy hash.obj objects
del hash.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  dag.c
copy dag.obj objects
del dag.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  dmake.c
copy dmake.obj objects
del dmake.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  path.c
copy path.obj objects
del path.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  imacs.c
copy imacs.obj objects
del imacs.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  sysintf.c
copy sysintf.obj objects
del sysintf.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  parse.c
copy parse.obj objects
del parse.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  getinp.c
copy getinp.obj objects
del getinp.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  quit.c
copy quit.obj objects
del quit.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  state.c
copy state.obj objects
del state.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  basename.c
copy basename.obj objects
del basename.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  dmdump.c
copy dmdump.obj objects
del dmdump.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  macparse.c
copy macparse.obj objects
del macparse.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  rulparse.c
copy rulparse.obj objects
del rulparse.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  percent.c
copy percent.obj objects
del percent.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  function.c
copy function.obj objects
del function.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  msdos\ruletab.c
copy ruletab.obj objects
del ruletab.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  msdos\dirbrk.c
copy dirbrk.obj objects
del dirbrk.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  msdos\runargv.c
copy runargv.obj objects
del runargv.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  msdos\arlib.c
copy arlib.obj objects
del arlib.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  msdos\_chdir.c
copy _chdir.obj objects
del _chdir.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  msdos\switchar.c
copy switchar.obj objects
del switchar.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  msdos\rmprq.c
copy rmprq.obj objects
del rmprq.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  msdos\find.c
copy find.obj objects
del find.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  msdos\spawn.c
copy spawn.obj objects
del spawn.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  msdos\tccdos\tempnam.c
copy tempnam.obj objects
del tempnam.obj
tcc -c -I. -Imsdos -Imsdos\tccdos -f- -d -O -N- -w-nod -ml  msdos\tccdos\utime.c
copy utime.obj objects
del utime.obj
copy msdos\tccdos\startup.mk startup.mk
tlink  @msdos\tccdos\objswp.rsp,dmake.exe,NUL.MAP,@msdos\tccdos\libswp.rsp
