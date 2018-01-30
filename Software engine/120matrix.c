/*** 2 x 2 Matrices ***/

/* Pretty-prints the given matrix, with one line of text per row of matrix. */
void mat22Print(double m[2][2]) {
	printf("%f %f\n", m[0][0], m[0][1]);
	printf("%f %f\n", m[1][0], m[1][1]);
}

/* Returns the determinant of the matrix m. If the determinant is 0.0, then the 
matrix is not invertible, and mInv is untouched. If the determinant is not 0.0, 
then the matrix is invertible, and its inverse is placed into mInv. */
double mat22Invert(double m[2][2], double mInv[2][2]) {
	double determinant = (m[0][0] * m[1][1]) - (m[0][1] * m[1][0]);
	if(determinant != 0.0){
	    mInv[0][0] = (m[1][1] * (1/determinant));
	    mInv[0][1] = (-m[0][1] * (1/determinant));
	    mInv[1][0] = (-m[1][0] * (1/determinant));
	    mInv[1][1] = (m[0][0] * (1/determinant));
	    
	    return determinant;
	}
	return determinant;
}

/* Multiplies a 2x2 matrix m by a 2-column v, storing the result in mTimesV. 
The output should not */
void mat22Multiply(double m[2][2], double v[], double mTimesV[]) {
	mTimesV[0] = (m[0][0] * v[0]) + (m[0][1] * v[1]);
	mTimesV[1] = (m[1][0] * v[0]) + (m[1][1] * v[1]);
}

/* Fills the matrix m from its two columns. */
void mat22Columns(double col0[2], double col1[2], double m[2][2]) {
	m[0][0] = col0[0];
	m[0][1] = col1[0];
	m[1][0] = col0[1];
	m[1][1] = col0[1];
}

/*My own method to multiply 2 by 2 arrays together. It's not used in the code except for
some testing.*/
void mat22MultiplyTwo(double m[2][2], double v[2][2], double mTimesV[2][2]){
    mTimesV[0][0] = (m[0][0] * v[0][0]) + (m[0][1] * v[1][0]);
    mTimesV[0][1] = (m[0][0] * v[0][1]) + (m[0][1] * v[1][1]);
    mTimesV[1][0] = (m[1][0] * v[0][0]) + (m[1][1] * v[1][0]);
    mTimesV[1][1] = (m[1][0] * v[0][1]) + (m[1][1] * v[1][1]);
}

/* Multiplies the 3x3 matrix m by the 3x3 matrix n. */
void mat333Multiply(double m[3][3], double n[3][3], double mTimesN[3][3]){
    for(int i = 0; i < 3; i += 1){
        for(int j = 0; j < 3; j += 1){
            mTimesN[i][j] = (m[i][0] * n[0][j]) + (m[i][1] * n[1][j]) + (m[i][2] * n[2][j]);
        }
    }
}

/* Multiplies the 3x3 matrix m by the 3x1 matrix v. */
void mat331Multiply(double m[3][3], double v[3], double mTimesV[3]){
    mTimesV[0] = (m[0][0] * v[0]) + (m[0][1] * v[1]) + (m[0][2] * v[2]);
    mTimesV[1] = (m[1][0] * v[0]) + (m[1][1] * v[1]) + (m[1][2] * v[2]);
    mTimesV[2] = (m[2][0] * v[0]) + (m[2][1] * v[1]) + (m[2][2] * v[2]);
}

/* Builds a 3x3 matrix representing 2D rotation and translation in homogeneous 
coordinates. More precisely, the transformation first rotates through the angle 
theta (in radians, counterclockwise), and then translates by the vector (x, y). 
*/
void mat33Isometry(double theta, double x, double y, double isom[3][3]){
    double rotation[3][3] = {{sin(theta), -1.0 * cos(theta), 0.0},
        {cos(theta), sin(theta), 0.0}, {0.0, 0.0, 1.0}};
    double translation[3][3] = {{1.0, 0.0, x}, {0.0, 1.0, y}, {0.0, 0.0, 1.0}};
    mat333Multiply(rotation, translation, isom); 
}

/* Given a length-1 3D vector axis and an angle theta (in radians), builds the 
rotation matrix for the rotation about that axis through that angle. Based on 
Rodrigues' rotation formula R = I + (sin theta) U + (1 - cos theta) U^2. */
void mat33AngleAxisRotation(double theta, double axis[3], double rot[3][3]){
    double U[3][3] = {{0.0, -axis[2], axis[1]},
                {axis[2], 0.0, -axis[0]},
                {-axis[1], axis[0], 0.0}};
    double USquared[3][3] = {{(axis[0] * axis[0]) - 1, axis[0] * axis[1], axis[0] * axis[2]},
                            {axis[0] * axis[1], (axis[1] * axis[1]) - 1, axis[1] * axis[2]},
                            {axis[0] * axis[2], axis[1] * axis[2], (axis[2] * axis[2]) - 1}};
    double I[3][3] = {{1.0, 0.0, 0.0},
                    {0.0, 1.0, 0.0},
                    {0.0, 0.0, 1.0}};
                    
    for(int i = 0; i < 3; i += 1){
        for(int j = 0; j < 3; j += 1){
            rot[i][j] = I[i][j] + (sin(theta) * U[i][j]) + ((1 - cos(theta)) * USquared[i][j]); 
        }
    } 
}

