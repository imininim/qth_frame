#!/bin/bash

#step1 编译boost 
cd ./3rd
sh ./boost_build.sh

#step2 依据参数1 自动编译qthserver
cd ../build

releaseMode="Release"
if [ -n "$1" ]
then
	if [ $1 = $releaseMode ]; then
		cd ./Release
		sh ./release.sh
	else
		cd ./Debug
		sh ./debug.sh
	fi

else
	#不包含第一个参数，默认处理为编译 Debug
	echo 'Debug.......'
	cd ./Debug
	sh ./debug.sh
fi


