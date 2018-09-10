#!/bin/sh
echo "star build Debug...."

cmake -DCMAKE_BUILD_TYPE=Debug ../..
make
