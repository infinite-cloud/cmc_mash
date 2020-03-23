#!/bin/bash

cd build

for scene in 1 2 3
do
    ./rt -scene ${scene} -threads 8 -out out_${scene}.bmp
    echo
done
