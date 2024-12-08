#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "morphoMaths.h"

// Take two SDL_Surface with the same size, 
// The first one being an image in black and white or grayscale
// The second one will be replace by the erosion process of the first one
void erosion8(SDL_Surface *imageInit, SDL_Surface *imageResult)
{
    Uint32 *pixels = imageInit->pixels;
    Uint32 *rpixels = imageResult->pixels;

    for (int i = 0; i < imageInit->h; i++)
    {
        for (int j = 0; j < imageInit->w; j++)
        {
            Uint32 pixel = pixels[i * imageInit->w + j];
            rpixels[i * imageInit->w + j] = pixel;
            Uint32 rpixel = rpixels[i * imageInit->w + j];

            if (j + 1 < imageInit->w)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) < 
                    (Uint8)(pixels[i* imageInit->w +j+1] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[i * imageInit->w + j + 1];
                }

                if (i + 1 < imageInit->h)
                {
                    if ((Uint8)(rpixel >> 16 & 0xFF) <
                        (Uint8)(pixels[(i+1)* imageInit->w +j+1] >> 16 & 0xFF))
                    {
                        rpixels[i * imageInit->w + j] = 
                            pixels[(i+1) * imageInit->w + j + 1];
                    }
                }

                if (i - 1 >= 0)
                {
                    if ((Uint8)(rpixel >> 16 & 0xFF) <
                        (Uint8)(pixels[(i-1)* imageInit->w +j+1] >> 16 & 0xFF))
                    {
                        rpixels[i * imageInit->w + j] = 
                            pixels[(i-1) * imageInit->w + j + 1];
                    }
                }
            }

            if (j - 1 >= 0)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) < 
                    (Uint8)(pixels[i* imageInit->w +j-1] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[i * imageInit->w + j - 1];
                }
                if (i + 1 < imageInit->h)
                {
                    if ((Uint8)(rpixel >> 16 & 0xFF) <
                        (Uint8)(pixels[(i+1)* imageInit->w +j-1] >> 16 & 0xFF))
                    {
                        rpixels[i * imageInit->w + j] = 
                            pixels[(i+1) * imageInit->w + j - 1];
                    }
                }
                if (i - 1 >= 0)
                {
                    if ((Uint8)(rpixel >> 16 & 0xFF) <
                        (Uint8)(pixels[(i-1)* imageInit->w +j-1] >> 16 & 0xFF))
                    {
                        rpixels[i * imageInit->w + j] = 
                            pixels[(i-1) * imageInit->w + j - 1];
                    }
                }
            }
            
            if (i + 1 < imageInit->h)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) <
                    (Uint8)(pixels[(i+1) * imageInit->w + j] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[(i+1) * imageInit->w + j];
                }
            }
            if (i - 1 >= 0)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) <
                    (Uint8)(pixels[(i-1) * imageInit->w + j] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[(i-1) * imageInit->w + j];
                }
            }
        }
    }
}

// Take two SDL_Surface with the same size, 
// The first one being an image in black and white or grayscale
// The second one will be replace by the erosion process of the first one
void erosion4(SDL_Surface *imageInit, SDL_Surface *imageResult)
{
    Uint32 *pixels = imageInit->pixels;
    Uint32 *rpixels = imageResult->pixels;

    for (int i = 0; i < imageInit->h; i++)
    {
        for (int j = 0; j < imageInit->w; j++)
        {
            Uint32 pixel = pixels[i * imageInit->w + j];
            rpixels[i * imageInit->w + j] = pixel;
            Uint32 rpixel = rpixels[i * imageInit->w + j];

            if (j + 1 < imageInit->w)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) < 
                    (Uint8)(pixels[i* imageInit->w +j+1] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[i * imageInit->w + j + 1];
                }
            }

            if (j - 1 >= 0)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) < 
                    (Uint8)(pixels[i* imageInit->w +j-1] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[i * imageInit->w + j - 1];
                }
            }
            
            if (i + 1 < imageInit->h)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) <
                    (Uint8)(pixels[(i+1) * imageInit->w + j] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[(i+1) * imageInit->w + j];
                }
            }
            if (i - 1 >= 0)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) <
                    (Uint8)(pixels[(i-1) * imageInit->w + j] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[(i-1) * imageInit->w + j];
                }
            }
        }
    }
}

