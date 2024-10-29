#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <err.h>
#include <dirent.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define nbInputs 784
#define nbHiddenNodes1 128
#define nbHiddenNodes2 128
#define nbOutputs 26

void initiateTraining(double **trainInputs, double **trainOutputs,
                      char *possible_outputs, int nbTrainingInputs)
{
    DIR *d;
    struct dirent *dir;
    char * named = "learning_data_base/";
    d = opendir(named);
    int k = 0;
    if(d)
    {
        while((dir = readdir(d)) != NULL && k < nbTrainingInputs)
        {
            if (dir->d_name[0] != '.')
            {
                char *path = 
                    malloc((strlen(named) + strlen(dir->d_name)+1)*sizeof(char));

                path[0] = 0;
                path = strcat(path, named);
                path = strcat(path, dir->d_name);
                
                // SDL_Surface *surface = IMG_Load(path);

                SDL_Surface *surface = SDL_ConvertSurfaceFormat(IMG_Load(path),
                        SDL_PIXELFORMAT_RGB888, 0);

                Uint32 *pixels = (Uint32 *)surface->pixels;

                for(int i = 0; i < surface->h; i++)
                {
                    for(int j = 0; j < surface->w; j++)
                    {
                        Uint32 pixel = pixels[i * surface->w + j];
                        Uint8 r = pixel >> 16 & 0xFF;
                        Uint8 g = pixel >> 8 & 0xFF;
                        Uint8 b = pixel & 0xFF;

                        trainInputs[k][i * surface->w + j] = 
                            0.212671f * r + 0.715160f * g + 0.072169 * b;
                        // printf("%g", trainInputs[k][i * surface->w + j]);
                        if (trainInputs[k][i * surface->w + j] > 190)
                            trainInputs[k][i * surface->w + j] = 0.0;
                        else
                            trainInputs[k][i * surface->w + j] = 1.0;
                    }
                }
                SDL_FreeSurface(surface);
                free(path);
                for (int i = 0; i < nbOutputs; i++)
                    trainOutputs[k][i] = 0.0;
                switch (dir->d_name[0])
                {
                    case 'a': trainOutputs[k][0] = 1.0f;
                              break;
                    case 'b': trainOutputs[k][1] = 1.0f;
                              break;
                    case 'c': trainOutputs[k][2] = 1.0f;
                              break;
                    case 'd': trainOutputs[k][3] = 1.0f;
                              break;
                    case 'e': trainOutputs[k][4] = 1.0f;
                              break;
                    case 'f': trainOutputs[k][5] = 1.0f;
                              break;
                    case 'g': trainOutputs[k][6] = 1.0f;
                              break;
                    case 'h': trainOutputs[k][7] = 1.0f;
                              break;
                    case 'i': trainOutputs[k][8] = 1.0f;
                              break;
                    case 'j': trainOutputs[k][9] = 1.0f;
                              break;
                    case 'k': trainOutputs[k][10] = 1.0f;
                              break;
                    case 'l': trainOutputs[k][11] = 1.0f;
                              break;
                    case 'm': trainOutputs[k][12] = 1.0f;
                              break;
                    case 'n': trainOutputs[k][13] = 1.0f;
                              break;
                    case 'o': trainOutputs[k][14] = 1.0f;
                              break;
                    case 'p': trainOutputs[k][15] = 1.0f;
                              break;
                    case 'q': trainOutputs[k][16] = 1.0f;
                              break;
                    case 'r': trainOutputs[k][17] = 1.0f;
                              break;
                    case 's': trainOutputs[k][18] = 1.0f;
                              break;
                    case 't': trainOutputs[k][19] = 1.0f;
                              break;
                    case 'u': trainOutputs[k][20] = 1.0f;
                              break;
                    case 'v': trainOutputs[k][21] = 1.0f;
                              break;
                    case 'w': trainOutputs[k][22] = 1.0f;
                              break;
                    case 'x': trainOutputs[k][23] = 1.0f;
                              break;
                    case 'y': trainOutputs[k][24] = 1.0f;
                              break;
                    case 'z': trainOutputs[k][25] = 1.0f;
                              break;
                }
                possible_outputs[k] = dir->d_name[0];
                k++;
            }
        }
        closedir(d);
    }
}

