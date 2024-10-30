# Makefile

CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lSDL2 -lSDL2_image
LDLIBS =

all: solver cross

solver: solver.c
	$(CC) $(CFLAGS) solver.c -o solver $(LDFLAGS)

cross: cross.c
	$(CC) $(CFLAGS) cross.c -o cross $(LDFLAGS)

.PHONY: clean

clean:
	${RM} solver
	$(RM) cross

# END

