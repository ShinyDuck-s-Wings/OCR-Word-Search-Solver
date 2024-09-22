#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <err.h>
#include <string.h>

int main(int argc, char **argv)
{
    IMG_Init(IMG_INIT_PNG);

    SDL_Surface *image = IMG_Load(argv[1]);

    IMG_Quit();
}
