#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "helpers.h"



int main(int argc, char *argv[]) {

    // Remember filenames
    char *inFile = argv[1];
    char *outFile = argv[2];

    // Open files
    FILE *inPtr = fopen(inFile, "r");
    if (inPtr == NULL) {
        printf("Couldn't open %s. \n", inFile);
        return 1;
    }

    FILE *outPtr = fopen(outFile, "w");
    if (outPtr == NULL) {
        printf("Couldn't open %s. \n", outFile);
        fclose(inPtr);
        return 2;
    }

    // Read inFile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inPtr);

    // Check if inFile is bitmap image
    if (bf.bfType != 0x4d42) {
        printf("Unsupported file format. \n");
        fclose(inPtr);
        fclose(outPtr);
        return 3;
    }

    // Check size of info header (differs with version)
    DWORD infoHeaderSize;
    fread(&infoHeaderSize, sizeof(DWORD), 1, inPtr);

    // Read height and width of image
    DWORD height;
    fread(&height, sizeof(DWORD), 1, inPtr);
    height = abs((int) height);
    DWORD width;
    fread(&width, sizeof(DWORD), 1, inPtr);
    width = abs((int) width);

    // Go back to the start of the header
    fseek(inPtr, -3 * sizeof(DWORD), SEEK_CUR);

    // Allocate memory for image
    RGBTRIPLE image[height][width];

    // Determine padding for scanlines
    int padding = (4 - ((width * sizeof(RGBTRIPLE)) % 4)) % 4;

    // Go to start of image bits
    fseek(inPtr, infoHeaderSize, SEEK_CUR);

    // Iterate over image data
    for (int i = 0; i < height; i++) {
        // Read array of pixel's into image (one scanline)
        fread(&image[i], sizeof(RGBTRIPLE), width, inPtr);

        // Skip over padding
        fseek(inPtr, padding, SEEK_CUR);
    }

    // Go back to start of info header
    fseek(inPtr, sizeof(BITMAPFILEHEADER), SEEK_SET);


    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j].rgbtBlue = 50 * pow(image[i][j].rgbtBlue);
            image[i][j].rgbtGreen = 50 * log(1 + image[i][j].rgbtGreen);
            image[i][j].rgbtRed = 50 * log(1 + image[i][j].rgbtRed);
        }
    }

    // Choose and read header
    switch (infoHeaderSize) {
        // BITMAPINFOHEADER
        case 40:
            transV0(inPtr, bf, height, width, image, padding, outPtr);
            break;

        // BITMAPV4HEADER
        case 108:
            transV4(inPtr, bf, height, width, image, padding, outPtr);
            break;

        // BITMAPV5HEADER
        case 124:
            transV5(inPtr, bf, height, width, image, padding, outPtr);
            break;

        // Unsupported file format
        default:
            printf("File format not supported. \n");
            fclose(inPtr);
            fclose(outPtr);
            return 4;
    }

    // Close files
    fclose(inPtr);
    fclose(outPtr);

    return 0;
}

