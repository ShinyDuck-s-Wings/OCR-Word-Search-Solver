#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "morphoMaths.h"
#include "identify.h"

/*struct letter
{
    int xmid;
    int ymid;
    int xmin;
    int ymin;
    int xmax;
    int ymax;
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
};*/

int is_black(Uint32 pixel, SDL_PixelFormat *format)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(pixel,format,&r,&g,&b);
    if(r < 50 && g < 50 && b < 50)
    {
        return 1;
    }
    return 0;
}

//v1.0
/*int mi = 0;

struct black
{
    int* x;
    int* y;
    int last;
}

void expand(SDL_Surface *surface, int x, int y, struct black* found, int first)
{
    Uint32 *pix = (*surface).pixels;
    SDL_PixelFormat *format = (*surface).format;
    Uint32 curr = pix[y * (*surface).h + x];
    if(is_black(curr, format))
    {
        int count = 0;
        int* currx = found[mi].x;
        int* curry = found[mi].y;
        int already = 0;
        while(currx != NULL && *(currx + count) != -1)
        {
            if(x == *(currx + count) && y == *(curry + count))
            {
                already = 1;
                break;
            }
            count++;
        }
        if(already == 0)
        {
            int mounds_num = mi + 1;
            if(first)
            {
                found = realloc(found,(mounds_num + 1) * sizeof(struct black));
                (*(found+mi)).x = malloc(sizeof(int));
                (*(found+mi)).x[0] = -1;
                (*(found+mi)).y = malloc(sizeof(int));
                (*(found+mi)).y[0] = -1;
                (*(found+mi)).last = 0;
                struct black l = {NULL,NULL,1};
                (*(found+mounds_num)) = l;
                mi++;
            }
            found[mi-1].x = realloc(found[mi-1].x,(count+2)*sizeof(int));
            found[mi-1].y = realloc(found[mi-1].y,(count+2)*sizeof(int));
            (found[mi-1].x)[count+1] = -1;
            (found[mi-1].x)[count] = x;
            (found[mi-1].y)[count+1] = -1;
            (found[mi-1].y)[count] = y;
            if(x != 0)
                expand(surface,x-1,y,found,0);
            if(x != surface->w -1)
                expand(surface,x+1,y,found,0);
            if(y != 0)
                expand(surface,x,y-1,found,0);
            if(y != surface->h -1)
                expand(surface,x,y+1,found,0);
            return;
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }
}

void find_mounds(SDL_Surface *surface, struct black* mounds)
{
    int horizontal = (*surface).w;
    int vertical = (*surface).h;
    struct black init = {NULL,NULL,1};
    mounds = malloc(sizeof(struct black));
    *mounds = init;
    for(int y = 0; y < vertical; y++)
    {
        for(int x = 0; x < horizontal; x++)
        {
            expand(surface,x,y,mounds,1);
        }
    }
    return;
}*/

void surface_remove_white(SDL_Surface *surface, int** black_grid)
{
    Uint32 *pix = (*surface).pixels;
    SDL_PixelFormat *format = (*surface).format;
    for(int x = 0; x < (*surface).w; x++)
    {
        for(int y = 0; y < (*surface).h; y++)
        {
            if(!black_grid[x][y])
            {
                pix[y * (*surface).w + x] = SDL_MapRGB(format,255,255,255);
            }
        }
    }
}

void surface_color_connex(SDL_Surface *surface, int** black_grid)
{
    Uint32 *pix = (*surface).pixels;
    SDL_PixelFormat *format = (*surface).format;
    for(int x = 0; x < (*surface).w; x++)
    {
        for(int y = 0; y < (*surface).h; y++)
        {
            if(!black_grid[x][y])
            {
                pix[y * (*surface).w + x] = SDL_MapRGB(format,255,255,255);
            }
            else
            {
                pix[y * (*surface).w + x] = 
                    SDL_MapRGB(format,black_grid[x][y]*5,0,black_grid[x][y]*5);
            }
        }
    }
}


