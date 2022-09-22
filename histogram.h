#ifndef DEF_HISTOGRAM
#define DEF_HISTOGRAM

#include<stdio.h>

#include "def.h"
#include "nrio.h"
#include "nrarith.h"
#include "nralloc.h"

int horizontal_gradient[3][3] = {
    {-1,0,1},
    {-2,0,2},
    {-1,0,1}
};

int vertical_gradient[3][3] = {
    {-1,-2,-1},
    {0,0,0},
    {1,2,1}
};

void histogramme(byte **img, long nrh, long nch, int *histogramme, FILE *file);
void histogrammeRGB(rgb8 **img, long nrh, long nch, int *histogrammeR, int * histogrammeG, int* histogrammeB, FILE *file);
double euclidienneDistance(int* histogramme1,int* histogramme2);
double bhattacharyyaDistance(int* histogramme1,int* histogramme2);
byte** binarization(byte** orig, byte threshold, int nrl, int nrh, int ncl, int nch);
int pixelCounter(byte** gradient, int nrl, int nrh, int ncl, int nch);
byte** gradientNorm(byte** gradX, byte**gradY, int nrl, int nrh, int ncl, int nch);
byte** convolution(byte** orig, int mask[3][3], int nrl, int nrh, int ncl, int nch, int fraction);
int edgePixelCounter(byte** img, int nrl, int nrh, int ncl, int nch);

#endif