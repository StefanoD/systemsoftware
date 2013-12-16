#include <stdio.h>
#include <fcntl.h>

#define BUFFER_LIMIT 100

int main (int argc, char* argv[]) 
{
	int i,fd;
	char ch, read_buf[BUFFER_LIMIT];

	fd = open("/dev/buf", O_RDWR);

	if (fd == -1)
	{
	    printf("Error in opening file \n");
	    return fd;
	}

	if(argc > 1)
	{
		while(1)
		{
			int off = sizeof(argv[1])+1;	
			if(off > BUFFER_LIMIT)
				off=BUFFER_LIMIT;
			printf("test_buf write %s (%d)in buffer\n",argv[1],off);
			write(fd, argv[1], off);
		}
	}
	else
	{
		while(1)
		{
			read(fd, read_buf, BUFFER_LIMIT);
			printf ("test_buf read: %s\n", read_buf);
		}
	}
	close(fd);
	return 0;
}


