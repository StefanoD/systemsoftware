CPATH:=/home/flfroehl/systemarm/buildroot-2013.08.1/output/host/opt/ext-toolchain/bin
ARM=arm
COMP=arm-none-linux-gnueabi-
.PHONY: all
# modulename
FILE=mod1


ifneq ($(KERNELRELEASE),)
obj-m   := $(FILE).o
 
else
			  
KDIR:=/home/flfroehl/systemarm/buildroot-2013.08.1/output/build/linux-3.10.7
PWD     := $(shell pwd)
 
default: 
	$(MAKE) ARCH=$(ARM) CROSS_COMPILE=$(CPATH)/$(COMP) -C $(KDIR) M=$(PWD) modules

endif

install:
	scp $(PWD)/$(FILE).ko root@192.168.29.25:.
