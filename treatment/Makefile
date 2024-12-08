CC = gcc
CFLAGS = 
LDFLAGS = -lSDL2 -lSDL2_image -lm

all: treatment

treatment: load_image.c
	${CC} ${CFLAGS} load_image.c -o treatment ${LDFLAGS}

.PHONY: clean

clean:
	${RM} treatment
