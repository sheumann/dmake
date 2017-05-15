#include "extern.h"
#include "patchlvl.h"
#include "version.h"

extern char _warn;

/*
** print error message from variable arg list
*/

static int errflg = TRUE;
static int warnflg = FALSE;

/*
** Print error message and abort
*/

void Fatal (char *fmt, ...) {
   va_list args;
   int warn;

   va_start(args, fmt);
   Continue = FALSE;

   /* start errargs code */
   warn = _warn && warnflg && !(Glob_attr & A_SILENT);

   if( errflg || warn ) {
      char *f = Filename();

      fprintf( stderr, "%s:  ", Pname );
      if( f != NIL(char) ) fprintf(stderr, "%s:  line %d:  ", f, Line_number);

      if( errflg )
         fprintf(stderr, "Error -- ");
      else if( warn )
         fprintf(stderr, "Warning -- ");

      vfprintf( stderr, fmt, args );
      putc( '\n', stderr );
      if( errflg && !Continue ) Quit();
   }
   /* end errargs code */

   va_end(args);
}

/*
** error message and exit (unless -k)
*/
void Error (char *fmt, ...) {
   va_list args;
	int warn;

   va_start(args, fmt);

   /* start errargs code */
   warn = _warn && warnflg && !(Glob_attr & A_SILENT);

   if( errflg || warn ) {
      char *f = Filename();

      fprintf( stderr, "%s:  ", Pname );
      if( f != NIL(char) ) fprintf(stderr, "%s:  line %d:  ", f, Line_number);

      if( errflg )
         fprintf(stderr, "Error -- ");
      else if( warn )
         fprintf(stderr, "Warning -- ");

      vfprintf( stderr, fmt, args );
      putc( '\n', stderr );
      if( errflg && !Continue ) Quit();
   }
   /* end errargs code */

   va_end(args);
}


/*
** non-fatal message
*/
void Warning (char *fmt, ...) {
   va_list args;
	int warn;

   va_start(args, fmt);
   warnflg = TRUE;
   errflg = FALSE;

   /* start errargs code */
   warn = _warn && warnflg && !(Glob_attr & A_SILENT);

   if( errflg || warn ) {
      char *f = Filename();

      fprintf( stderr, "%s:  ", Pname );
      if( f != NIL(char) ) fprintf(stderr, "%s:  line %d:  ", f, Line_number);

      if( errflg )
         fprintf(stderr, "Error -- ");
      else if( warn )
         fprintf(stderr, "Warning -- ");

      vfprintf( stderr, fmt, args );
      putc( '\n', stderr );
      if( errflg && !Continue ) Quit();
   }
   /* end errargs code */

   errflg = TRUE;
   warnflg = FALSE;
   va_end(args);
}
