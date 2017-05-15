#ifndef _EXEC_h_
#define _EXEC_h_

#ifndef ANSI
#if defined(__STDC__) || defined(__TURBOC__)
#define ANSI(x) x
#else
#define ANSI(x) ()
#endif
#endif

extern int  exec ANSI((int, char far *, char far *, unsigned int, char far *));

#ifndef MK_FP
#define MK_FP(seg,ofs) \
	((void far *) (((unsigned long)(seg) << 16) | (unsigned)(ofs)))
#endif

#endif
