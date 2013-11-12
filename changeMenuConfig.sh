#!/bin/sh

oldconfig=.config_old
currentconfig=.config
restore=restore
var=$1


export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabi-

if [ -z $var ] 
then
  echo "Save current Config  $oldconfig"
  cp $currentconfig $oldconfig
  make menuconfig
  exit 0
fi

if [ $var = $restore ]
then
  echo "restore $oldconfig"
  cp $oldconfig $currentconfig
  exit 0
else
  echo "unknown parameter!"
  exit 1
fi




