CC            = /opt/zlg/m1808-sdk-v1.6.0-ga/host/bin/aarch64-linux-gnu-gcc
CXX           = /opt/zlg/m1808-sdk-v1.6.0-ga/host/bin/aarch64-linux-gnu-g++

CFLAGS        = -pipe -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Os --sysroot=/opt/zlg/m1808-sdk-v1.6.0-ga/host/aarch64-buildroot-linux-gnu/sysroot -Wall -Wextra -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Os --sysroot=/opt/zlg/m1808-sdk-v1.6.0-ga/host/aarch64-buildroot-linux-gnu/sysroot -Wall -Wextra -D_REENTRANT -fPIC $(DEFINES)

INCLUDES       = -I/opt/zlg/m1808-sdk-v1.6.0-ga/host/aarch64-buildroot-linux-gnu/sysroot/usr/include/libdrm

LIBS    = -lpthread \
					-lavutil \
					-lavformat \
					-lavcodec \
					-lswscale \
					-lavfilter \
					-lrga \
						-ldrm \
						-lopencv_core \
						-lopencv_flann \
						-lopencv_imgproc \
						-lopencv_highgui \
						-lopencv_imgcodecs 

#-ldrm
OBJS    = ffDec.o

TARGET  = ffDec.bin

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
