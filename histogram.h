
#define FILTER_WIDTH 3
#define FILTER_HEIGHT 3

#include<stdio.h>
#include "def.h"
#include "nrio.h"
#include "nrarith.h"
#include "nralloc.h"

byte** binarization(byte** orig, byte threshold, int nrl, int nrh, int ncl, int nch);
int pixelCounter(byte** gradient, int nrl, int nrh, int ncl, int nch);
byte** gradientNorm(byte** gradX, byte**gradY, int nrl, int nrh, int ncl, int nch);
byte** convolution(byte** orig, int mask[3][3], int nrl, int nrh, int ncl, int nch, int fraction);
int edgePixelCounter(byte** img, int nrl, int nrh, int ncl, int nch);
int* histogramme(rgb8 **rgb8Img, long nrl, long nrh, long ncl, long nch);
double euclidienneDistance(int* histogramme1,int* histogramme2);
double bhattacharyyaDistance(int* histogramme1,int* histogramme2);
void colorRate(rgb8 **img, long nrl, long nrh, long ncl, long nch, double *rater, double *rateg, double *rateb);
double normGradientAverage(byte** img, long nrl, long nrh, long ncl, long nch);
