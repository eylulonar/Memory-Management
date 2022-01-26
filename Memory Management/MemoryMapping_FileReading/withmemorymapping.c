#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>

int main(){
    int file_des = open ("loremipsum.txt", O_RDONLY);
    struct stat s;
    size_t size;
    int status = fstat(file_des, &s);
    size = s.st_size;
	int count=0;
	off_t i;



    char *ptr = mmap(0,size,PROT_READ,MAP_PRIVATE,file_des, 0);
	
    if(ptr == MAP_FAILED){
        
		printf("Mapping Failed\n");
        return 1;
    }
	
	for (i = 0; i < size; i++)
    {
       
       if(ptr[i]== 'a')
	   {
        count++;
	  
	   }
    }	
	
	printf("The occurence of character 'a' is %d times. \n", count);	
 
    close(file_des);
   
    
    status = munmap(ptr, size);
    
	if(status != 0){
        printf("Unmapping Failed\n");
		return 1;
    }
    return 0;
}

