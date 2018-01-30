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
#include "130vector.c"
#include "130matrix.c"
#include "040texture.c"

#define renVARYDIMBOUND 16
double renRho;
double renPhi;
double renTheta;
double renTarget[3];

#include "110depth.c"
#include "130renderer.c"

#define renVERTNUMBOUND 1000
#define renTEXNUMBOUND 5
#define renATTRX 0
#define renATTRY 1
#define renATTRZ 2
#define renATTRS 3
#define renATTRT 4
#define renATTRVECX 5
#define renATTRVECY 6
#define renATTRVECZ 7
#define renVARYX 0
#define renVARYY 1
#define renVARYZ 2
#define renVARYW 3
#define renVARYS 4
#define renVARYT 5
#define renVARYI 6
#define renVARYJ 7
#define renVARYWORLDX 8
#define renVARYWORLDY 9
#define renVARYWORLDZ 10
#define renVARYVECX 11
#define renVARYVECY 12
#define renVARYVECZ 13
#define renVARYR 14
#define renVARYG 15
#define renVARYB 16
#define renUNIFR 0
#define renUNIFG 1
#define renUNIFB 2
#define renUNIFALPHA 3
#define renUNIFPHI 4
#define renUNIFTHETA 5
#define renUNIFTRANSX 6
#define renUNIFTRANSY 7
#define renUNIFTRANSZ 8
#define renUNIFISOMETRY 9
#define renUNIFVIEWING 25
#define renUNIFTEX 41
#define renUNIFLIGHTX 42
#define renUNIFLIGHTY 43
#define renUNIFLIGHTZ 44
#define renUNIFLIGHTR 45
#define renUNIFLIGHTG 46
#define renUNIFLIGHTB 47
#define renUNIFCAMERAX 48
#define renUNIFCAMERAY 49
#define renUNIFCAMERAZ 50
#define renUNIFSHINY 51
#define renTEXR 0
#define renTEXG 1
#define renTEXB 2

/* Sets rgb, based on the other parameters, which are unaltered. attr is an 
interpolated attribute vector. It also returns the current z value of the desired
pixel. */
void colorPixel(renRenderer *ren, double unif[], texTexture (*tex)[], 
        double vary[], double rgbz[]) {
    texSample(&(*tex)[(int)unif[renUNIFTEX]], vary[renVARYS], vary[renVARYT]);
    rgbz[0] = (*tex)[(int)unif[renUNIFTEX]].sample[renTEXR] * unif[renUNIFR];// * attr[renATTRR];
    rgbz[1] = (*tex)[(int)unif[renUNIFTEX]].sample[renTEXG] * unif[renUNIFG];// * attr[renATTRG];
    rgbz[2] = (*tex)[(int)unif[renUNIFTEX]].sample[renTEXB] * unif[renUNIFB];// * attr[renATTRB];
    rgbz[3] = depthGetZ(ren->depth, vary[renVARYI], vary[renVARYJ]);
    
    double normal[3] = {vary[renVARYVECX], vary[renVARYVECY], vary[renVARYVECZ]};
    double unitNormal[3];
    double lightvec[3] = {unif[renUNIFLIGHTX] - vary[renVARYWORLDX], unif[renUNIFLIGHTY] - 
    vary[renVARYWORLDY], unif[renUNIFLIGHTZ] - vary[renVARYWORLDZ]};
    
    double unitLightvec[3];
    
    vecUnit(3, normal, unitNormal);
    vecUnit(3, lightvec, unitLightvec);
    
    double dot = vecDot(3, unitNormal, unitLightvec);
    double difIntensity;
    
    if(dot > 0){
        difIntensity = dot;
    }
    else{
        difIntensity = 0;
    }
    
    double p[3];
    double r[3];
    vecScale(3, dot, unitNormal, p);
    vecScale(3, 2.0, p, p);
    vecSubtract(3, p, unitLightvec, r);
    double specIntensity;
    
    double cameravec[3] = {unif[renUNIFCAMERAX] - vary[renVARYWORLDX], unif[renUNIFCAMERAY] - 
    vary[renVARYWORLDY], unif[renUNIFCAMERAZ] - vary[renVARYWORLDZ]};
    double unitCameravec[3];
    vecUnit(3, cameravec, unitCameravec);
    
    dot = vecDot(3, unitCameravec, r);
    if(dot > 0){
        specIntensity = dot;
    }
    else{
        specIntensity = 0;
    }
    specIntensity = pow(specIntensity, unif[renUNIFSHINY]);
    rgbz[0] = rgbz[0] * difIntensity * unif[renUNIFLIGHTR] + specIntensity;
    rgbz[1] = rgbz[1] * difIntensity * unif[renUNIFLIGHTG] + specIntensity;
    rgbz[2] = rgbz[2] * difIntensity * unif[renUNIFLIGHTB] + specIntensity;
}

