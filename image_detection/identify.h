#pragma once

struct letter
{
    int xmid;
    int ymid;
    int xmin;
    int ymin;
    int xmax;
    int ymax;
    int xgrid;
    int ygrid;
};

struct word
{
    int xmid;
    int ymid;
    int xmin;
    int ymin;
    int xmax;
    int ymax;

    int nbletters;
    int *letters;
};

int is_black(Uint32 pixel, SDL_PixelFormat *format);

void surface_remove_white(SDL_Surface *surface, int** black_grid);

void surface_color_connex(SDL_Surface *surface, int** black_grid);

void erase_grid(int*** black_grid, SDL_Surface *surface,
        int x1, int y1, int x2, int y2);

int expand(SDL_Surface *surface, int x, int y, int*** black_grid, int count);

void detect_connexe(SDL_Surface *surface,int*** black_grid, int* maxid);

void getridof(int*** grid, int id, int maxx, int maxy);

struct letter* box_letters(int maxid, int* counts, 
        int** img, int height, int width);

struct letter* box_words(int maxid, int* counts, 
        int** img, int height, int width);

int getidfrombox(int x, int y,int maxid, struct letter* ltr);

int is_grid(struct letter* ltr,int maxid, int** grid, struct letter corner1,
        struct letter corner2,int* finalx, int* finaly);

void find_grid(struct letter* ltr, int maxid, int** grid,int* countx, 
        int* county, int* posxmin, int* posymin, int*posxmax, int*posymax);

void clear_not_letters(int** counts,int height,int width,int*** black_grid, 
        int maxid, int repet);

void remove_grid_letters(struct letter** letters, struct letter** gridletters,
        int x1, int y1, int x2, int y2, int maxid);

unsigned char GetPixelComp32(SDL_Surface* surface,int x,int y,int c);

void PutPixelComp32(SDL_Surface* surface,int x,int y,int c,unsigned char val);

void Stretch_Nearest(SDL_Surface* src,SDL_Surface* dest);

void Stretch_Linear(SDL_Surface* src,SDL_Surface* dest);

void Strechblit(SDL_Surface* src,SDL_Surface* dest);

void split_grid(SDL_Surface *surface, struct letter* ltr, int maxid);

void split_words(SDL_Surface *surface, struct word *words, 
        int nbWords, struct letter *ltr);

SDL_Surface *find_list(SDL_Surface *surface, struct letter *ltr, int **connex, 
        int maxid, struct word **words, size_t *nbWords);
