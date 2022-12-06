#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FILE_NAME_SIZE 64
#define BLOCK_SIZE 4096

int main(int argc, char const *argv[])
{
    char devname[FILE_NAME_SIZE];  
    int fd; 
    int n; 
    off_t offset; 
    int blocknum;  
    unsigned char buffer[BLOCK_SIZE];

    strcpy(devname, argv[1]);

    fd = open (devname, O_RDONLY);

    if (fd < 0) 
    { 
        printf ("can not open disk file\n"); 
        exit(1); 
    } 

    printf("Disk opened\n");

    return 0;
}
