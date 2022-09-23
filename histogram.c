#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "histogram.h"

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


int* histogramme(rgb8 **rgb8Img, long nrl, long nrh, long ncl, long nch) {

    byte ** img = bmatrix(nrl, nrh, ncl, nch);
    int *histogramme = (int *)malloc(sizeof(int)*256);

    for(long i=0; i<=nrh; i++){
        for(long j=0; j<=nch; j++){
            img[i][j] =(byte)(0.3* rgb8Img[i][j].r + 0.59*rgb8Img[i][j].g + 0.11*rgb8Img[i][j].b);
        }
    }

    //initialise l'histogramme
    for (int i = 0; i < 256; i++) {
        histogramme[i] = 0;
    }

    //classifie l'image suivant les différents niveaux de gris
    for (int j = nrl; j <=nrh; j++) {
        for (int k = ncl; k <=nch; k++) {
            int index = img[j][k];
            histogramme[index] = histogramme[index] + 1;
        }
    }

    free_bmatrix(img, nrl, nrh, ncl, nch);
    return histogramme;
}

double euclidienneDistance(int* histogramme1,int* histogramme2){
    double distance=0.0;
    for(int i=0;i < 256 ; i++){
        distance+= pow((histogramme1[i]-histogramme2[i]), 2);
    }
    distance=sqrt(distance);
    return distance;
}

double bhattacharyyaDistance(int* histogramme1,int* histogramme2){

    double mean1 = 0.0;
    double mean2 = 0.0;
    for(int i = 0; i < 256; i++){
        mean1 += histogramme1[i];
        mean2 += histogramme2[i];
    }
    mean1 = mean1/255;
    mean2 = mean2/255;

    double distance = 0.0;
    for (int i = 0; i < 256; i++) {
        distance += sqrt(histogramme1[i] * histogramme2[i]);
    }
    distance = sqrt(1 - (1 / (sqrt(mean1*mean2*255*255))) *distance);
    return distance;

}

void colorRate(rgb8 **img, long nrl, long nrh, long ncl, long nch, double *rater, double *rateg, double *rateb) {
    (*rater) = 0.0;
    (*rateg) = 0.0;
    (*rateb) = 0.0;
    for (int i = 0; i < nrh-nrl; i++) {
        for (int j = 0; j < nch-ncl; j++) {
            (*rater) += img[i][j].r;
            (*rateg) += img[i][j].g;
            (*rateb) += img[i][j].b;
        }
    }
    int total=((*rater) + (*rateg) + (*rateb));
    (*rater) = (*rater) / total;
    (*rateg) = (*rateg) / total;
    (*rateb) = (*rateb) / total;

}

double normGradientAverage(byte** img, long nrl, long nrh, long ncl, long nch){
    double average = 0.0;
    int horizontalFilter[FILTER_WIDTH][FILTER_HEIGHT] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int verticalFilter[FILTER_WIDTH][FILTER_HEIGHT] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    byte** imgn = bmatrix(nrl, nrh, ncl, nch);

    
    for (long h = nrl + 1; h < nrh; h++)
    {
        for (long w = ncl + 1; w < nch; w++)
        {
            // horizontal gradient
            long totalH = 0l;
            // top line
            totalH += img[h - 1][w - 1] * horizontalFilter[0][0];
            totalH += img[h - 1][w] * horizontalFilter[0][1];
            totalH += img[h - 1][w + 1] * horizontalFilter[0][2];
            // middle line
            totalH += img[h][w - 1] * horizontalFilter[1][0];
            totalH += img[h][w] * horizontalFilter[1][1];
            totalH += img[h][w + 1] * horizontalFilter[1][2];
            // bottom line
            totalH += img[h + 1][w - 1] * horizontalFilter[2][0];
            totalH += img[h + 1][w] * horizontalFilter[2][1];
            totalH += img[h + 1][w + 1] * horizontalFilter[2][2];

            totalH = abs(totalH) / 4l;

            
            // vertical gradient
            long totalV = 0l;
            // top line
            totalV += img[h - 1][w - 1] * verticalFilter[0][0];
            totalV += img[h - 1][w] * verticalFilter[0][1];
            totalV += img[h - 1][w + 1] * verticalFilter[0][2];
            // middle line
            totalV += img[h][w - 1] * verticalFilter[1][0];
            totalV += img[h][w] * verticalFilter[1][1];
            totalV += img[h][w + 1] * verticalFilter[1][2];
            // bottom line
            totalV += img[h + 1][w - 1] * verticalFilter[2][0];
            totalV += img[h + 1][w] * verticalFilter[2][1];
            totalV += img[h + 1][w + 1] * verticalFilter[2][2];

            totalV = abs(totalV) / 4l;
            
            
            // gradient norm
            byte norm = sqrt((totalH * totalH) + (totalV * totalV));
            imgn[h][w] = norm;
        }
    }

    for (long h = nrl + 1; h < nrh; h++)
    {
        for (long w = ncl + 1; w < nch; w++)
        {
            average += imgn[h][w];
        }
    }

    average = average/((nrh + 1) * (nch + 1));

    free_bmatrix(imgn, nrl, nrh, ncl, nch);

    return average;
}