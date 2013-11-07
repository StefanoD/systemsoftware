#!/bin/sh

export CROSS_COMPILE=arm-linux-gnueabi-

echo "Compile Busybox"

currentpath=$(pwd)

if [ -z $1 ]; then
	echo "No Busyboxpath arg given.. trying to start make"
else

	busyboxpath=$1
	if [ -z "$kernelpath/Makefile" ]; then
		echo "ERROR: Makefile not found"
		exit 1
	fi

	cd $busyboxpath
fi

make 

binarypath=$(find busybox -mmin -1)

echo $changed
if [ -z "$binarypath" ]; then
	echo "No changes made to binary"
	exit 1
fi

cp  $binarypath $currentpath/systemarm/bin
cd $currentpath

echo "New Binary Created"
