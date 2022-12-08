#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "block_processing.h"

#define FILE_NAME_SIZE 64

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

    // SUPER BLOCK
    printf("Super Block Information\n");

    lseek(fd, SUPERBLOCK_OFFSET, SEEK_SET);

    n = read(fd, buffer, BLOCK_SIZE);

    if (n == BLOCK_SIZE) 
    {
        unsigned char number[4];
        for (int i = 4; i < 8; i++)
        {
            number[i - 4] = buffer[i];
            printf("%d\n", number[i - 4]);
        }
        printf("%d\n", convert_block(number, 4));
    }
    else
    {
        printf("Error\n");
    }

    return 0;
}