#include <stdio.h>

typedef struct renRenderer renRenderer;

/* Holds all of the necessary values and functions that can be passed around */
struct renRenderer {
    int unifDim;
    int texNum;
    int attrDim;
    int varyDim;
    void (*colorPixel)(renRenderer*, double[], texTexture(*)[], double[], double[]);
    void (*transformVertex)(renRenderer*, double[], double[], double[]);
    void (*updateUniform)(renRenderer*, double[], double[]);
};