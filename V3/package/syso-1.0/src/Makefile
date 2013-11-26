CROSS_COMPILE=arm-none-linux-gnueabi-

PATH=$PATH:/opt/toolchains/Sourcery-CodeBench-ARM-2013.05/bin
#PATH=$PATH:/opt/toolchains/Sourcery-CodeBench-ARM-2013.05/bin:/home/robin/SystemSoftware/buildroot-2013.08.1/output/host/opt/ext-toolchain/bin

.PHONY: all

all: static dynamic

static:
	$(CROSS_COMPILE)gcc -static -g -o out/timeinfo -g timeinfo.c
	$(CROSS_COMPILE)gcc -static -g -o out/sysinfo -g sysinfo.c
	$(CROSS_COMPILE)gcc -static -g -o out/sysinfo_html -g sysinfo_html.c

dynamic:
	$(CROSS_COMPILE)gcc -g -o out/timeinfo2 -g timeinfo.c 
