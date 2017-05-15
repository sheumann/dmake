md objects
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs infer.c
copy infer.obj objects
del infer.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs make.c
copy make.obj objects
del make.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs stat.c
copy stat.obj objects
del stat.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs expand.c
copy expand.obj objects
del expand.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs dmstring.c
copy dmstring.obj objects
del dmstring.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs hash.c
copy hash.obj objects
del hash.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs dag.c
copy dag.obj objects
del dag.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs dmake.c
copy dmake.obj objects
del dmake.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs path.c
copy path.obj objects
del path.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs imacs.c
copy imacs.obj objects
del imacs.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs sysintf.c
copy sysintf.obj objects
del sysintf.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs parse.c
copy parse.obj objects
del parse.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs getinp.c
copy getinp.obj objects
del getinp.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs quit.c
copy quit.obj objects
del quit.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs state.c
copy state.obj objects
del state.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs basename.c
copy basename.obj objects
del basename.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs dmdump.c
copy dmdump.obj objects
del dmdump.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs macparse.c
copy macparse.obj objects
del macparse.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs rulparse.c
copy rulparse.obj objects
del rulparse.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs percent.c
copy percent.obj objects
del percent.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs function.c
copy function.obj objects
del function.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs os2\ruletab.c
copy ruletab.obj objects
del ruletab.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs os2\runargv.c
copy runargv.obj objects
del runargv.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs os2\_chdir.c
copy _chdir.obj objects
del _chdir.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs os2\switchar.c
copy switchar.obj objects
del switchar.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs msdos\dirbrk.c
copy dirbrk.obj objects
del dirbrk.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs msdos\arlib.c
copy arlib.obj objects
del arlib.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs unix\rmprq.c
copy rmprq.obj objects
del rmprq.obj
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\ibm -AL -D_MSC_VER=510 -Osl -Gs os2\ibm\tempnam.c
copy tempnam.obj objects
del tempnam.obj
copy os2\ibm\startup.mk startup.mk
link /stack:8192 /exe /packc /batch @os2\ibm\obj.rsp,dmake.exe,NUL.MAP,,os2\dmake.def;
