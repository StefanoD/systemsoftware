CROSS_COMPILE=arm-none-linux-gnueabi-

PATH=$PATH:/opt/toolchains/Sourcery-CodeBench-ARM-2013.05/bin

.PHONY: all

all: static dynamic

static:
	$(CROSS_COMPILE)gcc -static -o timeinfo timeinfo.c 

dynamic:
	$(CROSS_COMPILE)gcc -o timeinfo2 timeinfo.c 
