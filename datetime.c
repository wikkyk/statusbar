/* simple strftime of localtime */

#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <time.h>
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
	time_t t;
	struct tm tm;

	t = time(NULL);
	localtime_r(&t, &tm);

	return strftime(out, n, data, &tm);
}

Module datetime = {
	.init = init,
	.tick = tick,
	.name = "time",
};