/* Simply takes one 3 by 3 matrix, copies it and then transposes the copy. The same matrix
can be passed in as both parameters in order to transpose the original matrix. */
void mat33Transpose(double a[3][3], double b[3][3]){
    for(int i = 0; i < 3; i += 1){
        for(int j = 0; j < 3; j += 1){
            b[i][j] = a[i][j];   
        }
    }
    double temp = b[0][1];
    b[0][1] = b[1][0];
    b[1][0] = temp;
    temp = b[0][2];
    b[0][2] = b[2][0];
    b[2][0] = temp;
    temp = b[1][2];
    b[1][2] = b[2][1];
    b[2][1] = temp;
}

/* Given two length-1 3D vectors u, v that are perpendicular to each other. 
Given two length-1 3D vectors a, b that are perpendicular to each other. Builds 
the rotation matrix that rotates u to a and v to b. */
void mat33BasisRotation(double u[3], double v[3], double a[3], double b[3], 
        double rot[3][3]){
    double w[3];
    vec3Cross(u, v, w);
    double R[3][3] = {{u[0], v[0], w[0]},
                    {u[1], v[1], w[1]},
                    {u[2], v[2], w[2]}};
    
    double c[3];
    vec3Cross(a, b, c);
    double S[3][3] = {{a[0], b[0], c[0]},
                    {a[1], b[1], c[1]},
                    {a[2], b[2], c[2]}};
    
    mat33Transpose(R, R);
    
    mat333Multiply(R, S, rot);
    
}

/* Multiplies m by n, placing the answer in mTimesN. */
void mat444Multiply(double m[4][4], double n[4][4], double mTimesN[4][4]){
    for(int i = 0; i < 4; i += 1){
        for(int j = 0; j < 4; j += 1){
            mTimesN[i][j] = (m[i][0] * n[0][j]) + (m[i][1] * n[1][j]) + (m[i][2] * n[2][j])
                + (m[i][3] * n[3][j]);
        }
    }
}

/* Multiplies m by v, placing the answer in mTimesV. */
void mat441Multiply(double m[4][4], double v[4], double mTimesV[4]){
    for(int i = 0; i < 4; i += 1){
        mTimesV[i] = (m[i][0] * v[0]) + (m[i][1] * v[1]) + (m[i][2] * v[2]) + (m[i][3] * v[3]);
    }
}

/* Given a rotation and a translation, forms the 4x4 homogeneous matrix 
representing the rotation followed in time by the translation. */
void mat44Isometry(double rot[3][3], double trans[3], double isom[4][4]){
    for(int i = 0; i < 4; i += 1){
        for(int j = 0; j < 4; j += 1){
            if(j == 3 && i < 3)
                isom[i][j] = trans[i];
            else if(i == 3 && j < 3)
                isom[i][j] = 0.0;
            else if(i == 3 && j == 3)
                isom[i][j] = 1.0;
            else
                isom[i][j] = rot[i][j];
        }
    }
}

/* Given a rotation and translation, forms the 4x4 homogeneous matrix 
representing the inverse translation followed in time by the inverse rotation. 
That is, the isom produced by this function is the inverse to the isom 
produced by mat44Isometry on the same inputs. */
void mat44InverseIsometry(double rot[3][3], double trans[3], 
        double isom[4][4]){
   
    double transTwo[3];
    
    double inverseRot[3][3];
    mat33Transpose(rot, inverseRot);
    mat331Multiply(inverseRot, trans, transTwo);
    vecScale(3, -1.0, transTwo, transTwo);
    for(int i = 0; i < 3; i += 1){
        for(int j = 0; j < 3; j += 1){
            isom[i][j] = inverseRot[i][j];
        }
    }
    
    for(int i = 0; i < 3; i += 1){
        isom[i][3] = transTwo[i];
    }
    
    for(int i = 0; i < 3; i += 1){
        isom[3][i] = 0.0;
    }
    
    isom[3][3] = 1.0;
}

/* Simply copies one 4 by 4 matrix into another */ 
void mat44Copy(double v[4][4], double copy[4][4]){
    for(int i = 0; i < 4; i += 1){
        for(int j = 0; j < 4; j += 1){
            copy[i][j] = v[i][j];
        }
    }
}