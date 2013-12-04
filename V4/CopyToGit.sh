#!/bin/sh

BR_PATH=~/buildroot-2013.08.1
K_PATH=$BR_PATH/output/build/linux-3.10.7
BB_PATH=$BR_PATH/output/build/busybox-1.21.1
THIS=$(pwd)

echo "\nSave Current Settings in (old)"
cp $THIS/br.config $THIS/br\(old\).config
cp $THIS/bb.config $THIS/bb\(old\).config
cp $THIS/k.config $THIS/k\(old\).config


echo "Copy Configs to GIT-Repository! in $THIS"
cp $BR_PATH/.config $THIS/br.config
cp $BB_PATH/.config $THIS/bb.config
cp $K_PATH/.config $THIS/k.config

