/*Alex Schneider
02-03-17
Graphics*/

/* This method doesn't clip anything. The vertices are copied and divided by w to put
the values into the 2 x 2 x 2 cube that is clip space and then multiplied by the 
viewport matrix to put the vertices into screen space. TriRender is then called and
they're put on the screen just like before */
void clipNothing(renRenderer *ren, double unif[], texTexture (*tex)[], double a[], 
        double b[], double c[]){
    double aBefore[ren->varyDim], bBefore[ren->varyDim], cBefore[ren->varyDim];
        vecCopy(ren->varyDim, a, aBefore);
        vecCopy(ren->varyDim, b, bBefore);
        vecCopy(ren->varyDim, c, cBefore);
        
        double aAfter[ren->varyDim], bAfter[ren->varyDim], cAfter[ren->varyDim];
        vecCopy(ren->varyDim, a, aAfter);
        vecCopy(ren->varyDim, b, bAfter);
        vecCopy(ren->varyDim, c, cAfter);
        
        vecDivide(4, aBefore, aBefore[renVARYW]);
        vecDivide(4, bBefore, bBefore[renVARYW]);
        vecDivide(4, cBefore, cBefore[renVARYW]);

        mat441Multiply(ren->viewport, aBefore, aAfter);
        mat441Multiply(ren->viewport, bBefore, bAfter);
        mat441Multiply(ren->viewport, cBefore, cAfter);
    
        triRender(ren, unif, tex, aAfter, bAfter, cAfter);
}

/* This method is passed 3 vertices and it's assumed that the a vertex is out of bounds.
The method calculates the 2 vertices on the near plane and then calls triRender on the
two successive triangles using b, c, and the 2 new vertices. */
void clipOneVertex(renRenderer *ren, double unif[], texTexture (*tex)[], double a[], 
        double b[], double c[]){
    printf("a is clipped\n");
    double t = (a[renVARYW] - a[renVARYZ])/((a[renVARYW] - a[renVARYZ]) + (c[renVARYW] - c[renVARYZ]));
    double aMinusC[ren->varyDim];
    vecSubtract(ren->varyDim, a, c, aMinusC);
    vecScale(ren->varyDim, t, aMinusC, aMinusC);
    double aPlusAMinusC[ren->varyDim];
    vecAdd(ren->varyDim, a, aMinusC, aPlusAMinusC);
        
    double aBefore[ren->varyDim], bBefore[ren->varyDim], cBefore[ren->varyDim];
    vecCopy(ren->varyDim, b, bBefore);
    vecCopy(ren->varyDim, c, cBefore);
        
    double aAfter[ren->varyDim], bAfter[ren->varyDim], cAfter[ren->varyDim];
    vecCopy(ren->varyDim, a, aAfter);
    vecCopy(ren->varyDim, b, bAfter);
    vecCopy(ren->varyDim, c, cAfter);
    
    vecDivide(4, aPlusAMinusC, aPlusAMinusC[renVARYW]);
    vecDivide(4, bBefore, bBefore[renVARYW]);
    vecDivide(4, cBefore, cBefore[renVARYW]);
    
    mat441Multiply(ren->viewport, aPlusAMinusC, aAfter);
    mat441Multiply(ren->viewport, bBefore, bAfter);
    mat441Multiply(ren->viewport, cBefore, cAfter);
        
    triRender(ren, unif, tex, aAfter, bAfter, cAfter);
        
    t = (a[renVARYW] - a[renVARYZ])/((a[renVARYW] - a[renVARYZ]) + (b[renVARYW] - b[renVARYZ]));
    double aMinusB[ren->varyDim];
    vecSubtract(ren->varyDim, a, b, aMinusB);
    vecScale(ren->varyDim, t, aMinusB, aMinusB);
    double aPlusAMinusB[ren->varyDim];
    vecAdd(ren->varyDim, a, aMinusB, aPlusAMinusB);
        
    vecDivide(4, aPlusAMinusB, aPlusAMinusB[renVARYW]);
    vecCopy(ren->varyDim, aAfter, cAfter);
        
    mat441Multiply(ren->viewport, aPlusAMinusB, aAfter);
        
    triRender(ren, unif, tex, aAfter, bAfter, cAfter);
}

