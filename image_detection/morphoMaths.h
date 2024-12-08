#pragma once

void dilatation8(SDL_Surface *imageInit, SDL_Surface *imageResult);

void erosion8(SDL_Surface *imageInit, SDL_Surface *imageResult);

void dilatation4(SDL_Surface *imageInit, SDL_Surface *imageResult);

void erosion4(SDL_Surface *imageInit, SDL_Surface *imageResult);

void dilatation2x(SDL_Surface *imageInit, SDL_Surface *imageResult);

void erosion2x(SDL_Surface *imageInit, SDL_Surface *imageResult);

void dilatation2y(SDL_Surface *imageInit, SDL_Surface *imageResult);

void erosion2y(SDL_Surface *imageInit, SDL_Surface *imageResult);

void opening8(SDL_Surface *imageInit, SDL_Surface *imageResult);

void closure8(SDL_Surface *imageInit, SDL_Surface *imageResult);

void opening4(SDL_Surface *imageInit, SDL_Surface *imageResult);

void closure4(SDL_Surface *imageInit, SDL_Surface *imageResult);
