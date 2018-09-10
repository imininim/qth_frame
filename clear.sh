#!/bin/bash

echo "Enter build"
cd build/Debug
rm -vrf 3rd CM* cm* Cm* src test Make*

cd ../Release
rm -vrf 3rd CM* cm* Cm* src test Make*
echo "Leave build"

echo "Enter bin..."
cd ../../bin/Debug
rm -vrf ./*

cd ../Release
rm -vrf ./*
echo "Leave bin."

echo "Enter lib..."
cd ../../lib/Debug
rm -vrf ./*

cd ../Release
rm -vrf ./*
echo "Leave lib."

echo "All clear Done."
