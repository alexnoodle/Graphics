/*Modified by Alex Schneider
02-03-17
Graphics*/

/* Copies the dim-dimensional vector v to the dim-dimensional vector copy. */
void vecCopy(int dim, double v[], double copy[]) {
	for(int i = 0; i < dim; i += 1){
	    copy[i] = v[i];
	}
}

/* Adds the dim-dimensional vectors v and w. */
void vecAdd(int dim, double v[], double w[], double vPlusW[]) {
	for(int i = 0; i < dim; i += 1){
	    vPlusW[i] = (v[i] + w[i]);
	}
}

/* Subtracts the dim-dimensional vectors v and w. */
void vecSubtract(int dim, double v[], double w[], double vMinusW[]) {
	for(int i = 0; i < dim; i += 1){
	    vMinusW[i] = (v[i] - w[i]);
	}
}

/* Scales the dim-dimensional vector w by the number c. */
void vecScale(int dim, double c, double w[], double cTimesW[]) {
	for(int i = 0; i < dim; i += 1){
	    cTimesW[i] = (c * w[i]);
	}
}

/* Assumes that there are dim + 2 arguments, the last dim of which are doubles. 
Sets the dim-dimensional vector v to those doubles. */
void vecSet(int dim, double v[],...){
    va_list argumentPointer;
    va_start(argumentPointer, v);
    for(int i = 0; i < dim; i += 1){
        v[i] = va_arg(argumentPointer, double);
    }
    va_end(argumentPointer);
}

/* Returns the dot product of the dim-dimensional vectors v and w. */
double vecDot(int dim, double v[], double w[]){
    double toReturn;
    for(int i = 0; i < dim; i += 1){
        toReturn += v[i] * w[i];
    }
    return toReturn;
}

/* Returns the length of the dim-dimensional vector v. */
double vecLength(int dim, double v[]){
    double toReturn;
    for(int i = 0; i < dim; i += 1){
        toReturn += v[i] * v[i];
    }
    toReturn = sqrt(toReturn);
    return toReturn;
}

/* Returns the length of the dim-dimensional vector v. If the length is 
non-zero, then also places a scaled version of v into the dim-dimensional 
vector unit, so that unit has length 1. */
double vecUnit(int dim, double v[], double unit[]){
    double toReturn;
    for(int i = 0; i < dim; i += 1){
        toReturn += v[i] * v[i];
    }
    toReturn = sqrt(toReturn);
    if(toReturn != 0){
        for(int i = 0; i < dim; i += 1){
            unit[i] = (1 / toReturn) * v[i];
        }
    }
    return toReturn;
}

/* Computes the cross product of the 3-dimensional vectors v and w, and places 
it into vCrossW. */
void vec3Cross(double v[3], double w[3], double vCrossW[3]){
    vCrossW[0] = (v[1] * w[2]) - (v[2] * w[1]);
    vCrossW[1] = (v[2] * w[0]) - (v[0] * w[2]);
    vCrossW[2] = (v[0] * w[1]) - (v[1] * w[0]);
}

/* Computes the 3-dimensional vector v from its spherical coordinates. 
rho >= 0.0 is the radius. 0 <= phi <= pi is the co-latitude. -pi <= theta <= pi 
is the longitude or azimuth. */
void vec3Spherical(double rho, double phi, double theta, double v[3]){
    v[0] = rho * sin(phi) * cos(theta);
    v[1] = rho * sin(phi) * sin(theta);
    v[2] = rho * cos(phi);
    
}

void vecDivide(int dim, double v[], double divide){
    for(int i = 0; i < dim; i += 1){
        v[i] = v[i] / divide;
    }
}