#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void swap(Uint8* a, Uint8* b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

Uint8 median(Uint8 *array, size_t len)
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

Uint8 median_find_threshold(Uint8 array[], size_t len, Uint8* min, Uint8* max)
{
    for (size_t i = 0; i < len; ++i)
    {
        size_t min = i;

        for (size_t j = i + 1; j < len; ++j)
            if (array[j] < array[min])
                min = j;

        swap(array + i, array + min);
    }

    *min = array[0];
    *max = array[len - 1];

    return array[len/2];
}

Uint8 calculate_threshold(SDL_Surface *surface)
{
    Uint32 *pixels = (Uint32*) surface->pixels;
    Uint8 min = pixels[0] & 0xFF;
    Uint8 max = pixels[0] & 0xFF;
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

void to_grayscale(SDL_Surface *surface)
{
    if(SDL_LockSurface(surface) != 0)
	{
		printf("SLD_LockSurface: %s\n",SDL_GetError());
		return;
	}

    Uint32 *pixels = (Uint32*) surface->pixels;
    for(int i = 0; i < surface->h; i++)
    {
        for(int j = 0; j < surface->w; j++)
        {
            Uint32 pixel = pixels[i * surface->w + j];

            Uint8 r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);

            // Calculate the grayscale value
            Uint8 gray = (Uint8)(0.212671f * r + 0.715160f * g + 0.072169f * b);

            // Set the pixel to grayscale
            pixel = SDL_MapRGB(surface->format, gray, gray, gray);
            pixels[i * surface->w + j] = pixel;
        }
    }

    SDL_UnlockSurface(surface);
}

int is_noisy(SDL_Surface* original, SDL_Surface* blurred)
{
    Uint8 tone = calculate_threshold(original);
    if(tone > 150)
    {
        return 1;
    }

    Uint32 *original_pixels = (Uint32*) original->pixels;
    Uint32 *blurred_pixels = (Uint32*) blurred->pixels;

    int width = original->w;
    int height = original->h;

    long diff = 0;
    const long threshold = 400000;

    for(long i = 0; i < height; i++)
    {
        for(long j = 0; j < width; j++)
        {
            long index = i * width + j;
            int original_pixel = original_pixels[index] & 0xFF;
            int blurred_pixel = blurred_pixels[index] & 0xFF;

            if(abs(original_pixel - blurred_pixel) != 0)
            {
                diff += 1;
            }
        }
    }

    if(diff >= 330000 && diff <= 340000)
    {
        return 2;
    }

    if(diff > threshold)
    {
        return 1;
    }

    return 0;
}

void remove_column(SDL_Surface* surface, int column, int start, int end)
{
    Uint32 *pixels = (Uint32*) surface->pixels;
    int width = surface->w;

    for(int i = start; i < end; i++)
    {
        Uint32 pixel = SDL_MapRGB(surface->format, 255, 255, 255);
        pixels[i * width + column] = pixel;
    }
}

void remove_row(SDL_Surface* surface, int row, int start, int end)
{
    Uint32 *pixels = (Uint32*) surface->pixels;
    int width = surface->w;

    for(int i = start; i < end; i++)
    {
        Uint32 pixel = SDL_MapRGB(surface->format, 255, 255, 255);
        pixels[row * width + i] = pixel;
    }
}

