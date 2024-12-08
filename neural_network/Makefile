CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lSDL2 -lSDL2_image -lm

all: XAND neuralNetwork

XAND: neuralNetworkXAND.c
	${CC} ${CFLAGS} neuralNetworkXAND.c -o XAND ${LDFLAGS}

neuralNetwork: neuralNetwork.c
	${CC} ${CFLAGS} neuralNetwork.c trainNeural.c -o NeuralNetwork ${LDFLAGS}

.PHONY: clean

clean:
	${RM} XAND
	${RM} neuralNetwork
