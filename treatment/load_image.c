#include <stdio.h>
#include <SDL2/SDL.h>
#include "preprocess.c"
//#include <SDL2/SDL_image.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("argc != 2\n");
        exit(EXIT_FAILURE);
    }

    if(SDL_Init(SDL_INIT_VIDEO)!=0)
    {
        printf("SLD_init problem\n");
        exit(EXIT_FAILURE);
    }

    SDL_Window* window = SDL_CreateWindow("grille", 0, 0, SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(window == NULL)
    {
        printf("SDL_createwindow problem: %s", SDL_GetError);
        exit(EXIT_FAILURE);
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("SDL_CreateRenderer problem: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Surface* t = SDL_LoadBMP(argv[1]);
    if (t == NULL)
    {
        printf("SDL_LoadBMP: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(t, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(t);
    if (surface == NULL)
    {
        printf("SDL_ConvertSurfaceFormat: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_SetWindowSize(window,  surface->w, surface->h);

    surface = preprocess(surface);

    IMG_SavePNG(surface, "../imgf/treated.jpg");
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    
    SDL_Event event;
    while (1)
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
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                        SDL_RenderCopy(renderer,texture,NULL,NULL);
                        SDL_RenderPresent(renderer);
                }
                break;
        }
    }
    return EXIT_SUCCESS;
}
