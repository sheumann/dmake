md objects
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  infer.c
copy infer.obj objects
del infer.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  make.c
copy make.obj objects
del make.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  stat.c
copy stat.obj objects
del stat.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  expand.c
copy expand.obj objects
del expand.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  dmstring.c
copy dmstring.obj objects
del dmstring.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  hash.c
copy hash.obj objects
del hash.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  dag.c
copy dag.obj objects
del dag.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  dmake.c
copy dmake.obj objects
del dmake.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  path.c
copy path.obj objects
del path.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  imacs.c
copy imacs.obj objects
del imacs.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  sysintf.c
copy sysintf.obj objects
del sysintf.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  parse.c
copy parse.obj objects
del parse.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  getinp.c
copy getinp.obj objects
del getinp.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  quit.c
copy quit.obj objects
del quit.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  state.c
copy state.obj objects
del state.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  basename.c
copy basename.obj objects
del basename.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  dmdump.c
copy dmdump.obj objects
del dmdump.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  macparse.c
copy macparse.obj objects
del macparse.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  rulparse.c
copy rulparse.obj objects
del rulparse.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  percent.c
copy percent.obj objects
del percent.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  function.c
copy function.obj objects
del function.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  msdos\ruletab.c
copy ruletab.obj objects
del ruletab.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  msdos\dirbrk.c
copy dirbrk.obj objects
del dirbrk.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  msdos\runargv.c
copy runargv.obj objects
del runargv.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  msdos\arlib.c
copy arlib.obj objects
del arlib.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  msdos\_chdir.c
copy _chdir.obj objects
del _chdir.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  msdos\switchar.c
copy switchar.obj objects
del switchar.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  msdos\rmprq.c
copy rmprq.obj objects
del rmprq.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  msdos\tee.c
copy tee.obj objects
del tee.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  msdos\bccdos\tempnam.c
copy tempnam.obj objects
del tempnam.obj
bcc -c -I. -Imsdos -Imsdos\bccdos -d -O -N- -w-nod -ml  msdos\bccdos\utime.c
copy utime.obj objects
del utime.obj
copy msdos\bccdos\startup.mk startup.mk
tlink  @msdos\bccdos\obj.rsp,dmake.exe,NUL.MAP,@msdos\bccdos\lib.rsp
