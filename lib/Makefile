CC	 =	/usr/bin/gcc
RM	 =	/bin/rm -f
AR	 =	/usr/bin/ar

CFLAGS	+=	-I../ -Wall -Werror
CFLAGS  +=      -Wstrict-prototypes -Wmissing-prototypes
CFLAGS  +=      -Wmissing-declarations -Wshadow
CFLAGS  +=      -Wpointer-arith -Wcast-qual
CFLAGS  +=      -Wsign-compare

OBJS	 =	putil.o pastebin.o urlcode.o
BIN	 =	libputil.a

.PHONY: clean

.c.o:
	$(CC) $(CFLAGS) -c $<;

all: $(BIN)

$(BIN): $(OBJS)
	$(AR) rcsv $(BIN) $(OBJS);

clean:
	$(RM) $(BIN) $(OBJS) cscope.*
