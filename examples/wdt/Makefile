EXEC    = ./bin/wdt_test
OBJS    = wdt_test.o

CROSS = arm-linux-gnueabihf-
CC      = $(CROSS)gcc
STRIP   = $(CROSS)strip
CFLAGS  = -Wall -g -O2

all: clean $(EXEC)
$(EXEC):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	$(STRIP) $@

.PHONY:clean
clean:
	rm -f $(EXEC) $(OBJS) *.~

