#!/bin/sh

#cd ../syso

echo "Creating sysinfo for arm"

arm-linux-gnueabi-gcc -static sysinfo.c -o systemarm/bin/sysinfo

echo "Creating keymap for arm"

#arm-linux-gnueabi-gcc -static keymap.c -o systemarm/bin/keymap

echo "Creating initramfs_list"

echo "# directory structure

dir /proc 755 0 0
dir /sys 755 0 0
dir /dev 755 0 0
nod /dev/console 644 0 0 c 5 1
dir /sbin 755 0 0
dir /bin 755 0 0
dir /usr 755 0 0
dir /usr/bin 755 0 0
dir /etc 755 0 0
dir /var 755 0 0
dir /tmp 755 0 0 

#links
slink /bin/sh /bin/busybox 777 0 0
slink /bin/cd /bin/busybox 777 0 0
slink /bin/ln /bin/busybox 777 0 0 " > initramfs_list

#busybox
busybox=$(find . | grep busybox | tr -d .)

if [ -z "$busybox" ]; then
	echo "ERROR: busybox not found"
	exit 1
fi

echo "\n#busybox\n" >> initramfs_list
echo "file /bin/busybox ../systemsoftware/V2$busybox 755 0 0" >> initramfs_list


#sysinfo
sysinfo=$(find . | grep sysinfo$ | tr -d .)

if [ -z "$sysinfo" ]; then
	echo "ERROR: busybox not found"
	exit 1
fi

echo "\n#sysinfo\n" >> initramfs_list
echo "file /usr/bin/sysinfo ../systemsoftware/V2$sysinfo 755 0 0" >> initramfs_list


#keymap
keymap=$(find . | grep keymap$ | tr -d .)

if [ -z "$keymap" ]; then
        echo "ERROR: keymap not found"
        exit 1
fi

echo "\n#keymap\n" >> initramfs_list
echo "file /usr/bin/keymap ../systemsoftware/V2$keymap 755 0 0" >> initramfs_list

#initscript
init=$(find . | grep init$ | tr -d .)

if [ -z "$init" ]; then
	echo "ERROR: busybox not found"
	exit 1
fi

echo "\n#initscript\n" >> initramfs_list
echo "file /init ../systemsoftware/V2$init 755 0 0" >> initramfs_list

