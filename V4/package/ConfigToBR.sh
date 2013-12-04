#!/bin/sh

BR_PATH=~/systemarm/buildroot-2013.08.1
K_PATH=$BR_PATH/output/build/linux-3.10.7
BB_PATH=$BR_PATH/output/build/busybox-1.21.1
THIS=$(pwd)

echo "\nSave Current Settings in (old)"
cp $BR_PATH/br.config $BR_PATH/br\(old\).config
cp $BB_PATH/bb.config $BB_PATH/bb\(old\).config
cp $K_PATH/k.config $K_PATH/k\(old\).config


echo "Copy Configs to GIT-Repository! in $THIS"
cp  $THIS/br.config $BR_PATH/.config
cp  $THIS/bb.config $BB_PATH/.config
cp  $THIS/k.config $K_PATH/.config