void clean(SDL_Surface* surface)
{
    if(SDL_LockSurface(surface) != 0)
	{
		printf("SLD_LockSurface: %s\n",SDL_GetError());
		return;
	}

    Uint32 *pixels = (Uint32*) surface->pixels;

    int width = surface->w;
    int height = surface->h;
    int acceptable_space = width / 3.5;

    for(int i = 0; i < height; i++)
    {
        int space_size = 0;
        int end = 0;
        int start = 0;
        for(int j = 0; j < width; j++)
        {
            Uint32 pixel = pixels[i * width + j];
            Uint8 v = pixel & 0xFF;

            if(v <= 50)
            {
                //pixel is black
                int prev = end;
                end = j;

                if(space_size > acceptable_space)
                {
                    while (start < width && pixels[i * width + start] <= 50)
                    {
                        start += 1;
                    }
                    //printf("start = %i; end = %i\n", start, end);
                    remove_row(surface, i, start, end);
                }

                start = prev;

                while (j < width && v <= 50)
                {
                    j++;
                    v = pixels[i * width + j] & 0xFF;
                }

                space_size = 0;
            }

            if(v >= 200)
            {
                //white pixel
                space_size += 1;
            }
            else
            {
                space_size = 0;
            }
        }
    }

    acceptable_space = height/2.2;
    for(int i = 0; i < width; i++)
    {
        int space_size = 0;
        int end = 0;
        int start = 0;
        for(int j = 0; j < height; j++)
        {
            Uint32 pixel = pixels[j * width + i];
            Uint8 v = pixel & 0xFF;

            if(v <= 50)
            {
                //pixel is black
                int prev = end;
                end = j;

                if(space_size > acceptable_space)
                {
                    while (start < height && pixels[start * width + i] <= 50)
                    {
                        start += 1;
                    }

                    remove_column(surface, i, start, end);
                }

                start = prev;

                while (j < width && v <= 50)
                {
                    j++;
                    v = pixels[j * width + i] & 0xFF;
                }

                space_size = 0;
            }

            if(v >= 200)
            {
                //white pixel
                space_size += 1;
            }
            else
            {
                space_size = 0;
            }
        }
    }

    SDL_UnlockSurface(surface);
}

void mean_filter(SDL_Surface *surface, int radius)
{
    if(SDL_LockSurface(surface) != 0)
	{
		printf("SLD_LockSurface: %s\n",SDL_GetError());
		return;
	}

    SDL_Surface *temp = SDL_ConvertSurface(surface, surface->format, 0);

    Uint32 *pixels = (Uint32*) surface->pixels;
    Uint32 *temp_pixels = (Uint32*) temp->pixels;

    int width = surface->w;
    int height = surface->h;
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
                    Uint32 pixel = temp_pixels[k * width + l];
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

void median_filter(SDL_Surface *surface, int radius)
{
    if(SDL_LockSurface(surface) != 0)
	{
		printf("SLD_LockSurface: %s\n",SDL_GetError());
		return;
	}

    SDL_Surface *temp = SDL_ConvertSurface(surface, surface->format, 0);

    Uint32 *pixels = (Uint32*) surface->pixels;
    Uint32 *temp_pixels = (Uint32*) temp->pixels;

    int width = surface->w;
    int height = surface->h;
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
                    Uint32 pixel = temp_pixels[k * width + l];
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

void remove_particle(SDL_Surface *surface, int radius)
{
    if(SDL_LockSurface(surface) != 0)
	{
		printf("SLD_LockSurface: %s\n",SDL_GetError());
		return;
	}

    SDL_Surface *temp = SDL_ConvertSurface(surface, surface->format, 0);

    Uint32 *pixels = (Uint32*) surface->pixels;
    Uint32 *temp_pixels = (Uint32*) temp->pixels;

    int width = surface->w;
    int height = surface->h;

    for(int i = radius; i < height - radius; i++)
    {
        for(int j = radius; j < width - radius; j++)
        {
            int is_line = 0;

            for(int k = i - radius; k <= i + radius; k++)
            {
                for(int l = j - radius; l <= j + radius; l++)
                {
                    Uint32 pixel = temp_pixels[k * width + l];
                    Uint8 v = pixel & 0xFF;

                    if(k == i - radius || k == i + radius ||
                    l == j - radius || l == j + radius)
                    {
                        //on the border
                        if(v <= 50)
                        {
                            //pixel is black
                            is_line = 1;
                        }
                    }
                }
            }

            if(is_line == 0)
            {
                for(int k = i - radius; k <= i + radius; k++)
                {
                    for(int l = j - radius; l <= j + radius; l++)
                    {
                        pixels[k * width + l] = SDL_MapRGB(surface->format, 255, 255, 255);
                    }
                }
            }
        }
    }

    SDL_UnlockSurface(surface);
}

void median_contrast(SDL_Surface *surface)
{
    if(SDL_LockSurface(surface) != 0)
	{
		printf("SLD_LockSurface: %s\n",SDL_GetError());
		return;
	}

    SDL_Surface *temp = SDL_ConvertSurface(surface, surface->format, 0);

    Uint32 *pixels = (Uint32*) surface->pixels;
    Uint32 *temp_pixels = (Uint32*) temp->pixels;

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
                    Uint32 pixel = temp_pixels[k * width + l];
                    Uint8 v = pixel & 0xFF;

                    pixel_array[pixel_array_index] = v;
                    pixel_array_index += 1;
                }
            }

            Uint8 min, max;

            Uint8 m = median_find_threshold(pixel_array, (size_t)operant_count, &min, &max);
            Uint8 threshold = ((int)min + (int)max) / 2;

            Uint32 pixel = SDL_MapRGB(surface->format, 255, 255, 255);
            if(m <= threshold)
            {
                pixel = SDL_MapRGB(surface->format, 0, 0, 0);
            }

            pixels[i * width + j] = pixel;            
        }
    }

    SDL_UnlockSurface(surface);
}

