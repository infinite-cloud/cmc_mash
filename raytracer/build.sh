#!/bin/bash

mkdir build
cd build
cmake -DCMAKE\_BUILD\_TYPE=Release ..
make -j8
