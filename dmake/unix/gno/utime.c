/*
** change access and modify times of file
*/
/*
 * Notes about this GS version: Only sets the modification time,
 * access time is not used. Assumes that timep[1] contains mod
 * time and timep[0] access time.
 * Does not create the file if it does not exist. Assumes that the
 * file exists, is fine and healthy etc.
 */

#include <time.h>
#include <string.h>
#if !defined(__GNUC__)
# include <gsos.h>
# include <orca.h>
#endif

int
utime(char *name, time_t timep[2])
{
   static GSString255 gst;
   static FileInfoRecGS fir = { 8, &gst };
   struct tm *the_time;
   time_t current_time;

   /* first get the File Info */
   fir.pathname->length = strlen (name);
   strcpy (fir.pathname->text, name);
   GetFileInfoGS (&fir);
   if (toolerror()) return (-1);

   /* if timep is null, set the time to the current time, otherwise */
   /*    use timep[1], which is the modification time               */
   if (timep == NULL) {
      current_time = time ((time_t) NULL);
      if (current_time == -1) return -1;         /* there is no system time */
	   the_time = localtime (&current_time);
   } else
      the_time = localtime (&timep[1]);

   /* Access time does not exist on the GS, therefore only change */
   /*     the modification time.                                  */

   fir.modDateTime.second = the_time->tm_sec;
   fir.modDateTime.minute = the_time->tm_min;
   fir.modDateTime.hour = the_time->tm_hour;
   fir.modDateTime.year = the_time->tm_year;
   fir.modDateTime.day = the_time->tm_mday - 1;
   fir.modDateTime.month = the_time->tm_mon;
   fir.modDateTime.weekDay = the_time->tm_wday;

   SetFileInfoGS (&fir); /* set again with modified time and date */
   if (toolerror()) return (-1);

   return (0);
}
