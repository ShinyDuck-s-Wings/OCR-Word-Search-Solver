#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

void reinforce_contrast(SDL_Surface *surface)
{
    if(SDL_LockSurface(surface) != 0)
	{
		printf("SLD_LockSurface: %s\n",SDL_GetError());
		return;
	}
    Uint32 *pixels = (Uint32*) surface->pixels;
    int width = surface->w;
    int height = surface->h;
    const int radius = 10;
    const int operant_count = (1 + 2*radius) * (1 + 2 * radius);
    for(int i = radius; i < height - radius; i++)
    {
        for(int j = radius; j < width - radius; j++)
        {
            Uint8 min, max = pixels[(i - radius) * width + (j - radius)];
            for(int k = i - radius; k <= i + radius; k++)
            {
                for(int l = j - radius; l <= j + radius; l++)
                {
                    Uint32 pixel = pixels[k * width + l];
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

            Uint8 threshold = (min + max)/2 + 65;
            for(int k = i - radius; k <= i + radius; k++)
            {
                for(int l = j - radius; l <= j + radius; l++)
                {
                    Uint32 pixel = pixels[k * width + l];
                    Uint8 v = pixel & 0xFF;

                    if(v < threshold)
                    {
                        Uint32 pixel = SDL_MapRGB(surface->format, 0, 0, 0);
                        pixels[k * width + l] = pixel;
                    }
                    else
                    {
                        Uint32 pixel = SDL_MapRGB(surface->format, 255, 255, 255);
                        pixels[k * width + l] = pixel;
                    }
                }
            }

            j += radius;
        }
        i += radius;
    }

    SDL_UnlockSurface(surface);
}

void detect_edges(SDL_Surface *surface)
{
    if(SDL_LockSurface(surface) != 0)
	{
		printf("SLD_LockSurface: %s\n",SDL_GetError());
		return;
	}

    Uint32 *pixels = (Uint32*) surface->pixels;
    int width = surface->w;
    int height = surface->h;

    int lx_array[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int ly_array[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

    int *x_mat = malloc(height * width * sizeof(int));
    int *y_mat = malloc(height * width * sizeof(int));

    for(int i = 1; i + 1 < height; i++)
    {
        for(int j = 1; j + 1 < width; j++)
        {
            size_t l_index = 0;
            for(int k = i - 1; k <= i + 1; k++)
            {
                for(int l = j - 1; l <= j + 1; l++)
                {
                    Uint32 pixel = pixels[k * width + l];
                    Uint8 v = pixel & 0xFF;

                    int lx = v * lx_array[l_index];
                    int ly = v * ly_array[l_index];

                    x_mat[i * width + j] += lx;
                    y_mat[i * width + j] += ly;

                    l_index += 1;

                }
            }

        }
    }

    int *magnitude_matrix = malloc(height * width * sizeof(int));;
    long cumul = 0;

    for(int i = 1; i + 1 < height; i++)
    {
        for(int j = 1; j + 1 < width; j++)
        {
            size_t index = i * width + j;
            magnitude_matrix[index] = (int)round(sqrt(x_mat[index] * x_mat[index] + y_mat[index] * y_mat[index]));
            cumul += magnitude_matrix[index];
        }
    }



    int threshold = (int)(cumul/(long)((height - 2) * (width - 2))) + 10;
    for(int i = 1; i + 1 < height; i++)
    {
        for(int j = 1; j + 1 < width; j++)
        {
            int magnitude = magnitude_matrix[i * width + j];
            if(magnitude > threshold)
            {
                Uint32 pixel = SDL_MapRGB(surface->format, 0, 0, 0);
                pixels[i * width + j] = pixel;
            }
            else
            {
                Uint32 pixel = SDL_MapRGB(surface->format, 255, 255, 255);
                pixels[i * width + j] = pixel;
            }
        }
    }

    free(x_mat);
    free(y_mat);
    free(magnitude_matrix);

    SDL_UnlockSurface(surface);
}