#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

int main() {
	struct timeval t1;
	struct timeval t2;
	gettimeofday(&t1,NULL);
	
	while(1) {
		gettimeofday(&t2,NULL);
		long res = t2.tv_sec - t1.tv_sec;
		printf("Zeit: %ld\n", res);
		fflush(stdout);
		sleep(1);
	}
	
}
