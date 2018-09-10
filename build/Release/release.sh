#!/bin/bash
echo "star build Release...."

cmake -DCMAKE_BUILD_TYPE=Release ../..
make


