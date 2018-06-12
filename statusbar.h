typedef unsigned long long msec;

typedef struct Module{
	void *(*init)(char *);
	ssize_t (*tick)(char *, size_t, void *, msec);
	char *name;
}Module;
