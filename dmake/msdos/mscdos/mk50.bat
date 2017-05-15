md objects
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs infer.c
copy infer.obj objects
del infer.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs make.c
copy make.obj objects
del make.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs stat.c
copy stat.obj objects
del stat.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs expand.c
copy expand.obj objects
del expand.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs dmstring.c
copy dmstring.obj objects
del dmstring.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs hash.c
copy hash.obj objects
del hash.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs dag.c
copy dag.obj objects
del dag.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs dmake.c
copy dmake.obj objects
del dmake.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs path.c
copy path.obj objects
del path.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs imacs.c
copy imacs.obj objects
del imacs.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs sysintf.c
copy sysintf.obj objects
del sysintf.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs parse.c
copy parse.obj objects
del parse.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs getinp.c
copy getinp.obj objects
del getinp.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs quit.c
copy quit.obj objects
del quit.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs state.c
copy state.obj objects
del state.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs basename.c
copy basename.obj objects
del basename.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs dmdump.c
copy dmdump.obj objects
del dmdump.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs macparse.c
copy macparse.obj objects
del macparse.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs rulparse.c
copy rulparse.obj objects
del rulparse.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs percent.c
copy percent.obj objects
del percent.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs function.c
copy function.obj objects
del function.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs msdos\ruletab.c
copy ruletab.obj objects
del ruletab.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs msdos\dirbrk.c
copy dirbrk.obj objects
del dirbrk.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs msdos\runargv.c
copy runargv.obj objects
del runargv.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs msdos\arlib.c
copy arlib.obj objects
del arlib.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs msdos\_chdir.c
copy _chdir.obj objects
del _chdir.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs msdos\switchar.c
copy switchar.obj objects
del switchar.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs msdos\rmprq.c
copy rmprq.obj objects
del rmprq.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs msdos\tee.c
copy tee.obj objects
del tee.obj
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D_MSC_VER=500 -Oscl -Gs msdos\mscdos\tempnam.c
copy tempnam.obj objects
del tempnam.obj
copy msdos\mscdos\startup.mk startup.mk
link /stack:4096 /exe /packc /batch @msdos\mscdos\obj.rsp,dmake.exe,NUL.MAP;
