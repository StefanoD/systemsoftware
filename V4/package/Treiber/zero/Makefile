KDIR := ~/buildroot-2013.08.1/output/build/linux-3.10.7
CROSSP := /opt/toolchains/Sourcery-CodeBench-ARM-2013.05/bin

ifneq ($(KERNELRELEASE),)
obj-m := zero.o
 
else
PWD := $(shell pwd)
 
default:
	make ARCH=arm CROSS_COMPILE=$(CROSSP)/arm-none-linux-gnueabi- -C $(KDIR) M=$(PWD) modules

clean:
	 rm -rf *~  modules.order Module.symvers *.mod.c *.o .* 
endif 
