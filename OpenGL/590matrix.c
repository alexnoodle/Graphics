/*Alex Schneider
02-03-17
Graphics*/

/*** 2 x 2 Matrices ***/

/* Pretty-prints the given matrix, with one line of text per row of matrix. */
void mat22Print(GLdouble m[2][2]) {
	printf("%f %f\n", m[0][0], m[0][1]);
	printf("%f %f\n", m[1][0], m[1][1]);
}

/* Returns the determinant of the matrix m. If the determinant is 0.0, then the 
matrix is not invertible, and mInv is untouched. If the determinant is not 0.0, 
then the matrix is invertible, and its inverse is placed into mInv. */
double mat22Invert(GLdouble m[2][2], GLdouble mInv[2][2]) {
	GLdouble determinant = (m[0][0] * m[1][1]) - (m[0][1] * m[1][0]);
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
void mat22Multiply(GLdouble m[2][2], GLdouble v[], GLdouble mTimesV[]) {
	mTimesV[0] = (m[0][0] * v[0]) + (m[0][1] * v[1]);
	mTimesV[1] = (m[1][0] * v[0]) + (m[1][1] * v[1]);
}

/* Fills the matrix m from its two columns. */
void mat22Columns(GLdouble col0[2], GLdouble col1[2], GLdouble m[2][2]) {
	m[0][0] = col0[0];
	m[0][1] = col1[0];
	m[1][0] = col0[1];
	m[1][1] = col0[1];
}

/*My own method to multiply 2 by 2 arrays together. It's not used in the code except for
some testing.*/
void mat22MultiplyTwo(GLdouble m[2][2], GLdouble v[2][2], GLdouble mTimesV[2][2]){
    mTimesV[0][0] = (m[0][0] * v[0][0]) + (m[0][1] * v[1][0]);
    mTimesV[0][1] = (m[0][0] * v[0][1]) + (m[0][1] * v[1][1]);
    mTimesV[1][0] = (m[1][0] * v[0][0]) + (m[1][1] * v[1][0]);
    mTimesV[1][1] = (m[1][0] * v[0][1]) + (m[1][1] * v[1][1]);
}

/* Multiplies the 3x3 matrix m by the 3x3 matrix n. */
void mat333Multiply(GLdouble m[3][3], GLdouble n[3][3], GLdouble mTimesN[3][3]){
    for(int i = 0; i < 3; i += 1){
        for(int j = 0; j < 3; j += 1){
            mTimesN[i][j] = (m[i][0] * n[0][j]) + (m[i][1] * n[1][j]) + (m[i][2] * n[2][j]);
        }
    }
}

/* Multiplies the 3x3 matrix m by the 3x1 matrix v. */
void mat331Multiply(GLdouble m[3][3], GLdouble v[3], GLdouble mTimesV[3]){
    mTimesV[0] = (m[0][0] * v[0]) + (m[0][1] * v[1]) + (m[0][2] * v[2]);
    mTimesV[1] = (m[1][0] * v[0]) + (m[1][1] * v[1]) + (m[1][2] * v[2]);
    mTimesV[2] = (m[2][0] * v[0]) + (m[2][1] * v[1]) + (m[2][2] * v[2]);
}

/* Builds a 3x3 matrix representing 2D rotation and translation in homogeneous 
coordinates. More precisely, the transformation first rotates through the angle 
theta (in radians, counterclockwise), and then translates by the vector (x, y). 
*/
void mat33Isometry(GLdouble theta, GLdouble x, GLdouble y, GLdouble isom[3][3]){
    GLdouble rotation[3][3] = {{sin(theta), -1.0 * cos(theta), 0.0},
        {cos(theta), sin(theta), 0.0}, {0.0, 0.0, 1.0}};
    GLdouble translation[3][3] = {{1.0, 0.0, x}, {0.0, 1.0, y}, {0.0, 0.0, 1.0}};
    mat333Multiply(rotation, translation, isom); 
}

/* Given a length-1 3D vector axis and an angle theta (in radians), builds the 
rotation matrix for the rotation about that axis through that angle. Based on 
Rodrigues' rotation formula R = I + (sin theta) U + (1 - cos theta) U^2. */
void mat33AngleAxisRotation(GLdouble theta, GLdouble axis[3], GLdouble rot[3][3]){
    GLdouble U[3][3] = {{0.0, -axis[2], axis[1]},
                {axis[2], 0.0, -axis[0]},
                {-axis[1], axis[0], 0.0}};
    GLdouble USquared[3][3] = {{(axis[0] * axis[0]) - 1, axis[0] * axis[1], axis[0] * axis[2]},
                            {axis[0] * axis[1], (axis[1] * axis[1]) - 1, axis[1] * axis[2]},
                            {axis[0] * axis[2], axis[1] * axis[2], (axis[2] * axis[2]) - 1}};
    GLdouble I[3][3] = {{1.0, 0.0, 0.0},
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
void mat33Transpose(GLdouble a[3][3], GLdouble b[3][3]){
    for(int i = 0; i < 3; i += 1){
        for(int j = 0; j < 3; j += 1){
            b[i][j] = a[i][j];   
        }
    }
    GLdouble temp = b[0][1];
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
void mat33BasisRotation(GLdouble u[3], GLdouble v[3], GLdouble a[3], GLdouble b[3], 
        GLdouble rot[3][3]){
    GLdouble w[3];
    vec3Cross(u, v, w);
    GLdouble R[3][3] = {{u[0], v[0], w[0]},
                    {u[1], v[1], w[1]},
                    {u[2], v[2], w[2]}};
    
    GLdouble c[3];
    vec3Cross(a, b, c);
    GLdouble S[3][3] = {{a[0], b[0], c[0]},
                    {a[1], b[1], c[1]},
                    {a[2], b[2], c[2]}};
    
    mat33Transpose(R, R);
    
    mat333Multiply(S, R, rot);
    
}

/* Multiplies m by n, placing the answer in mTimesN. */
void mat444Multiply(GLdouble m[4][4], GLdouble n[4][4], GLdouble mTimesN[4][4]){
    for(int i = 0; i < 4; i += 1){
        for(int j = 0; j < 4; j += 1){
            mTimesN[i][j] = (m[i][0] * n[0][j]) + (m[i][1] * n[1][j]) + (m[i][2] * n[2][j])
                + (m[i][3] * n[3][j]);
        }
    }
}

/* Multiplies m by v, placing the answer in mTimesV. */
void mat441Multiply(GLdouble m[4][4], GLdouble v[4], GLdouble mTimesV[4]){
    for(int i = 0; i < 4; i += 1){
        mTimesV[i] = (m[i][0] * v[0]) + (m[i][1] * v[1]) + (m[i][2] * v[2]) + (m[i][3] * v[3]);
    }
}

/* Given a rotation and a translation, forms the 4x4 homogeneous matrix 
representing the rotation followed in time by the translation. */
void mat44Isometry(GLdouble rot[3][3], GLdouble trans[3], GLdouble isom[4][4]){
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
void mat44InverseIsometry(GLdouble rot[3][3], GLdouble trans[3], 
        GLdouble isom[4][4]){
   
    GLdouble transTwo[3];
    
    GLdouble inverseRot[3][3];
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
void mat44Copy(GLdouble v[4][4], GLdouble copy[4][4]){
    for(int i = 0; i < 4; i += 1){
        for(int j = 0; j < 4; j += 1){
            copy[i][j] = v[i][j];
        }
    }
}

/* Builds a 4x4 matrix representing orthographic projection with a boxy viewing 
volume [left, right] x [bottom, top] x [far, near]. That is, on the near plane 
the box is the rectangle R = [left, right] x [bottom, top], and on the far 
plane the box is the same rectangle R. Keep in mind that 0 > near > far. Maps 
the viewing volume to [-1, 1] x [-1, 1] x [-1, 1]. */
void mat44Orthographic(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, 
        GLdouble far, GLdouble near, GLdouble proj[4][4]){
    proj[0][0] = 2.0 / (right - left);
    proj[0][1] = 0;
    proj[0][2] = 0;
    proj[0][3] = (-right - left) / (right - left);
    proj[1][0] = 0;
    proj[1][1] = 2.0 / (top - bottom);
    proj[1][2] = 0;
    proj[1][3] = (-top - bottom) / (top - bottom);
    proj[2][0] = 0;
    proj[2][1] = 0;
    proj[2][2] = -2.0 / (near - far);
    proj[2][3] = -((-near - far) / (near - far));
    proj[3][0] = 0;
    proj[3][1] = 0;
    proj[3][2] = 0;
    proj[3][3] = 1.0;      
}

/* Builds a 4x4 matrix that maps a projected viewing volume 
[-1, 1] x [-1, 1] x [-1, 1] to screen [0, w - 1] x [0, h - 1] x [-1, 1]. */
void mat44Viewport(GLdouble width, GLdouble height, GLdouble view[4][4]){
    view[0][0] = (width - 1.0) / 2.0;
    view[0][1] = 0;
    view[0][2] = 0;
    view[0][3] = (width - 1.0) / 2.0;
    view[1][0] = 0;
    view[1][1] = (height - 1.0) / 2.0;
    view[1][2] = 0;
    view[1][3] = (height - 1.0) / 2.0;
    view[2][0] = 0;
    view[2][1] = 0;
    view[2][2] = 1.0;
    view[2][3] = 0;
    view[3][0] = 0;
    view[3][1] = 0;
    view[3][2] = 0;
    view[3][3] = 1.0;
}

/* Builds a 4x4 matrix representing perspective projection. The viewing frustum 
is contained between the near and far planes, with 0 > near > far. On the near 
plane, the frustum is the rectangle R = [left, right] x [bottom, top]. On the 
far plane, the frustum is the rectangle (far / near) * R. Maps the viewing 
volume to [-1, 1] x [-1, 1] x [-1, 1]. */
void mat44Perspective(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, 
        GLdouble far, GLdouble near, GLdouble proj[4][4]){
    proj[0][0] = (-2.0 * near) / (right - left);
    proj[0][1] = 0;
    proj[0][2] = (right + left) / (right - left);
    proj[0][3] = 0;
    proj[1][0] = 0;
    proj[1][1] = (-2.0 * near) / (top - bottom);
    proj[1][2] = (top + bottom) / (top - bottom);
    proj[1][3] = 0;
    proj[2][0] = 0;
    proj[2][1] = 0;
    proj[2][2] = -((-near - far) / (near - far));
    proj[2][3] = -((2.0 * near * far) / (near - far));
    proj[3][0] = 0;
    proj[3][1] = 0;
    proj[3][2] = -1.0;
    proj[3][3] = 0;
}

/* Builds a 3 x 3 identity matrix in the array that's passed in */
void mat33Identity(GLdouble m[3][3]){
    for(int i = 0; i < 3; i += 1){
        for(int j = 0; j < 3; j += 1){
            if(i == j)
                m[i][j] = 1.0;
            else
                m[i][j] = 0.0;
        }
    }
}

/* Builds a 4 x 4 identity matrix in the array that's passed in */
void mat44Identity(GLdouble m[4][4]){
    for(int i = 0; i < 4; i += 1){
        for(int j = 0; j < 4; j += 1){
            if(i == j)
                m[i][j] = 1.0;
            else
                m[i][j] = 0.0;
        }
    }
}

/* This inverts the given 4 x 4 matrix and replaces all of the doubles with GLfloats.
The inversion is because 2 dimensional arrays are stored column by column in openGL
instead of row by row like they are in C. */
void mat44OpenGL(double m[4][4], GLfloat openGL[4][4]) {
	for (int i = 0; i < 4; i += 1)
		for (int j = 0; j < 4; j += 1)
			openGL[i][j] = m[j][i];
}