double sigmoid(double x)
{
    return 1.0f / (1.0f + (double)(exp(-x)));
}

double dSigmoid(double x)
{
    return sigmoid(x) * (1.0f - sigmoid(x));
}

double init_weights()
{
    double res = ((double)rand()) / ((double)RAND_MAX);
    return res;
}

void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n-1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

void trainLetter(int nbTrainingInputs)
{
    double lr = 0.1f;

    double weightsHiddenNodes1[nbHiddenNodes1][nbInputs];
    double weightsHiddenNodes2[nbHiddenNodes2][nbHiddenNodes1];
    double weightsOutputs[nbOutputs][nbHiddenNodes2];

    double biasHiddenNodes1[nbHiddenNodes1];
    double biasHiddenNodes2[nbHiddenNodes2];
    double biasOutputs[nbOutputs];

    double layerHiddenNodes1[nbHiddenNodes1];
    double layerHiddenNodes2[nbHiddenNodes2];
    double layerOutputs[nbOutputs];


    double **trainInputs = malloc(nbTrainingInputs * sizeof(double*));
    for (int i = 0; i < nbTrainingInputs; i++)
        trainInputs[i] = malloc(nbInputs * sizeof(double));

    double **trainOutputs = malloc(nbTrainingInputs * sizeof(double*));
    for (int i = 0; i < nbTrainingInputs;i++)
        trainOutputs[i] = malloc(nbOutputs * sizeof(double));

    char *possible_outputs = malloc(nbTrainingInputs * sizeof(char));

    initiateTraining(trainInputs, trainOutputs, possible_outputs, nbTrainingInputs);

    char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g',
                               'h', 'i', 'j', 'k', 'l', 'm', 'n',
                               'o', 'p', 'q', 'r', 's', 't', 'u',
                               'v', 'w', 'x', 'y', 'z'};
    
    // printf("\nBalise after Init Training\n");

    for (int i = 0; i < nbHiddenNodes1; i++)
        for (int j = 0; j < nbInputs; j++)
            weightsHiddenNodes1[i][j] = init_weights();

    for (int i = 0; i < nbHiddenNodes2; i++)
        for (int j = 0; j < nbHiddenNodes1; j++)
            weightsHiddenNodes2[i][j] = init_weights();

    for (int i = 0; i < nbOutputs; i++)
        for (int j = 0; j < nbHiddenNodes2; j++)
            weightsOutputs[i][j] = init_weights();

    for (int i = 0; i < nbHiddenNodes1; i++)
        biasHiddenNodes1[i] = init_weights();

    for (int i = 0; i < nbHiddenNodes2; i++)
        biasHiddenNodes2[i] = init_weights();

    for (int i = 0; i < nbOutputs; i++)
        biasOutputs[i] = init_weights();

    // printf("\nBalise before Training\n");

    // T R A I N I N G

    int trainSetOrder[nbTrainingInputs];
    for (int i = 0; i < nbTrainingInputs; i++)
        trainSetOrder[i] = i;

    int nbEpochs = 9984;

    for (int epoch = 0; epoch < nbEpochs; epoch++)
    {
        shuffle(trainSetOrder, nbTrainingInputs);

        for (int x = 0; x < nbTrainingInputs; x++)
        {
            int i = trainSetOrder[x];

            // Forward Propagation

            for (int j = 0; j < nbHiddenNodes1; j++)
            {
                double activation = biasHiddenNodes1[j];

                for (int k = 0; k < nbInputs; k++)
                    activation += trainInputs[i][k] * weightsHiddenNodes1[j][k];

                layerHiddenNodes1[j] = sigmoid(activation);
                //if (epoch == 0)
                //    printf("\n%g\n", activation);
            }

            for (int j = 0; j < nbHiddenNodes2; j++)
            {
                double activation = biasHiddenNodes2[j];

                for (int k = 0; k < nbHiddenNodes1; k++)
                    activation += layerHiddenNodes1[k] * weightsHiddenNodes2[j][k];

                layerHiddenNodes2[j] = sigmoid(activation);
            }

            for (int j = 0; j < nbOutputs; j++)
            {
                double activation = biasOutputs[j];

                for (int k = 0; k < nbHiddenNodes2; k++)
                    activation += layerHiddenNodes2[k] * weightsOutputs[j][k];

                layerOutputs[j] = sigmoid(activation);
            }
            
            if (epoch == nbEpochs)
            {
                char mp = alphabet[0];
                double oL = layerOutputs[0];
                printf("%g,", layerOutputs[0]);
                for (int l = 1; l < nbOutputs; l++)
                {
                    printf("%g,", layerOutputs[l]);
                    if (oL < layerOutputs[l])
                    {
                        oL = layerOutputs[l];
                        mp = alphabet[l];
                    }
                }
                printf("\nInput: %g -- %g     Output: %c     Predicted Output: %c\n",
                        trainInputs[i][0], trainInputs[i][nbInputs - 1],
                        mp, possible_outputs[i]);
            }

            // Backward Propagation

            double dPredictedOutput[nbOutputs];

            for (int j = 0; j < nbOutputs; j++)
            {
                double error = trainOutputs[i][j] - layerOutputs[j];
                dPredictedOutput[j] = error * dSigmoid(layerOutputs[j]);
            }

            double dHidden2[nbHiddenNodes2];

            for (int j = 0; j < nbHiddenNodes2; j++)
            {
                double error = 0.0f;
                for (int k = 0; k < nbOutputs; k++)
                {
                    error += dPredictedOutput[k] * weightsOutputs[k][j];
                }
                dHidden2[j] = error * dSigmoid(layerHiddenNodes2[j]);
            }

            double dHidden1[nbHiddenNodes1];
            
            for (int j = 0; j < nbHiddenNodes1; j++)
            {
                double error = 0.0f;
                for (int k = 0; k < nbHiddenNodes2; k++)
                {
                    error += dHidden2[k] * weightsHiddenNodes2[k][j];
                }
                dHidden1[j] = error * dSigmoid(layerHiddenNodes1[j]);
            }

            for (int j = 0; j < nbOutputs; j++)
            {
                biasOutputs[j] += dPredictedOutput[j] * lr;
                for (int k = 0; k < nbHiddenNodes2; k++)
                {
                    weightsOutputs[j][k] += 
                        layerHiddenNodes2[k] * dPredictedOutput[j];
                }
            }

            for (int j = 0; j < nbHiddenNodes2; j++)
            {
                biasHiddenNodes2[j] += dHidden2[j] * lr;
                for (int k = 0; k < nbHiddenNodes1; k++)
                {
                    weightsHiddenNodes2[j][k] += 
                        layerHiddenNodes1[k] * dHidden2[j];
                }
            }

            for (int j = 0; j < nbHiddenNodes1; j++)
            {
                biasHiddenNodes1[j] += dHidden1[j] * lr;
                for (int k = 0; k < nbInputs; k++)
                {
                    weightsHiddenNodes1[j][k] += 
                        trainInputs[i][k] * dHidden1[j];
                }
            }
        }
    }
    for (int i = 0; i < nbTrainingInputs; i++)
        free(trainInputs[i]);
    free(trainInputs);
    for (int i = 0; i < nbTrainingInputs; i++)
        free(trainOutputs[i]);
    free(trainOutputs);
    free(possible_outputs);
}

int main(int argc, char** argv)
{
    trainLetter(26);

    return EXIT_SUCCESS;
}
