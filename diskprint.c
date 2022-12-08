#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FILE_NAME_SIZE 64
#define BLOCK_SIZE 4096

#define SUPERBLOCK_OFFSET 1024

int binary_to_decimal(char binary[], int len, int i);

char *decimal_to_binary(int decimal, int binary_length);

int convert_block(unsigned char[], int size );

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

int binary_to_decimal(char binary[], int len, int i)
{
   if (i == len - 1)
        return (binary[i] - '0');

   int temp = binary[i] - '0';

   temp = temp << (len - i - 1);
   temp = temp + binary_to_decimal(binary, len, i + 1);
   return temp;
}

char *decimal_to_binary(int decimal, int binary_length)
{
    int binaryNum[32];

    char *binary = malloc(binary_length);
  
    // counter for binary array
    int i = 0;
    while (decimal > 0) 
    {
        binaryNum[i] = decimal % 2;
        decimal = decimal / 2;
        i++;
    }

    for (int j = binary_length - 1; j >= 0; j--)
    {
        if (j >= i)
        {
            binary[binary_length - j - 1] = '0';
        }
        else
        {
            binary[binary_length - j - 1] = binaryNum[j] + '0';
        }
        
    }
    return binary;   
}

int convert_block(unsigned char block[], int size )
{
    char **binaries = malloc(sizeof(char *) * size);

    for (int i = 0; i < size; i++)
    {
        binaries[i] = decimal_to_binary(block[size - i - 1], 8);
    }

    char binary[32];

    int k = 0;
    int l = 0;

    for (int i = 0; i < 32; i++)
    {
        binary[i] = binaries[k][l];

        l = (l + 1) % 8;

        if (l == 0)
            k++;
    }

    for (int i = 0; i < 32; i++)
        printf("%c", binary[i]);
    printf("\n");

    int decimal = binary_to_decimal(binary, 32, 0);
    
    for (int i = 0; i < size; i++)
    {
        free(binaries[i]);
    }

    free(binaries);

    return decimal;
}