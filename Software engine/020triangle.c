/*Alex Schneider
01-07-17
Graphics*/

#include <stdio.h>
#include <math.h>
#include "000pixel.h"

/*this draws the triangle, one half at a time*/
void rasterizeTypeA(double a0, double a1, double b0, double b1, double c0,
    double c1, double r, double g, double b){
        double aSlope = (b1 - a1)/(b0 - a0);
        double aInt = a1 - (aSlope * a0);
        /*This loop dictates the first half of the triangle to draw, from point a on 
        the x axis to point c on the x axis*/
        for(int i = (int)ceil(a0); i <= (int)floor(c0); i += 1){
            if(a0 == c0)
                i = (int)floor(c0) + 1;
            else{
                double cSlope = (c1 - a1)/(c0 - a0);
                double cInt = c1 - (cSlope * c0);
                /*This loop dictates which pixels are between the line going from a to b and the
                line going from a to c and then turns them on with the appropriate color values*/
                for(int j = (int)ceil((aSlope * i) + aInt); j <= (int)floor((cSlope * i) + cInt); j += 1){
                    pixSetRGB(i, j, r, g, b);
                }
            }
        }
        /*This loop dictates the second half of the triangle to draw, from point c on 
        the x axis to point b on the x axis*/
        for(int i = (int)ceil(c0); i <= (int)floor(b0); i += 1){
            double bSlope = (b1 - c1)/(b0 - c0);
            double bInt = b1 - (bSlope * b0);
            /*This loop dictates which pixels are between the line going from a to b and the
            line going from c to b and then turns them on with the appropriate color values*/
            for(int j = (int)ceil((aSlope * i) + aInt); j <= (int)floor((bSlope * i) + bInt); j += 1){
                pixSetRGB(i, j, r, g, b);
            }
        }
    }

/*this draws the triangle, one half at a time*/ 
void rasterizeTypeB(double a0, double a1, double b0, double b1, double c0,
    double c1, double r, double g, double b){
        double cSlope = (c1 - a1)/(c0 - a0);
        double cInt = c1 - (cSlope * c0);
        /*This loop dictates the first half of the triangle to draw, from point a on 
        the x axis to point b on the x axis*/
        for(int i = (int)ceil(a0); i <= (int)floor(b0); i += 1){
            double aSlope = (b1 - a1)/(b0 - a0);
            double aInt = a1 - (aSlope * a0);
            /*This loop dictates which pixels are between the line going from a to b and the
            line going from a to c and then turns them on with the appropriate color values*/
            for(int j = (int)ceil((aSlope * i) + aInt); j <= (int)floor((cSlope * i) + cInt); j += 1){
                pixSetRGB(i, j, r, g, b);
            }
        }
        /*This loop dictates the second half of the triangle to draw, from point b on 
        the x axis to point c on the x axis*/
        for(int i = (int)ceil(b0); i <= (int)floor(c0); i += 1){
            if(b0 == c0)
                i = (int)floor(c0) + 1;
            else{
                double bSlope = (c1 - b1)/(c0 - b0);
                double bInt = b1 - (bSlope * b0);
                /*This loop dictates which pixels are between the line going from b to c and the
                line going from a to c and then turns them on with the appropriate color values*/
                for(int j = (int)ceil((bSlope * i) + bInt); j <= (int)floor((cSlope * i) + cInt); j += 1){
                    pixSetRGB(i, j, r, g, b);
                }
            }
        }
    }

/*This is where the points and color values are either passed to one rasterize method or the other.
The difference between the two methods is that for rasterizeTypeA, c is between a and b
on the x axis, while for rasterizeTypeB, c is the farthest point to the left on the
x axis.*/
void separate(double a0, double a1, double b0, double b1, double c0,
    double c1, double r, double g, double b){
        if(c0 < b0)
            rasterizeTypeA(a0, a1, b0, b1, c0, c1, r, g, b);
        else
            rasterizeTypeB(a0, a1, b0, b1, c0, c1, r, g, b);
    }

/*This is where the data is initially passed into. The logic is there to figure out
where a, b, and c are and then align them to the convention of a being the point farthest
to the left. if the a passed in isn't farthest to the left then the point that is is found
and designated as the new a and the other points are shuffled around accordingly
while being passed to the separate method.*/
void triRender(double a0, double a1, double b0, double b1, double c0,
    double c1, double r, double g, double b){
        if(a0 < b0){
            if(a0 < c0)
                separate(a0, a1, b0, b1, c0, c1, r, g, b);
            else
                separate(c0, c1, a0, a1, b0, b1, r, g, b);
        }
        else if (b0 < a0){
            if(b0 < c0)
                separate(b0, b1, c0, c1, a0, a1, r, g, b);
            else
                separate(c0, c1, a0, a1, b0, b1, r, g, b);
        }
        else{
            if(a0 < c0)
                separate(b0, b1, c0, c1, a0, a1, r, g, b);
            else
                separate(c0, c1, a0, a1, b0, b1, r, g, b);
        }
    }
