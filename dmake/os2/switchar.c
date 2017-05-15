/*
** return switch char
*/
#if defined(OS2) || defined(_MSC_VER)
#include <stdlib.h>
#endif
#if !defined(OS2)
#include <dos.h>
#endif /* !OS2 */
#include <stdio.h>
#include "stdmacs.h"

getswitchar()/*
===============
   Try the environment first.  If you don't find SWITCHAR there, then use
   the DOS call.  The call is undocumented, and doesn't work for DOS versions
   4.0 and up, so the check of the environment will fix that. */
{
#if defined(M_I86)
#if !defined(OS2)
   union REGS rg;
#endif /* ! OS2 */
   static char *_env_switchar = NIL(char);

   if( _env_switchar != NIL(char) ||
       (_env_switchar = (char *)getenv("SWITCHAR")) != NIL(char) )
      return(*_env_switchar);

#if !defined(OS2)
   rg.h.ah = 0x37;      /* switch char request */
   rg.h.al = 0;         /* get (not set) */

   intdos(&rg, &rg);
   return (rg.h.dl);
#endif /* ! OS2 */
#endif /* M_I86 */

   return ('/');
}
