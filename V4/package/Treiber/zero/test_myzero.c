#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


int main()
{
	int fileDeskriptor, writtenBytes, readBytes, i, j;
	char buf[16];
	
	printf("Trying to open FileDeskriptor...\n");
	fileDeskriptor = open("/dev/mydevice", O_RDONLY);
	if (fileDeskriptor < 0) 
	{
		perror("open failed\n");
		return -1;
	}
	printf("FileDeskriptor opened!\n");
	printf("Trying to read...\n");
	readBytes = read(fileDeskriptor, buf, sizeof(buf));
	if (readBytes < 0)
	{
		printf("read failed: %s\n", strerror(errno));
		return -2;
	}
	printf("Read %d Bytes!\n", readBytes);
	printf("Buffer: %s\n", buf);
	printf("FileDeskriptor closing...\n");
	close(fileDeskriptor);
	printf("FileDeskriptor closed\n");
	return readBytes;
}
