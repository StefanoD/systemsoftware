#!/bin/sh
echo "Mounting tmpfs.."
mkdir -p tmpfs
mount -t tmpfs -o size=10M,mode=700 tmpfs /tmpfs
echo /sbin/mdev > /proc/sys/kernel/hotplug
mdev -s