void erase_grid(int*** black_grid, SDL_Surface *surface,
        int x1, int y1, int x2, int y2)
{
    Uint32 *pix = (*surface).pixels;
    SDL_PixelFormat *format = (*surface).format;
    for(int x = x1; x < x2; x++)
    {
        for(int y = y1; y < y2; y++)
        {
            (*black_grid)[x][y] = 0;
            pix[y * (*surface).w + x] = SDL_MapRGB(format,255,255,255);
        }
    }
}

int expand(SDL_Surface *surface, int x, int y, int*** black_grid, int count)
{
    Uint32 *pix = (*surface).pixels;
    SDL_PixelFormat *format = (*surface).format;
    Uint32 curr = pix[y * (*surface).w + x];
    if((*black_grid)[x][y] == -1)
    {
        if(is_black(curr,format))
        {
            (*black_grid)[x][y] = count;
            if(x!=0)
            {
                expand(surface,x-1,y,black_grid,count);
                /*if(y!=0)
                    expand(surface,x-1,y-1,black_grid,count);
                if(y!=(*surface).h - 1)
                    expand(surface,x-1,y+1,black_grid,count);*/
            }
            if(x!=(*surface).w - 1)
            {
                expand(surface,x+1,y,black_grid,count);
                /*if(y!=0)
                    expand(surface,x+1,y-1,black_grid,count);
                if(y!=(*surface).h - 1)
                    expand(surface,x+1,y+1,black_grid,count);*/
            }
            if(y!=0)
                expand(surface,x,y-1,black_grid,count);
            if(y!=(*surface).h - 1)
                expand(surface,x,y+1,black_grid,count);
            return 1;
        }
        else
        {
            (*black_grid)[x][y] = 0;
        }
        return 0;
    }
    return 0;
}

void detect_connexe(SDL_Surface *surface,int*** black_grid, int* maxid)
{
    int horizontal = (*surface).w;
    int vertical = (*surface).h;
    *black_grid = malloc(horizontal * sizeof(int*));
    int count = 1;
    for(int i = 0; i < horizontal; i++)
    {
        (*black_grid)[i] = malloc(vertical * sizeof(int));
    }
    for(int y = 0; y < vertical; y++)
    {
        for(int x = 0; x < horizontal; x++)
        {
            (*black_grid)[x][y] = -1;
        }
    }
    for(int y = 0; y < vertical; y++)
    {
        for(int x = 0; x < horizontal; x++)
        {
            if(expand(surface,x,y,black_grid,count))
            {
                count++;
            }
        }
    }
    *maxid = count;
}

void getridof(int*** grid, int id, int maxx, int maxy)
{
    for(int y = 0; y < maxy; y++)
    {
        for(int x = 0; x < maxx; x++)
        {
            if((*grid)[x][y] == id)
            {
                (*grid)[x][y] = 0;
            }
        }
    }
}

struct letter* box_letters(int maxid, int* counts, 
        int** img, int height, int width)
{
    struct letter* letters = malloc(maxid * sizeof(struct letter));
    for(int i = 0; i < maxid; i++)
    {
        struct letter init = {0,0,0,0,0,0,0,0};
        letters[i] = init;
        if(counts[i] != 0)
        {
            int ymaxi = 0;
            int ymini = height;
            int xmaxi = 0;
            int xmini = width;
            for(int y = 0; y < height; y++)
            {
                for(int x = 0; x < width; x++)
                {
                    if(img[x][y] == i)
                    {
                        if(x > xmaxi)
                            xmaxi = x;
                        if(x < xmini)
                            xmini = x;
                        if(y > ymaxi)
                            ymaxi = y;
                        if(y < ymini)
                            ymini = y;
                    }
                }
            }
            int ymiddle = ymini + (ymaxi - ymini)/2;
            int xmiddle = xmini + (xmaxi - xmini)/2;
            int xlen = xmaxi - xmini;
            int ylen = ymaxi - ymini;
            if(ylen > xlen)
            {
                xmini = xmini - (ylen - xlen) / 2;
                xmaxi = xmaxi + (ylen - xlen) / 2;
            }
            else
            {
                ymini = ymini - (xlen - ylen) / 2;
                ymaxi = ymaxi + (xlen - ylen) / 2;
            }
            ymini--;
            ymaxi++;
            xmini--;
            xmaxi++;
            struct letter curr = {xmiddle,ymiddle,xmini,ymini,xmaxi,ymaxi,0,0};
            letters[i] = curr;
        }
    }
    return letters;
}

