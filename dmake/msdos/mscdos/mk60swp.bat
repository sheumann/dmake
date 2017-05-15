md objects
masm -t -mx -Dmlarge msdos\exec.asm;
mv exec.obj objects
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\infer.obj infer.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\make.obj make.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\stat.obj stat.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\expand.obj expand.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\dmstring.obj dmstring.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\hash.obj hash.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\dag.obj dag.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\dmake.obj dmake.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\path.obj path.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\imacs.obj imacs.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\sysintf.obj sysintf.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\parse.obj parse.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\getinp.obj getinp.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\quit.obj quit.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\state.obj state.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\basename.obj basename.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\dmdump.obj dmdump.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\macparse.obj macparse.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\rulparse.obj rulparse.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\percent.obj percent.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\function.obj function.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\ruletab.obj msdos\ruletab.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\dirbrk.obj msdos\dirbrk.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\runargv.obj msdos\runargv.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\arlib.obj msdos\arlib.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\_chdir.obj msdos\_chdir.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\switchar.obj msdos\switchar.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\rmprq.obj msdos\rmprq.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\find.obj msdos\find.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\spawn.obj msdos\spawn.c
cl -c -I. -Imsdos -Imsdos\mscdos -AL -D__STDC__=1 -Osecgl -Gs -Foobjects\tempnam.obj msdos\mscdos\tempnam.c
copy msdos\mscdos\startup.mk startup.mk
link /stack:4096 /exe /packc /batch @msdos\mscdos\objswp.rsp,dmake.exe,NUL.MAP;
