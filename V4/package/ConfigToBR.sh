#!/bin/sh

BR_PATH=~/buildroot-2013.08.1
K_PATH=$BR_PATH/output/build/linux-3.10.7
BB_PATH=$BR_PATH/output/build/busybox-1.21.1
THIS=$(pwd)

echo "\nSave Buildroot settings -> .config(old)"
cp $BR_PATH/.config $BR_PATH/.config\(old\)
cp $BB_PATH/.config $BB_PATH/.config\(old\)
cp $K_PATH/.config $K_PATH/.config\(old\)


echo "Copy Git .configs to Buildroot-Repository!"
cp  $THIS/br.config $BR_PATH/.config
cp  $THIS/bb.config $BB_PATH/.config
cp  $THIS/k.config $K_PATH/.config

