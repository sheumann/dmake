md objects
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\infer.obj infer.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\make.obj make.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\stat.obj stat.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\expand.obj expand.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\dmstring.obj dmstring.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\hash.obj hash.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\dag.obj dag.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\dmake.obj dmake.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\path.obj path.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\imacs.obj imacs.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\sysintf.obj sysintf.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\parse.obj parse.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\getinp.obj getinp.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\quit.obj quit.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\state.obj state.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\basename.obj basename.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\dmdump.obj dmdump.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\macparse.obj macparse.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\rulparse.obj rulparse.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\percent.obj percent.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\function.obj function.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\ruletab.obj msdos\ruletab.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\dirbrk.obj msdos\dirbrk.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\runargv.obj msdos\runargv.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\arlib.obj msdos\arlib.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\_chdir.obj msdos\_chdir.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\switchar.obj msdos\switchar.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\rmprq.obj msdos\rmprq.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\tee.obj msdos\tee.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\tempnam.obj msdos\ztcdos\tempnam.c
ztc -c -I. -Imsdos -Imsdos\ztcdos -mL -DM_I86=1 -DMSDOS -b -mi -p -o -oobjects\environ.obj msdos\ztcdos\environ.c
copy msdos\ztcdos\startup.mk startup.mk
blink  @msdos\ztcdos\obj.rsp,dmake.exe,NUL.MAP;
