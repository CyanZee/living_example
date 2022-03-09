CC            = /opt/zlg/m1126-sdk-v1.8.0-ga/host/bin/arm-linux-gnueabihf-gcc
CXX           = /opt/zlg/m1126-sdk-v1.8.0-ga/host/bin/arm-linux-gnueabihf-g++

DEFINES       = 
CFLAGS        = -pipe -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Os --sysroot=/opt/zlg/m1126-sdk-v1.8.0-ga/host/bin/../arm-buildroot-linux-gnueabihf/sysroot -O2 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Os --sysroot=/opt/zlg/m1126-sdk-v1.8.0-ga/host/bin/../arm-buildroot-linux-gnueabihf/sysroot -std=c++11 -g -DBOOST_LOG_DYN_LINK -O2 -O3 -Wall -W -D_REENTRANT -fPIC $(DEFINES)

INCLUDES       = -I/opt/zlg/m1126-sdk-v1.8.0-ga/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/libdrm

#LIBS		   = -wl,-rpath=./:./lib
LIBS		+= -L/opt/zlg/m1126-sdk-v1.8.0-ga/host/arm-buildroot-linux-gnueabihf/sysroot -ldrm -lrga
#LIBS		+= `pkg-config opencv --libs`

#CFLAGS = -Wall -ggdb -std=c++11
#LIBS = $(LIB_DIR) -Wl,-rpath=./:./lib

OBJS = main.o rkdrm_display.o

TARGET = main

all:$(TARGET)

$(TARGET):$(OBJS)  
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DEFINES) $(INCLUDES) $(LIBS)

%.o:%.c 
	$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)
%.o:%.cpp
	$(CXX) -c $< -o $@ $(INCLUDES) $(CXXFLAGS)

.PHONY:
clean:
	rm -f $(TARGET) *.o
