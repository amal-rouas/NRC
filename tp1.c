//tp1.c
#include<stdio.h>
#include<stdlib.h>

#include "def.h"
#include "nrio.h"
#include "nralloc.h"
#include "nrarith.h"



int main(){

	long nrh,nrl,nch,ncl;
	byte **I;
	byte **R;


	
	int i,j;
	
	I=LoadPGM_bmatrix("cubesx3.pgm",&nrl,&nrh,&ncl,&nch);
	
	
	R=bmatrix(nrl,nrh,ncl,nch);


	SavePGM_bmatrix(I,nrl,nrh,ncl,nch,"imageTest.pgm");
	
	free_bmatrix(I,nrl,nrh,ncl,nch);


	return 1;

}