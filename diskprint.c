#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "block_processing.h"

#define FILE_NAME_SIZE 64
#define ROOT_INO 2

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

    blocknum = 0;
    offset = blocknum * BLOCK_SIZE;

    lseek(fd, offset, SEEK_SET);

    n = read(fd, buffer, BLOCK_SIZE);

    if (n == BLOCK_SIZE) 
    {
        printf("1) s_inodes_count --> %d\n", retrieve_field(buffer, 0 + SUPERBLOCK_START_POS, 4));
        printf("2) s_blocks_count --> %d\n", retrieve_field(buffer, 4 + SUPERBLOCK_START_POS, 4));
        printf("3) s_r_blocks_count --> %d\n", retrieve_field(buffer, 8 + SUPERBLOCK_START_POS, 4));
        printf("4) s_free_blocks_count --> %d\n", retrieve_field(buffer, 12 + SUPERBLOCK_START_POS, 4));
        printf("5) s_inodes_count --> %d\n", retrieve_field(buffer, 16 + SUPERBLOCK_START_POS, 4));
        printf("6) s_first_data_block --> %d\n", retrieve_field(buffer, 20 + SUPERBLOCK_START_POS, 4));
        printf("7) s_log_block_size --> %d\n", retrieve_field(buffer, 24 + SUPERBLOCK_START_POS, 4));
        printf("8) s_log_frag_size --> %d\n", retrieve_field(buffer, 28 + SUPERBLOCK_START_POS, 4));
        printf("9) s_block_per_group --> %d\n", retrieve_field(buffer, 32 + SUPERBLOCK_START_POS, 4));
        printf("10) s_frags_per_group --> %d\n", retrieve_field(buffer, 36 + SUPERBLOCK_START_POS, 4));
    }
    else
    {
        printf("Error reading on block %ld\n", offset);
    }

    int inode_size = retrieve_field(buffer, 88 + SUPERBLOCK_START_POS, 2);

    // ROOT DIRECTORY

    printf("Root Directory Information\n");

    blocknum = 1; // Group Descriptor Table
    offset = blocknum * BLOCK_SIZE;

    lseek(fd, offset, SEEK_SET);

    n = read(fd, buffer, BLOCK_SIZE);

    int first_inode_table = retrieve_field(buffer, 8, 4); // block id of the first block of the “inode table” for the group represented.

    blocknum = first_inode_table;
    offset = blocknum * BLOCK_SIZE + (ROOT_INO - 1) * inode_size;

    lseek(fd, offset, SEEK_SET);

    n = read(fd, buffer, BLOCK_SIZE);

    int root_file_length = retrieve_field(buffer, 4, 4);
    int root_first_data_block = retrieve_field(buffer, 40, 4);

    printf("%d %d\n", root_first_data_block, root_file_length);

    blocknum = root_first_data_block;
    offset = root_first_data_block * BLOCK_SIZE;

    lseek(fd, offset, SEEK_SET);

    n = read(fd, buffer, BLOCK_SIZE);

    int ino_number = retrieve_field(buffer, 0, 4);
    int file_type = retrieve_field(buffer, 7, 1);

    printf("%d %d\n", ino_number, file_type);

    return 0;
}