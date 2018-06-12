/* battery icons pressent at U+E010-U+E01A in leggie */

#include <stdio.h>
#include <stdlib.h>
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

	if(n < 3)
		return 0;

	full = readull(b->fullfile);
	now = readull(b->nowfile);

	if(full == 0 || now == 0){
		out[0] = '?';
		out[1] = 0;
		return 1;
	}
	/* UTF-8 */
	return sprintf(out, "\xee\x80%c", (int)(0x90 + now * 10 / full));
}

Module baticon = {
	.init = init,
	.tick = tick,
	.name = "baticon",
};
