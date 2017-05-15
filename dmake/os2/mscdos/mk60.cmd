md objects
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\infer.obj infer.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\make.obj make.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\stat.obj stat.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\expand.obj expand.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\dmstring.obj dmstring.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\hash.obj hash.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\dag.obj dag.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\dmake.obj dmake.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\path.obj path.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\imacs.obj imacs.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\sysintf.obj sysintf.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\parse.obj parse.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\getinp.obj getinp.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\quit.obj quit.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\state.obj state.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\basename.obj basename.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\dmdump.obj dmdump.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\macparse.obj macparse.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\rulparse.obj rulparse.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\percent.obj percent.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\function.obj function.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\ruletab.obj os2\ruletab.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\runargv.obj os2\runargv.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\_chdir.obj os2\_chdir.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\switchar.obj os2\switchar.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\dirbrk.obj msdos\dirbrk.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\arlib.obj msdos\arlib.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\rmprq.obj unix\rmprq.c
cl -c -I. -Ios2 -DOS2 -G2 -Ios2\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\tempnam.obj os2\mscdos\tempnam.c
copy os2\mscdos\startup.mk startup.mk
link /stack:8192 /exe /packc /batch @os2\mscdos\obj.rsp,dmake.exe,NUL.MAP,,os2\dmake.def;
