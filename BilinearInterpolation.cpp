#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define LIMIT_UBYTE(n) (n > USHRT_MAX) ? USHRT_MAX : (n < 0) ? 0 : n

void histo();

int main()
{
    histo();

    printf("\n 3000x1628 32bit Real\n");
    printf("Success processing !!\n");
    return 0;
}

void histo() {
    FILE* infp, * outfp;
    char inputfile[] = "./grid6CLAHE.raw";
    char outfile[] = "./CLAHE_Interpolation_.jpg";

    float* inimg, * outimg;

    int width = 3000;
    int height = 1628;
    int imageSize = width * height;

    int offset;

    int grid = 6, cnt = 0;

    inimg = (float*)malloc(sizeof(float) * imageSize);
    outimg = (float*)malloc(sizeof(float) * imageSize);

    memset(inimg, 0, sizeof(float) * imageSize);
    memset(outimg, 0, sizeof(float) * imageSize);
    
    FILE* histofp;
    char histofile[] = "./histoTest.raw";
    float* histoimg;
    histoimg = (float*)malloc(sizeof(float) * imageSize);
    memset(histoimg, 0, sizeof(float) * imageSize);

    if ((histofp = fopen(histofile, "rb")) == NULL) {
        printf("No such file or folder\n");
        return;
    }
    fread(histoimg, sizeof(float) * imageSize, 1, histofp);
    fclose(histofp);
    

    if ((infp = fopen(inputfile, "rb")) == NULL) {
        printf("No such file or folder\n");
        return;
    }
    fread(inimg, sizeof(float) * imageSize, 1, infp);
    fclose(infp);


    float rx, ry, p, q;
    int value;
    int x1, x2, y1, y2;

    float result = 0.0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int offset = x + (y * width);

            rx = (float)(grid - 1) * x / (width - 1);
            ry = (float)(grid - 1) * y / (height - 1);

            x1 = (int)rx;
            y1 = (int)ry;

            x2 = x1 + 1; if (x2 == grid) x2 = grid - 1;
            y2 = y1 + 1; if (y2 == grid) y2 = grid - 1;

            p = rx - x1;
            q = ry - y1;

            value = (1. - p) * (1. - q) * histoimg[x1 + (y1 * width)]
                + p * (1. - q) * histoimg[x2 + (y1 * width)]
                + (1. - p) * q * histoimg[x1 + (y2 * width)]
                + p * q * histoimg[x2 + (y2 * width)];

            result = LIMIT_UBYTE( (value + .5) - histoimg[offset] );
            
            outimg[offset] = abs(65535 - result);
        }
    }



    if ((outfp = fopen(outfile, "wb")) == NULL) {
        fprintf(stderr, "cannot open this file");
        return;
    }

    fwrite(outimg, sizeof(float) * imageSize, 1, outfp);

    free(inimg);
    free(outimg);
    free(histoimg);

    fclose(outfp);
}
