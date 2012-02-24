CC	 =	/usr/bin/gcc
RM	 =	/bin/rm -f
MAKE	 =	/usr/bin/make

CFLAGS	+=	-I. -Wall -Werror
CFLAGS  +=      -Wstrict-prototypes -Wmissing-prototypes
CFLAGS  +=      -Wmissing-declarations -Wshadow
CFLAGS  +=      -Wpointer-arith -Wcast-qual
CFLAGS  +=      -Wsign-compare

OBJS	 =	main.o
PULIB	 =	lib/libputil.a
BIN	 =	putil

.PHONY: clean

.c.o:
	$(CC) $(CFLAGS) -c $<;

all: $(BIN)

$(PULIB):
	@(cd lib/; $(MAKE));

$(BIN): $(PULIB) $(OBJS)
	$(CC) -static $(OBJS) -Llib/ -lputil -o $@;

clean:
	@(cd lib/; $(MAKE) clean);
	$(RM) $(BIN) $(OBJS) cscope.*
