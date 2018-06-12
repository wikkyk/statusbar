/* common battery routines */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "batcommon.h"

int
batinit(char **full, char **now, const char *path)
{
	*full = malloc(strlen(path) + sizeof("/energy_full"));
	if(*full == NULL)
		return -1;
	*now = malloc(strlen(path) + sizeof("/energy_now"));
	if(*now == NULL){
		free(*full);
		return -1;
	}
	strcpy(*full, path);
	strcat(*full, "/energy_full");
	strcpy(*now, path);
	strcat(*now, "/energy_now");

	return 0;
}

unsigned long long
readull(const char *file)
{
	int fd;
	char buf[22];
	ssize_t r;
	unsigned long long ret;

	if((fd = open(file, O_RDONLY)) == -1)
		return 0;

	r = read(fd, buf, sizeof(buf));
	close(fd);
	if(r < 1)
		return 0;
	
	errno = 0;
	ret = strtoull(buf, NULL, 10);
	if(errno)
		return 0;
	return ret;
}
