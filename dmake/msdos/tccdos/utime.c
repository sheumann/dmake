/*
** change access and modify times of file
*/
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int
utime(name, timep)/*
====================
    Broken for turbo C it only sets the file time to the current time by
    touching a character in the file */
char*	name;
time_t	timep[2];
{
	struct  stat buf;
	int	fil;
	char	data;

	if (stat(name, &buf) != 0)
		return (-1);
	if (buf.st_size != 0)  {
		if ((fil = open(name, O_RDWR, S_IWRITE)) < 0)
			return (-1);
		if (read(fil, &data, 1) < 1) {
			close(fil);
			return (-1);
		}
		lseek(fil, 0L, 0);
		if (write(fil, &data, 1) < 1) {
			close(fil);
			return (-1);
		}
		close(fil);
		return (0);
	} else 	if ((fil = creat(name, S_IWRITE)) < 0) {
		return (-1);
	} else {
		close(fil);
		return (0);
	}
}
