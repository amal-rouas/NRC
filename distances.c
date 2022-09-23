#include <stdio.h>
#include <dirent.h> 
#include <string.h>
#include "histogram.h"



void indexFolder(char* foldername);
void indexPGM(char* filePath);
void indexPPM(char* filePath);
char *get_filename_ext(char *filename);


int main() {
FILE *fpt;

fpt = fopen("comparison.csv", "w+");
fprintf(fpt, "image: euclidian_dist: bhattacharyya_istance\n");

    // fpt = fopen("Results.csv", "w+");
    // fprintf(fpt, "nom, contour, tauxDeRouge, tauxDeVert, tauxDeBleu, moyenneDuGradient, histogramme\n");

    // char* foldername = "archivePPMPGM";
    // indexFolder(foldername);
    // fclose(fpt);
    // char* toComparName = "1.jpg";
    // char* ppmImageName = "archivePPMPGM/archive500ppm/1.ppm";
    // calculate_distance_for_image(toComparName, ppmImageName);

    calculate_distance_for_all_images();

    return 0;
}

void indexFolder(char* path) {
    
    DIR * d = opendir(path); // open the path
    if(d==NULL) return; // if was not able, return
    struct dirent * dir; // for the directory entries
    while ((dir = readdir(d)) != NULL) // if we were able to read somehting from the directory
        {
        if(dir-> d_type != DT_DIR) { 
            // if the type is not directory just print it with blue color
            char filePath[255];
            sprintf(filePath, "%s/%s", path, dir->d_name);
            char* extension;
            extension = get_filename_ext(filePath);
            if ( strcmp(extension, "ppm") == 0 ) { 
                //ppm file
                indexPPM(filePath);
            } else if ( strcmp(extension, "pgm") == 0 ) {
                //pgm file
                indexPGM(filePath);
            }
            
        }
            else
            if(dir -> d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0 ) // if it is a directory
            {
                char d_path[255]; // here I am using sprintf which is safer than strcat
                sprintf(d_path, "%s/%s", path, dir->d_name);
                indexFolder(d_path); // recall with the new path
            }
        }
        
        closedir(d); // finally close the directory

}

void indexPPM(char* filePath) {

    //fprintf(fpt, "%s, %s, %s, %s, %s, %s, %s\n", filePath, filePath, filePath, filePath, filePath, filePath, filePath);
}

void indexPGM(char* filePath) {
    // long nrh, nrl, nch, ncl;
    // byte** I;
    // I = LoadPGM_bmatrix(filePath, &nrl, &nrh, &ncl, &nch);
    // fprintf(fpt, "%s, %d, %s, %s, %s, %s, %s\n", 
    // filePath, // nom
    // edgePixelCounter(I, nrl, nrh, ncl, nch), // contours
    // filePath, // taux rouge
    // filePath, // taux vert
    // filePath, // taux bleu
    // filePath, // moy grad
    // filePath); // histogramme
}

char *get_filename_ext(char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

void get_filename(char *filename, char* fileName) {
    int index = strcspn(filename, ".");
    char* name = malloc(sizeof(char) * 7);
    memset(name, '\0', sizeof(name));
    strncpy(name, filename, index+1);
    strcat(name, "jpg");
    strcpy(fileName, name);
    free(name);
}

void distance_matrix(){
    char* path = "archivePPMPGM/archive500ppm";

    FILE *fpt;
    fpt = fopen("comparison.csv", "w+");
    fprintf(fpt, "image, euclidian_dist, bhattacharyya_istance\n");

    DIR * d = opendir(path); // open the path
    if(d==NULL) return; // if was not able, return
    struct dirent * dir; // for the directory entries
    while ((dir = readdir(d)) != NULL) // if we were able to read somehting from the directory
        {
            if(dir-> d_type != DT_DIR) { 
            // if the type is not directory just print it with blue color
            char filePath[255];
            sprintf(filePath, "%s/%s", path, dir->d_name);

            char* extension;
            extension = get_filename_ext(filePath);
            if ( strcmp(extension, "ppm") == 0 ) {
                char name[7];
                get_filename(dir->d_name, name);
                fprintf(fpt, "%s, %s, %s\n", name, filePath, filePath);
            }
        }
            else
            if(dir -> d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0 ) // if it is a directory
            {
                char d_path[255]; // here I am using sprintf which is safer than strcat
                sprintf(d_path, "%s/%s", path, dir->d_name);
                indexFolder(d_path); // recall with the new path
            }
        }

        closedir(d); // finally close the directory
        }

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

            calculate_distance_for_image(fpt, name, filePath);

            free(filePath);
            free(name);
            
        }
    }
    closedir(d);
}

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

                 free(filePath);
                free(name);
        }
    }
    closedir(d);
}