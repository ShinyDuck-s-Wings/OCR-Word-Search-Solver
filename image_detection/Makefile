# Makefile

CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lSDL2 -lSDL2_image
LDLIBS =

all: identify

identify: identify.c
	$(CC) $(CFLAGS) identify.c -o identify $(LDFLAGS)

.PHONY: clean

clean:
	$(RM) identify

# END