struct letter* box_words(int maxid, int* counts, 
        int** img, int height, int width)
{
    struct letter* letters = malloc(maxid * sizeof(struct letter));
    for(int i = 0; i < maxid; i++)
    {
        struct letter init = {0,0,0,0,0,0,0,0};
        letters[i] = init;
        if(counts[i] != 0)
        {
            int ymaxi = 0;
            int ymini = height;
            int xmaxi = 0;
            int xmini = width;
            for(int y = 0; y < height; y++)
            {
                for(int x = 0; x < width; x++)
                {
                    if(img[x][y] == i)
                    {
                        if(x > xmaxi)
                            xmaxi = x;
                        if(x < xmini)
                            xmini = x;
                        if(y > ymaxi)
                            ymaxi = y;
                        if(y < ymini)
                            ymini = y;
                    }
                }
            }
            int ymiddle = ymini + (ymaxi - ymini)/2;
            int xmiddle = xmini + (xmaxi - xmini)/2;
            ymini--;
            ymaxi++;
            xmini--;
            xmaxi++;
            struct letter curr = {xmiddle,ymiddle,xmini,ymini,xmaxi,ymaxi,0,0};
            letters[i] = curr;
        }
    }
    return letters;
}


int getidfrombox(int x, int y,int maxid, struct letter* ltr)
{
    for(int i = 0; i < maxid; i++)
    {
        if(x >= ltr[i].xmin && 
           x <= ltr[i].xmax && 
           y >= ltr[i].ymin && 
           y <= ltr[i].ymax)
            return i;
    }
    return 0;
}

int is_grid(struct letter* ltr,int maxid, int** grid, struct letter corner1,
        struct letter corner2,int* finalx, int* finaly)
{
    int* found = malloc(maxid * sizeof(int));
    for(int i = 0; i < maxid; i++)
    {
        found[i] = 0;
    }
    for(int x = corner1.xmin; x < corner2.xmax; x++)
    {
        found[grid[x][corner1.ymid]] = 1;
    }
    int countx = 0;
    for(int i = 1; i < maxid; i++)
    {
        countx += found[i];
        found[i] = 0;
    }
    if(countx < 2)
        return 0;
    for(int y = corner1.ymin; y < corner2.ymax;y++)
    {
        found[grid[corner1.xmid][y]] = 1;
    }
    int county = 0;
    for(int i = 1; i < maxid; i++)
    {
        county += found[i];
        found[i] = 0;
    }
    if(county < 2)
        return 0;
    int* coords = malloc(maxid * sizeof(int));
    for(int i = 0; i < maxid;i++)
    {
        coords[i] = 0;
    }
    for(int y = corner1.ymin; y < corner2.ymax; y++)
    {
        coords[grid[corner1.xmid][y]] = ltr[grid[corner1.xmid][y]].ymid;
    }
    int y = 0;
    for(int i = 0; i < maxid; i++)
    {
        if(coords[i] == 0)
            continue;
        y = coords[i];
        for(int x = corner1.xmin; x < corner2.xmax; x++)
        {
            found[grid[x][y]] = 1;
        }
        int currcountx = 0;
        for(int i = 1; i < maxid; i++)
        {
            currcountx += found[i];
            found[i] = 0;
        }
        if(currcountx != countx)
            return 0;
    }
    int xi = 0;
    int yi = 0;
    //if(corner1.xmid == ltr[1].xmid && corner1.ymid == ltr[1].ymid)
        //printf("%i %i\n",county, countx);
    *finalx = countx;
    *finaly = county;
    free(coords);
    free(found);
    return 1;
}

