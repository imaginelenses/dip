#include <stdio.h>

#include "bmp.h"

int transV0(FILE *inPtr, BITMAPFILEHEADER bf, int height, int width, RGBTRIPLE image[height][width], int padding, FILE *outPtr);
int transV4(FILE *inPtr, BITMAPFILEHEADER bf, int height, int width, RGBTRIPLE image[height][width], int padding, FILE *outPtr);
int transV5(FILE *inPtr, BITMAPFILEHEADER bf, int height, int width, RGBTRIPLE image[height][width], int padding, FILE *outPtr);
void writeImg(int height, int width, RGBTRIPLE image[height][width], int padding, FILE *outPtr);