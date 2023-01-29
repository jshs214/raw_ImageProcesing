#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define LIMIT_UBYTE(n) (n > USHRT_MAX) ? USHRT_MAX : (n < 0) ? 0 : n

int main()
{
    FILE* infp, * outfp;
    char inputfile[] = "./stitching/Pano2pixel.raw";
    char outfile[] = "./histoTest2.raw";

    unsigned short* inimg, * outimg;

    int width = 2462;
    int height = 1628;
    int imageSize = width * height;
    int rowSize = width;

    float gray, gray_max = -1, gray_min = 65536;

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


    for (int i = 0; i < imageSize; i++) {   //histogram 분포
        gray = inimg[i];

        if (gray_max <= gray) gray_max = gray;    //최대, 최소  gray value 값 계산
        if (gray_min >= gray) gray_min = gray;
    }

    printf("gray max : %f, gray min : %f", gray_max, gray_min);

    /* histo gram stretching */
    for (int i = 0; i < imageSize; i++) {
        outimg[i] = (inimg[i] - gray_min / gray_max - gray_min ) * 65535;
        outimg[i] = ~outimg[i];
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