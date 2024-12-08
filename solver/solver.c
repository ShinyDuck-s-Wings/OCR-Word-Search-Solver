#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

/*int gridxi = 23;
int gridyi = 24;
int gridxf = 617;
int gridyf = 670;
int size = 12;
int height = 14;*/

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

void findword(size_t horizontal, size_t vertical, char** grid, char* word, int* xi, int* yi, int* xf, int* yf)
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

int id_color(Uint32 pixel_color, SDL_PixelFormat *format)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(pixel_color,format,&r,&g,&b);
    if(r == 255 && g == 0 && b == 0)
    {
        return 1;
    }
    if(r < 50 && g < 50 && b < 50)
    {
        return 2;
    }
    if(r > 200 && g > 200 && b > 200)
    {
        return 3;
    }
    return 0;
}

void DrawCircle(SDL_Surface* surface, int centreX, int centreY, int radius)
{
    SDL_PixelFormat* format  = surface->format;
    Uint32* pixels = surface->pixels;
    int diameter = (radius * 2);
    int x = (radius - 1);
    int y = 0;
    int tx = 1;
    int ty = 1;
    int error = (tx - diameter);
    while (x >= y)
    {
        // Each of the following renders an octant of the circle
        if(id_color(*(pixels + centreX + x + (centreY - y)*surface->w),format) == 3)
        {
            *(pixels + centreX + x + (centreY - y)*surface->w) = SDL_MapRGB(format,255,0,0);
        }
        //else if(id_color(*(pixels + centreX + x + (centreY - y)*surface->w),format) == 1)
        //{
        //    *(pixels + centreX + x + (centreY - y)*surface->w) = SDL_MapRGB(format,255,255,255);
        //}

        if(id_color(*(pixels + centreX + x + (centreY + y)*surface->w),format) == 3)
        {
           *(pixels + centreX + x + (centreY + y)*surface->w) = SDL_MapRGB(format,255,0,0);
        }
        //else if(id_color(*(pixels + centreX + x + (centreY + y)*surface->w),format) == 1)
        //{
        //    *(pixels + centreX + x + (centreY + y)*surface->w) = SDL_MapRGB(format,255,255,255);
        //}

        if(id_color(*(pixels + centreX - x + (centreY - y)*surface->w),format) == 3)
        {
            *(pixels + centreX - x + (centreY - y)*surface->w) = SDL_MapRGB(format,255,0,0);
        }
        //else if(id_color(*(pixels + centreX - x + (centreY - y)*surface->w),format) == 1)
        //{
        //    *(pixels + centreX - x + (centreY - y)*surface->w) = SDL_MapRGB(format,255,255,255);
        //}

        if(id_color(*(pixels + centreX - x + (centreY + y)*surface->w),format) == 3)
        {
            *(pixels + centreX - x + (centreY + y)*surface->w) = SDL_MapRGB(format,255,0,0);
        }
        //else if(id_color(*(pixels + centreX - x + (centreY + y)*surface->w),format) == 1)
        //{
        //    *(pixels + centreX - x + (centreY + y)*surface->w) = SDL_MapRGB(format,255,255,255);
        //}

        if(id_color(*(pixels + centreX + y + (centreY - x)*surface->w),format) == 3)
        {
            *(pixels + centreX + y + (centreY - x)*surface->w) = SDL_MapRGB(format,255,0,0);
        }
        //else if(id_color(*(pixels + centreX + y + (centreY - x)*surface->w),format) == 1)
        //{
        //    *(pixels + centreX + y + (centreY - x)*surface->w) = SDL_MapRGB(format,255,255,255);
        //}

        if(id_color(*(pixels + centreX + y + (centreY + x)*surface->w),format) == 3)
        {
            *(pixels + centreX + y + (centreY + x)*surface->w) = SDL_MapRGB(format,255,0,0);
        }
        //else if(id_color(*(pixels + centreX + y + (centreY + x)*surface->w),format) == 1)
        //{
        //    *(pixels + centreX + y + (centreY + x)*surface->w) = SDL_MapRGB(format,255,255,255);
        //}

        if(id_color(*(pixels + centreX - y + (centreY - x)*surface->w),format) == 3)
        {
            *(pixels + centreX - y + (centreY - x)*surface->w) = SDL_MapRGB(format,255,0,0);
        }
        //else if(id_color(*(pixels + centreX - y + (centreY - x)*surface->w),format) == 1)
        //{
        //    *(pixels + centreX - y + (centreY - x)*surface->w) = SDL_MapRGB(format,255,255,255);
        //}

        if(id_color(*(pixels + centreX - y + (centreY + x)*surface->w),format) == 3)
        {
            *(pixels + centreX - y + (centreY + x)*surface->w) = SDL_MapRGB(format,255,0,0);
        }
        //else if(id_color(*(pixels + centreX - y + (centreY + x)*surface->w),format) == 1)
        //{
        //    *(pixels + centreX - y + (centreY + x)*surface->w) = SDL_MapRGB(format,255,255,255);
        //}
        if (error <= 0)
        {
  	        ++y;
  	        error += ty;
  	        ty += 2;
        }
        if (error > 0)
        {
  	        --x;
  	        tx += 2;
  	        error += (tx - diameter);
        }
    }
       
}