void find_grid(struct letter* ltr, int maxid, int** grid,int* countx, 
        int* county, int* posxmin, int* posymin, int*posxmax, int*posymax)
{
    int* found = malloc(maxid * sizeof(int)); 
    *countx = 0;
    *county = 0;
    for(int i = 0; i < maxid; i++)
    {
        found[i] = 0;
    }
    for(int id = 0; id < maxid; id++)
    {
        if(ltr[id].xmid == 0)
            continue;
        for(int id2 = id; id2 < maxid; id2++)
        {
            if(ltr[id2].xmid == 0)
                continue;
            int x = 0;
            int y = 0;
            if(is_grid(ltr,maxid,grid,ltr[id],ltr[id2],&x,&y))
            {
                if(x * y > *countx * *county)
                {
                    *countx = x;
                    *county = y;
                    *posxmin = ltr[id].xmin;
                    *posymin = ltr[id].ymin;
                    *posxmax = ltr[id2].xmax;
                    *posymax = ltr[id2].ymax;
                }
            }
        }
    }
    int xid = 0;
    int yid = 0;
    int ligne = 0;
    for(int y = *posymin; y < *posymax;y++)
    {
        for(int x = *posxmin; x < *posxmax;x++)
        {
            if(grid[x][y])
            {
                y = ltr[grid[x][y]].ymid;
                ligne = (ltr[grid[x][y]].ymax - ltr[grid[x][y]].ymin);
            }
        }
        for(int x = *posxmin; x < *posxmax; x++)
        {
            if(found[grid[x][y]] == 0 && grid[x][y])
            {
                found[grid[x][y]] = grid[x][y];
                ltr[grid[x][y]].xgrid = xid;
                ltr[grid[x][y]].ygrid = yid;
                xid++;
                xid = xid % *countx;
                if(xid == 0)
                {
                    yid++;
                }
            }
        }
        if(ligne)
        {
            y += ligne;
            ligne = 0;
        }
    }
    free(found);
    printf("Grid position at (%i,%i) / (%i,%i) with %i letters on y and %i letters on x\n", *posymin,*posxmin,*posymax,*posxmax,*county,*countx);
}

void clear_not_letters(int** counts,int height,int width,int*** black_grid, 
        int maxid, int repet)
{
    for(int test = 0; test < repet; test++)
    {
        for(int i = 0; i < maxid; i++)
        {
            (*counts)[i] = 0;
        }
        for(int y = 0; y < height ; y++)
        {
            for(int x = 0; x < width ;x++)
            {
                if((*black_grid)[x][y])
                {
                    if(x < width / 100 || 
                       x > width * 99/100 || 
                       y < height / 100 || 
                       y > height * 99/100)
                    {
                        getridof(black_grid,(*black_grid)[x][y],width,height);
                    }
                    (*counts)[(*black_grid)[x][y]]++;
                }
            }
        }
        int thresholdmin = 0;
        int thresholdmax = 0;
        int total = 0;
        for(int i = 0; i < maxid; i++)
        {
            total += (*counts)[i];
        }
        thresholdmin = (total / maxid) / 6;
        thresholdmax = (total / maxid) * 4.5;
        for(int i = 0; i < maxid; i++)
        {
            if((*counts)[i] > thresholdmax || (*counts)[i] < thresholdmin)
            {
                getridof(black_grid,i,width,height);
            }
        }
    }
}

void remove_grid_letters(struct letter** letters, struct letter** gridletters,
        int x1, int y1, int x2, int y2, int maxid)
{
    struct letter init = {0,0,0,0,0,0,0,0};
    for(int id = 0; id < maxid; id++)
    {
        int x = (*letters)[id].xmid;
        int y = (*letters)[id].ymid;
        if(x > x1 && 
           x < x2 && 
           y > y1 && 
           y < y2 && 
           ((*letters)[id].xmax - (*letters)[id].xmin > 0 && 
            (*letters)[id].ymax - (*letters)[id].ymin > 0))
        {
            (*gridletters)[id] = (*letters)[id];
            (*letters)[id] = init;
        }
        else
        {
            (*gridletters)[id] = init;
        }
    }
}

unsigned char GetPixelComp32(SDL_Surface* surface,int x,int y,int c)
{ 
    // recupere le pixel x,y de la surface 
    // "surface", la composante c (3 composantes RGB)
    unsigned char *p = 
        ((unsigned char*)surface->pixels) + y * surface->pitch + x * 4;
    return p[c];
}

