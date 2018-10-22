SRC_LIB_DIR= ./lib
CC     = g++

VPATH    = 
LIB_DIR  = 
INCLUDES = -I $(SRC_LIB_DIR) -I ./include  -I ./include/BasicUsageEnvironment -I ./include/groupsock -I ./include/liveMedia -I ./include/UsageEnvironment 
CFLAGS   = -Wall -ggdb -std=c++11 -I ./include  -I ./include/BasicUsageEnvironment -I ./include/groupsock -I ./include/liveMedia -I ./include/UsageEnvironment 

LIBS = -ggdb -Wall -Wl,-Map=$@.map \
		  ./lib/libliveMedia.a ./lib/libgroupsock.a ./lib/libBasicUsageEnvironment.a  ./lib/libUsageEnvironment.a \
		  -Wl,-rpath=./:./lib
		  

OBJS    = src/main.o
TARGET  = testRTSPClient

all:$(TARGET)

testRTSPClient:./src/main.o $(OBJS)
	$(CC) -o $@ $^ $(INCLUDES) $(LIBS)

%.o:%.c
	$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)

%.d:%.c
	@set -e; $(CC) -MM $< $(INCLUDES) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o:%.cpp
	$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)

%.d:%.cpp
	@set -e; $(CC) -MM $< $(INCLUDES) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
			
sinclude $(OBJS:.o=.d)

.PHONY:
clean:
	rm -f $(TARGET) src/*.o src/*.d src/*.d.* 
install:$(TARGET)
	cp $(TARGET) $(INSTALL_DIR)
