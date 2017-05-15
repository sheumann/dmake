md objects
masm -t -mx -Dmlarge msdos\exec.asm;
mv exec.obj objects
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  infer.c
copy infer.obj objects
del infer.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  make.c
copy make.obj objects
del make.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  stat.c
copy stat.obj objects
del stat.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  expand.c
copy expand.obj objects
del expand.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  dmstring.c
copy dmstring.obj objects
del dmstring.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  hash.c
copy hash.obj objects
del hash.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  dag.c
copy dag.obj objects
del dag.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  dmake.c
copy dmake.obj objects
del dmake.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  path.c
copy path.obj objects
del path.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  imacs.c
copy imacs.obj objects
del imacs.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  sysintf.c
copy sysintf.obj objects
del sysintf.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  parse.c
copy parse.obj objects
del parse.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  getinp.c
copy getinp.obj objects
del getinp.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  quit.c
copy quit.obj objects
del quit.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  state.c
copy state.obj objects
del state.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  basename.c
copy basename.obj objects
del basename.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  dmdump.c
copy dmdump.obj objects
del dmdump.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  macparse.c
copy macparse.obj objects
del macparse.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  rulparse.c
copy rulparse.obj objects
del rulparse.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  percent.c
copy percent.obj objects
del percent.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  function.c
copy function.obj objects
del function.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  msdos\ruletab.c
copy ruletab.obj objects
del ruletab.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  msdos\dirbrk.c
copy dirbrk.obj objects
del dirbrk.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  msdos\runargv.c
copy runargv.obj objects
del runargv.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  msdos\arlib.c
copy arlib.obj objects
del arlib.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  msdos\_chdir.c
copy _chdir.obj objects
del _chdir.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  msdos\switchar.c
copy switchar.obj objects
del switchar.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  msdos\rmprq.c
copy rmprq.obj objects
del rmprq.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  msdos\find.c
copy find.obj objects
del find.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  msdos\spawn.c
copy spawn.obj objects
del spawn.obj
cl -c -I. -Imsdos -Imsdos\mscdos -mL -DM_I86=1  msdos\mscdos\tempnam.c
copy tempnam.obj objects
del tempnam.obj
copy msdos\mscdos\startup.mk startup.mk
link /stack:4096 @msdos\mscdos\objswp.rsp,dmake.exe,NUL.MAP;