void PutPixelComp32(SDL_Surface* surface,int x,int y,int c,unsigned char val)
{ 
    // ecrit la composante c (3 composantes RGB) 
    // sur le pixel x,y de la surface "surface"
    unsigned char *p = 
        ((unsigned char*)surface->pixels) + y * surface->pitch + x * 4;
    p[c] = val;
}

void Stretch_Nearest(SDL_Surface* src,SDL_Surface* dest)
{
    int i,j,k;
    double rx,ry;
    rx = dest->w*1.0/src->w;
    ry = dest->h*1.0/src->h;
    for(i=0;i<dest->w;i++)
        for(j=0;j<dest->h;j++)
            for(k=0;k<3;k++)
            {
                unsigned char pix;
                pix = GetPixelComp32(src,(int)(i/rx),(int)(j/ry),k);
                PutPixelComp32(dest,i,j,k,pix);
            }
}

void Stretch_Linear(SDL_Surface* src,SDL_Surface* dest)
{
    int i,j,k;
    double rx,ry;
    rx = dest->w*1.0/src->w;
    ry = dest->h*1.0/src->h;
    for(i=0;i<dest->w;i++)
        for(j=0;j<dest->h;j++)
        {
            unsigned char pix;
            double valx,valy,fx,fy;
            int minx,miny,maxx,maxy;
            valx = i/rx;
            valy = j/ry;
            minx = (int)valx;
            miny = (int)valy;
            maxx = minx+1;
            if (maxx>=src->w)
                maxx--;
            maxy = miny+1;
            if (maxy>=src->h)
                maxy--;
            fx = valx-minx;  // garde partie flottante
            fy = valy-miny;
            for(k=0;k<3;k++)
            {
                pix = (unsigned char)(GetPixelComp32(src,minx,miny,k)*(1-fx)*
                        (1-fy) + GetPixelComp32(src,maxx,miny,k)*fx*(1-fy)
                        + GetPixelComp32(src,minx,maxy,k)*(1-fx)*fy + 
                        GetPixelComp32(src,maxx,maxy,k)*fx*fy);
                PutPixelComp32(dest,i,j,k,pix);
            }
        }
}

void Strechblit(SDL_Surface* src,SDL_Surface* dest)
{
    SDL_Surface* img = SDL_CreateRGBSurface(0,dest->w,dest->h,32,0,0,0,0);
    Stretch_Nearest(src,img);
    //Stretch_Linear(src,img);
    SDL_BlitSurface(img,NULL,dest,NULL);
    SDL_FreeSurface(img);
}

void split_grid(SDL_Surface *surface, struct letter* ltr, int maxid)
{
    system("exec rm -r ../letters/*");
    for(int i = 0; i < maxid; i++)
    {
        if(ltr[i].xmid != 0 && ltr[i].ymid != 0)
        {
            int w = (ltr[i].xmax - ltr[i].xmin);
            int h = (ltr[i].ymax - ltr[i].ymin);
            SDL_Surface* dest = SDL_CreateRGBSurface(0,
                    w,
                    h,
                    32,
                    surface->format->Rmask,
                    surface->format->Gmask,
                    surface->format->Bmask,
                    surface->format->Amask);
            if(dest == NULL)
                errx(EXIT_FAILURE,"%s",SDL_GetError());
            SDL_Rect let= {ltr[i].xmin,ltr[i].ymin,w,h};
            if(SDL_BlitSurface(surface,&let,dest,NULL) == -1)
                errx(EXIT_FAILURE,"%s",SDL_GetError());

            SDL_Surface* final = SDL_CreateRGBSurface(0,28,28,32,0,0,0,0);
            Strechblit(dest,final);
            char* path = malloc((24) * sizeof(char));
            path[23] = 0;
            path[0] = '.';
            path[1] = '.';
            path[2] = '/';
            path[3] = 'l';
            path[4] = 'e';
            path[5] = 't';
            path[6] = 't';
            path[7] = 'e';
            path[8] = 'r';
            path[9] = 's';
            path[10] = '/';
            path[11] = 'i';
            path[12] = 'm';
            path[13] = 'g';
            path[14] = '_';
            path[15] = 'n';
            if(i >= 1000)
            {
                errx(EXIT_FAILURE,"Pas pris en charge\n");
            }
            if(i >= 100)
            {
                path[16] = i / 100 + '0';
                path[17] = (i%100) / 10 + '0';
                path[18] = i%10 + '0';
                path[19] = '.';
                path[20] = 'j';
                path[21] = 'p';
                path[22] = 'g';
            }
            else if(i >= 10)
            {
                path[16] = i / 10 + '0';
                path[17] = i % 10 + '0';
                path[18] = '.';
                path[19] = 'j';
                path[20] = 'p';
                path[21] = 'g';
                path[22] = 0;
            }
            else
            {
                path[16] = i + '0';
                path[17] = '.';
                path[18] = 'j';
                path[19] = 'p';
                path[20] = 'g';
                path[21] = 0;
                path[22] = 0;
            }
            IMG_SaveJPG(final,path,1);
            free(path);
            SDL_FreeSurface(dest);
            SDL_FreeSurface(final);
        }
    }
}

