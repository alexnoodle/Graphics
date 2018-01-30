/*Alex Schneider
01-07-17
Graphics*/

#include <stdio.h>
#include <math.h>

/*this draws the triangle, one half at a time*/
void rasterizeTypeA(double a[2], double b[2], double c[2], double rgb[3], texTexture *tex,
     double alpha[3], double beta[3], double gamma[3]){
        double aSlope = (b[1] - a[1])/(b[0] - a[0]);
        double aInt = a[1] - (aSlope * a[0]);
        /*This loop dictates the first half of the triangle to draw, from point a on 
        the x axis to point c on the x axis*/
        for(int i = (int)ceil(a[0]); i <= (int)floor(c[0]); i += 1){
            if(a[0] == c[0]){
                i = (int)floor(c[0]) + 1;
            }
            else{
                double cSlope = (c[1] - a[1])/(c[0] - a[0]);
                double cInt = c[1] - (cSlope * c[0]);
                /*This loop dictates which pixels are between the line going from a to b and the
                line going from a to c and then turns them on with the appropriate color values
                taken from the texture struct.*/
                for(int j = (int)ceil((aSlope * i) + aInt); j <= (int)floor((cSlope * i) + cInt); j += 1){
                    double toInv[2][2] = {{(b[0] - a[0]), (c[0] - a[0])}, {(b[1] - a[1]), (c[1] - a[1])}};
                    double xMinusA[2] = {(i - a[0]), (j - a[1])};
                    double inv[2][2];
                    mat22Invert(toInv, inv);
                    double pq[2];
                    mat22Multiply(inv, xMinusA, pq);
                    
                    double chi[2];
                    double pTimesBetaMinusAlpha[2];
                    vecSubtract(2, beta, alpha, pTimesBetaMinusAlpha);
                    vecScale(2, pq[0], pTimesBetaMinusAlpha, pTimesBetaMinusAlpha);
                    
                    double qTimesGammaMinusAlpha[2];
                    vecSubtract(2, gamma, alpha, qTimesGammaMinusAlpha);
                    vecScale(2, pq[1], qTimesGammaMinusAlpha, qTimesGammaMinusAlpha);
                    
                    vecAdd(2, alpha, pTimesBetaMinusAlpha, chi);
                    vecAdd(2, chi, qTimesGammaMinusAlpha, chi);
                    
                    texSample(tex, chi[0], chi[1]);
                    double *sample = tex->sample;
                    pixSetRGB(i, j, sample[0], sample[1], sample[2]);
                }
            }
        }
        /*This loop dictates the second half of the triangle to draw, from point c on 
        the x axis to point b on the x axis*/
        for(int i = (int)ceil(c[0]); i <= (int)floor(b[0]); i += 1){
            double bSlope = (b[1] - c[1])/(b[0] - c[0]);
            double bInt = b[1] - (bSlope * b[0]);
            /*This loop dictates which pixels are between the line going from a to b and the
            line going from c to b and then turns them on with the appropriate color values
            taken from the texture struct.*/
            for(int j = (int)ceil((aSlope * i) + aInt); j <= (int)floor((bSlope * i) + bInt); j += 1){
                    double toInv[2][2] = {{(b[0] - a[0]), (c[0] - a[0])}, {(b[1] - a[1]), (c[1] - a[1])}};
                    double xMinusA[2] = {(i - a[0]), (j - a[1])};
                    double inv[2][2];
                    mat22Invert(toInv, inv);
                    double pq[2];
                    mat22Multiply(inv, xMinusA, pq);
                    
                    double chi[2];
                    double pTimesBetaMinusAlpha[2];
                    vecSubtract(2, beta, alpha, pTimesBetaMinusAlpha);
                    vecScale(2, pq[0], pTimesBetaMinusAlpha, pTimesBetaMinusAlpha);
                    
                    double qTimesGammaMinusAlpha[2];
                    vecSubtract(2, gamma, alpha, qTimesGammaMinusAlpha);
                    vecScale(2, pq[1], qTimesGammaMinusAlpha, qTimesGammaMinusAlpha);
                    
                    vecAdd(2, alpha, pTimesBetaMinusAlpha, chi);
                    vecAdd(2, chi, qTimesGammaMinusAlpha, chi);
                    
                    texSample(tex, chi[0], chi[1]);
                    double *sample = tex->sample;
                    pixSetRGB(i, j, sample[0], sample[1], sample[2]);
            }
        }
    }

