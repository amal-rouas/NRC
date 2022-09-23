#include <stdio.h>
#include <dirent.h> 
#include <string.h>
#include "histogram.h"



void indexFolder(char* foldername);
char *get_filename_ext(char *filename);
int main() {
    FILE *fpt;
    fpt = fopen("comparison.csv", "w+");
    fprintf(fpt, "image: euclidian_dist: bhattacharyya_istance\n");
    calculate_distance_for_all_images();
    return 0;
}

// get file extension
char *get_filename_ext(char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

// get file name
void get_filename(char *filename, char* fileName) {
    int index = strcspn(filename, ".");
    char* name = malloc(sizeof(char) * 7);
    memset(name, '\0', sizeof(name));
    strncpy(name, filename, index+1);
    strcat(name, "jpg");
    strcpy(fileName, name);
    free(name);
}

// calculate the distance between all the images
calculate_distance_for_all_images(){
    char* path = "archivePPMPGM/archive500ppm";
    DIR * d = opendir(path);
    if(d==NULL) return;
    struct dirent * dir;
    FILE *fpt;
    fpt = fopen("comparison.csv", "a");
    while ((dir = readdir(d)) != NULL){
        if(dir-> d_type != DT_DIR) { 

            char* filePath = malloc(sizeof(char) *255);
            char* name = malloc(sizeof(char) *7);
            //file path

            //file name
            get_filename(dir->d_name, name);
            sprintf(filePath, "%s/%s", path, dir->d_name);

            printf("filePath: %s\n", filePath);
            printf("name: %s\n", name);

            calculate_distance_for_image(fpt, name, filePath);

            free(filePath);
            free(name);
            
        }
    }
    closedir(d);
}

//calculate the distance between an image and all the images in the directory 
calculate_distance_for_image(FILE *fpt, char* toComparName, char* ppmImageName){

    char* path = "archivePPMPGM/archive500ppm";
    DIR * d = opendir(path);
    if(d==NULL) return;
    struct dirent * dir;

    while ((dir = readdir(d)) != NULL){
        if(dir-> d_type != DT_DIR) { 

            char* filePath = malloc(sizeof(char) *255);
            char* name = malloc(sizeof(char) *7);
            //file path
            sprintf(filePath, "%s/%s", path, dir->d_name);

            //file name
            get_filename(dir->d_name, name);


            long nrh, nrl, nch, ncl;
            rgb8** I;
            I = LoadPPM_rgb8matrix(ppmImageName, &nrl, &nrh, &ncl, &nch);
            long nrh1, nrl1, nch1, ncl1;

            rgb8** I1;
            I1 = LoadPPM_rgb8matrix(filePath, &nrl1, &nrh1, &ncl1, &nch1);

            int* hist = histogramme(I, nrl, nrh, ncl, nch);
            int* hist1 = histogramme(I1, nrl1, nrh1, ncl1, nch1);

            double euc = euclidienneDistance(hist, hist1);
            double bat = bhattacharyyaDistance(hist, hist1);

            fprintf(fpt,"%s:%s:%f:%f\n", toComparName, name, euc, bat);

            free_rgb8matrix(I, nrl, nrh, ncl, nch);
            free_rgb8matrix(I1, nrl1, nrh1, ncl1, nch1);
            free(filePath);
            free(name);
            free(hist);
            free(hist1);
        }
    }
    closedir(d);
}