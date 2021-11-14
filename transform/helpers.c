#include "helpers.h"

#include <stdio.h>

// Transform version 0 BMP
int transV0(FILE *inPtr, BITMAPFILEHEADER bf, int height, int width, 
    RGBTRIPLE image[height][width], int padding, FILE *outPtr) {

    // Read bitmap info header
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inPtr);

    // Write headers to outFile
    fwrite(&bf, sizeof(bf), 1, outPtr);
    fwrite(&bi, sizeof(bi), 1, outPtr);
    
    // Write new image data to outFile
    writeImg(height, width, image, padding, outPtr);

    return 0;
}


// Transform version 4 BMP
int transV4(FILE *inPtr, BITMAPFILEHEADER bf, int height, int width, 
    RGBTRIPLE image[height][width], int padding, FILE *outPtr) {

    // Read bitmap info header
    BITMAPV4HEADER bV4;
    fread(&bV4, sizeof(BITMAPV4HEADER), 1, inPtr);

    // Write headers to outFile
    fwrite(&bf, sizeof(bf), 1, outPtr);
    fwrite(&bV4, sizeof(bV4), 1, outPtr);
    
    // Write new image data to outFile
    writeImg(height, width, image, padding, outPtr);
    return 0;
}


// Transform version 0 BMP
int transV5(FILE *inPtr, BITMAPFILEHEADER bf, int height, int width, 
    RGBTRIPLE image[height][width], int padding, FILE *outPtr) {

    // Read bitmap info header
    BITMAPV5HEADER bV5;
    fread(&bV5, sizeof(BITMAPV5HEADER), 1, inPtr);

    // Write headers to outFile
    fwrite(&bf, sizeof(bf), 1, outPtr);
    fwrite(&bV5, sizeof(bV5), 1, outPtr);

    // Write new image data to outFile
    writeImg(height, width, image, padding, outPtr);
    return 0;
}

// Write new image data to outFile
void writeImg(int height, int width, RGBTRIPLE image[height][width], int padding, FILE *outPtr) {
    for (int i = 0; i < height; i++) {
        // Write row to outFile
        fwrite(image[i], sizeof(RGBTRIPLE), width, outPtr);

        // Add padding to end of row
        for (int j = 0; j < padding; j++) {
            fputc(0x00, outPtr);
        }
    }
}