#include <stdio.h>
#include <stdlib.h>
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
		int off = sizeof(argv[1])+1;	//size must be larger due to EOF sign
			if(off > BUFFER_LIMIT)
				off=BUFFER_LIMIT;

		while(1)
		{
			int st = rand()%1000000;
			write(fd, argv[1], off);
			printf("test_buf write %s (%d)in buffer  (sleep%d)\n",argv[1],off, st);
			usleep(st);
		}
	}
	else
	{
		while(1)
		{	
			int st = rand()%1000000;
			read(fd, read_buf, BUFFER_LIMIT);
			printf ("test_buf read: %s (sleep%d)\n", read_buf, st);
			usleep(st);
		}
	}
	close(fd);
	return 0;
}


