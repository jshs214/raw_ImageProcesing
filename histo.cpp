#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define LIMIT_UBYTE(n) (n > USHRT_MAX) ? USHRT_MAX : (n < 0) ? 0 : n

int main()
{
    FILE* infp, * outfp;
    char inputfile[] = "./stitching/Pano2pixel.raw";
    char outfile[] = "./histoTest.raw";

    unsigned short* inimg, * outimg;

    int width = 2462;
    int height = 1628;
    int imageSize = width * height;
    int rowSize = width;

    int k;

    unsigned long histogram[65536], sum, sum_of_h[65536];
    float gray;
    double constant;

    inimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
    outimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);

    memset(inimg, 0, sizeof(unsigned short) * imageSize);
    memset(outimg, 0, sizeof(unsigned short) * imageSize);

    if ((infp = fopen(inputfile, "rb")) == NULL) {
        printf("No such file or folder\n");
        return -1;
    }

    fread(inimg, sizeof(unsigned short) * imageSize, 1, infp);
    fclose(infp);
    
    for (int i = 0; i < 65536; i++) histogram[i] = 0;   //histogram 배열 초기화

    for (int i = 0; i < imageSize; i++) {   //histogram 분포
        gray = inimg[i];
        histogram[(unsigned short)(gray)] += 1;
        outimg[i] = gray;
    }

    //histogram 누적 합
    for (int i = 0, sum = 0; i < 65536; i++){
        sum += histogram[i];
        sum_of_h[i] = sum;
    }

    /* constant = new # of gray levels div by area */
    constant = (float)(65536) / (float)(height * width);
    for (int i = 0; i < imageSize; i++) {
        k = outimg[i];
        outimg[i] = sum_of_h[k] * constant;
    }

    if ((outfp = fopen(outfile, "wb")) == NULL) {
        fprintf(stderr, "cannot open this file");
        return -1;
    }

    fwrite(outimg, sizeof(unsigned short) * imageSize, 1, outfp);


    free(inimg);
    free(outimg);

    fclose(outfp);
    printf("\n2462x1628 16bit unsigned\n");
    printf("Success processing !!\n");
    return 0;
}