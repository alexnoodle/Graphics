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
void mat22Multiply(double m[2][2], double v[2], double mTimesV[2]) {
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