void trace_skeleton(SDL_Surface *surface)
{
    if(SDL_LockSurface(surface) != 0)
	{
		printf("SLD_LockSurface: %s\n",SDL_GetError());
		return;
	}

    SDL_Surface *temp = SDL_ConvertSurface(surface, surface->format, 0);

    Uint32 *pixels = (Uint32*) surface->pixels;
    Uint32 *temp_pixels = (Uint32*) temp->pixels;

    int width = surface->w;
    int height = surface->h;
    const int radius = 1;
    Uint8 threshold = calculate_threshold(surface);
    
    for(int i = radius; i < height - radius; i++)
    {
        for(int j = radius; j < width - radius; j++)
        {
            
            Uint32 pixel = SDL_MapRGB(surface->format, 255, 255, 255);
            Uint8 center = temp_pixels[i * width + j] & 0xFF;

            if(center < threshold)
            {
                Uint8 min = center;
                for(int k = i - radius; k <= i + radius; k++)
                {
                    for(int l = j - radius; l <= j + radius; l++)
                    {
                        Uint32 pixel = temp_pixels[k * width + l];
                        Uint8 v = pixel & 0xFF;

                        if(min > v)
                        {
                            min = v;
                        }
                    }
                }


                if(center <= min + 90)
                {
                    pixel = SDL_MapRGB(surface->format, 0, 0, 0);
                }
            }

            pixels[i * width + j] = pixel;
        }
    }

    SDL_UnlockSurface(surface);
}

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

    Uint8 threshold = calculate_threshold(surface);

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            Uint32 pixel = pixels[i * width + j];
            Uint8 v = pixel & 0xFF;

            pixel = SDL_MapRGB(surface->format, 255, 255, 255);
            if(v < threshold + 40)
            {
                pixel = SDL_MapRGB(surface->format, 0, 0, 0);
            }

            pixels[i * width + j] = pixel;
        }
    }

    SDL_UnlockSurface(surface);
}
/*
void reinforce_contrast(SDL_Surface *surface)
{
    if(SDL_LockSurface(surface) != 0)
	{
		printf("SLD_LockSurface: %s\n",SDL_GetError());
		return;
	}

    SDL_Surface *temp = SDL_ConvertSurface(surface, surface->format, 0);

    Uint32 *pixels = (Uint32*) surface->pixels;
    Uint32 *temp_pixels = (Uint32*) temp->pixels;

    int width = surface->w;
    int height = surface->h;
    const int radius = 10;

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            Uint8 center_pixel = temp_pixels[i* width + j];
            Uint8 min = center_pixel;
            Uint8 max = center_pixel;
            for(int k = i - radius; k <= i + radius; k++)
            {
                if(k >= 0 && k < height)
                {
                    Uint32 pixel = temp_pixels[k * width + j];
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

            for(int k = j - radius; k <= j + radius; k++)
            {
                if(k >= 0 && k < width)
                {
                    Uint32 pixel = temp_pixels[i * width + k];
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

            int k = i - radius;
            int l = j - radius;
            while(k <= i + radius)
            {
                if(k >= 0 && k < height && l >= 0 && l < width)
                {
                    Uint32 pixel = temp_pixels[k * width + l];
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

                k++;
                l++;
            }

            k = i - radius;
            l = j + radius;
            while(k <= i + radius)
            {
                if(k >= 0 && k < height && l >= 0 && l < width)
                {
                    Uint32 pixel = temp_pixels[k * width + l];
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

                k++;
                l--;
            }

            Uint8 threshold = ((float)min + (float)max)/2;
            //const int contrast = 5;

            Uint32 pixel = SDL_MapRGB(surface->format, 255, 255, 255);
            if(center_pixel < threshold + 10)// && ((int)max - (int)min) > contrast)
            {
                pixel = SDL_MapRGB(surface->format, 0, 0, 0);
            }

            pixels[i * width + j] = pixel;
        }
    }

    SDL_UnlockSurface(surface);
}
*/

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

    int *x_mat = calloc(1, height * width * sizeof(int));
    int *y_mat = calloc(1, height * width * sizeof(int));

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
            Uint32 pixel = SDL_MapRGB(surface->format, 255, 255, 255);
            int magnitude = magnitude_matrix[i * width + j];

            if(magnitude > threshold)
            {
                
                if((magnitude >= magnitude_matrix[i * width + j - 1] &&
                magnitude >= magnitude_matrix[i * width + j + 1]) ||
                (magnitude >= magnitude_matrix[(i - 1) * width + j] &&
                magnitude >= magnitude_matrix[(i + 1) * width + j]) ||
                (magnitude >= magnitude_matrix[(i - 1) * width + j - 1] &&
                magnitude >= magnitude_matrix[(i + 1) * width + j + 1]) ||
                (magnitude >= magnitude_matrix[(i - 1) * width + j + 1] &&
                magnitude >= magnitude_matrix[(i + 1) * width + j - 1]))
                {
                    pixel = SDL_MapRGB(surface->format, 0, 0, 0);
                }


            }

            pixels[i * width + j] = pixel;
        }
    }

    free(x_mat);
    free(y_mat);
    free(magnitude_matrix);

    SDL_UnlockSurface(surface);
}

