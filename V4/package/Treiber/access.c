#include <stdio.h>
#include <fcntl.h>
#include <string.h>


int main()
{
	int FileDeskriptor, WrittenBytes, ReadBytes;
	char buf[128];
	char *s = "dasdasdasdasdsadasda";
	
	FileDeskriptor = open( "/dev/mydevice", O_RDONLY);
	if (FileDeskriptor < 0) 
	{
		printf("open failed\n");
		return -1;
	}
	WrittenBytes = write( FileDeskriptor, buf, strlen(s));
	if (WrittenBytes < 0)
	{
		printf("write failed\n");
		return -1;
	}
	if ( WrittenBytes != strlen(s))
	{
		printf("not all data written\n");
		return -2;
	}
	ReadBytes = read( FileDeskriptor, buf, sizeof(buf));
	if (ReadBytes <= 0)
	{
		printf("read failed\n");
		return -2;
	}
	close(FileDeskriptor);
	return ReadBytes;
}