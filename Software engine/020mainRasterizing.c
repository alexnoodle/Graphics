/*Alex Schneider
01-07-17
Graphics*/

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "000pixel.h"
#include "020triangle.c"
int testNum = 0;

double red = 0.2;
double green = 0.2;
double blue = 0.2;

/*Every time t is pressed on the keyboard this tests all the cases where the actual 
values of a, b, and c are the same or different from the convention where a is on 
the right for the A type triangles and B type triangles (for the A type b is left 
of c and for the B type it's the other way around. I did make up this terminology 
but it works so I'm sticking with it) Also it clears the screen and prints two more 
triangles whose sides are vertical*/
void test(void){	
	if(testNum == 0){
	    red = 0.4;
	    green = 0.4;
	    blue = 0.4;
	
	    triRender(25.0, 25.0, 451.0, 130.0, 297.0, 345.0, red, green, blue);
	    printf("case one\n");
	}
	
	else if(testNum == 1){
	    red = 0.6;
	    green = 0.6;
	    blue = 0.6;
	    
	    triRender(317.0, 365.0, 45.0, 45.0, 471.0, 150.0, red, green, blue);
	    printf("case two\n");
	}
	
	else if(testNum == 2){
	    red = 0.8;
	    green = 0.8;
	    blue = 0.8;
	    
	    triRender(491.0, 170.0, 337.0, 385.0, 65.0, 65.0, red, green, blue);
	    printf("case three\n");
	}
	
	else if(testNum == 3){
	    red = 0.4;
	    green = 0.4;
	    blue = 0.4;
	    
	    triRender(25.0, 245.0, 350.0, 130.0, 450.0, 445.0, red, green, blue);
	    printf("case four\n");
	}
	
	else if(testNum == 4){
	    red = 0.6;
	    green = 0.6;
	    blue = 0.6;
	    
	    triRender(470.0, 465.0, 45.0, 265.0, 370.0, 150.0, red, green, blue);
	    printf("case five\n");
	}
	
	else if(testNum == 5){
	    red = 0.8;
	    green = 0.8;
	    blue = 0.8;
	    
	    triRender(390.0, 170.0, 490.0, 485.0, 65.0, 285.0, red, green, blue);
	    printf("case six\n");
	}
	
	else if(testNum == 6){
	    pixClearRGB(0.0, 0.0, 0.0);
	    red = 0.3;
	    green = 0.3;
	    blue = 0.3;
	    
	    triRender(50.0, 50.0, 300.0, 60.0, 50.0, 300.0, red, green, blue);
	    printf("case six\n");
	}
	
	else if(testNum == 7){
	    red = 0.6;
	    green = 0.6;
	    blue = 0.6;
	    
	    triRender(50.0, 50.0, 300.0, 60.0, 300.0, 300.0, red, green, blue);
	    printf("case six\n");
	}
	
	else
	    testNum = 0;
} 

/*taken from 000linking.c*/
void handleKeyUp(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	printf("key up %d, shift %d, control %d, altOpt %d, supComm %d\n",
		key, shiftIsDown, controlIsDown, altOptionIsDown, superCommandIsDown);
	if(key == 84)
	    test();
	    testNum += 1;
	    
}

/*taken from 000linking.c*/
void handleTimeStep(double oldTime, double newTime) {
	if (floor(newTime) - floor(oldTime) >= 1.0)
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
}

/*This sets up the window and then draws one triangle to prove that it's possible. Also
if you press t on the keyboard repeatedly then it will cycle through other triangles of
other shades and locations*/
int main(void){
    if (pixInitialize(512, 512, "Pixel Stuff") != 0)
		return 1;
	else {
	    pixSetTimeStepHandler(handleTimeStep);
	    pixSetKeyUpHandler(handleKeyUp);
	    pixClearRGB(0.0, 0.0, 0.0);
	    
	    
	    red = 0.0;
	    green = 1.0;
	    blue = 1.0;
	    
	    triRender(25.0, 25.0, 451.0, 130.0, 297.0, 345.0, red, green, blue);
	    
        pixRun();
        return 0;
    }
}