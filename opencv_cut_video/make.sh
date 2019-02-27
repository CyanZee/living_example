#!/bin/bash

	g++ main.cpp short_video.cpp -o main `pkg-config --cflags --libs opencv`
