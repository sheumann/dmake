/*
** change access and modify times of file
*/
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/file.h>

int
utime(name, timep)
char*	name;
time_t	timep[2];
{
	struct timeval tv[2], *tvp;
	struct stat buf;
	int	fil;
	char	data;

	if (timep!=0)
	{
		tvp = tv, tv[0].tv_sec = timep[0], tv[1].tv_sec = timep[1];
		if (utimes(name, tvp)==0)
			return (0);
	}

	if (stat(name, &buf) != 0)
		return (-1);
	if (buf.st_size != 0)  {
		if ((fil = open(name, O_RDWR, 0666)) < 0)
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
	} else 	if ((fil = creat(name, 0666)) < 0) {
		return (-1);
	} else {
		close(fil);
		return (0);
	}
}