void circle(SDL_Surface *surface, int x1, int y1, int x2, int y2,int radius)
{
    Uint32 *pixels = (*surface).pixels;
    SDL_LockSurface(surface);
    SDL_PixelFormat *format = (*surface).format;
    int dx = x2 - x1;
    int dy = y2 - y1;
    while(dy != 0 || dx != 0)
    {
        //DrawCircle(surface, x1 + dx, y1 + dy, radius);
        /**(pixels + x1 + dx + (y1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 + 1 + dx + (y1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 - 1 + dx + (y1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 + dx + (y1 + 1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 + dx + (y1 - 1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 + 1 + dx + (y1 + 1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 + 1 + dx + (y1 - 1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 - 1 + dx + (y1 + 1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);
        *(pixels + x1 - 1 + dx + (y1 - 1 + dy)*surface->w) = SDL_MapRGB(format,255,0,0);*/
        if(dy > 0)
        {
            DrawCircle(surface,x1+dx,y1+dy,radius);
            dy--;
        }
        else if(dy < 0)
        {
            DrawCircle(surface,x1+dx,y1+dy,radius);
            dy++;
        }
        if(dx > 0)
        {
            DrawCircle(surface,x1+dx,y1+dy,radius);
            dx--;
        }
        else if(dx < 0)
        {
            DrawCircle(surface,x1+dx,y1+dy,radius);
            dx++;
            
        }
    }
    
    SDL_UnlockSurface(surface);
}

int main(int argc, char** argv)
{
    if(argc < 13)
        errx(EXIT_FAILURE,"Usage: <image> <gridxi> <gridyi> <gridxf> <gridyf> <numx> <numy> <let1> <let2> .... <letx*y> <word1> <word2> ... <wordn>\n");
    int gridxi = atoi(argv[2]);
    int gridyi = atoi(argv[3]);
    int gridxf = atoi(argv[4]);
    int gridyf = atoi(argv[5]);
    int size = atoi(argv[6]);
    int height = atoi(argv[7]);
    /*char grid[][12] = {
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
        };*/
    char** grid = malloc(sizeof(char*) * height);
    for(int y = 0; y < height; y++)
    {
        grid[y] = malloc(sizeof(char) * size);
    }
    for(int i = 1; i <= height*size; i++)
    {
        grid[(i-1)/size][(i-1)%size] = argv[7 + i][0];
    }
    int rest = 8 + height*size;
    int wordnum = argc - (rest);
    char **words = malloc(sizeof(char*) * wordnum);
    for(int j = 0; j < wordnum; j++)
    {
        int len = 0;
        for(int i = 0; argv[rest + j][i] != 0; i++)
        {
            len++;
        }
        words[j] = (char*) malloc(sizeof(char) * (len + 1));
        int count = 0;
        while(argv[rest + j][count] != 0)
        {
            words[j][count] = argv[rest + j][count];
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
    int xi;
    int yi;
    int xf;
    int yf;
    int errx = ((gridxf - gridxi) / size) / 4;
    int erry = ((gridyf - gridyi) / size) / 4;
    gridxf += errx + 1;
    gridxi -= errx;
    gridyf += erry + 1;
    gridyi -= erry;
    int sizeboxx = (gridxf - gridxi) / size;
    int sizeboxy = (gridyf - gridyi) / height;
    int sizeboxmid = (sizeboxx + sizeboxy) / 2;
    for(int i = 0; i < wordnum; i++)
    {
        findword(size, height, grid, words[i], &xi, &yi, &xf, &yf);
        int xfirst = gridxi + (xi*sizeboxx) + sizeboxx/2;
        int yfirst = gridyi + (yi*sizeboxy) + sizeboxy/2;
        int xlast = gridxi + (xf*sizeboxx) + sizeboxx/2;
        int ylast = gridyi + (yf*sizeboxy) + sizeboxy/2;
        if((xi == 0) && (xf == 0) && (yi == 0) && (yf == 0))
        {
            continue;
        }
        circle(surface, xfirst, yfirst, xlast, ylast, sizeboxmid/2);
    }
    for(int i = 0; i < wordnum; i++)
    {
        free(words[i]);
    }
    free(words);
    for(int i = 0; i < height; i++)
    {
        free(grid[i]);
    }
    free(grid);
    //draw_line(surface,xfirst,yfirst,xlast,ylast);
    //start in 9,13 / 632,684
    //height is 623,671
    //case is 52,48
    
    SDL_SaveBMP(surface, "../imgf/imgres.bmp");

    /*SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
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
    }*/

    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    return EXIT_SUCCESS;
}
