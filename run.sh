#!/bin/bash

rm -fr ./build
mkdir build
cd build
cmake ..
make
LC_ALL=en_US.UTF-8 ./opengl
