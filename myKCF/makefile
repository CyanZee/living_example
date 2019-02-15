CC	= g++
LIB_DIR = ./lib
INCLUDES= -I ./include
CFLAGS  = -Wall -ggdb -std=c++11
LIBS    = -L $(LIB_DIR) `pkg-config --cflags --libs opencv` \
		-lpthread -Wl,-rpath=./:./lib
OBJS    = src/main.o src/KCFTrackerEX.o src/kcftracker.o src/fhog.o 

TARGET  = main

all:$(TARGET)

main:./src/main.o $(OBJS)
	$(CC) -o $@ $^ $(INCLUDES) $(LIBS)

%.o:%.cpp
	$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)

%.o:%.c
	$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)

.PHONY:
clean:
	rm -f $(TARGET) src/*.o

