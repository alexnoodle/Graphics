/*Alex Schneider
01-12-17
Graphics*/

#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdarg.h>
#include "000pixel.h"
#include "stb_image.h"
#include "090matrix.c"
#include "070vector.c"
#include "040texture.c"

#define renVARYDIMBOUND 16

#include "090renderer.c"

#define renVERTNUMBOUND 16
#define renTEXNUMBOUND 5
#define renATTRX 0
#define renATTRY 1
#define renATTRS 2
#define renATTRT 3
#define renATTRR 4
#define renATTRG 5
#define renATTRB 6
#define renVARYX 0
#define renVARYY 1
#define renVARYS 2
#define renVARYT 3
#define renVARYR 4
#define renVARYG 5
#define renVARYB 6
#define renUNIFR 0
#define renUNIFG 1
#define renUNIFB 2
#define renUNIFTHETA 3
#define renUNIFTRANSX 4
#define renUNIFTRANSY 5
#define renUNIFISOMETRY 6
#define renUNIFTEX 16
#define renTEXR 0
#define renTEXG 1
#define renTEXB 2

/* Sets rgb, based on the other parameters, which are unaltered. attr is an 
interpolated attribute vector. */
void colorPixel(renRenderer *ren, double unif[], texTexture (*tex)[], 
        double vary[], double rgb[]) {
    texSample(&(*tex)[(int)unif[renUNIFTEX]], vary[renVARYS], vary[renVARYT]);
    rgb[0] = (*tex)[(int)unif[renUNIFTEX]].sample[renTEXR] * unif[renUNIFR];// * attr[renATTRR];
    rgb[1] = (*tex)[(int)unif[renUNIFTEX]].sample[renTEXG] * unif[renUNIFG];// * attr[renATTRG];
    rgb[2] = (*tex)[(int)unif[renUNIFTEX]].sample[renTEXB] * unif[renUNIFB];// * attr[renATTRB];
}

/* If unifParent is NULL, then sets the uniform matrix to the 
rotation-translation M described by the other uniforms. If unifParent is not 
NULL, but instead contains a rotation-translation P, then sets the uniform 
matrix to the matrix product P * M. */
void updateUniform(renRenderer *ren, double unif[], double unifParent[]) {
    if (unifParent == NULL){
        /* The nine uniforms for storing the matrix start at index 
        renUNIFISOMETRY. So &unif[renUNIFISOMETRY] is an array containing those 
        nine numbers. We use '(double(*)[3])' to cast it to a 3x3 matrix. */
        mat33Isometry(unif[renUNIFTHETA], unif[renUNIFTRANSX], 
            unif[renUNIFTRANSY], (double(*)[3])(&unif[renUNIFISOMETRY]));
    }
    else {
        double m[3][3];
        mat33Isometry(unif[renUNIFTHETA], unif[renUNIFTRANSX], 
            unif[renUNIFTRANSY], m);
        mat333Multiply((double(*)[3])(&unifParent[renUNIFISOMETRY]), m, 
            (double(*)[3])(&unif[renUNIFISOMETRY]));
    }
}

/* Writes the vary vector, based on the other parameters. */
void transformVertex(renRenderer *ren, double unif[], double attr[], 
        double vary[]) {
    /* Creates a scratch array to store values that are multiplied and
    placed in the vary vector */
    double change[3] = {attr[renATTRX], attr[renATTRY], 1};
    mat331Multiply((double(*)[3])(&unif[renUNIFISOMETRY]), change, vary);

    vary[renVARYS] = attr[renATTRS];
    vary[renVARYT] = attr[renATTRT];
}

#include "090triangle.c"
#include "090mesh.c"
#include "090scene.c"


/*Just some global variables*/
sceneNode nodeA, nodeB, nodeC, nodeD;
texTexture textureA, textureB, textureC;
texTexture *tex[3];
renRenderer ren;
meshMesh meshA, meshB, meshC, meshD;
double unif[17];

