# include Husky-Makefile-Config
include ../huskymak.cfg

ifeq ($(DEBUG), 1)
  CFLAGS	=  -I$(INCDIR)/smapi -I$(INCDIR) $(DEBCFLAGS) -DDEBUGLEVEL
  LFLAGS	= $(DEBLFLAGS)
else
  CFLAGS	=  -I$(INCDIR)/smapi -I$(INCDIR) $(OPTCFLAGS)
  LFLAGS	= $(OPTLFLAGS)
endif

CDEFS=-D$(OSTYPE) $(ADDCDEFS)

ifeq ($(SHORTNAME), 1)
  LIBS=-L$(LIBDIR) -lfidoconf -lsmapi
else
  LIBS=-L$(LIBDIR) -lfidoconfig -lsmapi
endif

OBJS	= log$(OBJ) config$(OBJ) main$(OBJ)

.c$(OBJ):
	$(CC) -c $(CFLAGS) $(CDEFS) -o $*$(OBJ) $<

all: carc$(EXE)

carc$(EXE): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o carc$(EXE) $(LIBS)

install: carc$(EXE)
	$(INSTALL) $(IBOPT) carc$(EXE) $(BINDIR)

clean:
	-$(RM) *$(OBJ)
	-$(RM) core
	-$(RM) *~

distclean: clean
	-$(RM) carc$(EXE)

