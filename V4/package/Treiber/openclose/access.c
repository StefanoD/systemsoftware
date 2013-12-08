#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


int main()
{
	int fileDeskriptor, writtenBytes, readBytes, i, j;
	char buf[16];
	static char *s = "Awesome String";
	
	fileDeskriptor = open("/dev/mydevice", O_RDWR);
	if (fileDeskriptor < 0) 
	{
		perror("open failed\n");
		return -1;
	}
	printf("fileDeskriptor opened!\n");
	writtenBytes = write(fileDeskriptor, s, strlen(s));
	if (writtenBytes < 0)
	{
		perror("write failed\n");
		return -1;
	}
	if ( writtenBytes != strlen(s))
	{
		perror("not all data written\n");
		return -2;
	};
	printf("Writing: %s\n", s);
	readBytes = read(fileDeskriptor, buf, sizeof(buf));
	if (readBytes < 0)
	{
		printf("read failed: %s\n", strerror(errno));
		return -2;
	}
	printf("Read %d Bytes\n", readBytes);
	printf("Buffer: %s\n", buf);
	for (i=0; i<(sizeof(buf)/16); i++)
	{ 
		for( j=0; j<16; j++ ) 
		{ 
			printf( "%2.2x ",buf[i*16+j] ); 
		} 
		printf("\n"); 
	} 
	close(fileDeskriptor);
	printf("fileDeskriptor closed\n");
	return readBytes;
}