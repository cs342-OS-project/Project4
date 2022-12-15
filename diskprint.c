#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

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

    struct timeval time1, time2;

    strcpy(devname, argv[1]);

    fd = open(devname, O_RDONLY);

    if (fd < 0) 
    { 
        printf("can not open disk file\n");
        exit(1);
    }

    gettimeofday(&time1, NULL); // START TIMER

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
        printf("10) s_frags_per_group --> %d\n\n", retrieve_field(buffer, 36 + SUPERBLOCK_START_POS, 4));
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

    blocknum = root_first_data_block;
    offset = root_first_data_block * BLOCK_SIZE;

    lseek(fd, offset, SEEK_SET);

    n = read(fd, buffer, BLOCK_SIZE);

    offset = 0;
    int i = 1;

    while (1)
    {
        int rec_len = retrieve_field(buffer, offset + 4, 2);

        int file_name_length = retrieve_field(buffer, offset + 6, 1);

        if (file_name_length == 0)
            break;

        char *file_name =  retrieve_field_str(buffer, offset + 8, file_name_length + 1);

        printf("File %d name --> %s\n", i, file_name);

        int inode_number = retrieve_field(buffer, offset, 4);

        unsigned char inode_buffer[BLOCK_SIZE];

        lseek(fd, first_inode_table * BLOCK_SIZE + (inode_number - 1) * inode_size, SEEK_SET);
        n = read(fd, inode_buffer, BLOCK_SIZE);

        if (strcmp(file_name, ".") != 0 && strcmp(file_name, "..") != 0)
        {
            printf("Inode Information of file %s\n", file_name);

            printf("1) i_mode --> %d\n", retrieve_field(inode_buffer, 0, 2));
            printf("2) i_uid --> %d\n", retrieve_field(inode_buffer, 2, 2));
            printf("3) i_size --> %d\n", retrieve_field(inode_buffer, 4, 4));
            printf("4) i_atime --> %d\n", retrieve_field(inode_buffer, 8, 4));
            printf("5) i_ctime --> %d\n", retrieve_field(inode_buffer, 12, 4));
            printf("6) i_mtime --> %d\n", retrieve_field(inode_buffer, 16, 4));
            printf("7) i_dtime --> %d\n", retrieve_field(inode_buffer, 20, 4));
            printf("8) i_gid --> %d\n", retrieve_field(inode_buffer, 24, 4));
            printf("9) i_links_count --> %d\n", retrieve_field(inode_buffer, 26, 2));
            printf("10) i_blocks --> %d\n\n", retrieve_field(inode_buffer, 28, 4));
        }

        free(file_name);

        offset = offset + rec_len;
        i++;
    }

    gettimeofday(&time2, NULL); // STOP TIMER

    printf("Total time = %ld miliseconds\n", (time2.tv_usec - time1.tv_usec) );

    return 0;
}