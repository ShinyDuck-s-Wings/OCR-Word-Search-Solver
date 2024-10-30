#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

int xi = 0;
int xf = 0;
int yi = 0;
int yf = 0;
int size = 12;
int height = 14;

SDL_Window *create_window(char *title, unsigned int width, unsigned int height)
{
  if(SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    return NULL;
  }
  return SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED
          ,width,height,SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
}

SDL_Renderer *create_renderer(SDL_Window *window)
{
  return SDL_CreateRenderer(window,-1,0);
}

void close_program(SDL_Window *window, SDL_Renderer *renderer)
{
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

void draw_line(SDL_Surface *surface, int x1, int y1, int x2, int y2)
{
    Uint32 *pixels = (*surface).pixels;
    SDL_LockSurface(surface);
    SDL_PixelFormat *format = (*surface).format;
    int dx = x2 - x1;
    int dy = y2 - y1;
    while(dy != 0 || dx != 0)
    {
        *(pixels + x1 + dx + (y1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 + 1 + dx + (y1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 - 1 + dx + (y1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 + dx + (y1 + 1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 + dx + (y1 - 1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 + 1 + dx + (y1 + 1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 + 1 + dx + (y1 - 1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 - 1 + dx + (y1 + 1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 - 1 + dx + (y1 - 1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        if(dy > 0)
            dy--;
        else if(dy < 0)
            dy++;
        if(dx > 0)
            dx--;
        else if(dx < 0)
            dx++;
    }

    SDL_UnlockSurface(surface);
}

void findword(size_t horizontal, size_t vertical, char grid[vertical][horizontal], char word[], int* xi, int* yi, int* xf, int* yf)
{
    size_t len = 0;
    while(word[len] != 0)
    {
        len++;
    }
    for(size_t i = 0; i < vertical; i++)
    {
        for(size_t j = 0; j < horizontal; j++)
        {
            if(grid[i][j] == word[0])
            {
                size_t count = 1;
                if(i >= len - 1)
                {
                    while(grid[i-count][j] == word[count])
                    {
                        if(count == len - 1)
                        {
                            *xi = j;
                            *yi = i;
                            *xf = j;
                            *yf = (i - count);
                            return;
                        }
                        count++;
                    }
                    count = 1;
                    if(j >= len - 1)
                    {
                        while(grid[i-count][j-count] == word[count])
                        {
                            if(count == len - 1)
                            {
                                *xi = j;
                                *yi = i;
                                *xf = (j - count);
                                *yf = (i - count);
                                return;
                            }
                            count++;
                        }
                    }
                    count = 1;
                    if(j <= horizontal - len)
                    {
                        while(grid[i-count][j+count] == word[count])
                        {
                            if(count == len - 1)
                            {
                                *xi = j;
                                *yi = i;
                                *xf = (j + count);
                                *yf = (i - count);
                                return;
                            }
                            count++;
                        }
                    }
                }
                if(i <= vertical - len)
                {
                    count = 1;
                    while(grid[i+count][j] == word[count])
                    {
                        if(count == len - 1)
                        {
                            *xi = j;
                            *yi = i;
                            *xf = j;
                            *yf = (i + count);
                            return;
                        }
                        count++;
                    }
                    count = 1;
                    if(j >= len - 1)
                    {
                        while(grid[i+count][j-count] == word[count])
                        {
                            if(count == len - 1)
                            {
                                *xi = j;
                                *yi = i;
                                *xf = (j - count);
                                *yf = (i + count);
                                return;
                            }
                            count++;
                        }
                    }
                    count = 1;
                    if(j <= horizontal - len)
                    {
                        while(grid[i+count][j+count] == word[count])
                        {
                            if(count == len - 1)
                            {
                                *xi = j;
                                *yi = i;
                                *xf = (j + count);
                                *yf = (i + count);
                                return;
                            }
                            count++;
                        }
                    }
                }
                count = 1;
                if(j >= len - 1)
                {
                    while(grid[i][j-count] == word[count])
                    {
                        if(count == len - 1)
                        {
                            *xi = j;
                            *yi = i;
                            *xf = (j - count);
                            *yf = i;
                            return;
                        }
                        count++;
                    }
                }
                count = 1;
                if(j <= horizontal - len)
                {
                    while(grid[i][j+count] == word[count])
                    {
                        if(count == len - 1)
                        {
                            *xi = j;
                            *yi = i;
                            *xf = (j + count);
                            *yf = i;
                            return;
                        }
                        count++;
                    }
                }
            }
        }
    }
    *xi = 0;
    *xf = 0;
    *yi = 0;
    *yf = 0;
}

int main(int argc, char** argv)
{
    if(argc < 3)
        errx(EXIT_FAILURE,"Usage: <image> <word1> <word2> ...\n");
       

    char grid[][12] = {
        {'M','S','W','A','T','E','R','M','E','L','O','N'},
        {'Y','T','B','N','E','P','E','W','R','M','A','E'},
        {'R','R','L','W','P','A','P','A','Y','A','N','A'},
        {'R','A','N','L','E','M','O','N','A','N','E','P'},
        {'E','W','L','E','A','P','R','I','A','B','P','R'},
        {'B','B','I','L','B','B','W','B','R','L','A','Y'},
        {'K','E','M','P','M','A','W','L','R','A','R','B'},
        {'C','R','E','P','R','N','R','E','R','R','G','R'},
        {'A','R','Y','A','Y','A','O','A','N','L','A','M'},
        {'L','Y','Y','A','R','N','E','R','K','I','W','I'},
        {'B','E','B','A','A','A','N','A','A','P','R','T'},
        {'Y','R','R','E','B','P','S','A','R','N','N','W'},
        {'Y','R','R','E','B','E','U','L','B','L','G','I'},
        {'T','Y','P','A','T','E','A','E','P','A','C','E'}
        };
    
    int wordnum = argc - 2;
    char **words = malloc(sizeof(char*) * wordnum);
    for(int j = 0; j < wordnum; j++)
    {
        int len = 0;
        for(int i = 0; argv[2 + j][i] != 0; i++)
        {
            len++;
        }
        words[j] = (char*) malloc(sizeof(char) * (len + 1));
        int count = 0;
        while(argv[2 + j][count] != 0)
        {
            words[j][count] = argv[2 + j][count];
            count++;
        }
        words[j][len] = 0;
    }
    
    //findword(size, height, grid, word, &xi, &yi, &xf, &yf);
    //printf("Mot %s en (%i,%i):(%i,%i)\n",word,yi,xi,yf,xf);
    
    SDL_Window *window = create_window("Solved !", 800, 500);

    if(!window)
        errx(EXIT_FAILURE, "Failed to create SDL window: %s", SDL_GetError());

    SDL_Renderer *renderer = create_renderer(window);

    if(!renderer)
        errx(EXIT_FAILURE, "Failed to create SDL renderer: %s", SDL_GetError());

    SDL_Surface *t = IMG_Load(argv[1]);
    if(t == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(t,SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(t);
    if(surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_SetWindowSize(window, surface->w, surface->h);
    
    for(int i = 0; i < wordnum; i++)
    {
        findword(size, height, grid, words[i], &xi, &yi, &xf, &yf);
        int xfirst = 9 + (xi*52) + 26;
        int yfirst = 13 + (yi*48) + 24;
        int xlast = 9 + (xf*52) + 26;
        int ylast = 13 + (yf*48) + 24;
        if((xi == 0) && (xf == 0) && (yi == 0) && (yf == 0))
        {
            continue;
        }
        draw_line(surface,xfirst,yfirst,xlast,ylast);
    }

    //draw_line(surface,xfirst,yfirst,xlast,ylast);
    //start in 9,13 / 632,684
    //height is 623,671
    //case is 52,48

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);
    SDL_Event event;
    while(1)
    {       
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return EXIT_SUCCESS;
            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    SDL_RenderCopy(renderer,texture,NULL,NULL);
                    SDL_RenderPresent(renderer);
                }
                break;
        }
    }
    return EXIT_SUCCESS;
}
