#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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

#define nbInputs 2
#define nbHiddenNodes 2
#define nbOutputs 1
#define nbTrainingSets 4

int main()
{
    const double lr = 0.1f; // Speed Rate for learning

    double hiddenLayer[nbHiddenNodes];
    double outputLayer[nbOutputs];

    double hiddenLayerBias[nbHiddenNodes];
    double outputLayerBias[nbOutputs];

    double hiddenWeights[nbInputs][nbHiddenNodes];
    double outputWeights[nbHiddenNodes][nbOutputs];



    double training_inputs[nbTrainingSets][nbInputs] = {{0.0f, 0.0f},
                                                        {1.0f, 0.0f},
                                                        {0.0f, 1.0f},
                                                        {1.0f, 1.0f}};

    double training_outputs[nbTrainingSets][nbOutputs] = {{0.0f},
                                                          {1.0f},
                                                          {1.0f},
                                                          {0.0f}};


    for (int i = 0; i < nbInputs; i++)
    {
        for (int j = 0; j < nbHiddenNodes; j++)
        {
            hiddenWeights[i][j] = init_weights();
        }
    }

    for (int i = 0; i < nbHiddenNodes; i++)
    {
        for (int j = 0; j < nbOutputs; j++)
        {
            outputWeights[i][j] = init_weights();
        }
    }

    for (int i = 0; i < nbHiddenNodes; i++)
    {
        outputLayerBias[i] = init_weights();
    }


    // T R A I N I N G

    int trainingSetOrder[] = {0,1,2,3};
    
    int nbOfEpochs = 10000;
    
    // Train the neural network for a number of epochs
    for (int epoch = 0; epoch < nbOfEpochs; epoch++)
    {
        shuffle(trainingSetOrder, nbTrainingSets);

        for (int x = 0; x < nbTrainingSets; x++)
        {
            int i = trainingSetOrder[x];

            // Forward pass
            
            // Compute hidden layer activation
            for (int j = 0; j < nbHiddenNodes; j++)
            {
                double activation = hiddenLayerBias[j];

                for (int k = 0; k < nbInputs; k++)
                {
                    activation += training_inputs[i][k] * hiddenWeights[k][j];
                }

                hiddenLayer[j] = sigmoid(activation);
            }

            // Compute output layer activation
            for (int j = 0; j < nbOutputs; j++)
            {
                double activation = hiddenLayerBias[j];

                for (int k = 0; k < nbHiddenNodes; k++)
                {
                    activation += hiddenLayer[k] * outputWeights[k][j];
                }

                outputLayer[j] = sigmoid(activation);
            }

            printf("Input: %g -- %g     Output: %g      Predicted Output: %g\n", 
                    training_inputs[i][0], training_inputs[i][1],
                    outputLayer[0], training_outputs[i][0]);
            

            // Backprop

            // Compute change in output weights
            
            double deltaOutput[nbOutputs];

            for (int j = 0; j < nbOutputs; j++)
            {
                double error = (training_outputs[i][j] - outputLayer[j]);
                deltaOutput[j] = error * dSigmoid(outputLayer[j]);
            }
            
            // Compute change in hidden weight

            double deltaHidden[nbHiddenNodes];

            for (int j = 0; j < nbHiddenNodes; j++)
            {
                double error = 0.0f;
                for(int k = 0; k < nbOutputs; k++)
                {
                    error += deltaOutput[k] * outputWeights[j][k];
                }
                deltaHidden[j] = error * dSigmoid(hiddenLayer[j]);
            }


            // Apply change in output weights
            for (int j = 0; j < nbOutputs; j++)
            {
                outputLayerBias[j] += deltaOutput[j] * lr;
                for (int k = 0; k < nbHiddenNodes; k++)
                {
                    outputWeights[k][j] += hiddenLayer[k] * deltaOutput[j] * lr;
                }
            }

            // Apply change in hidden weights
            for (int j = 0; j < nbHiddenNodes; j++)
            {
                hiddenLayerBias[j] += deltaHidden[j] * lr;
                for (int k = 0; k < nbInputs; k++)
                {
                    hiddenWeights[k][j] += training_inputs[i][k] * deltaHidden[j] * lr;
                }
            }
        }
    }
}