// Take two SDL_Surface with the same size, 
// The first one being an image in black and white or grayscale
// The second one will be replace by the erosion process of the first one
void erosion2x(SDL_Surface *imageInit, SDL_Surface *imageResult)
{
    Uint32 *pixels = imageInit->pixels;
    Uint32 *rpixels = imageResult->pixels;

    for (int i = 0; i < imageInit->h; i++)
    {
        for (int j = 0; j < imageInit->w; j++)
        {
            Uint32 pixel = pixels[i * imageInit->w + j];
            rpixels[i * imageInit->w + j] = pixel;
            Uint32 rpixel = rpixels[i * imageInit->w + j];

            if (j + 1 < imageInit->w)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) < 
                    (Uint8)(pixels[i* imageInit->w +j+1] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[i * imageInit->w + j + 1];
                }
            }

            if (j - 1 >= 0)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) < 
                    (Uint8)(pixels[i* imageInit->w +j-1] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[i * imageInit->w + j - 1];
                }
            }
        }
    }
}

// Take two SDL_Surface with the same size, 
// The first one being an image in black and white or grayscale
// The second one will be replace by the erosion process of the first one
void erosion2y(SDL_Surface *imageInit, SDL_Surface *imageResult)
{
    Uint32 *pixels = imageInit->pixels;
    Uint32 *rpixels = imageResult->pixels;

    for (int i = 0; i < imageInit->h; i++)
    {
        for (int j = 0; j < imageInit->w; j++)
        {
            Uint32 pixel = pixels[i * imageInit->w + j];
            rpixels[i * imageInit->w + j] = pixel;
            Uint32 rpixel = rpixels[i * imageInit->w + j];
            
            if (i + 1 < imageInit->h)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) <
                    (Uint8)(pixels[(i+1) * imageInit->w + j] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[(i+1) * imageInit->w + j];
                }
            }

            if (i - 1 >= 0)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) <
                    (Uint8)(pixels[(i-1) * imageInit->w + j] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[(i-1) * imageInit->w + j];
                }
            }
        }
    }
}

// Take two SDL_Surface with the same size, 
// The first one being an image in black and white or grayscale
// The second one will be replace by the delatation process of the first one
void dilatation8(SDL_Surface *imageInit, SDL_Surface *imageResult)
{
    Uint32 *pixels = imageInit->pixels;
    Uint32 *rpixels = imageResult->pixels;

    for (int i = 0; i < imageInit->h; i++)
    {
        for (int j = 0; j < imageInit->w; j++)
        {
            Uint32 pixel = pixels[i * imageInit->w + j];
            rpixels[i * imageInit->w + j] = pixel;
            Uint32 rpixel = rpixels[i * imageInit->w + j];

            if (j + 1 < imageInit->w)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) > 
                    (Uint8)(pixels[i* imageInit->w +j+1] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[i * imageInit->w + j + 1];
                }

                if (i + 1 < imageInit->h)
                {
                    if ((Uint8)(rpixel >> 16 & 0xFF) >
                        (Uint8)(pixels[(i+1)* imageInit->w +j+1] >> 16 & 0xFF))
                    {
                        rpixels[i * imageInit->w + j] = 
                            pixels[(i+1) * imageInit->w + j + 1];
                    }
                }

                if (i - 1 >= 0)
                {
                    if ((Uint8)(rpixel >> 16 & 0xFF) >
                        (Uint8)(pixels[(i-1)* imageInit->w +j+1] >> 16 & 0xFF))
                    {
                        rpixels[i * imageInit->w + j] = 
                            pixels[(i-1) * imageInit->w + j + 1];
                    }
                }
            }

            if (j - 1 >= 0)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) > 
                    (Uint8)(pixels[i* imageInit->w +j-1] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[i * imageInit->w + j - 1];
                }
                if (i + 1 < imageInit->h)
                {
                    if ((Uint8)(rpixel >> 16 & 0xFF) >
                        (Uint8)(pixels[(i+1)* imageInit->w +j-1] >> 16 & 0xFF))
                    {
                        rpixels[i * imageInit->w + j] = 
                            pixels[(i+1) * imageInit->w + j - 1];
                    }
                }
                if (i - 1 >= 0)
                {
                    if ((Uint8)(rpixel >> 16 & 0xFF) >
                        (Uint8)(pixels[(i-1)* imageInit->w +j-1] >> 16 & 0xFF))
                    {
                        rpixels[i * imageInit->w + j] = 
                            pixels[(i-1) * imageInit->w + j - 1];
                    }
                }
            }
            
            if (i + 1 < imageInit->h)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) >
                    (Uint8)(pixels[(i+1) * imageInit->w + j] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[(i+1) * imageInit->w + j];
                }
            }
            if (i - 1 >= 0)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) >
                    (Uint8)(pixels[(i-1) * imageInit->w + j] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[(i-1) * imageInit->w + j];
                }
            }
        }
    }
}

