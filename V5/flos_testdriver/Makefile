# Makefile – makefile of our first driver
# if KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq (${KERNELRELEASE},)
    obj-m := flosd.o buf.o
# Otherwise we were called directly from the command line.
# Invoke the kernel build system.
else
	KERNEL_SOURCE := ~/buildroot-2013.08.1/output/build/linux-3.10.7
	CROSSP := /opt/toolchains/Sourcery-CodeBench-ARM-2013.05/bin
	PWD := $(shell pwd)


default: build rm-tmp

build:
	${MAKE} ARCH=arm CROSS_COMPILE=$(CROSSP)/arm-none-linux-gnueabi- -C ${KERNEL_SOURCE} SUBDIRS=${PWD} modules
	$(CROSSP)/arm-none-linux-gnueabi-gcc test_buf.c -o test_buf

rm-tmp:
	rm -r *.order .*.cmd *.o *.mod.c *.symvers .tmp_versions
 
clean:
    ${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} clean
endif

install: 
	scp *.ko root@192.168.29.25:~/
	scp test_buf root@192.168.29.25:~/

