/*Alex Schneider
02-03-17
Graphics*/

#include <stdio.h>
#include <math.h>

/*this draws the triangle, one half at a time*/
void rasterizeTypeA(renRenderer *ren, double unif[], texTexture (*tex)[], double a[], 
        double b[], double c[]){
        double aSlope = (b[1] - a[1])/(b[0] - a[0]);
        double aInt = a[1] - (aSlope * a[0]);
        double rgbz[4];
        int varyDim = ren->varyDim;
        /*This loop dictates the first half of the triangle to draw, from point a on 
        the x axis to point c on the x axis*/
        for(int i = (int)ceil(a[0]); i <= (int)floor(c[0]); i += 1){
            if(i >= 0 || i <= ren->depth->width){
                if(a[0] == c[0]){
                    i = (int)floor(c[0]) + 1;
                }
                else{
                    double cSlope = (c[1] - a[1])/(c[0] - a[0]);
                    double cInt = c[1] - (cSlope * c[0]);
                    double toInv[2][2] = {{(b[0] - a[0]), (c[0] - a[0])}, {(b[1] - a[1]), (c[1] - a[1])}};
                    /*This loop dictates which pixels are between the line going from a to b and the
                    line going from a to c and then turns them on with the appropriate color values
                    taken from the texture struct. It also implements backface culling and
                    depth testing.*/
                    for(int j = (int)ceil((aSlope * i) + aInt); j <= (int)floor((cSlope * i) + cInt); j += 1){
                        if(j >= 0 || j <= ren->depth->height){
                            double xMinusA[2] = {(i - a[0]), (j - a[1])};
                            double inv[2][2];
                            if(mat22Invert(toInv, inv) <= 0)
                                return;
                            double pq[2];
                            mat22Multiply(inv, xMinusA, pq);
                    
                            double attr[varyDim];
                            double pTimesBetaMinusAlpha[varyDim];
                            vecSubtract(varyDim, b, a, pTimesBetaMinusAlpha);
                            vecScale(varyDim, pq[0], pTimesBetaMinusAlpha, pTimesBetaMinusAlpha);
                    
                            double qTimesGammaMinusAlpha[varyDim];
                            vecSubtract(varyDim, c, a, qTimesGammaMinusAlpha);
                            vecScale(varyDim, pq[1], qTimesGammaMinusAlpha, qTimesGammaMinusAlpha);
                    
                            vecAdd(varyDim, a, pTimesBetaMinusAlpha, attr);
                            vecAdd(varyDim, attr, qTimesGammaMinusAlpha, attr);
                    
                            attr[renVARYI] = (double)i;
                            attr[renVARYJ] = (double)j;
                            ren->colorPixel(ren, unif, tex, attr, rgbz);
                            if(rgbz[3] <= attr[renVARYZ]){
                                depthSetZ(ren->depth, i, j, attr[renVARYZ]);
                                pixSetRGB(i, j, rgbz[0], rgbz[1], rgbz[2]);
                            }
                        }
                    }
                }
            }
        }
        /*This loop dictates the second half of the triangle to draw, from point c on 
        the x axis to point b on the x axis*/
        for(int i = (int)ceil(c[0]); i <= (int)floor(b[0]); i += 1){
            if(i >= 0 || i <= ren->depth->width){
                double bSlope = (b[1] - c[1])/(b[0] - c[0]);
                double bInt = b[1] - (bSlope * b[0]);
                double toInv[2][2] = {{(b[0] - a[0]), (c[0] - a[0])}, {(b[1] - a[1]), (c[1] - a[1])}};
                /*This loop dictates which pixels are between the line going from a to b and the
                line going from c to b and then turns them on with the appropriate color values
                taken from the texture struct. It also implements backface culling and
                depth testing.*/
                for(int j = (int)ceil((aSlope * i) + aInt); j <= (int)floor((bSlope * i) + bInt); j += 1){
                    if(j >= 0 || j <= ren->depth->height){
                        double xMinusA[2] = {(i - a[0]), (j - a[1])};
                        double inv[2][2];
                        if(mat22Invert(toInv, inv) <= 0)
                            return;
                        double pq[2];
                        mat22Multiply(inv, xMinusA, pq);
                    
                        double attr[varyDim];
                        double pTimesBetaMinusAlpha[varyDim];
                        vecSubtract(varyDim, b, a, pTimesBetaMinusAlpha);
                        vecScale(varyDim, pq[0], pTimesBetaMinusAlpha, pTimesBetaMinusAlpha);
                    
                        double qTimesGammaMinusAlpha[varyDim];
                        vecSubtract(varyDim, c, a, qTimesGammaMinusAlpha);
                        vecScale(varyDim, pq[1], qTimesGammaMinusAlpha, qTimesGammaMinusAlpha);
                    
                        vecAdd(varyDim, a, pTimesBetaMinusAlpha, attr);
                        vecAdd(varyDim, attr, qTimesGammaMinusAlpha, attr);
                    
                        attr[renVARYI] = (double)i;
                        attr[renVARYJ] = (double)j;
                        ren->colorPixel(ren, unif, tex, attr, rgbz);
                        if(rgbz[3] <= attr[renVARYZ]){
                            depthSetZ(ren->depth, i, j, attr[renVARYZ]);
                            pixSetRGB(i, j, rgbz[0], rgbz[1], rgbz[2]);
                        }
                    }
                }
            }
        }
    }