double lightX, lightY, lightZ;
double lightR, lightG, lightB;
/* If unifParent is NULL, then sets the uniform matrix to the 
rotation-translation M described by the other uniforms. If unifParent is not 
NULL, but instead contains a rotation-translation P, then sets the uniform 
matrix to the matrix product P * M. */
void updateUniform(renRenderer *ren, double unif[], double unifParent[]) {
    unif[renUNIFLIGHTX] = lightX;
    unif[renUNIFLIGHTY] = lightY;
    unif[renUNIFLIGHTZ] = lightZ;
    unif[renUNIFLIGHTR] = lightR;
    unif[renUNIFLIGHTG] = lightG;
    unif[renUNIFLIGHTB] = lightB; 
    
    double spherical[3];
    vec3Spherical(1.0, unif[renUNIFPHI], unif[renUNIFTHETA], spherical);
    vec3Spherical(renRho, renPhi, renTheta, (double(*))(&unif[renUNIFCAMERAX]));
    double rot[3][3];
    mat33AngleAxisRotation(unif[renUNIFALPHA], spherical, rot);
    double trans[3] = {unif[renUNIFTRANSX], unif[renUNIFTRANSY], unif[renUNIFTRANSZ]};

    if (unifParent == NULL){
        /* The sixteen uniforms for storing the matrix start at index 
        renUNIFISOMETRY. So &unif[renUNIFISOMETRY] is an array containing those 
        sixteen numbers. We use '(double(*)[4])' to cast it to a 4x4 matrix. The 
        viewing vector is then placed in the sixteen uniforms placed at 
        unif[renUNIFVIEWING]. */
        mat44Isometry(rot, trans, (double(*)[4])(&unif[renUNIFISOMETRY]));
    }
    else {
        double m[4][4];
        mat44Isometry(rot, trans, m);
        mat444Multiply((double(*)[4])(&unifParent[renUNIFISOMETRY]), m, (double(*)[4])(&unif[renUNIFISOMETRY]));
    }
    mat44Copy(ren->viewing, (double(*)[4])(&unif[renUNIFVIEWING]));
}

/* Writes the vary vector, based on the other parameters. */
void transformVertex(renRenderer *ren, double unif[], double attr[], 
        double vary[]) {
    /* Creates a scratch array to store values that are multiplied and
    placed in the vary vector */
    
    double change[4] = {attr[renATTRX], attr[renATTRY], attr[renATTRZ], 1.0};
    double temp[4];
    double result[4];
    mat441Multiply((double(*)[4])(&unif[renUNIFISOMETRY]), change, temp);
    vary[renVARYWORLDX] = temp[0];
    vary[renVARYWORLDY] = temp[1];
    vary[renVARYWORLDZ] = temp[2];
    
    double unitvec[4] = {attr[renATTRVECX], attr[renATTRVECY], attr[renATTRVECZ], 0.0};
    mat441Multiply((double(*)[4])(&unif[renUNIFISOMETRY]), unitvec, result);
    
    vary[renVARYVECX] = result[0];
    vary[renVARYVECY] = result[1];
    vary[renVARYVECZ] = result[2];
    
    mat441Multiply((double(*)[4])(&unif[renUNIFVIEWING]), temp, vary);
    
    vary[renVARYS] = attr[renATTRS];
    vary[renVARYT] = attr[renATTRT];
}

#include "140triangle.c"
#include "140clipping.c"
#include "140mesh.c"
#include "090scene.c"


/*Just some global variables*/
sceneNode nodeC, nodeD, nodeE, nodeF, nodeG, nodeH, nodeI, nodeJ, nodeK;
texTexture textureA, textureB, textureC, textureD;
texTexture texA[1];
texTexture *texB[1];
texTexture *texC[1];
texTexture *texD[1];
renRenderer ren;
int renPROJECTTYPE = renPERSPECTIVE;
meshMesh meshA, meshB, meshC;
double unif[3 + 3 + 3 + 16 + 16 + 1 + 3 + 3 + 3 + 1];
depthBuffer depth;

