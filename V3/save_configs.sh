#!/bin/sh

echo "\n\tCopy Configs!"
cp .config ../../systemsoftware/V3/br.config
cp output/build/linux-3.10.7/.config ../../systemsoftware/V3/k.config
cp output/build/busybox-1.21.1/.config ../../systemsoftware/V3/bb.config
echo "\tdone!\n"


