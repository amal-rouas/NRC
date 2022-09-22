#ifndef DEF_HISTOGRAM
#define DEF_HISTOGRAM

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "histogram.h"

#include "def.h"
#include "nrio.h"
#include "nrarith.h"
#include "nralloc.h"



void histogramme(byte **img, long nrh, long nch, int *histogramme, FILE *file) {

    //initialise l'histogramme
    for (int i = 0; i < 256; i++) {
        histogramme[i] = 0;
    }

    //classifie l'image suivant les différents niveaux de gris
    for (int j = 0; j <= nrh; j++) {
        for (int k = 0; k <= nch; k++) {
            int index = img[j][k];
            histogramme[index] = histogramme[index] + 1;
        }
    }

    //sauvgarde de l'histogramme dans un fichier.csv
    for(int i = 0; i < 256; i++)
    {
        // printf("%d : %d\n", i, histogramme[i]);
        fprintf(file, "%d,", histogramme[i]);
    }
   
}


void histogrammeRGB(rgb8 **img, long nrh, long nch, int *histogrammeR, int * histogrammeG, int* histogrammeB, FILE *file) {

    //initialise l'histogramme
    for (int i = 0; i < 256; i++) {
        histogrammeR[i] = 0;
        histogrammeG[i] = 0;
        histogrammeB[i] = 0;
    }

    //classifie l'image suivant les différents niveaux de gris
    for (int j = 0; j <= nrh; j++) {
        for (int k = 0; k <= nch; k++) {
            int indexR = img[j][k].r;
            int indexG = img[j][k].g;
            int indexB = img[j][k].b;
            histogrammeR[indexR] = histogrammeR[indexR] + 1;
            histogrammeG[indexG] = histogrammeG[indexG] + 1;
            histogrammeB[indexB] = histogrammeB[indexB] + 1;
        }
    }
 
    //sauvgarde de l'histogramme dans un fichier.csv
    for(int i = 0; i < 256; i++)
    {
        // printf("%d : %d\n", i, histogrammeR[i]);
        // printf("%d : %d\n", i, histogrammeG[i]);
        // printf("%d : %d\n", i, histogrammeB[i]);

        fprintf(file, "%d,", histogrammeR[i]);
        fprintf(file, "%d,", histogrammeG[i]);
        fprintf(file, "%d,", histogrammeB[i]);
    }
}

double euclidienneDistance(int* histogramme1,int* histogramme2){
    double distance=0.0;
    for(int i=0;i < 256 ; i++){
        distance+=(histogramme1[i]-histogramme2[i])*(histogramme1[i]-histogramme2[i]);
    }
    distance=sqrt(distance);
    return distance;
}

double bhattacharyyaDistance(int* histogramme1,int* histogramme2){
    double distance=0.0;
    for(int i=0;i < 256 ; i++){
        distance+=sqrt(histogramme1[i]*histogramme2[i]);
    }
    distance=-log(distance);
    return distance;
}

byte** binarization(byte** orig, byte threshold, int nrl, int nrh, int ncl, int nch) {
    int i = 0, j = 0;
    byte **transformed = bmatrix(nrl, nrh, ncl, nch);

    for(i = 0 ; i < nrh ; i++) {
        for (j = 0 ; j < nch ; j++) {
            if(orig[i][j] < threshold) {
                transformed[i][j] = (byte) 0;
            }
            else {
                transformed[i][j] = (byte) 255;
            }
        }
    }

    return transformed;
}

int pixelCounter(byte** gradient, int nrl, int nrh, int ncl, int nch) {
    int sum = 0, i = 0, j = 0;
    for(i=nrl+1 ; i<nrh ; i++) {
        for(j=ncl+1 ; j<nch ; j++) {
            sum += gradient[i][j];
        }
    }
    return sum / 255;
}

byte** gradientNorm(byte** gradX, byte**gradY, int nrl, int nrh, int ncl, int nch) {
    int i = 0, j = 0;
    byte** gradient = bmatrix(nrl, nrh, ncl, nch);
    for(i=nrl+1 ; i<nrh ; i++) {
        for(j=ncl+1 ; j<nch ; j++) {
            double squaredX = pow((double) gradX[i][j], 2);
            double squaredY = pow((double) gradY[i][j], 2);
            int pixel = sqrt(squaredX + squaredY);
            gradient[i][j] = (byte) pixel;
        }
    }
    return gradient;
}

byte** convolution(byte** orig, int mask[3][3], int nrl, int nrh, int ncl, int nch, int fraction) {
    int i = 0, j = 0;
    byte** transformed = bmatrix(nrl, nrh, ncl, nch);

    for (long h = nrl + 1; h < nrh; h++)
    {
        for (long w = ncl + 1; w < nch; w++)
        { 
            long total = 0l;
            // top line
            total += orig[h - 1][w - 1] * mask[0][0];
            total += orig[h - 1][w] * mask[0][1];
            total += orig[h - 1][w + 1] * mask[0][2];
            // middle line
            total += orig[h][w - 1] * mask[1][0];
            total += orig[h][w] * mask[1][1];
            total += orig[h][w + 1] * mask[1][2];
            // bottom line
            total += orig[h + 1][w - 1] * mask[2][0];
            total += orig[h + 1][w] * mask[2][1];
            total += orig[h + 1][w + 1] * mask[2][2];
            
            total /= fraction;
            transformed[h][w] = total;
        }
    }

    return transformed;
}

int edgePixelCounter(byte** img, int nrl, int nrh, int ncl, int nch) {
    int mask_gradX[3][3] = {{-1, 0, +1}, {-2, 0, +2}, {-1, 0, +1}};
    int mask_gradY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {+1, +2, +1}};
    byte** gradX = convolution(img, mask_gradX, nrl, nrh, ncl, nch, 2);
    byte** gradY = convolution(img, mask_gradY, nrl, nrh, ncl, nch, 2);
    byte** gradient = gradientNorm(gradX, gradY, nrl, nrh, ncl, nch);
    byte** gradient_bin = binarization(gradient, 70, nrl, nrh, ncl, nch);

    int nb = pixelCounter(gradient_bin, nrl, nrh, ncl, nch);

    free_bmatrix(gradX, nrl, nrh, ncl, nch);
    free_bmatrix(gradY, nrl, nrh, ncl, nch);
    free_bmatrix(gradient, nrl, nrh, ncl, nch);
    free_bmatrix(gradient_bin, nrl, nrh, ncl, nch);

    return nb;
}

#endif