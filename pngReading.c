#include <stdio.h>
#include <err.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main(int argc, char **argv) 
{
    unsigned char png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    FILE *file = fopen(argv[1], "rb");
    
    if (!file)
    {
        errx(1, "The file doesn't exist\n");
    }
    
    unsigned char file_8_first_byte[8];
    fread(file_8_first_byte, 1, 8, file);
    
    if (memcmp(png_signature, file_8_first_byte, 8) != 0)
    {
        fclose(file);
        errx(2, "The file is not a PNG file\n");
    }

    unsigned char length_data[4];
    unsigned int length = 0;
    fread(length_data, 1, 4, file);
    for (size_t i = 0; i < 4; i++)
    {
        length *= 256;
        length += length_data[i];
    }
    printf("length of data = %u\n", length);

    char chunk_name[5];
    fread(chunk_name, 1, 4, file);
    chunk_name[4] = '\0';

    if (strcmp(chunk_name, "IHDR") != 0)
    {
        fclose(file);
        errx(3, "The chunk name is not valid, found %s\n", chunk_name);
    }

    unsigned char ihdr_data[length];
    fread(ihdr_data, 1, 13, file);

    unsigned int width = 0;
    unsigned int height = 0;

    for (int i = 0; i < 8; i++)
    {
        if (i < 4)
        {
            width *= 256;
            width += ihdr_data[i];
        }
        else
        {
            height *= 256;
            height += ihdr_data[i];
        }
    }

    unsigned char bit_depth = ihdr_data[8];
    unsigned char color_type = ihdr_data[9];
    
    printf("width = %u\nheight = %u\nbit_depth = %u\ncolor_type = %u\n",
            width, height, bit_depth, color_type); 

    fclose(file);
    return 0;
}