void fill_letters(SDL_Surface *surface, SDL_Surface* reference)
{
    if(SDL_LockSurface(surface) != 0)
	{
		printf("SLD_LockSurface: %s\n",SDL_GetError());
		return;
	}

    SDL_Surface *temp = SDL_ConvertSurface(surface, surface->format, 0);

    Uint32 *pixels = (Uint32*)surface->pixels;
    Uint32 *temp_pixels = (Uint32*)temp->pixels;
    Uint32* reference_pixels = (Uint32*)reference->pixels;

    int width = surface->w;
    int height = surface->h;
    const int radius = 5;

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int edge_encountered = 0;

            Uint8 center_pixel = temp_pixels[i* width + j];
            if((center_pixel & 0xFF) == 0)
            {
                //the pixel is black
                long index = i* width + j;
                pixels[index] = reference_pixels[index];
                continue;
            }

            int k = i;
            int l = j;
            
            while(k >= 0 && k < height && k <= i + radius)
            {
                Uint32 pixel = temp_pixels[k * width + j];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;
                    break;
                }

                k++;
            }

            k = i;
            while(k >= 0 && k < height && k >= i - radius)
            {
                Uint32 pixel = temp_pixels[k * width + j];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;
                    break;
                }

                k--;
            }

            k = j;
            while(k >= 0 && k < width && k <= j + radius)
            {
                Uint32 pixel = temp_pixels[i * width + k];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;
                    break;
                }

                k++;
            }

            k = j;
            while(k >= 0 && k < width && k >= j - radius)
            {
                Uint32 pixel = temp_pixels[i * width + k];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;
                    break;
                }

                k--;
            }

            k = i;
            while(k >= 0 && k < height && k <= i + radius)
            {
                Uint32 pixel = temp_pixels[k * width + l];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;
                    break;
                }

                k++;
                l--;
            }

            k = i;
            l = j;
            while(k >= 0 && k < height && k <= i + radius)
            {
                Uint32 pixel = temp_pixels[k * width + l];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;
                    break;
                }

                k++;
                l++;
            }

            k = i;
            l = j;
            while(k >= 0 && k < height && k >= i - radius)
            {
                Uint32 pixel = temp_pixels[k * width + l];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;
                    break;
                }

                k--;
                l++;
            }

            k = i;
            l = j;
            while(k >= 0 && k < height && k >= i - radius)
            {
                Uint32 pixel = temp_pixels[k * width + l];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;
                    break;
                }

                k--;
                l--;
            }

            if(edge_encountered >= 6)
            {
                long index = i * width + j;
                pixels[index] = reference_pixels[index];
            }
        }
    }

    SDL_UnlockSurface(surface);
}