void split_words(SDL_Surface *surface, struct word *words, 
        int nbWords, struct letter *ltr)
{
    system("exec rm -r ../words/*");
    for (int i = 0; i < nbWords; i++)
    {
        for (int j = 0; j < words[i].nbletters; j++)
        {
            int w =
                (ltr[words[i].letters[j]].xmax-ltr[words[i].letters[j]].xmin);
            int h =
                (ltr[words[i].letters[j]].ymax-ltr[words[i].letters[j]].ymin);
            SDL_Surface* dest = SDL_CreateRGBSurface(0,
                    w,
                    h,
                    32,
                    surface->format->Rmask,
                    surface->format->Gmask,
                    surface->format->Bmask,
                    surface->format->Amask);
            if(dest == NULL)
                errx(EXIT_FAILURE,"%s",SDL_GetError());
            SDL_Rect let= {ltr[words[i].letters[j]].xmin,
                ltr[words[i].letters[j]].ymin,w,h};
            if(SDL_BlitSurface(surface,&let,dest,NULL) == -1)
                errx(EXIT_FAILURE,"%s",SDL_GetError());

            SDL_Surface* final = SDL_CreateRGBSurface(0,28,28,32,0,0,0,0);
            Strechblit(dest,final);
            char* path = malloc((22) * sizeof(char));
            path[21] = 0;
            path[0] = '.';
            path[1] = '.';
            path[2] = '/';
            path[3] = 'w';
            path[4] = 'o';
            path[5] = 'r';
            path[6] = 'd';
            path[7] = 's';
            path[8] = '/';
            path[9] = 'i';
            path[10] = 'm';
            path[11] = 'g';
            path[12] = '_';
            path[13] = 'n';
            if(words[i].letters[j] >= 1000)
            {
                errx(EXIT_FAILURE,"Pas pris en charge\n");
            }
            else if(words[i].letters[j] >= 100)
            {
                path[14] = (words[i].letters[j]) / 100 + '0';
                path[15] = ((words[i].letters[j]) % 100) / 10 + '0';
                path[16] = (words[i].letters[j]) % 10 + '0';
                path[17] = '.';
                path[18] = 'j';
                path[19] = 'p';
                path[20] = 'g';
            }
            else if(words[i].letters[j] >= 10)
            {
                path[14] = (words[i].letters[j]) / 10 + '0';
                path[15] = (words[i].letters[j]) % 10 + '0';
                path[16] = '.';
                path[17] = 'j';
                path[18] = 'p';
                path[19] = 'g';
                path[20] = 0;
            }
            else
            {
                path[14] = words[i].letters[j] + '0';
                path[15] = '.';
                path[16] = 'j';
                path[17] = 'p';
                path[18] = 'g';
                path[19] = 0;
                path[20] = 0;
            }
            IMG_SaveJPG(final,path,1);
            free(path);
            SDL_FreeSurface(dest);
            SDL_FreeSurface(final);
        }
    }
}

