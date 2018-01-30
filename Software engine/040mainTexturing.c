/*Alex Schneider
01-12-17
Graphics*/

#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "000pixel.h"
#include "stb_image.h"
#include "030matrix.c"
#include "030vector.c"
#include "040texture.c"
#include "040triangle.c"


/*Just some global variables for rgb*/
double red = 0.2;
double green = 0.2;
double blue = 0.2;

texTexture tex;

/*This method clears the screen and redraws the texture with the opposite kind of
filtering. The texture coordinates are made smaller to make it the differences between
the filters clear.*/
void reDraw(void){
    pixClearRGB(0.0,0.0,0.0);
    
    double alpha[2] = {0.8, 1.0};
	double beta[2] = {1.0, 0.0};
	double gamma[2] = {1.0, 1.0};
	double rgb[3] = {red, green, blue};
	double a[2] = {10.0, 500.0};
	double b[2] = {250.0, 10.0};
	double c[2] = {500.0, 500.0};
	
	triRender(a, b, c, rgb, &tex, alpha, beta, gamma);
    
}

/*taken from 000linking.c. Changes the filter style and redraws the texture
when the enter key is pressed.*/
void handleKeyUp(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	printf("key up %d, shift %d, control %d, altOpt %d, supComm %d\n",
		key, shiftIsDown, controlIsDown, altOptionIsDown, superCommandIsDown);
		if(key == 257){
		    if(tex.filtering == 0)
		        texSetFiltering(&tex, 1);     
		    else{
		        texSetFiltering(&tex, 0);
		    }
		    reDraw();
		}
	    
}

/*taken from 000linking.c*/
void handleTimeStep(double oldTime, double newTime) {
	if (floor(newTime) - floor(oldTime) >= 1.0)
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
}

/*This sets up the window and then draws one triangle to prove that it's possible. 
Currently the texture inside the triangle is a lovely giraffe scene.*/
int main(void){
    if (pixInitialize(512, 512, "Pixel Stuff") != 0)
		return 1;
	else {
	    pixSetTimeStepHandler(handleTimeStep);
	    pixSetKeyUpHandler(handleKeyUp);
	    pixClearRGB(0.0, 0.0, 0.0);
	    
	    
	    red = 1.0;
	    green = 1.0;
	    blue = 1.0;
	    double alpha[2] = {0.0, 1.0};
	    double beta[2] = {0.5, 0.0};
	    double gamma[2] = {1.0, 1.0};
	    double rgb[3] = {red, green, blue};
	    double a[2] = {10.0, 500.0};
	    double b[2] = {250.0, 10.0};
	    double c[2] = {500.0, 500.0};
	    
	    char *path = "giraffe4.jpg";
	    
	    if(texInitializeFile(&tex, path) != 1){
	    
	        triRender(a, b, c, rgb, &tex, alpha, beta, gamma);
            pixRun();
        }
        return 0;
    }
}