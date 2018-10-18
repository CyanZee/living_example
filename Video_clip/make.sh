#!/bin/bash

	g++ main.cpp -o main `pkg-config --cflags --libs opencv`