void fill(SDL_Surface *surface, int radius)
{
    if(SDL_LockSurface(surface) != 0)
	{
		printf("SLD_LockSurface: %s\n",SDL_GetError());
		return;
	}

    SDL_Surface *temp = SDL_ConvertSurface(surface, surface->format, 0);

    Uint32 *pixels = (Uint32*) surface->pixels;
    Uint32 *temp_pixels = (Uint32*) temp->pixels;

    int width = surface->w;
    int height = surface->h;

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int edge_encountered = 0;

            Uint8 center_pixel = temp_pixels[i* width + j];
            if((center_pixel & 0xFF) == 0)
            {
                //the pixel is black
                continue;
            }

            int k = i;
            int l = j;
            int is_close = 0;
            int is_between = 0;
            
            while(k >= 0 && k < height && k <= i + radius)
            {
                Uint32 pixel = temp_pixels[k * width + j];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;

                    if(k <= i + 2)
                    {
                        is_close += 1;
                    }

                    break;
                }

                k++;
            }

            k = i;
            while(k >= 0 && k < height && k >= i - radius)
            {
                Uint32 pixel = temp_pixels[k * width + j];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;

                    if(k >= i - 2)
                    {
                        is_close += 1;
                    }

                    break;
                }

                k--;
            }

            k = j;
            while(k >= 0 && k < width && k <= j + radius)
            {
                Uint32 pixel = temp_pixels[i * width + k];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;

                    if(k >= i - 2)
                    {
                        is_between += 1;
                    }

                    break;
                }

                k++;
            }

            k = j;
            while(k >= 0 && k < width && k >= j - radius)
            {
                Uint32 pixel = temp_pixels[i * width + k];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;

                    if(k >= i - 2)
                    {
                        is_between += 1;
                    }

                    break;
                }

                k--;
            }

            k = i;
            while(k >= 0 && k < height && k <= i + radius)
            {
                Uint32 pixel = temp_pixels[k * width + l];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;
                    break;
                }

                k++;
                l--;
            }

            k = i;
            l = j;
            while(k >= 0 && k < height && k <= i + radius)
            {
                Uint32 pixel = temp_pixels[k * width + l];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;
                    break;
                }

                k++;
                l++;
            }

            k = i;
            l = j;
            while(k >= 0 && k < height && k >= i - radius)
            {
                Uint32 pixel = temp_pixels[k * width + l];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;
                    break;
                }

                k--;
                l++;
            }

            k = i;
            l = j;
            while(k >= 0 && k < height && k >= i - radius)
            {
                Uint32 pixel = temp_pixels[k * width + l];
                Uint8 v = pixel & 0xFF;

                if(v <= 50)
                {
                    edge_encountered += 1;
                    break;
                }

                k--;
                l--;
            }

            if(((edge_encountered >= 6 || is_close == 2) && is_between != 2) || edge_encountered == 8)
            {
                pixels[i * width + j] = SDL_MapRGB(surface->format, 0, 0, 0);
            }
        }
    }

    SDL_UnlockSurface(surface);
}

double calculate_skew_angle(SDL_Surface* surface)
{
    Uint32* pixels = (Uint32*)surface->pixels;

    int width = surface->w;
    int height = surface->h;
    const double PI = 3.14159265;
    int max_angle = 0;
    long max_intensity = width/2;

    // Test angles between -45 to 45 degrees to find the angle with max intensity
    for(int angle = -45; angle <= 45; angle++)
    {
        double rad = (double)angle * PI / 180.0;
        double intensity = 0;

        for(int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                // Rotate coordinates based on angle
                int x_rotated = (int)(x * cos(rad) - y * sin(rad));
                int y_rotated = (int)(x * sin(rad) + y * cos(rad));

                // Check if within image bounds
                if(x_rotated >= 0 && x_rotated < width && y_rotated >= 0 && y_rotated < height)
                {
                    Uint32 pixel = pixels[y_rotated * width + x_rotated];
                    Uint8 v = pixel & 0xFF;
                    // Sum pixel intensities for this angle
                    intensity = intensity + (255 - v);

                    //printf("intensity = %f\n", intensity);
                }
            }
        }

        // Keep the angle with the highest intensity
        if(intensity > max_intensity)
        {
            max_intensity = intensity;
            max_angle = angle;
        }
    }

    return max_angle;
}

