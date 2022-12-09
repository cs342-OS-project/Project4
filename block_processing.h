#ifndef BLOCK_PROCESSING
#define BLOCK_PROCESSING

// Definitions

#define BLOCK_SIZE 4096
#define SUPERBLOCK_START_POS 1024
#define BYTE 8
#define INT_SIZE 32

int binary_to_decimal(char binary[], int len, int i);

char *decimal_to_binary(int decimal, int binary_length);

int convert_region(unsigned char region[], int size );

int retrieve_field(unsigned char buffer[BLOCK_SIZE], int offset, int size);

// Implementation

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
    int binaryNum[INT_SIZE];

    char *binary = malloc(binary_length);

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

int convert_region(unsigned char region[], int size )
{
    char **binaries = malloc(sizeof(char *) * size);

    for (int i = 0; i < size; i++)
    {
        binaries[i] = decimal_to_binary(region[size - i - 1], BYTE);
    }

    char binary[size * BYTE];

    int k = 0;
    int l = 0;

    for (int i = 0; i < size * BYTE; i++)
    {
        binary[i] = binaries[k][l];

        l = (l + 1) % BYTE;

        if (l == 0)
            k++;
    }

    int decimal = binary_to_decimal(binary, size * BYTE, 0);
    
    for (int i = 0; i < size; i++)
    {
        free(binaries[i]);
    }

    free(binaries);

    return decimal;
}

int retrieve_field(unsigned char buffer[BLOCK_SIZE], int offset, int size)
{
    unsigned char *field = malloc(sizeof(unsigned char) * size);
    for (int i = offset; i < offset + size; i++)
    {
        field[i - offset] = buffer[i];
    }

    int field_num =  convert_region(field, size);
    free(field);
    return field_num;
}

#endif