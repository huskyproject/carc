CC	= gcc
INC	= -I../fidoconfig -I../smapi

#chose second line to get rid of debug infos
CFLAGS	= -Wall $(INC) -DUNIX -DDEBUGLEVEL
#CFLAGS	= -Wall $(INC) -DUNIX
OBJS	= log.o config.o main.o
RM	= rm

.c.o: $(CC) -o $*.o $<

all: carc

carc: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o carc -lfidoconfig

clean:
	$(RM) *.o

distclean: clean
	$(RM) *~ core carc

