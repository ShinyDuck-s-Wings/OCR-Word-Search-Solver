#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <err.h>
#include <dirent.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Simple nn that can learn xor

double sigmoid(double x) { return 1 / (1 + exp(-x)); }
double dSigmoid(double x) { return x * (1 - x); }

double init_weights() { return ((double)rand()) / ((double)RAND_MAX); }

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

#define nbInputs 784
#define nbHiddenNodes1 42
#define nbHiddenNodes2 42
#define nbOutputs 26
#define nbTrainingSets 26

void init_training(double **training_inputs, double **training_outputs, size_t size)
{
    DIR *d;
    struct dirent *dir;
    d = opendir("learning_data_base");
    size_t i = 0;
    if (d)
    {
        while ((dir = readdir(d)) != NULL && i < size) 
        {
            if (dir->d_name[0] != '.')    
            {
                // function to make the image in black and white
                training_inputs[i];
                switch (dir->d_name[0])
                {
                    case 'a': training_outputs[i] = {1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'b': training_outputs[i] = {0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'c': training_outputs[i] = {0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'd': training_outputs[i] = {0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'e': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'f': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'g': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'h': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'i': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'j': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'k': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'l': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'm': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'n': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'o': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'p': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'q': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'r': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 's': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 't': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'u': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'v': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00};
                              break;
                    case 'w': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00};
                              break;
                    case 'x': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00};
                              break;
                    case 'y': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00};
                              break;
                    case 'z': training_outputs[i] = {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00};
                              break;
                }
            }
        }
        closedir(d);
    }
}

int main()
{
    const double lr = 0.1f; // Speed Rate for learning

    double hiddenLayer1[nbHiddenNodes1];
    double hiddenLayer2[nbHiddenNodes2];
    double outputLayer[nbOutputs];

    double hiddenLayerBias1[nbHiddenNodes1];
    double hiddenLayerBias2[nbHiddenNodes2];
    double outputLayerBias[nbOutputs];

    double hiddenWeights1[nbInputs][nbHiddenNodes1];
    double hiddenWeights2[nbHiddenNodes1][nbHiddenNodes2];
    double outputWeights[nbHiddenNodes2][nbOutputs];



    double training_inputs[nbTrainingSets][nbInputs];

    double training_outputs[nbTrainingSets][nbOutputs];

    init_training(training_inputs, training_outputs, nbTrainingSets);

    char predicted_outputs[nbOutputs] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};


    for (int i = 0; i < nbInputs; i++)
    {
        for (int j = 0; j < nbHiddenNodes1; j++)
        {
            hiddenWeights1[i][j] = init_weights();
        }
    }

    for (int i = 0; i < nbHiddenNodes1; i++)
    {
        for (int j = 0; j < nbHiddenNodes2; j++)
        {
            hiddenWeights2[i][j] = init_weights();
        }
    }

    for (int i = 0; i < nbHiddenNodes2; i++)
    {
        for (int j = 0; j < nbOutputs; j++)
        {
            outputWeights[i][j] = init_weights();
        }
    }

    for (int i = 0; i < nbHiddenNodes2; i++)
    {
        outputLayerBias[i] = init_weights();
    }


    // T R A I N I N G

    int trainingSetOrder[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
    
    int nbOfEpochs = 99996;
    
    // Train the neural network for a number of epochs

    for (int epoch = 0; epoch < nbOfEpochs; epoch++)
    {
        shuffle(trainingSetOrder, nbTrainingSets);

        for (int x = 0; x < nbTrainingSets; x++)
        {
            int i = trainingSetOrder[x];

            // Forward pass
            
            // Compute hidden layer 1 activation

            for (int j = 0; j < nbHiddenNodes1; j++)
            {
                double activation = hiddenLayerBias1[j];

                for (int k = 0; k < nbInputs; k++)
                {
                    activation += training_inputs[i][k] * hiddenWeights1[k][j];
                }

                hiddenLayer1[j] = sigmoid(activation);
            }

            // Compute hidden layer 2 activation

            for (int j = 0; j < nbHiddenNodes2; j++)
            {
                double activation = hiddenLayerBias1[j];

                for (int k = 0; k < nbHiddenNodes1; k++)
                {
                    activation += hiddenLayer1[k] * hiddenWeights2[k][j];
                }

                hiddenLayer2[j] = sigmoid(activation);
            }


            // Compute output layer activation

            for (int j = 0; j < nbOutputs; j++)
            {
                double activation = hiddenLayerBias2[j];

                for (int k = 0; k < nbHiddenNodes2; k++)
                {
                    activation += hiddenLayer2[k] * outputWeights[k][j];
                }

                outputLayer[j] = sigmoid(activation);
            }

            char mp = predicted_outputs[0];
            int oL = outputLayer[0];
            for (int l = 1; l < nbOutputs; l++)
            {
                if (oL < outputLayer[l])
                {
                    oL = outputLayer[l];
                    mp = predicted_outputs[l];
                }
            }
            printf("Input: %g -- %g     Output: %c     Predicted Output: %c\n", 
                    training_inputs[i][0], training_inputs[i][nbInputs - 1],
                    mp, predicted_outputs[i]);
            

            // Backprop

            // Compute change in output weights
            
            double deltaOutput[nbOutputs];

            for (int j = 0; j < nbOutputs; j++)
            {
                double error = (training_outputs[i][j] - outputLayer[j]);
                deltaOutput[j] = error * dSigmoid(outputLayer[j]);
            }

            // Compute change in hidden weight 2

            double deltaHidden2[nbHiddenNodes2];

            for (int j = 0; j < nbHiddenNodes2; j++)
            {
                double error = 0.0f;
                for(int k = 0; k < nbOutputs; k++)
                {
                    error += deltaOutput[k] * outputWeights[j][k];
                }
                deltaHidden2[j] = error * dSigmoid(hiddenLayer2[j]);
            }
            
            // Compute change in hidden weight 1

            double deltaHidden1[nbHiddenNodes1];

            for (int j = 0; j < nbHiddenNodes1; j++)
            {
                double error = 0.0f;
                for(int k = 0; k < nbHiddenNodes2; k++)
                {
                    error += deltaHidden2[k] * hiddenWeights2[j][k];
                }
                deltaHidden1[j] = error * dSigmoid(hiddenLayer1[j]);
            }


            // Apply change in output weights
            
            for (int j = 0; j < nbOutputs; j++)
            {
                outputLayerBias[j] += deltaOutput[j] * lr;
                for (int k = 0; k < nbHiddenNodes2; k++)
                {
                    outputWeights[k][j] += hiddenLayer2[k] * deltaOutput[j] * lr;
                }
            }

            // Apply change in hidden weights 2

            for (int j = 0; j < nbHiddenNodes2; j++)
            {
                hiddenLayerBias2[j] += deltaHidden2[j] * lr;
                for (int k = 0; k < nbHiddenNodes1; k++)
                {
                    hiddenWeights2[k][j] += hiddenLayer1[k] * deltaHidden2[j] * lr;
                }
            }

            // Apply change in hidden weights 1

            for (int j = 0; j < nbHiddenNodes1; j++)
            {
                hiddenLayerBias1[j] += deltaHidden1[j] * lr;
                for (int k = 0; k < nbInputs; k++)
                {
                    hiddenWeights1[k][j] += training_inputs[i][k] * deltaHidden1[j] * lr;
                }
            }
        }
    }
}
