#PATH=$PATH:/home/robin/SystemSoftware/buildroot-2013.08.1/output/host/opt/ext-toolchain/bin
KDIR := ~/SystemSoftware/buildroot-2013.08.1/output/build/linux-3.10.7
CROSSP := ~/SystemSoftware/buildroot-2013.08.1/output/host/opt/ext-toolchain/bin

ifneq ($(KERNELRELEASE),)
obj-m := treiber.o
 
else
PWD := $(shell pwd)
 
default:
	make ARCH=arm CROSS_COMPILE=$(CROSSP)/arm-none-linux-gnueabi- -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf Makefile~  modules.order Module.symvers *.mod.c *.o .*
endif 
