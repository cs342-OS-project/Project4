#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FILE_NAME_SIZE 64
#define BLOCK_SIZE 4096

#define SUPERBLOCK_OFFSET 1024

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

    lseek(fd, 1024, SEEK_SET);

    n = read(fd, buffer, BLOCK_SIZE);

    if (n == BLOCK_SIZE) 
    {
        char number[4];
        for (int i = 0; i < 4; i++)
        {
            number[i] = buffer[i];
            printf("%c\n", number[i]);
        }

        
    }
    else
    {
        printf("Error\n");
    }

    return 0;
}
