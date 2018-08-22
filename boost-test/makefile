CC	= g++

VPATH   = 
LIB_DIR = 
INCLUDES= 
CFLAGS  = -Wall -g -std=c++11
LIBS    = -lboost_thread -lboost_system

OBJS    = 

TARGET  = boost

all:$(TARGET)

boost:tester.o $(OBJS)
	$(CC) -o $@ $^ $(INCLUDES) $(LIBS)
%.o:%.c
	$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)
%.d:%.c
	@set -e; $(CC) -MM $< $(INCLUDES) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\l.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
%.o:%.cpp
	$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)
%.d:%.cpp
	@set -e; $(CC) -MM $< $(INCLUDES) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\l.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
sinclude $(OBJS:.o=.d)

.PHONY:
clean:
	rm -f $(TARGET) *.o *.d *.d.*
install:$(TARGET)
	cp $(TARGET) $(INSTALL_DIR)
