#include <stdio.h>
#include <dirent.h> 
#include <string.h>

#include "def.h"
#include "nrio.h"
#include "nrarith.h"
#include "nralloc.h"
#include "histogram.h"

void indexFolder(char* foldername);
void indexPGM(char* filePath);
void indexPPM(char* filePath);
char *get_filename_ext(char *filename);

FILE *fpt;

int main() {

    fpt = fopen("Results.csv", "w+");
    fprintf(fpt, "nom, contour, tauxDeRouge, tauxDeVert, tauxDeBleu, moyenneDuGradient, histogramme\n");

    char* foldername = "archivePPMPGM";
    indexFolder(foldername);
    fclose(fpt);

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

    fprintf(fpt, "%s, %s, %s, %s, %s, %s, %s\n", filePath, filePath, filePath, filePath, filePath, filePath, filePath);
}

void indexPGM(char* filePath) {
    long nrh, nrl, nch, ncl;
    byte** I;
    I = LoadPGM_bmatrix(filePath, &nrl, &nrh, &ncl, &nch);
    fprintf(fpt, "%s, %d, %s, %s, %s, %s, %s\n", 
    filePath, // nom
    edgePixelCounter(I, nrl, nrh, ncl, nch), // contours
    filePath, // taux rouge
    filePath, // taux vert
    filePath, // taux bleu
    filePath, // moy grad
    filePath); // histogramme
}

char *get_filename_ext(char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}