// Take two SDL_Surface with the same size, 
// The first one being an image in black and white or grayscale
// The second one will be replace by the delatation process of the first one
void dilatation4(SDL_Surface *imageInit, SDL_Surface *imageResult)
{
    Uint32 *pixels = imageInit->pixels;
    Uint32 *rpixels = imageResult->pixels;

    for (int i = 0; i < imageInit->h; i++)
    {
        for (int j = 0; j < imageInit->w; j++)
        {
            Uint32 pixel = pixels[i * imageInit->w + j];
            rpixels[i * imageInit->w + j] = pixel;
            Uint32 rpixel = rpixels[i * imageInit->w + j];

            if (j + 1 < imageInit->w)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) > 
                    (Uint8)(pixels[i* imageInit->w +j+1] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[i * imageInit->w + j + 1];
                }
            }

            if (j - 1 >= 0)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) > 
                    (Uint8)(pixels[i* imageInit->w +j-1] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[i * imageInit->w + j - 1];
                }
            }
            
            if (i + 1 < imageInit->h)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) >
                    (Uint8)(pixels[(i+1) * imageInit->w + j] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[(i+1) * imageInit->w + j];
                }
            }
            if (i - 1 >= 0)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) >
                    (Uint8)(pixels[(i-1) * imageInit->w + j] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[(i-1) * imageInit->w + j];
                }
            }
        }
    }
}

// Take two SDL_Surface with the same size, 
// The first one being an image in black and white or grayscale
// The second one will be replace by the delatation process of the first one
void dilatation2x(SDL_Surface *imageInit, SDL_Surface *imageResult)
{
    Uint32 *pixels = imageInit->pixels;
    Uint32 *rpixels = imageResult->pixels;

    for (int i = 0; i < imageInit->h; i++)
    {
        for (int j = 0; j < imageInit->w; j++)
        {
            Uint32 pixel = pixels[i * imageInit->w + j];
            rpixels[i * imageInit->w + j] = pixel;
            Uint32 rpixel = rpixels[i * imageInit->w + j];

            if (j + 1 < imageInit->w)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) > 
                    (Uint8)(pixels[i* imageInit->w +j+1] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[i * imageInit->w + j + 1];
                }
            }

            if (j - 1 >= 0)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) > 
                    (Uint8)(pixels[i* imageInit->w +j-1] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[i * imageInit->w + j - 1];
                }
            }
        }
    }
}

// Take two SDL_Surface with the same size, 
// The first one being an image in black and white or grayscale
// The second one will be replace by the erosion process of the first one
void dilatation2y(SDL_Surface *imageInit, SDL_Surface *imageResult)
{
    Uint32 *pixels = imageInit->pixels;
    Uint32 *rpixels = imageResult->pixels;

    for (int i = 0; i < imageInit->h; i++)
    {
        for (int j = 0; j < imageInit->w; j++)
        {
            Uint32 pixel = pixels[i * imageInit->w + j];
            rpixels[i * imageInit->w + j] = pixel;
            Uint32 rpixel = rpixels[i * imageInit->w + j];
            
            if (i + 1 < imageInit->h)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) >
                    (Uint8)(pixels[(i+1) * imageInit->w + j] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[(i+1) * imageInit->w + j];
                }
            }

            if (i - 1 >= 0)
            {
                if ((Uint8)(rpixel >> 16 & 0xFF) >
                    (Uint8)(pixels[(i-1) * imageInit->w + j] >> 16 & 0xFF))
                {
                    rpixels[i * imageInit->w + j] = 
                        pixels[(i-1) * imageInit->w + j];
                }
            }
        }
    }
}


