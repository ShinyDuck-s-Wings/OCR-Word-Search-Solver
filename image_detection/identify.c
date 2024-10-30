#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

struct black
{
    int* x;
    int* y;
};

int is_black(Uint32 pixel, SDL_PixelFormat *format)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(pixel,format,&r,&g,&b);
    if(r < 50 && g < 50 && b < 50)
    {
        return 1;
    }
    return 0;
}

void expand(SDL_Surface *surface, int x, int y, struct black* found)
{
    Uint32 *pix = (*surface).pixels;
    SDL_PixelFormat *format = (*surface).format;
    Uint32 curr = pix[y * (*surface).h + x];
    if(is_black(curr, format))
    {
        int count = 0;
        int* currx = (*found)->x;
        int* curry = (*found)->y;
        int already = 0;
        while(*(currx + count) != NULL)
        {
            if(x == *(currx + count) && y == *(curry + count))
            {
                already = 1;
                break;
            }
            count++;
        }
        if(already == 0)
        {
            (*found)->x = realloc((*found)->x,(count+1)*sizeof(int));
            (*found)->y = realloc((*found)->y,(count+1)*sizeof(int));
            ((*found)->x)[count] = NULL;
            ((*found)->x)[count-1] = x;
            ((*found)->y)[count] = NULL;
            ((*found)->y)[count-1] = y;
            if(x != 0)
                expand(surface,x-1,y,found);
            if(x != surface->w -1)
                expand(surface,x+1,y,found);
            if(y != 0)
                expand(surface,x,y-1,found);
            if(y != surface->h -1)
                expand(surface,x,y+1,found);
            return;
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }
}

void find_mounds(SDL_Surface *surface, struct black* mounds)
{
    Uint32 *pix = (*surface).pixels;
    SDL_LockSurface(surface);
    SDL_PixelFormat *format = (*surface).format;
    int horizontal = (*surface).w;
    int vertical = (*surface).h;
    mounds = NULL;
    for(int y = 0; y < vertical; y++)
    {
        for(int x = 0; x < horizontal; x++)
        {
            int currind = y * vertical + x;
            expand(surface,x,y,mounds);
    
        }
    }
    return;
}

int main(int argc, char** argv)
{
    if(argc != 2)
        errx(EXIT_FAILURE,"Usage: <image>")

    SDL_Surface *t = IMG_Load(argv[1]);
    if(t == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(t,SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(t);
    if(surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
}