/*taken from 000linking.c. Changes the filter style and redraws the texture
when the enter key is pressed. It also alters the camera angle when the wasd keys
are pressed. w and s alter the theta value while a and d alter the phi value.*/
void handleKeyUp(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	printf("key up %d, shift %d, control %d, altOpt %d, supComm %d\n",
		key, shiftIsDown, controlIsDown, altOptionIsDown, superCommandIsDown);
		if(key == 257){
		    if(renPROJECTTYPE == renPERSPECTIVE){
		        renPROJECTTYPE = renORTHOGRAPHIC;
		        renSetFrustum(&ren, renPROJECTTYPE, M_PI / 6.0, renRho, 10.0); 
		    }   
		    else{
		        renPROJECTTYPE = renPERSPECTIVE;
		        renSetFrustum(&ren, renPROJECTTYPE, M_PI / 6.0, renRho, 10.0);
		    }
		}
		//d
		else if(key == 68)
		    renPhi += 0.2;
		//a
		else if(key == 65)
		    renPhi -= 0.2;
		//s
		else if(key == 83)
		    renTheta += 0.2;
		//w
		else if(key == 87)
		    renTheta -= 0.2;
		//q
		else if(key == 69)
		    renRho += 20.0;
		//e
		else if(key == 81)
		    renRho -= 20.0;
		    
	    
}

/* This draws the whole scene and is called by the time step handler. It clears the screen,
    clears the z-buffer, updates the camera viewing vector, and then renders all the 
    scene nodes. */
void draw(void){
    pixClearRGB(0.0, 0.0, 0.0);
    depthClearZs(&depth, -1000);
    renLookAt(&ren, renTarget, renRho, renPhi, renTheta);
    renUpdateViewing(&ren);
    //triRender(&ren, unif, &texA, aOne, bOne, cOne);
    //triRender(&ren, unif, &texA, aTwo, bTwo, cTwo);
    sceneRender(&nodeD, &ren, NULL);
}

/*taken from 000linking.c*/
void handleTimeStep(double oldTime, double newTime) {
	if (floor(newTime) - floor(oldTime) >= 1.0){
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
		printf("theta: %f\n", nodeD.unif[renUNIFTHETA]);
        printf("alpha: %f\n", nodeD.unif[renUNIFALPHA]);
        printf("phi: %f\n", nodeD.unif[renUNIFPHI]);
	}
	//nodeD.unif[renUNIFALPHA] += newTime - oldTime;
	draw();
}

