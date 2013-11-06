#!/bin/sh

export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabi-

echo "Compile Kernel Image"
currentpath=$(pwd)

if [ -z $1 ]; then
	echo "No Kernelpath arg given.. trying to start make"
else

	kernelpath=$1
	if [ -z "$kernelpath/Makefile" ]; then
		echo "ERROR: Makefile not found"
		exit 1
	fi

	cd $kernelpath
fi

make 

imagepath=$(find arch/$ARCH/boot/zImage -mmin -1)

if [ -z "$imagepath" ]; then
	echo "No changes made to image"
	exit 1
fi

cp $imagepath $currentpath
cd $currentpath

echo "New Image Created"

