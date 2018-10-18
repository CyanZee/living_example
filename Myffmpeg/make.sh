#!/bin/bash

g++ main.cpp MyFFmpeg.cpp MP4Encoder.cpp -o main -I ./include -L ./lib/ffmpeg -L ./lib/ -lavformat -lswscale -std=c++11  -lavcodec -lavutil -lavfilter -lmp4v2  -Wl,-rpath=./:./lib/ffmpeg:./lib/
