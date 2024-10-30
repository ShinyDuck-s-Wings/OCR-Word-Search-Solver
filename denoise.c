#include <stdio.h>
#include <SDL2/SDL.h>

void swap(Uint8* a, Uint8* b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

Uint8 median(Uint8 array[], size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        size_t min = i;

        for (size_t j = i + 1; j < len; ++j)
            if (array[j] < array[min])
                min = j;

        swap(array + i, array + min);
    }

    return array[len/2];
}

Uint8 calculate_threshold(SDL_Surface *surface)
{
    Uint32 *pixels = (Uint32*) surface->pixels;
    Uint8 min, max = pixels[0];
    int width = surface->w;
    int height = surface->h;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            Uint32 pixel = pixels[i * width + j];
            Uint8 v = pixel & 0xFF;
            if(v < min)
            {
                min = v;
            }
            if(v > max)
            {
                max = v;
            }
        }
    }

    return (min + max) / 2;
}


void mean_filter(SDL_Surface *surface)
{
    if(SDL_LockSurface(surface) != 0)
	{
		printf("SLD_LockSurface: %s\n",SDL_GetError());
		return;
	}
    Uint32 *pixels = (Uint32*) surface->pixels;
    int width = surface->w;
    int height = surface->h;
    const int radius = 2;
    const int operant_count = (1 + 2*radius) * (1 + 2 * radius);
    for(int i = radius; i < height - radius; i++)
    {
        for(int j = radius; j < width - radius; j++)
        {
            int cumul = 0;
            for(int k = i - radius; k <= i + radius; k++)
            {
                for(int l = j - radius; l <= j + radius; l++)
                {
                    Uint32 pixel = pixels[k * width + l];
                    Uint8 v = pixel & 0xFF;

                    cumul += (int)v;
                }
            }

            Uint8 average = (Uint8)(cumul/operant_count);
            Uint32 pixel = SDL_MapRGB(surface->format, average, average, average);
            pixels[i * width + j] = pixel;
        }
    }

    SDL_UnlockSurface(surface);
}

void median_filter(SDL_Surface *surface)
{
    if(SDL_LockSurface(surface) != 0)
	{
		printf("SLD_LockSurface: %s\n",SDL_GetError());
		return;
	}
    Uint32 *pixels = (Uint32*) surface->pixels;
    int width = surface->w;
    int height = surface->h;
    const int radius = 2;
    const int operant_count = (1 + 2*radius) * (1 + 2 * radius);
    for(int i = radius; i < height - radius; i++)
    {
        for(int j = radius; j < width - radius; j++)
        {
            Uint8 pixel_array[operant_count];
            int pixel_array_index = 0;
            for(int k = i - radius; k <= i + radius; k++)
            {
                for(int l = j - radius; l <= j + radius; l++)
                {
                    Uint32 pixel = pixels[k * width + l];
                    Uint8 v = pixel & 0xFF;

                    pixel_array[pixel_array_index] = v;
                    pixel_array_index += 1;
                }
            }

            Uint8 m = median(pixel_array, (size_t)operant_count);
            Uint32 pixel = SDL_MapRGB(surface->format, m, m, m);
            pixels[i * width + j] = pixel;
        }
    }

    SDL_UnlockSurface(surface);
}

void denoise(SDL_Surface *surface)
{
    mean_filter(surface);
    median_filter(surface);
}