#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>              
#include <unistd.h>             
#include <sys/ioctl.h>   
       
#define magic_no 684
#define select_channel _IOR(magic_no, 1, int *)
#define select_allign _IOR(magic_no, 2, char *)
#define DEVICE_FILE_NAME "/dev/adc-dev"

uint16_t data;



int main()
{
    int fd;
    int channel;
    char align;
    
    fd = open(DEVICE_FILE_NAME, 0);
	if (fd < 0) 
	{
        	printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
        	exit(-1);
	}
		printf("Channel:");
		scanf("%d", &channel);
		printf("Alignment(r or l):");
		scanf(" %c", &align);
		
	if(channel <0 || channel>7 || (align != 'r' && align != 'l'))
	
	{
		printf("Invalid Selection of Channel or Alignment\n");
		return 1;
	}
	
	
  	
  	if (ioctl(fd, select_channel, channel) < 0) 
	{
       	printf("channel selection failed:\n");
        	return 1;
	}
    		
    
	
    	if (ioctl(fd, select_allign, align) < 0)
    	 {
       printf("alignment selection failed\n");
        exit(-1);
   	 }
    
		
		
	if(read(fd,&data,sizeof(data)))
	
	{
		if(align == 'l')
		{
		  printf("left alligned number %u \n", data);
		  printf("original number %u \n", data/16);
		}
		
	        else
		{
		  printf("right alligned number %u \n", data);
		  printf("original number %u \n", data/16);
        	}
	
	}
		
	close(fd);
	return 0;
}
