#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <dirent.h>
#include "../image_detection/morphoMaths.h"
#include "../image_detection/identify.h"
#include "../neural_network/neuralNetwork.h"
#include "../treatment/preprocess.c"

// gcc -Wall -Wextra main.c ../image_detection/identify.c ../image_detection/morphoMaths.c ../neural_network/neuralNetwork.c -o main -lm -I/usr/local/include/SDL2/ -lSDL2 -lSDL2_image

int numFromStringToInt(char *str)
{
    int res = 0;
    for (int i = 0; str[i] != 0; i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            res *= 10;
            res += str[i] - '0';
        }
    }
    return res;
}

int main(int argc, char** argv)
{
    if(argc != 2 && argc != 3)
        errx(EXIT_FAILURE,"Usage: <image> <show>");

    SDL_Surface *t = IMG_Load(argv[1]);

    if(t == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface *surface=SDL_ConvertSurfaceFormat(t,SDL_PIXELFORMAT_RGB888,0);
    SDL_FreeSurface(t);
    if(surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
   
    if ((numFromStringToInt(argv[1]) < 30 && numFromStringToInt(argv[1]) > 20))
        surface = preprocess(surface);

    IMG_SaveJPG(surface, "../imgf/treated.jpg", 100);

    int maxid = 0;
    int** black_grid = NULL;
    int width = (*surface).w;
    int height = (*surface).h;

    detect_connexe(surface,
            &black_grid,
            &maxid);

    int* counts = malloc(maxid*sizeof(int));

    clear_not_letters(&counts,
            height,
            width,
            &black_grid,
            maxid,
            2);

    struct letter* letters = box_letters(maxid,
            counts,
            black_grid,
            height,
            width);
    int numletterx = 0;
    int numlettery = 0;
    int gridposx1 = 0;
    int gridposx2 = 0;
    int gridposy1 = 0;
    int gridposy2 = 0;

    find_grid(letters,
            maxid,
            black_grid,
            &numletterx,
            &numlettery,
            &gridposx1,
            &gridposy1,
            &gridposx2,
            &gridposy2);

    free(counts);

    struct letter* gridletter = malloc(maxid * sizeof(struct letter));
    remove_grid_letters(&letters,
            &gridletter,
            gridposx1,
            gridposy1,
            gridposx2,
            gridposy2,
            maxid);

    split_grid(surface,gridletter,maxid);

    int **id_matrix = malloc(numlettery * sizeof(int *));
    for (int i = 0; i < numlettery; i++)
        id_matrix[i] = malloc(numletterx * sizeof(int));

    /*printf("%d, %d == %d, %d\n", gridposx1, gridposy1, 
            letters[2].xmin, letters[2].ymin);*/
    for (int i = 0; i < maxid; i++)
    {
        if(gridletter[i].xmin == gridposx1 && gridletter[i].ymin == gridposy1)
            id_matrix[0][0] = i;
    }

    //printf("[\n");
    for (int i = 0; i < numlettery; i++)
    {
        for (int j = 0; j < numletterx; j++)
        {
            if (i == 0 && j == 0)
            {
                //printf("%d ", id_matrix[i][j]);
                continue;
            }
            if (j == 0)
            {
                for (int k = 0; k < maxid; k++)
                {
                    if(gridletter[k].xmid<gridletter[id_matrix[i-1][j+1]].xmin
                      && gridletter[k].ymid>gridletter[id_matrix[i-1][j]].ymax)
                    {
                        id_matrix[i][j] = k;
                        break;
                    }
                }
            } 
            else if (i == 0)
            {
                for (int k = 0; k < maxid; k++)
                {
                    if(gridletter[k].xmid>gridletter[id_matrix[i][j-1]].xmax &&
                       gridletter[k].ymid<gridletter[id_matrix[i][j-1]].ymax &&
                       gridletter[k].xgrid == j && gridletter[k].ygrid == i)
                    {
                        id_matrix[i][j] = k;
                        break;
                    }
                }
            }
            else if (j == numletterx - 1)
            {
                for (int k = 0; k < maxid; k++)
                {
                    if(gridletter[k].xmid>gridletter[id_matrix[i-1][j-1]].xmax
                      && gridletter[k].ymid>gridletter[id_matrix[i-1][j]].ymax)
                    {
                        id_matrix[i][j] = k;
                        break;
                    }
                }
            }
            else
            {
                for (int k = 0; k < maxid; k++)
                {
                    if(gridletter[k].xmid>gridletter[id_matrix[i-1][j-1]].xmax
                     && gridletter[k].ymid>gridletter[id_matrix[i-1][j]].ymax
                     && gridletter[k].xmid<gridletter[id_matrix[i-1][j+1]].xmin)
                    {
                        id_matrix[i][j] = k;
                        break;
                    }
                }
            }
            // printf("(%i,%i) ", gridletter[id_matrix[i][j]].xgrid, gridletter[id_matrix[i][j]].ygrid);
            //printf("%d ", id_matrix[i][j]);
        }
        //printf("\n");
    } 
    //printf("]\n");

    char **c_matrix = malloc(numlettery * sizeof(char *));
    for (int i = 0; i < numlettery; i++)
        c_matrix[i] = malloc(numletterx * sizeof(char));

    DIR *d;
    struct dirent *dir;
    char *named = "../letters/";
    d = opendir(named);
    if (d)
    {
        while((dir = readdir(d)) != NULL)
        { 
            if (dir->d_name[0] != '.')
            {
                char *file = malloc(50*sizeof(char));
                asprintf(&file, "%s%s", named, dir->d_name);
                int idk = numFromStringToInt(dir->d_name);
                //printf("%d == %s\n", idk, file);
                for (int i = 0; i < numlettery; i++)
                {
                    for (int j = 0; j < numletterx; j++)
                    {
                        if (id_matrix[i][j] == idk)
                        {
                            c_matrix[i][j] = use(file, 0);
                        }
                    }
                }
                free(file);
            }
        }
    }

    printf("[\n");
    for (int i = 0; i < numlettery; i++)
    {
        for (int j = 0; j < numletterx; j++)
        {
            printf("%c ", c_matrix[i][j]);
        }
        printf("\n");
    }
    printf("]\n");

    erase_grid(&black_grid,
            surface,
            gridposx1,
            gridposy1,
            gridposx2,
            gridposy2);
    surface_remove_white(surface,black_grid);

    detect_connexe(surface, &black_grid,&maxid);
    int *c = malloc(maxid*sizeof(int));
    for (int i = 0; i < maxid; i++)
        c[i] = 0;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (black_grid[j][i])
                c[black_grid[j][i]]++;

    struct letter* ltr = box_letters(maxid,c,black_grid,height,width);
    struct word *words;
    size_t nbWords;
    SDL_Surface *bsurface = find_list(surface, ltr, black_grid, 
            maxid, &words, &nbWords);
    free(c);

    printf("nb Words == %ld\n", nbWords);

    char **c_words = malloc(nbWords * sizeof(char *));
    for (size_t i = 0; i < nbWords; i++)
    {
        c_words[i] = malloc((words[i].nbletters + 1) * sizeof(char));
        c_words[i][words[i].nbletters] = '\0';
    }

    DIR *dw;
    struct dirent *dirw;
    char *namedw = "../words/";
    dw = opendir(namedw);
    if (dw)
    {
        while((dirw = readdir(dw)) != NULL)
        { 
            if (dirw->d_name[0] != '.')
            {
                char *file = malloc(50*sizeof(char));
                asprintf(&file, "%s%s", namedw, dirw->d_name);
                int idk = numFromStringToInt(dirw->d_name);
                //printf("%d == %s\n", idk, file);
                for (size_t i = 0; i < nbWords; i++)
                {
                    for (int j = 0; j < words[i].nbletters; j++)
                    {
                        if (words[i].letters[j] == idk)
                            c_words[i][j] = use(file, 0);
                    }
                }
                free(file);
            }
        }
    }

    printf("[\n");
    for (size_t i = 0; i < nbWords; i++)
    {
        for (int j = 0; j < words[i].nbletters; j++)
        {
            printf("%c ", c_words[i][j]);
        }
        printf("\n");
    }
    printf("]\n");

    char **args = malloc((9 + (numletterx*numlettery) + nbWords) * sizeof(char*));
    args[0] = "../solver/solver"; 
    args[1] = "../imgf/treated.jpg";
    args[2] = malloc(15 * sizeof(char));
    asprintf(args + 2, "%d", gridposx1);
    args[3] = malloc(15 * sizeof(char));
    asprintf(args + 3, "%d", gridposy1);
    args[4] = malloc(15 * sizeof(char));
    asprintf(args + 4, "%d", gridposx2);
    args[5] = malloc(15 * sizeof(char));
    asprintf(args + 5, "%d", gridposy2);
    args[6] = malloc(15 * sizeof(char));
    asprintf(args + 6, "%d", numletterx);
    args[7] = malloc(15 * sizeof(char));
    asprintf(args + 7, "%d", numlettery);
    int k = 8;
    for (int i = 0; i < numlettery; i++)
    {
        for (int j = 0; j < numletterx; j++)
        {
            args[k] = malloc(2 * sizeof(char));
            //asprintf(args + k, "%s", c_grid_request[k-8]);
            //args[k][0] = c_grid_request[k-8][0];
            args[k][0] = c_matrix[i][j];
            args[k][1] = 0;
            //printf("%c == %s\n", c_matrix[i][j], args[k]);
            k++;
        }
    }
    for (size_t i = 0; i < nbWords; i++)
    {
        args[k] = malloc((words[i].nbletters + 1) * sizeof(char));
        asprintf(args + k, "%s", c_words[i]);
        //printf("%s\n", args[k - (numlettery*numletterx)]);
        k++;
    }
    args[8 + numletterx*numlettery + nbWords] = NULL;

    //printf("%s\n", args[7]);

    for (size_t i = 0; i < 8 + (numletterx*numlettery) + nbWords; i++)
        printf("%s ", args[i]);
    printf("\n");

    if (execvp(args[0], args) == -1)
        errx(EXIT_FAILURE, "error execvp(cross)");

    for (int i = 0; i < numlettery; i++)
        free(id_matrix[i]);
    free(id_matrix);

    for (int i = 0; i < numlettery; i++)
        free(c_matrix[i]);
    free(c_matrix);

    for (size_t i = 0; i < nbWords; i++)
        free(c_words[i]);
    free(c_words);

    for (size_t i = 2; i < 9 + numletterx*numlettery + nbWords; i++)
        free(args[i]);
    free(args);

    free(gridletter);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(bsurface);

    return EXIT_SUCCESS;
}
