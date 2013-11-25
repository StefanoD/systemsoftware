#!/bin/sh

#------------Variables------------------
gitpath=./systemsoftware/V2
systempath=systemarm
sourcepath=.$gitpath/$systempath

compiler="arm-linux-gnueabi-gcc -static"
system=arm
target=initramfs_list
#---------------------------------------

Exists () {
if [ ! -e "$1" ] 
then
	echo "ERROR: $1 does not exist"
	exit 1
fi
}

Create () {
if [ -z $1 ] || [ -z $2 ];
then
 echo "Create needs 2 parameter!"
 exit 1
fi
echo "\tCreating $2 from $1 for $system"
$compiler $1 -o $systempath/$2
Exists $systempath/$2
}

CreateDirectories() {
echo "\n  Create Directories .."

echo "
dir /proc 755 0 0
dir /sys 755 0 0
dir /dev 755 0 0
nod /dev/console 644 0 0 c 5 1
dir /sbin 755 0 0
dir /bin 755 0 0
dir /usr 755 0 0
dir /usr/bin 755 0 0
dir /etc 755 0 0
dir /etc/udhcpc 755 0 0
dir /var 755 0 0
dir /tmp 755 0 0
dir /lib 755 0 0
dir /www 755 0 0
dir /www/cgi-bin 755 0 0

#links
slink /bin/sh /bin/busybox 777 0 0
slink /bin/cd /bin/busybox 777 0 0
slink /bin/ln /bin/busybox 777 0 0 " > $target
}

### $1 = sourcefile  $2 = target
Copy() {

if [ -z $1 ] || [ -z $2 ];
then
 echo "Copy needs 2 parameter!"
 exit 1
fi
echo "\tCopy $1 to $2 .."
prog=$(find . | grep $1$ | tr -d .)

if [ -z "$prog" ]; then
	echo "ERROR: $1 not found"
	exit 1
fi

echo "\n#$1" >> initramfs_list
echo "file /$2 $gitpath$prog 755 0 0" >> initramfs_list
}

CopyDirectory() {

if [ -z $1 ];
then
 echo "Copy needs a parameter!"
 exit 1
fi
echo "\tCopy directory $1 from $sourcepath/$1.."


echo "\n#$1" >> initramfs_list
ls -p $systempath/$1/ | grep -v '/$' > .tmp
while read line
do
  
    echo "file /$1/$line $sourcepath/$1/$line 755 0 0" >> initramfs_list
done < .tmp
rm .tmp
}



###### START SCRIPT ######
echo "\n-------------------------------------------"
echo "Sourcepath: $sourcepath"
echo "Compiler:   $compiler"
echo "System:     $system"
echo "-------------------------------------------"
echo "\n  Compile Sources"
Create sysinfo.c bin/sysinfo
Create sysinfo_html.c www/cgi-bin/sysinfo_html
CreateDirectories

Copy busybox bin/busybox
Copy sysinfo usr/bin/sysinfo
Copy keymap etc/keymap
Copy init init

CopyDirectory etc/udhcpc
CopyDirectory lib
CopyDirectory www
CopyDirectory www/cgi-bin
echo "\n"
###### END SCRIPT ######


