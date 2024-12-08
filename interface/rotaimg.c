#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



char* rotate_image(const char* image_path, double angle) {
 int longeur, largeur;

    
    SDL_Window* get = SDL_CreateWindow("sizeget", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_HIDDEN);
    SDL_Renderer* rend = SDL_CreateRenderer(get, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = IMG_LoadTexture(rend, image_path);
    SDL_QueryTexture(texture, NULL, NULL, &largeur, &longeur);

 
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(get);


    float rad = angle * M_PI / 180.0;

   
    int new_longeur = (int)(longeur * fabs(cos(rad)) + largeur * fabs(sin(rad)));
    int new_largeur = (int)(largeur * fabs(cos(rad)) + longeur * fabs(sin(rad)));

   
    SDL_Window* window = SDL_CreateWindow("Rotated Image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, new_largeur, new_longeur, SDL_WINDOW_HIDDEN);
    rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

 
    texture = IMG_LoadTexture(rend, image_path);

    
    SDL_Point centre = {largeur / 2, longeur / 2};

    //fezk,lkfùokezjiopetzjipt'zju'ztiot'p "trijto fuck 
    SDL_Rect dest_rect = { (new_largeur - largeur) / 2, (new_longeur - longeur) / 2, largeur, longeur };

    SDL_RenderClear(rend);
    SDL_RenderCopyEx(rend, texture, NULL, &dest_rect, angle, &centre, SDL_FLIP_NONE);
    SDL_RenderPresent(rend);

    char* sortie = "rotimage.bmp";
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, new_largeur, new_longeur, 32, SDL_PIXELFORMAT_RGBA32);
    
    SDL_RenderReadPixels(rend, NULL, SDL_PIXELFORMAT_RGBA32, surface->pixels, surface->pitch);
    
    SDL_SaveBMP(surface, sortie);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return sortie;
    }
