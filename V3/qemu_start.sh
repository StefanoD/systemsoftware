#!/bin/sh

##### VARS ##############
TARGET=versatilepb
CONSOLE=ttyAMA0
#Buildroot Version
BUILDROOT=buildroot-2013.08.1
PATH=$PATH:/systemarm/$BUILDROOT
SCRIPT=qemu-ifup.sh
RFS=~/arm-vm/rfs
ROOTFS=output/images/rootfs.cpio
KERNEL=output/images/zImage
QEMU=qemu-system-arm
QEMU_RAM=128
#########################

$QEMU -m $QEMU_RAM -M $TARGET -nographic \
 -kernel $KERNEL \
 -initrd $ROOTFS \
 -append "console=$CONSOLE" \
 -net nic,model=smc91c111,macaddr=00:00:00:00:00:16 -net vde,sock=/tmp/vde2-tap0.ctl

#-net nic,vlan=0 -net tap,ifname=tap0,script=$SCRIPT
