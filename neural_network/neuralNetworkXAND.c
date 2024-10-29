#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <err.h>
#include <string.h>
#include <sys/stat.h>

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
#define nbHiddenNodes1 2
#define nbHiddenNodes2 2
#define nbOutputs 1
#define nbTrainingSets 4

void train(double i1, double i2)
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



    double training_inputs[nbTrainingSets][nbInputs] = {{0.0f, 0.0f},
                                                        {1.0f, 0.0f},
                                                        {0.0f, 1.0f},
                                                        {1.0f, 1.0f}};

    double training_outputs[nbTrainingSets][nbOutputs] = {{1.0f},
                                                          {0.0f},
                                                          {0.0f},
                                                          {1.0f}};


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

    int trainingSetOrder[] = {0,1,2,3};
    
    int epochs = 100000;
    
    // Train the neural network for a number of epochs
    for (int epoch = 0; epoch < epochs; epoch++)
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

            if (epoch == epochs - 1 && i1 > 1.0)
                printf("Input: %g -- %g  Output: %g   Predicted Output: %g\n",
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
                    outputWeights[k][j] += hiddenLayer2[k] * deltaOutput[j];
                }
            }

            // Apply change in hidden weights 2
            for (int j = 0; j < nbHiddenNodes2; j++)
            {
                hiddenLayerBias2[j] += deltaHidden2[j] * lr;
                for (int k = 0; k < nbHiddenNodes1; k++)
                {
                    hiddenWeights2[k][j] += hiddenLayer1[k] * deltaHidden2[j];
                }
            }

            // Apply change in hidden weights 1
            for (int j = 0; j < nbHiddenNodes1; j++)
            {
                hiddenLayerBias1[j] += deltaHidden1[j] * lr;
                for (int k = 0; k < nbInputs; k++)
                {
                    hiddenWeights1[k][j] += training_inputs[i][k] * deltaHidden1[j];
                }
            }
        }
    }
    if (i1 < 2.0)
    {
        for (int j = 0; j < nbHiddenNodes1; j++)
        {
            double activation = hiddenLayerBias1[j];
        
            activation +=  i1 * hiddenWeights1[0][j];
            activation +=  i2 * hiddenWeights1[1][j];

            hiddenLayer1[j] = sigmoid(activation);
        }

        for (int j = 0; j < nbHiddenNodes2; j++)
        {
            double activation = hiddenLayerBias1[j];

            for (int k = 0; k < nbHiddenNodes1; k++)
            {
                activation += hiddenLayer1[k] * hiddenWeights2[k][j];
            }

            hiddenLayer2[j] = sigmoid(activation);
        }

        for (int j = 0; j < nbOutputs; j++)
        {
            double activation = hiddenLayerBias2[j];

            for (int k = 0; k < nbHiddenNodes2; k++)
            {
                activation += hiddenLayer2[k] * outputWeights[k][j];
            }

            outputLayer[j] = sigmoid(activation);
        }
    
        if (outputLayer[0] > 0.5)
            printf(" = 1\n");
        else
            printf(" = 0\n");
    }
    
    char *folder = "Data";
    struct stat sb;

    if (!(stat(folder, &sb) == 0 && S_ISDIR(sb.st_mode)))
        mkdir("Data", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    FILE *fp = fopen("Data/xandHiddenNodesBais1.txt", "w");

    if (fp == NULL) 
        errx(EXIT_FAILURE, "Cannot save data");

    for (int i = 0; i < nbHiddenNodes1; i++)
        fprintf(fp, "%f\n", hiddenLayerBias1[i]);

    fclose(fp);

    fp = fopen("Data/xandHiddenNodesWeights1.txt", "w");

    if (fp == NULL)
        errx(EXIT_FAILURE, "Cannot save date");
    
    for (int i = 0; i < nbHiddenNodes1; i++)
        for (int j = 0; j < nbInputs; j++)
            fprintf(fp, "%f\n", hiddenWeights1[j][i]);

    fclose(fp);


    fp = fopen("Data/xandHiddenNodesBais2.txt", "w");

    if (fp == NULL) 
        errx(EXIT_FAILURE, "Cannot save data");

    for (int i = 0; i < nbHiddenNodes2; i++)
        fprintf(fp, "%f\n", hiddenLayerBias2[i]);

    fclose(fp);

    fp = fopen("Data/xandHiddenNodesWeights2.txt", "w");

    if (fp == NULL)
        errx(EXIT_FAILURE, "Cannot save date");
    
    for (int i = 0; i < nbHiddenNodes2; i++)
        for (int j = 0; j < nbHiddenNodes1; j++)
            fprintf(fp, "%f\n", hiddenWeights2[j][i]);

    fclose(fp);
    

    fp = fopen("Data/xandOutputsBais.txt", "w");

    if (fp == NULL) 
        errx(EXIT_FAILURE, "Cannot save data");

    for (int i = 0; i < nbOutputs; i++)
        fprintf(fp, "%f\n", outputLayerBias[i]);

    fclose(fp);

    fp = fopen("Data/xandOutputsWeights.txt", "w");

    if (fp == NULL)
        errx(EXIT_FAILURE, "Cannot save date");
    
    for (int i = 0; i < nbOutputs; i++)
        for (int j = 0; j < nbHiddenNodes2; j++)
            fprintf(fp, "%f\n", outputWeights[j][i]);

    fclose(fp);
}

void use(double i1, double i2) 
{
    double hiddenLayer1[nbHiddenNodes1];
    double hiddenLayer2[nbHiddenNodes2];
    double outputLayer[nbOutputs];

    double hiddenLayerBias1[nbHiddenNodes1];
    double hiddenLayerBias2[nbHiddenNodes2];
    double outputLayerBias[nbOutputs];

    double hiddenWeights1[nbInputs][nbHiddenNodes1];
    double hiddenWeights2[nbHiddenNodes1][nbHiddenNodes2];
    double outputWeights[nbHiddenNodes2][nbOutputs];

    char *folder = "Data";
    struct stat sb;

    if (!(stat(folder, &sb) == 0 && S_ISDIR(sb.st_mode)))
    {
        train(i1, i2);
        return;
    }

    FILE *fp = fopen("Data/xandHiddenNodesBais1.txt", "r");

    if (fp == NULL) 
        train(i1, i2);

    for (int i = 0; i < nbHiddenNodes1; i++)
        

    fclose(fp);
}

int main(int argc, char **argv)
{
    if (argc < 2)
        errx(EXIT_FAILURE,"You need more arguments");
    if (strcmp(argv[1], "train") == 0)
    {
        if (argc != 2)
            errx(EXIT_FAILURE, "Too much arguments");
        printf("TRAINING\n\n");
        train(2.0, 2.0);
    }
    else if (strcmp(argv[1], "use") == 0)
    {
        if (argc != 4)
            errx(EXIT_FAILURE, "You need 3 arguments");
        double i1 = 0.0f;
        double i2 = 0.0f;
        if (strcmp(argv[2], "1") == 0)
            i1 = 1.0f;
        if (strcmp(argv[3], "1") == 0)
            i2 = 1.0f;
        if ((strcmp(argv[2], "1") != 0 && strcmp(argv[2], "0") != 0) ||
            (strcmp(argv[3], "1") != 0 && strcmp(argv[3], "0") != 0))
            errx(EXIT_FAILURE, "Only 1 or 0 accepted");
        printf("%s XAND %s", argv[2], argv[3]);
        use(i1, i2);
    }
    else
        errx(EXIT_FAILURE, "Unknown arguments");
    
    return EXIT_SUCCESS;
}
