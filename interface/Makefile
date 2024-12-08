# Nom du compilateur
CC = gcc

# Options de compilation
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Wextra -rdynamic 

# Bibliothèques nécessaires
LIBS = `pkg-config --libs gtk+-3.0` -lm -lpthread -lSDL2 -lSDL2_image

# Fichiers source
SOURCES = main.c rotaimg.c

# Fichiers objets générés
OBJECTS = $(SOURCES:.c=.o)

# Nom de l'exécutable final
EXECUTABLE = Application

# Règle principale : génère l'exécutable
all: $(EXECUTABLE) main solver

# Comment construire l'exécutable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

main: ../main/main.c ../image_detection/identify.c ../image_detection/morphoMaths.c ../neural_network/neuralNetwork.c
	$(CC) -Wall -Wextra ../main/main.c ../image_detection/identify.c ../image_detection/morphoMaths.c ../neural_network/neuralNetwork.c -o ../main/main -lm -lSDL2 -lSDL2_image

solver: ../solver/solver.c
	$(CC) -Wall -Wextra ../solver/solver.c -o ../solver/solver -lSDL2 -lSDL2_image

NeuralNetwork: ../neural_network/neuralNetwork.c
	$(CC) -Wall -Wextra ../neural_network/neuralNetwork.c ../neural_network/trainNeural.c -o NeuralNetwork -lSDL2 -lSDL2_image -lm

# Règle générique pour convertir un .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyer les fichiers générés
.PHONY: clean
clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