/*this draws the triangle, one half at a time*/ 
void rasterizeTypeB(double a[2], double b[2], double c[2], double rgb[3], texTexture *tex,
     double alpha[3], double beta[3], double gamma[3]){
        double cSlope = (c[1] - a[1])/(c[0] - a[0]);
        double cInt = c[1] - (cSlope * c[0]);
        /*This loop dictates the first half of the triangle to draw, from point a on 
        the x axis to point b on the x axis*/
        for(int i = (int)ceil(a[0]); i <= (int)floor(b[0]); i += 1){
            double aSlope = (b[1] - a[1])/(b[0] - a[0]);
            double aInt = a[1] - (aSlope * a[0]);
            /*This loop dictates which pixels are between the line going from a to b and the
            line going from a to c and then turns them on with the appropriate color values
            taken from the texture struct.*/
            for(int j = (int)ceil((aSlope * i) + aInt); j <= (int)floor((cSlope * i) + cInt); j += 1){
                    double toInv[2][2] = {{(b[0] - a[0]), (c[0] - a[0])}, {(b[1] - a[1]), (c[1] - a[1])}};
                    double xMinusA[2] = {(i - a[0]), (j - a[1])};
                    double inv[2][2];
                    mat22Invert(toInv, inv);
                    double pq[2];
                    mat22Multiply(inv, xMinusA, pq);
                    
                    double chi[2];
                    double pTimesBetaMinusAlpha[2];
                    vecSubtract(2, beta, alpha, pTimesBetaMinusAlpha);
                    vecScale(2, pq[0], pTimesBetaMinusAlpha, pTimesBetaMinusAlpha);
                    
                    double qTimesGammaMinusAlpha[2];
                    vecSubtract(2, gamma, alpha, qTimesGammaMinusAlpha);
                    vecScale(2, pq[1], qTimesGammaMinusAlpha, qTimesGammaMinusAlpha);
                    
                    vecAdd(2, alpha, pTimesBetaMinusAlpha, chi);
                    vecAdd(2, chi, qTimesGammaMinusAlpha, chi);
                    
                    texSample(tex, chi[0], chi[1]);
                    double *sample = tex->sample;
                    pixSetRGB(i, j, sample[0], sample[1], sample[2]);
            }
        }
        /*This loop dictates the second half of the triangle to draw, from point b on 
        the x axis to point c on the x axis*/
        for(int i = (int)ceil(b[0]); i <= (int)floor(c[0]); i += 1){
            if(b[0] == c[0])
                i = (int)floor(c[0]) + 1;
            else{
                double bSlope = (c[1] - b[1])/(c[0] - b[0]);
                double bInt = b[1] - (bSlope * b[0]);
                /*This loop dictates which pixels are between the line going from b to c and the
                line going from a to c and then turns them on with the appropriate color values
                which are taken from the appropriate place on the texture object.*/
                for(int j = (int)ceil((bSlope * i) + bInt); j <= (int)floor((cSlope * i) + cInt); j += 1){
                    double toInv[2][2] = {{(b[0] - a[0]), (c[0] - a[0])}, {(b[1] - a[1]), (c[1] - a[1])}};
                    double xMinusA[2] = {(i - a[0]), (j - a[1])};
                    double inv[2][2];
                    mat22Invert(toInv, inv);
                    double pq[2];
                    mat22Multiply(inv, xMinusA, pq);
                    
                    double chi[2];
                    double pTimesBetaMinusAlpha[2];
                    vecSubtract(2, beta, alpha, pTimesBetaMinusAlpha);
                    vecScale(2, pq[0], pTimesBetaMinusAlpha, pTimesBetaMinusAlpha);
                    
                    double qTimesGammaMinusAlpha[2];
                    vecSubtract(2, gamma, alpha, qTimesGammaMinusAlpha);
                    vecScale(2, pq[1], qTimesGammaMinusAlpha, qTimesGammaMinusAlpha);
                    
                    vecAdd(2, alpha, pTimesBetaMinusAlpha, chi);
                    vecAdd(2, chi, qTimesGammaMinusAlpha, chi);
                    
                    texSample(tex, chi[0], chi[1]);
                    double *sample = tex->sample;
                    pixSetRGB(i, j, sample[0], sample[1], sample[2]);
                }
            }
        }
    }

/*This is where the points and color values are either passed to one rasterize method or the other.
The difference between the two methods is that for rasterizeTypeA, c is between a and b
on the x axis, while for rasterizeTypeB, c is the farthest point to the left on the
x axis.*/
void separate(double a[2], double b[2], double c[2], double rgb[3], texTexture *tex, 
    double alpha[3], double beta[3], double gamma[3]){
        if(c[0] < b[0])
            rasterizeTypeA(a, b, c, rgb, tex, alpha, beta, gamma);
        else
            rasterizeTypeB(a, b, c, rgb, tex, alpha, beta, gamma);
    }

/*This is where the data is initially passed into. The logic is there to figure out
where a, b, and c are and then align them to the convention of a being the point farthest
to the left. if the a passed in isn't farthest to the left then the point that is is found
and designated as the new a and the other points are shuffled around accordingly
while being passed to the separate method. Alpha, Beta, and Gamma are shuffled appropriately
as well*/
void triRender(double a[2], double b[2], double c[2], double rgb[3], texTexture *tex,
    double alpha[2], double beta[2], double gamma[2]){
        if(a[0] < b[0]){
            if(a[0] <= c[0])
                separate(a, b, c, rgb, tex, alpha, beta, gamma);
            else
                separate(c, a, b, rgb, tex, gamma, alpha, beta);
        }
        else if (b[0] < a[0]){
            if(b[0] < c[0])
                separate(b, c, a, rgb, tex, beta, gamma, alpha);
            else
                separate(c, a, b, rgb, tex, gamma, alpha, beta);
        }
        else{
            if(a[0] < c[0])
                separate(b, c, a, rgb, tex, beta, gamma, alpha);
            else
                separate(c, a, b, rgb, tex, gamma, alpha, beta);
        }
    }