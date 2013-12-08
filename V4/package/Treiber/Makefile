ifneq ($(KERNELRELEASE),)
obj-m   := myzero.o
 
else
KDIR    := /lib/modules/$(shell uname -r)/build
PWD     := $(shell pwd)
 
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf modules.order Module.symvers *.mod.c *.o .*
cleanall:
	rm -rf modules.order Module.symvers *.mod.c *.o .* *.ko
endif
