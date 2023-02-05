#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define LIMIT_UBYTE(n) (n > USHRT_MAX) ? USHRT_MAX : (n < 0) ? 0 : n

int main()
{
    FILE* infp, * outfp;
    char inputfile[] = "./postData230124_/Ceph_Sample_16_3000x2400.raw";
    char outfile[] = "./histoTest.raw";

    unsigned short* inimg, * outimg;

    int width = 3000;
    int height = 2400;
    int imageSize = width * height;

    int k;

    unsigned long histogram[65536], sum, sum_of_h[65536];
    float value;
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

    /* constant = new # of gray levels div by area */
    constant = (float)(65536) / (float)(height * width);

    /* ---------------------------------------------------------------------- */
    /* 0~ 2600 까지 히스토 분포 */

    for (int i = 0; i < 65536; i++) histogram[i] = 0;   //histogram 배열 초기화

    for (int y = 0; y < height; y++) {  //histogram 분포
        for (int x = 0; x < width - 400; x++) {
            int offset = x + y * width;
            value = inimg[offset];
            histogram[(unsigned short)(value)] += 1;
            outimg[offset] = value;
        }
    }

    //histogram 누적 합
    for (int i = 0, sum = 0; i < 65536; i++) {
        sum += histogram[i];
        sum_of_h[i] = sum;
    }

    for (int y = 0; y < height; y++) {  //histogram 분포
        for (int x = 0; x < width - 400; x++) {
            int offset = x + y * width;

            k = outimg[offset];
            outimg[offset] = sum_of_h[k] * constant;
        }
    }

    /* ---------------------------------------------------------------------- */
    /* 2600 ~ 3000 까지 히스토 분포 */
    for (int i = 0; i < 65536; i++) histogram[i] = 0;   //histogram 배열 초기화

    for (int y = 0; y < height; y++) {  //histogram 분포
        for (int x = width-400; x < width; x++) {
            int offset = x + y * width;
            value = inimg[offset];
            histogram[(unsigned short)(value)] += 1;
            outimg[offset] = value;
        }
    }

    //histogram 누적 합
    for (int i = 0, sum = 0; i < 65536; i++) {
        sum += histogram[i];
        sum_of_h[i] = sum;
    }

    for (int y = 0; y < height; y++) {  //histogram 분포
        for (int x = width-400; x < width; x++) {
            int offset = x + y * width;

            k = outimg[offset];
            outimg[offset] = sum_of_h[k] * constant;
        }
    }


    if ((outfp = fopen(outfile, "wb")) == NULL) {
        fprintf(stderr, "cannot open this file");
        return -1;
    }

    fwrite(outimg, sizeof(unsigned short) * imageSize, 1, outfp);


    free(inimg);
    free(outimg);

    fclose(outfp);
    printf("\n 3000x2400 16bit unsigned\n");
    printf("Success processing !!\n");
    return 0;
}