/* output a file */

#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "statusbar.h"

static void *
init(char *arg)
{
	int fd;

	if((fd = open(arg, O_RDONLY)) == -1)
		return NULL;
	close(fd);

	return strdup(arg);
}

static ssize_t
tick(char *out, size_t n, void *data, msec delay)
{
	int fd;
	ssize_t r;
	char *nl;

	if((fd = open(data, O_RDONLY)) == -1)
		return 0;
	r = read(fd, out, n - 1);
	close(fd);
	if(r == -1)
		return 0;	
	out[r] = 0;
	
	/* squash newlines */
	nl = out + strlen(out) - 1;
	if(*nl == '\n')
		*nl = 0;
	nl = out;
	while((nl = strchr(nl, '\n')) != NULL)
		*nl = ' ';

	return strlen(out);
}

Module file = {
	.init = init,
	.tick = tick,
	.name = "file",
};
