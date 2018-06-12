.POSIX:

MODULES = \
	baticon \
	batperc \
	battime \
	datetime \
	file \
	str \
	trun

MODOBJ = $(MODULES:=.o)
OBJ = statusbar.o batcommon.o $(MODOBJ)
LX = -lX11 -lxcb -lXau -lXdmcp

statusbar: $(OBJ)
	$(CC) -static $(LDFLAGS) $? $(LX) -o $@

statusbar.o: modules.h config.h

modules.h: $(MODOBJ)
	rm -f $@
	for m in $(MODULES);\
		do echo extern Module $$m\; >> $@;\
	done

clean:
	rm -f statusbar $(OBJ) modules.h
