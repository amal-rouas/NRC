#include<stdio.h>
#include<stdlib.h>

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
    double distance = 0.0;
    for (int i = 0; i < 256; i++) {
        distance += sqrt(histogramme1[i] * histogramme2[i]);
    }
    distance = -log(distance);
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

//-------------------------------------- main -----------------------------------

int main()
{

    long nrh, nrl, nch, ncl;
    int *histogram, *histogramR, *histogramG, *histogramB;
    FILE *file;
    byte** I;
    rgb8** IRGB;
    double rater, rateg, rateb;

    I = LoadPGM_bmatrix("cubesx3.pgm", &nrl, &nrh, &ncl, &nch);

    file = fopen ("data.csv", "w");
    fprintf(file,"nom; couleur; contour; tauxderouge; tauxdevert; tauxdebleu; moyennedugradient; histogramme\n");

    histogram =(int *)malloc(sizeof(int)*256);
    histogramR =(int *)malloc(sizeof(int)*256);
    histogramG =(int *)malloc(sizeof(int)*256);
    histogramB =(int *)malloc(sizeof(int)*256);


    histogramme(I, nrh, nch, histogram, file);
    fputc(';', file);
    
    IRGB = LoadPPM_rgb8matrix("/home/amal/Documents/Master 2/Atelier Indexation d’images/archivePPMPGM/archive10ppm/arbre1.ppm", &nrl, &nrh, &ncl, &nch);
    histogrammeRGB(IRGB, nrh, nch, histogramR, histogramG, histogramB, file);

    printf("euclidienneDistance :%f \n",euclidienneDistance(histogramR, histogramR));
    printf("bhattacharyyaDistance :%f \n",bhattacharyyaDistance(histogramR, histogramR));

    colorRate(IRGB, nrl, nrh, ncl, nch, &rater, &rateg, &rateb);
    printf(" rater :%f\n rateg :%f\n rateb :%f \n", rater, rateg, rateb);


    free_bmatrix(I, nrl, nrh, ncl, nch);
    free_rgb8matrix(IRGB, nrl, nrh, ncl, nch);
    free(histogram);
    free(histogramR);
    free(histogramG);
    free(histogramB);
    fclose(file);

    return 0;
}