/*This sets up the window and then draws 9 nodes in order to create a very
crude spaceship.*/
int main(void){
    if (pixInitialize(512, 512, "Pixel Stuff") != 0)
		return 1;
	else {
	    pixSetTimeStepHandler(handleTimeStep);
	    pixSetKeyUpHandler(handleKeyUp);
	    pixClearRGB(0.0, 0.0, 0.0);
        
        ren.texNum = 1;
        ren.unifDim = 3 + 3 + 3 + 16 + 16 + 1 + 3 + 3 + 3 + 1;
        ren.attrDim = 3 + 2 + 3;
        ren.varyDim = 4 + 2 + 2 + 3 + 3;
        ren.colorPixel = colorPixel;
        ren.transformVertex = transformVertex;
        ren.updateUniform = updateUniform;
        ren.depth = &depth;
        renRho = 1000.0;
        renPhi = 0.0;
        renTheta = 0.0;
        renTarget[0] = 0.0;
        renTarget[1] = 0.0;
        renTarget[2] = 0.0;
        
        if(depthInitialize(&depth, 512, 512) != 0)
	        printf("depth\n");
        renSetFrustum(&ren, renPROJECTTYPE, M_PI / 6.0, renRho, 5.0);
        printf("frustum is set\n");

        unif[renUNIFR] = 1.0;
        unif[renUNIFG] = 1.0;
        unif[renUNIFB] = 1.0;
        unif[renUNIFTHETA] = 0.0;
        unif[renUNIFALPHA] = 0.0;
        unif[renUNIFPHI] = 0.0;
        unif[renUNIFTRANSX] = 256.0;
        unif[renUNIFTRANSY] = 256.0;
        unif[renUNIFTRANSZ] = 256.0;
        unif[renUNIFISOMETRY] = 0.0;
        unif[renUNIFTEX] = 0.0;
        lightX = 1000.0;
        lightY = 1000.0;
        lightZ = 1000.0;
        lightR = 1.0;
        lightG = 1.0;
        lightB = 1.0;
        unif[renUNIFCAMERAX] = 0.0;
        unif[renUNIFCAMERAY] = 0.0;
        unif[renUNIFCAMERAZ] = 1000.0;
        unif[renUNIFSHINY] = 50.0;
	
	    char *pathA = "space.jpg";
	    char *pathB = "Spaceship-Texture.jpg";
	    char *pathC = "Spaceship-Texture.jpg";
	    char *pathD = "flame-2.jpg";
	    
	    
	    texA[0] = textureA;
	    texB[0] = &textureB;
	    texC[0] = &textureC;
	    texD[0] = &textureD;
	    
	    if(meshInitializeCapsule(&meshA, 50.0, 200.0, 4.0, 8.0) != 0)
	        printf("meshA\n");
	    if(meshInitializeCapsule(&meshC, 50.0, 100.0, 4.0, 8.0) != 0)
	        printf("meshA\n");
	    if(meshInitializeCapsule(&meshB, 100.0, 300.0, 4.0, 8.0) != 0)
	        printf("meshB\n");
        if(texInitializeFile(&textureA, pathA) != 0)
            printf("tex[0]\n");
        if(texInitializeFile(&textureB, pathB) != 0)
            printf("tex[1]\n");
        if(texInitializeFile(&textureC, pathC) != 0)
            printf("tex[2]\n");
        if(texInitializeFile(&textureD, pathD) != 0)
            printf("tex[3]\n");
        
        unif[renUNIFTRANSX] = 100.0;
        unif[renUNIFTRANSY] = 100.0;
        unif[renUNIFTRANSZ] = -100.0;
        if(sceneInitialize(&nodeC, &ren, unif, texC, &meshA, NULL, NULL) != 0)
            printf("nodeC\n");
        unif[renUNIFTRANSX] = -100.0;
        unif[renUNIFTRANSY] = 100.0;
        if(sceneInitialize(&nodeE, &ren, unif, texC, &meshA, NULL, &nodeC) != 0)
            printf("nodeE\n");
        unif[renUNIFTRANSX] = 100.0;
        unif[renUNIFTRANSY] = -100.0;
        if(sceneInitialize(&nodeF, &ren, unif, texC, &meshA, NULL, &nodeE) != 0)
            printf("nodeF\n");
        unif[renUNIFTRANSX] = -100.0;
        unif[renUNIFTRANSY] = -100.0;
        if(sceneInitialize(&nodeG, &ren, unif, texC, &meshA, NULL, &nodeF) != 0)
            printf("nodeG\n");
            
        unif[renUNIFTRANSX] = 100.0;
        unif[renUNIFTRANSY] = 100.0;
        unif[renUNIFTRANSZ] = -200.0;
        if(sceneInitialize(&nodeH, &ren, unif, texD, &meshC, NULL, &nodeG) != 0)
            printf("nodeH\n");
        unif[renUNIFTRANSX] = -100.0;
        unif[renUNIFTRANSY] = 100.0;
        if(sceneInitialize(&nodeI, &ren, unif, texD, &meshC, NULL, &nodeH) != 0)
            printf("nodeI\n");
        unif[renUNIFTRANSX] = 100.0;
        unif[renUNIFTRANSY] = -100.0;
        if(sceneInitialize(&nodeJ, &ren, unif, texD, &meshC, NULL, &nodeI) != 0)
            printf("nodeJ\n");
        unif[renUNIFTRANSX] = -100.0;
        unif[renUNIFTRANSY] = -100.0;
        if(sceneInitialize(&nodeK, &ren, unif, texD, &meshC, NULL, &nodeJ) != 0)
            printf("nodeK\n");
        
        unif[renUNIFTRANSX] = 0.0;
        unif[renUNIFTRANSY] = 0.0;
        unif[renUNIFTRANSZ] = 0.0;
        if(sceneInitialize(&nodeD, &ren, unif, texB, &meshB, &nodeK, NULL) != 0)
            printf("nodeD\n");
        
        renLookAt(&ren, renTarget, renRho, renPhi, renTheta);
        renUpdateViewing(&ren);
        printf("renderer is fully updated\n");
        sceneRender(&nodeD, &ren, NULL);
        printf("scene has been renderered\n");
        pixRun();
    
        meshDestroy(&meshA);
        meshDestroy(&meshB);
        
        sceneDestroyRecursively(&nodeD);
        texDestroy(&texA[0]);
        texDestroy(texB[0]);
        texDestroy(texC[0]);
        texDestroy(texD[0]);
        return 0;
    }
}