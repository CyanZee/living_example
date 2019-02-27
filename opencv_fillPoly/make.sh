#!/bin/bash

	g++ fillPoly.cpp -o main `pkg-config --cflags --libs opencv` -std=c++11