SDL_Surface *find_list(SDL_Surface *surface, struct letter *ltr, int **connex,
        int maxid, struct word **words, size_t *nbWords)
{
    int dmin[maxid];
    for (int i = 0; i < maxid; i++)
        dmin[i] = surface->w;
    for (int i = 1; i < maxid; i++)
    {
        int x = ltr[i].xmax + 1;

        while (x < surface->w && !connex[x][ltr[i].ymid])
            x++;
        if (x < surface->w)
        {
            int d = ltr[connex[x][ltr[i].ymid]].xmin - ltr[i].xmax;
            if (dmin[i] > d)
                dmin[i] = d;
            if (dmin[connex[x][ltr[i].ymid]] > d)
                dmin[connex[x][ltr[i].ymid]] = d;
        }
    }
    for (int i = 1; i < maxid; i++)
    {
        //printf("%d\n", dmin[i]);
        if (dmin[i] > 2)
            getridof(&connex, i, surface->w, surface->h);
    }
    //printf("%d\n", maxid);
    surface_remove_white(surface, connex);

    SDL_Surface *surfaceTemp =
        SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 
                surface->format->Rmask, 
                surface->format->Gmask, 
                surface->format->Bmask, 
                surface->format->Amask);
    SDL_BlitSurface(surface,NULL,surfaceTemp,NULL);

    SDL_Surface *surfaceT =
        SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 
                surface->format->Rmask, 
                surface->format->Gmask, 
                surface->format->Bmask, 
                surface->format->Amask);
    SDL_BlitSurface(surface,NULL,surfaceT,NULL);

    dilatation2x(surface, surfaceTemp);
    dilatation2x(surfaceTemp, surface);
    dilatation2x(surface, surfaceTemp);
    dilatation2x(surfaceTemp, surface);
    
    //SDL_FreeSurface(surfaceTemp);

    int **dconnex = NULL;
    int dmaxid = 0;
    detect_connexe(surface, &dconnex, &dmaxid);
    int* counts = malloc(maxid*sizeof(int));
    for (int i = 0; i < maxid; i++)
        counts[i] = 0;
    for (int i = 0; i < surface->h; i++)
        for (int j = 0; j < surface->w; j++)
            if (dconnex[j][i])
                counts[dconnex[j][i]]++;

    struct letter* dltr = box_words(dmaxid,
            counts,
            dconnex,
            surface->h,
            surface->w);
    
    int ddmin[dmaxid];
    //printf("%d\n", dmaxid);
    for (int i = 0; i < dmaxid; i++)
        ddmin[i] = surface->h;
    for (int i = 1; i < dmaxid; i++)
    {
        int y = dltr[i].ymax + 1;

        while (y < surface->h && 
               !dconnex[dltr[i].xmin + ((dltr[i].xmax - dltr[i].xmid) / 2)][y])
            y++;
        if (y < surface->h)
        {
            int d = 
            dltr[dconnex[dltr[i].xmin+((dltr[i].xmax-dltr[i].xmid)/2)][y]].ymin
            -dltr[i].ymax;
            if (ddmin[i] > d)
                ddmin[i] = d;
            if(ddmin[dconnex[dltr[i].xmin + 
                    ((dltr[i].xmax - dltr[i].xmid) / 2)][y]] > d)
                ddmin[dconnex[dltr[i].xmin + 
                    ((dltr[i].xmax - dltr[i].xmid) / 2)][y]] = d;
        }
    }
    int k = 0;
    *words = malloc(dmaxid * sizeof(struct word));
    for (int i = 1; i < dmaxid; i++)
    {
        //printf("%d\n", ddmin[i]);
        if (ddmin[i] > 25 || ddmin[i] <= 0)
            getridof(&dconnex, i, surface->w, surface->h); 
        else 
        {
            (*words)[k].xmid = dltr[i].xmid;
            (*words)[k].ymid = dltr[i].ymid;
            (*words)[k].xmin = dltr[i].xmin;
            (*words)[k].ymin = dltr[i].ymin;
            (*words)[k].xmax = dltr[i].xmax;
            (*words)[k].ymax = dltr[i].ymax;
            (*words)[k].nbletters = 0;
            k++;
        }
    }
    surface_remove_white(surface, dconnex);

    for (int i = 0; i < k; i++)
    {
        int last = 0;
        //printf("mots numero : %d\n", i);
        for (int x = (*words)[i].xmin; x < (*words)[i].xmax; x++)
        {
            if (connex[x][(*words)[i].ymid] != 0 && 
                connex[x][(*words)[i].ymid] != last)
            {
                last = connex[x][(*words)[i].ymid];
                (*words)[i].nbletters += 1;
            }
        }
        (*words)[i].letters = malloc((*words)[i].nbletters * sizeof(int));
        last = 0;
        int c = 0;
        for (int x = (*words)[i].xmin; x < (*words)[i].xmax; x++)
        {
            if (connex[x][(*words)[i].ymid] != 0 && 
                connex[x][(*words)[i].ymid] != last)
            {
                last = connex[x][(*words)[i].ymid];
                (*words)[i].letters[c] = last;
                c++;
                //printf(" - %d\n", last);
            }
        }
    }
    *nbWords = (size_t)k;
    split_words(surfaceT, (*words), k, ltr);
    return surfaceT;
}

