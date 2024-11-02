# OCR-Word-Search-Solver

### Treatment

In the directory `treatment/`, you can do the command `make all` to create the executable `treatment`.

With it you can apply the treatment function to one of the original image of the grids.

For example:

`./treatment ../image/level_x_image_y.png` 
(with x between 1 and 4 and y between 1 and 2)

### Manual Rotation



### Image Detection

In the directory `image_detection/`, you can do the command `make all` to create the executable `identify`.

With it you can split the letters from the grid and the letters from the words in two differents files `letters` and `words` in the root of this repository.

To use it, you need to indicate one of the pre-treated files.

For example:
  
`./identify ../image/bw_level_x_image_y.png` 
(with x between 1 and 4 (doesn't work on the level 2) and y between 1 and 2)

### Neural Network

In the directory `neural_network/`, you can do the command `make all` to create the executables `XAND` and `NeuralNetwork`.
  
With the first one, `XAND`, you have two possible mode:
* `./XAND train` -- It will create a directory `Data/` which contains all the weights and bais of the neural network XAND
* `./XAND use x y` -- If the command `train` was not execute it will do the training and will save the weights and bais in the `Data/` directory and then will give the solution of x XAND y
                      else it will use the weights and bais in the `Data/` directory and then will give the solution of x XAND y (x and y equal to 0 or 1)
  
With the second one, `NeuralNetwork`, you can train an neural network with the image of the letters in `learning_data_base/` for that execute `./NeuralNetwork`.

### Solver

In the directory `solver/`, you can do the command `make all` to create the executables `solver` and `cross`.
  
The first one is used by the second and is in fact not to use.
  
The second one allow you to see an example of how the solution is found, for the moment it only work with the first image of the level one since the different part are not link.
  
To see the result : 
  
`./cross ../image/level_1_image_1.png [list of words to search]`
