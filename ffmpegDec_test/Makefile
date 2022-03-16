CC            = /opt/zlg/m3568-sdk-v1.0.0-ga/host/bin/aarch64-linux-gcc
CXX           = /opt/zlg/m3568-sdk-v1.0.0-ga/host/bin/aarch64-linux-g++

CFLAGS        = -pipe -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Os --sysroot=/opt/zlg/m3568-sdk-v1.0.0-ga/host/aarch64-buildroot-linux-gnu/sysroot -Wall -Wextra -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Os --sysroot=/opt/zlg/m3568-sdk-v1.0.0-ga/host/aarch64-buildroot-linux-gnu/sysroot -Wall -Wextra -D_REENTRANT -fPIC $(DEFINES)

INCLUDES       = -I/opt/zlg/m3568-sdk-v1.0.0-ga/host/aarch64-buildroot-linux-gnu/sysroot/usr/include/libdrm

LIBS    = -lpthread \
					-lavutil \
					-lavformat \
					-lavcodec \
					-lswscale \
					-lavfilter 

#-ldrm
OBJS    = ffmpegDec.o

TARGET  = ffmpegDec.bin

all:$(TARGET)

$(TARGET):$(OBJS)  
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCLUDES) $(LIBS)

%.o:%.c
	$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)
%.o:%.cpp
	$(CXX) -c $< -o $@ $(INCLUDES) $(CXXFLAGS)

.PHONY:
clean:
	rm -f $(TARGET) *.o