// Old main for tests
/*
int main(int argc, char** argv)
{
    if(argc != 2 && argc != 3)
        errx(EXIT_FAILURE,"Usage: <image> <show>");
    SDL_Surface *t = IMG_Load(argv[1]);
    if(t == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface *surface=SDL_ConvertSurfaceFormat(t,SDL_PIXELFORMAT_RGB888,0);
    SDL_FreeSurface(t);
    if(surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    int maxid = 0;
    int** black_grid = NULL;
    int width = (*surface).w;
    int height = (*surface).h;
    
    detect_connexe(surface, 
            &black_grid,
            &maxid);
    
    int* counts = malloc(maxid*sizeof(int));
    
    clear_not_letters(&counts,
            height,
            width,
            &black_grid,
            maxid,
            2);
    
    struct letter* letters = box_letters(maxid,
            counts,
            black_grid,
            height,
            width);
    int numletterx = 0;
    int numlettery = 0;
    int gridposx1 = 0;
    int gridposx2 = 0;
    int gridposy1 = 0;
    int gridposy2 = 0;
    
    find_grid(letters,
            maxid,
            black_grid,
            &numletterx,
            &numlettery,
            &gridposx1,
            &gridposy1,
            &gridposx2,
            &gridposy2);
    
    free(counts);
     
    struct letter* gridletter = malloc(maxid * sizeof(struct letter));
    remove_grid_letters(&letters,
            &gridletter,
            gridposx1, 
            gridposy1, 
            gridposx2, 
            gridposy2, 
            maxid);
        
    split_grid(surface,gridletter,maxid);
    
    erase_grid(&black_grid,
            surface,
            gridposx1,
            gridposy1,
            gridposx2,
            gridposy2);
    surface_remove_white(surface,black_grid);

    detect_connexe(surface, &black_grid,&maxid);
    int *c = malloc(maxid*sizeof(int));
    for (int i = 0; i < maxid; i++)
        c[i] = 0;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (black_grid[j][i])
                c[black_grid[j][i]]++;

    struct letter* ltr = box_letters(maxid,c,black_grid,height,width);
    struct word *words;
    size_t nbWords;
    SDL_Surface *bsurface = find_list(surface, ltr, black_grid,
            maxid, &words, &nbWords);
    free(c);
    
    if(argc == 3)
        return 1;
    if(SDL_Init(SDL_INIT_VIDEO)!=0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Window* window = SDL_CreateWindow("Surface_to_no_green",
            0,
            0,
            0,
            0,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Renderer* renderer = SDL_CreateRenderer(window,
            -1,
            SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_SetWindowSize(window,  surface->w, surface->h);


    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,bsurface);
    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);
    SDL_Event event;
    while (1)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return EXIT_SUCCESS;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                        SDL_RenderCopy(renderer,texture,NULL,NULL);
                        SDL_RenderPresent(renderer);
                }
                break;
        }
    }
    return EXIT_SUCCESS;
}
*/
