/* estimate time until battery is (dis)charged */

/* The estimate is calculated by extrapolating battery level change reports
either towards max or 0. The two most recent changes are averaged for smoother
estimates as energy consumption (reports) can be quite bumpy. */

/* Inaccurate in the first few cycles after (un)plugging your cable. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "batcommon.h"
#include "statusbar.h"

typedef struct Battery{
	char *fullfile;
	char *nowfile;
	unsigned long long last;
	msec since;
	msec lasttime;
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
	b->last = 0;
	b->since = 0;

	return b;
}

static ssize_t
tick(char *out, size_t n, void *data, msec delay)
{
	unsigned long long full, now, diff, remain;
	msec left, tmp;
	size_t ret;
	Battery *b = data;

	if(n < 4)
		return 0;

	full = readull(b->fullfile);
	now = readull(b->nowfile);

	if(delay == 0){
		b->last = now;
		b->lasttime = 0;
		strcpy(out, "-:--");
		return 4;
	}
	if(full == 0 || now == 0){
		strcpy(out, "?:??");
		return 4;
	}
	if(now > full * 0.95){
		strcpy(out, "full");
		return 4;
	}

	b->since += delay;
	if(now < b->last){ /* discharging */
		diff = b->last - now;
		remain = now;
	}else if(now > b->last){ /* charging */
		diff = now - b->last;
		remain = full - now;
	}else{ /* no change this tick */
		left = b->lasttime;
		goto Print;
	}

	left = b->since * remain / diff;
	b->since = 0;

	/* smooth out bumpy readings */
	b->last = now;
	tmp = left;
	left = (left * 2 + b->lasttime) / 3;
	b->lasttime = tmp;

Print:
	ret = snprintf(out, n, "%llu:%02llu", left / 3600000, left / 60000 % 60);
	if(ret > n)
		return n;
	return ret;
}

Module battime = {
	.init = init,
	.tick = tick,
	.name = "battime",
};
