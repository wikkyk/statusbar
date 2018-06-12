/* battery percentage */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "batcommon.h"
#include "statusbar.h"

typedef struct Battery{
	char *fullfile;
	char *nowfile;
}Battery;

static void *
init(char *arg)
{
	Battery *b;

	if((b = malloc(sizeof(*b))) == NULL)
		return NULL;
	if(batinit(&b->fullfile, &b->nowfile, arg) == -1){
		free(b);
		return NULL;
	}

	return b;
}

static ssize_t
tick(char *out, size_t n, void *data, msec delay)
{
	unsigned long long full, now;
	Battery *b = data;

	if(n < 4)
		return 0;

	full = readull(b->fullfile);
	now = readull(b->nowfile);

	if(full == 0 || now == 0){
		strcpy(out, "?%");
		return 2;
	}
	return sprintf(out, "%llu%%", now * 100 / full);
}

Module batperc = {
	.init = init,
	.tick = tick,
	.name = "batperc",
};
