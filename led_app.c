#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(void)
{
	int fd;

	fd = open("/dev/led",O_RDWR);
	if(fd < 0){
		perror("open");
		exit(1);
	}

	int on;
	while(1){ 
		on = 1;
		write(fd,&on,sizeof(on));
		sleep(1);
		on = 0;
		write(fd,&on,sizeof(on));
		sleep(1);
	}



	close(fd);
	return 0;
}
