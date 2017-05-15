#ifdef __CCFRONT__
#include <14:pragma.h>
#endif

#include <string.h>
#include <unistd.h>

char buffer[256];

int main (int argc, char **argv) {

	int i;
	size_t length;
   char *p;

	for (i = 1; i<argc; i++) {		/* loop while we have more args */
	   length = strlen(argv[i]);
		p = argv[i] + length - 2;
		if (*p == '.') {				/* check the suffix */
	      p++;
	      switch (*p) {
         case 'o':	/* FALLTHROUGH */
         case 'O':	/* FALLTHROUGH */
         case 'a':	/* FALLTHROUGH */
	      case 'A':
	         if (length < 251) {	/* longer names aren't deleted */
	            /* 251 = 255 - (length of "root") */
               strcpy (buffer,argv[i]);
               buffer[length-1] = '\0';	/* delete old suffix */
               strcat (buffer,"root");		/* add new suffix */
               unlink(buffer);
            }
            break;
         default:	   /* don't delete anything not ending in *.o or *.a */
	         break;
         }
      }
   }
   return 0;
}