/*this draws the triangle, one half at a time*/ 
void rasterizeTypeB(renRenderer *ren, double unif[], texTexture (*tex)[], double a[], 
        double b[], double c[]){
        double cSlope = (c[1] - a[1])/(c[0] - a[0]);
        double cInt = c[1] - (cSlope * c[0]);
        double rgbz[4];
        int varyDim = ren->varyDim;
        /*This loop dictates the first half of the triangle to draw, from point a on 
        the x axis to point b on the x axis*/
        for(int i = (int)ceil(a[0]); i <= (int)floor(b[0]); i += 1){
            if(i >= 0 || i <= ren->depth->width){
                double aSlope = (b[1] - a[1])/(b[0] - a[0]);
                double aInt = a[1] - (aSlope * a[0]);
                double toInv[2][2] = {{(b[0] - a[0]), (c[0] - a[0])}, {(b[1] - a[1]), (c[1] - a[1])}};
                /*This loop dictates which pixels are between the line going from a to b and the
                line going from a to c and then turns them on with the appropriate color values
                taken from the texture struct. It also implements backface culling and
                depth testing.*/
                for(int j = (int)ceil((aSlope * i) + aInt); j <= (int)floor((cSlope * i) + cInt); j += 1){
                    if(j >= 0 || j <= ren->depth->height){
                        double xMinusA[2] = {(i - a[0]), (j - a[1])};
                        double inv[2][2];
                        if(mat22Invert(toInv, inv) <= 0)
                            return;
                        double pq[2];
                        mat22Multiply(inv, xMinusA, pq);
                    
                        double attr[varyDim];
                        double pTimesBetaMinusAlpha[varyDim];
                        vecSubtract(varyDim, b, a, pTimesBetaMinusAlpha);
                        vecScale(varyDim, pq[0], pTimesBetaMinusAlpha, pTimesBetaMinusAlpha);
                    
                        double qTimesGammaMinusAlpha[varyDim];
                        vecSubtract(varyDim, c, a, qTimesGammaMinusAlpha);
                        vecScale(varyDim, pq[1], qTimesGammaMinusAlpha, qTimesGammaMinusAlpha);
                    
                        vecAdd(varyDim, a, pTimesBetaMinusAlpha, attr);
                        vecAdd(varyDim, attr, qTimesGammaMinusAlpha, attr);
                    
                        attr[renVARYI] = (double)i;
                        attr[renVARYJ] = (double)j;
                        ren->colorPixel(ren, unif, tex, attr, rgbz);
                        if(rgbz[3] <= attr[renVARYZ]){
                            depthSetZ(ren->depth, i, j, attr[renVARYZ]);
                            pixSetRGB(i, j, rgbz[0], rgbz[1], rgbz[2]);
                        }
                    }
                }
            }
        }
        /*This loop dictates the second half of the triangle to draw, from point b on 
        the x axis to point c on the x axis*/
        for(int i = (int)ceil(b[0]); i <= (int)floor(c[0]); i += 1){
            if(i >= 0 || i <= ren->depth->width){
                if(b[0] == c[0])
                    i = (int)floor(c[0]) + 1;
                else{
                    double bSlope = (c[1] - b[1])/(c[0] - b[0]);
                    double bInt = b[1] - (bSlope * b[0]);
                    double toInv[2][2] = {{(b[0] - a[0]), (c[0] - a[0])}, {(b[1] - a[1]), (c[1] - a[1])}};
                    /*This loop dictates which pixels are between the line going from b to c and the
                    line going from a to c and then turns them on with the appropriate color values
                    which are taken from the appropriate place on the texture object. It also 
                    implements backface culling and depth testing.*/
                    for(int j = (int)ceil((bSlope * i) + bInt); j <= (int)floor((cSlope * i) + cInt); j += 1){
                        if(j >= 0 || j <= ren->depth->height){
                            double xMinusA[2] = {(i - a[0]), (j - a[1])};
                            double inv[2][2];
                            if(mat22Invert(toInv, inv) <= 0)
                                return;
                            double pq[2];
                            mat22Multiply(inv, xMinusA, pq);
                    
                            double attr[varyDim];
                            double pTimesBetaMinusAlpha[varyDim];
                            vecSubtract(varyDim, b, a, pTimesBetaMinusAlpha);
                            vecScale(varyDim, pq[0], pTimesBetaMinusAlpha, pTimesBetaMinusAlpha);
                    
                            double qTimesGammaMinusAlpha[varyDim];
                            vecSubtract(varyDim, c, a, qTimesGammaMinusAlpha);
                            vecScale(varyDim, pq[1], qTimesGammaMinusAlpha, qTimesGammaMinusAlpha);
                    
                            vecAdd(varyDim, a, pTimesBetaMinusAlpha, attr);
                            vecAdd(varyDim, attr, qTimesGammaMinusAlpha, attr);
                    
                            attr[renVARYI] = (double)i;
                            attr[renVARYJ] = (double)j;
                            ren->colorPixel(ren, unif, tex, attr, rgbz);
                            if(rgbz[3] <= attr[renVARYZ]){
                                depthSetZ(ren->depth, i, j, attr[renVARYZ]);
                                pixSetRGB(i, j, rgbz[0], rgbz[1], rgbz[2]);
                            }
                        }
                    }
                }
            }
        }
    }

