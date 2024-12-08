#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <err.h>
#include <dirent.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sys/stat.h>
#include "neuralNetwork.h"

int main(int argc, char **argv)
{
    if (argc < 2)
        errx(EXIT_FAILURE,"You need 2 arguments");
    if (strcmp(argv[1], "train") == 0)
    {
        DIR *d;
        struct dirent *dir;
        char *named = "../neural_network/learning_data_base/";
        d = opendir(named);
        int k = 0;
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
                if (dir->d_name[0] != '.')
                    k++;
            closedir(d);
        }
        printf("%d\n",k);
        trainLetter(k);
    }
    else if (strcmp(argv[1], "use") == 0)
    {
        if (argc != 3)
            errx(EXIT_FAILURE,"You need 3 arguments");
        DIR *d;
        struct dirent *dir;
        char *named = "../neural_network/learning_data_base/";
        d = opendir(named);
        int k = 0;
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
                if (dir->d_name[0] != '.')
                    k++;
            closedir(d);
        }
        //printf("%d\n",k);

        printf("%c\n", use(argv[2],k));
    }
    else
        errx(EXIT_FAILURE, "Unknown arguments");

    return EXIT_SUCCESS;
}