/* This method is passed 3 vertices and it's assumed that a and b need to be clipped.
Two new vertices are calculated on the near plane and the new triangle using the new
vertices and c is passed to triRender. */
void clipTwoVerteces(renRenderer *ren, double unif[], texTexture (*tex)[], double a[], 
        double b[], double c[]){
    double t = (a[renVARYW] - a[renVARYZ])/((a[renVARYW] - a[renVARYZ]) + (c[renVARYW] - c[renVARYZ]));
    double aMinusC[ren->varyDim];
    vecSubtract(ren->varyDim, a, c, aMinusC);
    vecScale(ren->varyDim, t, aMinusC, aMinusC);
    double aPlusAMinusC[ren->varyDim];
    vecAdd(ren->varyDim, a, aMinusC, aPlusAMinusC);        
        
    t = (b[renVARYW] - b[renVARYZ])/((b[renVARYW] - b[renVARYZ]) + (c[renVARYW] - c[renVARYZ]));
    vecSubtract(ren->varyDim, b, c, aMinusC);
    vecScale(ren->varyDim, t, aMinusC, aMinusC);
    double bPlusBMinusC[ren->varyDim];
    vecAdd(ren->varyDim, b, aMinusC, bPlusBMinusC);
    
    double cBefore[ren->varyDim];
    vecCopy(ren->varyDim, c, cBefore);
        
    double aAfter[ren->varyDim], bAfter[ren->varyDim], cAfter[ren->varyDim];
    vecCopy(ren->varyDim, a, aAfter);
    vecCopy(ren->varyDim, b, bAfter);
    vecCopy(ren->varyDim, c, cAfter);
        
    vecDivide(4, aPlusAMinusC, aPlusAMinusC[renVARYW]);
    vecDivide(4, bPlusBMinusC, bPlusBMinusC[renVARYW]);
    vecDivide(4, cBefore, cBefore[renVARYW]);
        
        
    mat441Multiply(ren->viewport, aPlusAMinusC, aAfter);
    mat441Multiply(ren->viewport, bPlusBMinusC, bAfter);
    mat441Multiply(ren->viewport, cBefore, cAfter);
        
    triRender(ren, unif, tex, aAfter, bAfter, cAfter);
}

/* This goes through all the possible cases: all vertices must be clipped, the 3 cases
where two vertices need to be clipped, the 3 cases where one vertex needs to be clipped,
and the case where nothing needs to be clipped. The vertices are passed to the requisite
methods to do so. For the cases where one or two vertices need to be clipped, the
vertices are rearranged if necessary to adhere to the assumptions in the above methods. */
void clipRender(renRenderer *ren, double unif[], texTexture (*tex)[], double a[], 
        double b[], double c[]){
    if((a[renVARYW] <= 0 || a[renVARYZ] > a[renVARYW]) && (b[renVARYW] <= 0 
    || b[renVARYZ] > b[renVARYW]) && (c[renVARYW] <= 0 || c[renVARYZ] > c[renVARYW])){
        printf("everything is clipped\n");
        return;
    }
    else if((a[renVARYW] <= 0 || a[renVARYZ] > a[renVARYW]) && (b[renVARYW] <= 0 
    || b[renVARYZ] > b[renVARYW])){
        clipTwoVerteces(ren, unif, tex, a, b, c);
    }
    else if((a[renVARYW] <= 0 || a[renVARYZ] > a[renVARYW]) && (c[renVARYW] <= 0 
    || c[renVARYZ] > c[renVARYW])){
        clipTwoVerteces(ren, unif, tex, c, a, b);
    }
    else if((c[renVARYW] <= 0 || c[renVARYZ] > c[renVARYW]) && (b[renVARYW] <= 0 
    || b[renVARYZ] > b[renVARYW])){
        clipTwoVerteces(ren, unif, tex, b, c, a);
    }
    else if((a[renVARYW] <= 0 || a[renVARYZ] > a[renVARYW])){
        clipOneVertex(ren, unif, tex, a, b, c);
    }
    else if((b[renVARYW] <= 0 || b[renVARYZ] > b[renVARYW])){
        clipOneVertex(ren, unif, tex, b, c, a);
    }
    else if((c[renVARYW] <= 0 || c[renVARYZ] > c[renVARYW])){
        clipOneVertex(ren, unif, tex, c, a, b);
    }
    else{
        clipNothing(ren, unif, tex, a, b, c);
    }
}