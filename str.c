/* output a string */

#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <unistd.h>
#include "statusbar.h"

static void *
init(char *arg)
{
	return strdup(arg);
}

static ssize_t
tick(char *out, size_t n, void *data, msec delay)
{
	strncpy(out, data, n);
	return strnlen(data, n);
}

Module str = {
	.init = init,
	.tick = tick,
	.name = "str",
};
