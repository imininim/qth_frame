#!/bin/bash

rm -vrf /usr/local/lib/libqthserver.so
rm -vrf /usr/local/include/asio
rm -vrf /usr/local/include/common
rm -vrf /usr/local/include/frame
rm -vrf /usr/local/include/log4cpp
rm -vrf /usr/local/include/utility
rm -vrf /usr/local/include/xml

releaseMode="Release"
if [ -n "$1" ]
then
	if [ $1 = $releaseMode ]; then
		echo "Enter build/Release"
		cd ./build/Release
	else
		echo "Enter build/Debug"
		cd ./build/Debug
	fi

else
	echo "Enter build/Debug"
	cd ./build/Debug
fi

make install
