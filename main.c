#include <stdio.h>
#include <dirent.h> 
#include <string.h>

void indexFolder(char* foldername);
void indexPGM(char* filePath);
void indexPPM(char* filePath);
char *get_filename_ext(char *filename);

FILE *fpt;

int main() {

    char* foldername = "archivePPMPGM";
    indexFolder(foldername);

    return 0;
}

void indexFolder(char* path) {

    fpt = fopen("Results.csv", "w+");
    fprintf(fpt, "nom, couleur, contour, tauxDeRouge, tauxDeVert, tauxDeBleu, moyenneDuGradient, histogramme\n");
    
    DIR * d = opendir(path); // open the path
    if(d==NULL) return; // if was not able, return
    struct dirent * dir; // for the directory entries
    while ((dir = readdir(d)) != NULL) // if we were able to read somehting from the directory
        {
        if(dir-> d_type != DT_DIR) {
            // if the type is not directory just print it with blue color
            //printf("FILE : %s\n", dir->d_name);
            char filePath[255];
            sprintf(filePath, "%s%s", path, dir->d_name);
            char* extension;
            extension = get_filename_ext(filePath);
            
            if ( strcmp(extension, "ppm") == 0 ) { 
                //printf("%s : %s\n", extension, filePath);
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
                //printf("DIRECTORY : %s\n", dir->d_name); // print its name in green
                char d_path[255]; // here I am using sprintf which is safer than strcat
                sprintf(d_path, "%s/%s", path, dir->d_name);
                indexFolder(d_path); // recall with the new path
            }
        }
        fclose(fpt);
        closedir(d); // finally close the directory

}

void indexPPM(char* filePath) {
    //printf("PPM FILE : %s\n", filePath);

    //fprintf(fpt, "%s, %s, %s, %s, %s, %s, %s, %s\n", filePath, filePath, filePath, filePath, filePath, filePath, filePath, filePath);
}

void indexPGM(char* filePath) {
    //printf("PGM FILE : %s\n", filePath);
}

char *get_filename_ext(char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}