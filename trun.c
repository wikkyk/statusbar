/* truncate a number of bytes by returning a negative value */

#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "statusbar.h"

static void *
init(char *arg)
{
	/* ugly but easy */
	return (void *)-strtol(arg, NULL, 0);
}

static ssize_t
tick(char *out, size_t n, void *data, msec delay)
{
	return (long)data;
}

Module trun = {
	.init = init,
	.tick = tick,
	.name = "trun",
};