void opening8(SDL_Surface *imageInit, SDL_Surface *imageResult)
{
    SDL_Surface *imageTemp = 
        SDL_CreateRGBSurface(0,imageInit->w,imageInit->h,32,0,0,0,0);

    erosion8(imageInit, imageTemp);

    dilatation8(imageTemp, imageResult);

    SDL_FreeSurface(imageTemp);
}

void opening4(SDL_Surface *imageInit, SDL_Surface *imageResult)
{
    SDL_Surface *imageTemp = 
        SDL_CreateRGBSurface(0,imageInit->w,imageInit->h,32,0,0,0,0);

    erosion4(imageInit, imageTemp);

    dilatation4(imageTemp, imageResult);

    SDL_FreeSurface(imageTemp);
}

void closure8(SDL_Surface *imageInit, SDL_Surface *imageResult)
{
    SDL_Surface *imageTemp = 
        SDL_CreateRGBSurface(0,imageInit->w,imageInit->h,32,0,0,0,0);

    dilatation8(imageInit, imageTemp);

    erosion8(imageTemp, imageResult);

    SDL_FreeSurface(imageTemp);
}

void closure4(SDL_Surface *imageInit, SDL_Surface *imageResult)
{
    SDL_Surface *imageTemp = 
        SDL_CreateRGBSurface(0,imageInit->w,imageInit->h,32,0,0,0,0);

    dilatation4(imageInit, imageTemp);

    erosion4(imageTemp, imageResult);

    SDL_FreeSurface(imageTemp);
}


//int main(int argc, char **argv)
//{
//    if (argc != 2)
//        errx(EXIT_FAILURE, "You need 2 arguments");
//
//    if(SDL_Init(SDL_INIT_VIDEO)!=0)
//        errx(EXIT_FAILURE, "%s", SDL_GetError());
//
//    SDL_Window* window = SDL_CreateWindow("Image To GrayScale", 0, 0, 0, 0,
//            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
//    if(window == NULL)
//        errx(EXIT_FAILURE, "%s", SDL_GetError());
//
//    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
//            SDL_RENDERER_ACCELERATED);
//    if (renderer == NULL)
//        errx(EXIT_FAILURE, "%s", SDL_GetError());
//
//    SDL_Surface* t = IMG_Load(argv[1]);
//    SDL_Surface* rt = IMG_Load(argv[1]);
//    if (t == NULL || rt == NULL)
//        errx(EXIT_FAILURE, "%s", SDL_GetError());
//    SDL_Surface *surface = SDL_ConvertSurfaceFormat(t, 
//            SDL_PIXELFORMAT_RGB888, 0);
//    SDL_Surface *rsurface = SDL_ConvertSurfaceFormat(rt, 
//            SDL_PIXELFORMAT_RGB888, 0);
//    SDL_FreeSurface(t);
//    SDL_FreeSurface(rt);
//    if (surface == NULL || rsurface == NULL)
//        errx(EXIT_FAILURE, "%s", SDL_GetError());
//
//    SDL_SetWindowSize(window,  surface->w, surface->h);
//    
//    dilatation2x(surface, rsurface);
//    dilatation2x(rsurface, surface);
//    dilatation2x(surface, rsurface);
//    dilatation2x(rsurface, surface);
//    erosion2y(surface, rsurface);
//    erosion2y(rsurface, surface);
//    erosion2y(surface, rsurface);
//    erosion2y(rsurface, surface);    
//    
//    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
//
//    SDL_RenderCopy(renderer,texture,NULL,NULL);
//    SDL_RenderPresent(renderer);
//
//    SDL_Event event;
//    while (1)
//    {
//        SDL_WaitEvent(&event);
//        switch (event.type)
//        {
//            case SDL_QUIT:
//                SDL_FreeSurface(surface);
//                SDL_FreeSurface(rsurface);
//                SDL_DestroyTexture(texture);
//                SDL_DestroyRenderer(renderer);
//                SDL_DestroyWindow(window);
//                SDL_Quit();
//                return EXIT_SUCCESS;
//
//            case SDL_WINDOWEVENT:
//                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
//                {
//                        SDL_RenderCopy(renderer,texture,NULL,NULL);
//                        SDL_RenderPresent(renderer);
//                }
//                break;
//        }
//    }
//    return EXIT_SUCCESS;
//}
