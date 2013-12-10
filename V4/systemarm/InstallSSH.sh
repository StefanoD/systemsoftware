#!/bin/sh

BUILDROOT_PATH=~/buildroot-2013.08.1
THIS=$(pwd)
echo "Installing rsa and dsa keys.."
if [ ! -d $BUILDROOT_PATH/output/target/etc/dropbear ] ; then
	echo "Create dropbear directory"	
	mkdir -p $BUILDROOT_PATH/output/target/etc/dropbear
fi
if [ ! -f ~/.ssh/id_dsa.pub ] ; then
		echo -n "Generating dsa key..\n"
		ssh-keygen -t dsa
		ssh-add ~/.ssh/id_dsa
else
	echo "id_dsa.pub bereits vorhanden"
fi


if [ ! -d $BUILDROOT_PATH/output/target/root/.ssh ] ; then
	echo "Create .ssh directory"	
	mkdir -p $BUILDROOT_PATH/output/target/root/.ssh
fi


echo "Kopiere to buildroot"
cp $THIS/dropbear_* $BUILDROOT_PATH/output/target/etc/dropbear
cat ~/.ssh/id_dsa.pub >> $BUILDROOT_PATH/output/target/root/.ssh/authorized_keys
chmod 0600 $BUILDROOT_PATH/output/target/root/.ssh/authorized_keys
echo "\n  \033[49;1;31m Fertig! Buildroot neu erstellen nicht vergessen! \033[0m"