/*taken from 000linking.c. Changes the filter style and redraws the texture
when the enter key is pressed.*/
void handleKeyUp(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	printf("key up %d, shift %d, control %d, altOpt %d, supComm %d\n",
		key, shiftIsDown, controlIsDown, altOptionIsDown, superCommandIsDown);
		if(key == 257){
		    if(tex[0]->filtering == 0)
		        texSetFiltering(tex[0], 1);     
		    else
		        texSetFiltering(tex[0], 0);
		}
	    
}

/*taken from 000linking.c*/
void handleTimeStep(double oldTime, double newTime) {
	if (floor(newTime) - floor(oldTime) >= 1.0){
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
	}
	pixClearRGB(0.0, 0.0, 0.0);
	nodeA.unif[renUNIFTHETA] += newTime - oldTime;
	sceneRender(&nodeA, &ren, NULL);
}

/*This sets up the window and then draws 4 nodes 2 at the midlevel are
siblings so the overall height is 2. I seem to be obsessed with polar
bears at the moment*/
int main(void){
    if (pixInitialize(512, 512, "Pixel Stuff") != 0)
		return 1;
	else {
	    pixSetTimeStepHandler(handleTimeStep);
	    pixSetKeyUpHandler(handleKeyUp);
	    pixClearRGB(0.0, 0.0, 0.0);
        
        ren.texNum = 3;
        ren.unifDim = 17;
        ren.attrDim = 4;
        ren.varyDim = 4;
        ren.colorPixel = colorPixel;
        ren.transformVertex = transformVertex;
        ren.updateUniform = updateUniform;

        unif[renUNIFR] = 1.0;
        unif[renUNIFG] = 1.0;
        unif[renUNIFB] = 1.0;
        unif[renUNIFTHETA] = 0.0;
        unif[renUNIFTRANSX] = 256.0;
        unif[renUNIFTRANSY] = 256.0;
        unif[renUNIFISOMETRY] = 0.0;
        unif[renUNIFTEX] = 1.0;
	
	    char *pathA = "polar-bear-2.jpg";
	    char *pathB = "polar-bear.jpg";
	    char *pathC = "giraffe4.jpg";
	    
	    
	    tex[0] = &textureA;
	    tex[1] = &textureB;
	    tex[2] = &textureC;
	
	    meshInitializeEllipse(&meshA, 0.0, 0.0, 200.0, 100.0, 10.0);
	    meshInitializeEllipse(&meshB, -250.0, -100.0, 100.0, 50.0, 10.0);
	    meshInitializeEllipse(&meshC, -250.0, -400.0, 100.0, 50.0, 10.0);
	    meshInitializeRectangle(&meshD, 0.0, 200.0, 0.0, 200.0); 
    
        texInitializeFile(&textureA, pathA);
        texInitializeFile(&textureB, pathB);
        texInitializeFile(&textureC, pathC);
    
        unif[renUNIFTEX] = 1.0;
        sceneInitialize(&nodeC, &ren, unif, tex, &meshC, NULL, NULL);
        unif[renUNIFTEX] = 2.0;
        unif[renUNIFTRANSX] = -350.0;
        unif[renUNIFTRANSY] = -50.0;
        sceneInitialize(&nodeD, &ren, unif, tex, &meshD, NULL, NULL);
        unif[renUNIFTEX] = 1.0;
        unif[renUNIFTRANSX] = 256.0;
        unif[renUNIFTRANSY] = 256.0;
        sceneInitialize(&nodeB, &ren, unif, tex, &meshB, &nodeD, &nodeC);
        unif[renUNIFTEX] = 0.0;
        sceneInitialize(&nodeA, &ren, unif, tex, &meshA, &nodeB, NULL);
        
        sceneRender(&nodeA, &ren, NULL);
    
        printf("we made it to pixRun\n");
        pixRun();
    
        meshDestroy(&meshA);
        meshDestroy(&meshB);
        meshDestroy(&meshC);
        meshDestroy(&meshD);
        sceneDestroyRecursively(&nodeA);
        texDestroy(tex[0]);
        texDestroy(tex[1]);
        texDestroy(tex[2]);
        return 0;
    }
}