/*This is where the points and color values are either passed to one rasterize method or the other.
The difference between the two methods is that for rasterizeTypeA, c is between a and b
on the x axis, while for rasterizeTypeB, c is the farthest point to the left on the
x axis.*/
void separate(renRenderer *ren, double unif[], texTexture (*tex)[], double a[], 
        double b[], double c[]){
        if(c[0] < b[0])
            rasterizeTypeA(ren, unif, tex, a, b, c);
        else
            rasterizeTypeB(ren, unif, tex, a, b, c);
    }

/*This is where the data is initially passed into. The logic is there to figure out
where a, b, and c are and then align them to the convention of a being the point farthest
to the left. if the a passed in isn't farthest to the left then the point that is is found
and designated as the new a and the other points are shuffled around accordingly
while being passed to the separate method. Alpha, Beta, and Gamma are shuffled appropriately
as well*/
void triRender(renRenderer *ren, double unif[], texTexture (*tex)[], double a[], 
        double b[], double c[]){
        if(a[0] < b[0]){
            if(a[0] <= c[0])
                separate(ren, unif, tex, a, b, c);
            else
                separate(ren, unif, tex, c, a, b);
        }
        else if (b[0] < a[0]){
            if(b[0] < c[0])
                separate(ren, unif, tex, b, c, a);
            else
                separate(ren, unif, tex, c, a, b);
        }
        else{
            if(a[0] < c[0])
                separate(ren, unif, tex, b, c, a);
            else
                separate(ren, unif, tex, c, a, b);
        }
    }