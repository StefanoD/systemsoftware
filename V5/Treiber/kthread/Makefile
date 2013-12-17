SRCS=$(wildcard *_test.c)

ifneq (${KERNELRELEASE},)
    obj-m := kthread.o
else
	KERNEL_SOURCE := ~/buildroot-2013.08.1/output/build/linux-3.10.7
	CROSSP := /opt/toolchains/Sourcery-CodeBench-ARM-2013.05/bin
	PWD := $(shell pwd)
endif

default: $(SRCS:.c=) driver rm-tmp

driver:
	${MAKE} ARCH=arm CROSS_COMPILE=$(CROSSP)/arm-none-linux-gnueabi- -C ${KERNEL_SOURCE}  SUBDIRS=${PWD} modules

.c:
	$(CROSSP)/arm-none-linux-gnueabi-gcc $< -o $@

rm-tmp:
	rm -r *.order .*.cmd *.o *.mod.c *.symvers .tmp_versions
 
clean:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} clean
	rm *_test

install: 
	scp *.ko *_test root@192.168.29.6:~/