int get_angle(SDL_Surface* surface)
{
    SDL_Surface* temp = SDL_ConvertSurface(surface, surface->format, 0);

    mean_filter(temp, 2);
    median_filter(temp, 2);
    detect_edges(temp);

    Uint32* pixels = (Uint32*)temp->pixels;

    int width = surface->w;
    int height = surface->h;
    int rotated_angle = 0;
    long max_intensity = 0;

    for(int b = 0; b < height; b++)
    {
        for(int angle = -45; angle <= 45; angle++)
        {
            long intensity = 0;
            for(int x = 0; x < width; x++)
            {
                double a = tan((double)angle);

                int y = round(a * -x + b);

                if(y >= 0 && y < height)
                {
                    //is within bounds
                    Uint32 pixel = pixels[y * width + x];
                    Uint8 v = pixel & 0xFF;
                    
                    if(v <= 50)
                    {
                        //pixel is black
                        intensity += 1;
                    }
                }
            }

            if(intensity > max_intensity)
            {
                max_intensity = intensity;
                rotated_angle = angle;
            }
        }
    }

    return rotated_angle * 2;
}


SDL_Surface* rotate_surface(SDL_Surface* surface, double angle)
{
    SDL_Surface* rotated = SDL_ConvertSurface(surface, surface->format, 0);

    Uint32* pixels = (Uint32*)surface->pixels;
    Uint32* rotated_pixels = (Uint32*)rotated->pixels;

    int width = surface->w;
    int height = surface->h;
    const double PI = 3.14159265;
    double rad = angle * PI / 180.0;

    int cx = width / 2;
    int cy = height / 2;

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++) {
            // Reverse rotation calculation
            int new_x = (int)((x - cx) * cos(rad) + (y - cy) * sin(rad) + cx);
            int new_y = (int)(-(x - cx) * sin(rad) + (y - cy) * cos(rad) + cy);

            // Check bounds and assign rotated pixel
            if(new_x >= 0 && new_x < width && new_y >= 0 && new_y < height) {
                rotated_pixels[y * width + x] = pixels[new_y * width + new_x];
            }
            else
            {
                rotated_pixels[y * width + x] = SDL_MapRGB(rotated->format, 255, 255, 255);  // Fill with white background
            }
        }
    }

    return rotated;
}

SDL_Surface* preprocess(SDL_Surface* surface)
{
    to_grayscale(surface);

    SDL_Surface *blurred_surface = SDL_ConvertSurface(surface, surface->format, 0);

    mean_filter(blurred_surface, 2);
    median_filter(blurred_surface, 2);

    double angle = get_angle(surface);

    int noise_level = is_noisy(surface, blurred_surface);

    if(noise_level == 1)
    {
        surface = blurred_surface;

        SDL_Surface *reference = SDL_ConvertSurface(surface, surface->format, 0);
        median_contrast(reference);

        detect_edges(surface);

        for(int i = 1; i <= 3; i++)
        {
            remove_particle(surface, i);
        }

        for(int i = 0; i < 5; i++)
        {
            fill_letters(surface, reference);
        }

        for(int i = 0; i < 5; i++)
        {
            clean(surface);
        }

        for(int i = 1; i <= 3; i++)
        {
            remove_particle(surface, i);
        }

        for(int i = 0; i < 2; i++)
        {
            fill(surface, 2);
        }
    }
    else if(noise_level == 0)
    {
        trace_skeleton(surface);

        for(int i = 1; i <= 3; i++)
        {
            remove_particle(surface, i);
        }

    }
    else
    {
        reinforce_contrast(surface);
    }

    if(abs((int)angle) > 1)
    {
        mean_filter(surface, 1);
        surface = rotate_surface(surface, angle);
    }
     
    return surface;
}
