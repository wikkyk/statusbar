#define _POSIX_C_SOURCE 200809L
#include <X11/Xlib.h>
#include <stdio.h>
#include <time.h>
#include "statusbar.h"
#include "modules.h"

#define count(x) (sizeof(x) / sizeof(*x))
#define min(a, b) ((a) < (b) ? (a) : (b))

typedef struct Modules{
	Module *module;
	char *arg;
	void *data;
}Modules;
#include "config.h"

static unsigned long long
tstomsec(struct timespec *ts)
{
	unsigned long long ms;

	ms = ts->tv_nsec / 1000000;
	ms += ts->tv_sec * 1000;

	return ms;
}

static void
msectots(struct timespec *ts, unsigned long long ms)
{
	ts->tv_sec = ms / 1000;
	ms -= ts->tv_sec * 1000;
	ts->tv_nsec = ms * 1000000;
}

static void
setstatus(Display *display, char *status)
{
	XStoreName(display, DefaultRootWindow(display), status);
	XSync(display, False);
}

int
main(int argc, char **argv)
{
	Display *display;
	char buf[maxlen + 1];
	size_t buflen;
	struct timespec ts, rem;

	if((display = XOpenDisplay(NULL)) == NULL){
		fprintf(stderr, "%s: cannot open display\n", argv[0]);
		return 1;
	}

	for(size_t i = 0; i < count(modules); ++i){
		modules[i].data = modules[i].module->init(modules[i].arg);
		if(modules[i].data == NULL){
			snprintf(buf, maxlen + 1, "%s init failed",
			 modules[i].module->name);
			setstatus(display, buf);
			return i + 1;
		}
	}
	msectots(&ts, refresh);
	msectots(&rem, refresh);

	while(1){
		buflen = 0;
		for(size_t i = 0; i < count(modules); ++i){
			if(buflen >= maxlen)
				break;
			buflen += modules[i].module->tick(buf+buflen, maxlen-buflen-1,
			 modules[i].data, tstomsec(&ts) - tstomsec(&rem));
		}
		buf[maxlen] = 0;
		setstatus(display, buf);

		if(nanosleep(&ts, &rem) == 0)
			msectots(&rem, 0);
	}

	XCloseDisplay(display);
}
