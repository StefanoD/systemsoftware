KDIR := ~/buildroot-2013.08.1/output/build/linux-3.10.7
CROSSP := /opt/toolchains/Sourcery-CodeBench-ARM-2013.05/bin

ifneq ($(KERNELRELEASE),)
obj-m := myzero.o
 
else
PWD := $(shell pwd)
 
default:
	make ARCH=arm CROSS_COMPILE=$(CROSSP)/arm-none-linux-gnueabi- -C $(KDIR) M=$(PWD) modules
	$(CROSSP)/arm-none-linux-gnueabi-gcc test_myzero.c -o test_myzero
	rm -rf *~  modules.order Module.symvers *.mod.c *.o .* 

clean:
	 rm -rf *~  modules.order Module.symvers *.mod.c *.o .* *.ko
endif 
