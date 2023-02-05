#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define LIMIT_UBYTE(n) (n > USHRT_MAX) ? USHRT_MAX : (n < 0) ? 0 : n

int main()
{
    FILE* infp, * outfp;
    char inputfile[] = "./postData230124_/Pano_Sample_float_3000x1628.raw";
    char outfile[] = "./CLAHE_Test.raw";

    float* inimg, * outimg;

    int width = 3000;
    int height = 1628;
    int imageSize = width * height;

    int k, offset;

    unsigned long histogram[65536], sum_of_h[65536], sum = 0.0;

    int value = 0;
    double constant;

    int grid = 10;
    int yBoundary = height % grid;

    inimg = (float*)malloc(sizeof(float) * imageSize);
    outimg = (float*)malloc(sizeof(float) * imageSize);

    memset(inimg, 0, sizeof(float) * imageSize);
    memset(outimg, 0, sizeof(float) * imageSize);

    if ((infp = fopen(inputfile, "rb")) == NULL) {
        printf("No such file or folder\n");
        return 0;
    }

    fread(inimg, sizeof(float) * imageSize, 1, infp);
    fclose(infp);

    /* constant = new # of gray levels div by area */ // ��� ó��
    constant = (float)(65536) / (float)(grid * grid);

    /* ���� ��� �ɸ��� ������*/
    for (int y = 0; y < height - grid; y += grid) {
        for (int x = 0; x < width; x += grid) {

            for (int i = 0; i < 65536; i++) {
                histogram[i] = 0;   //histogram �迭 �ʱ�ȭ
                sum_of_h[i] = 0;
            }
            // offset ~ offset+10 ������ ������׷� ����
            for (int i = 0; i < grid; i++) {
                for (int j = 0; j < grid; j++) {
                    offset = x + j + (y + i) * width;

                    value = inimg[offset];
                    histogram[value] += 1;
                    outimg[offset] = inimg[offset];
                }
            }

            // grid x grid������׷� ������ ���� ��
            for (int i = 0, sum = 0; i < 65536; i++) {
                sum += histogram[i];
                sum_of_h[i] = sum;
            }

            /* ����ȭ = new # of gray levels div by area */
            for (int i = 0; i < grid; i++) {
                for (int j = 0; j < grid; j++) {
                    offset = x + j + (y + i) * width;
                    k = outimg[offset];
                    outimg[offset] = LIMIT_UBYTE(sum_of_h[k] * constant);
                }
            }
        };
    };

    /* ��� */
    for (int y = height - yBoundary; y < height; y += yBoundary) {
        for (int x = 0; x < width; x += grid) {

            for (int i = 0; i < 65536; i++) {
                histogram[i] = 0;   //histogram �迭 �ʱ�ȭ
                sum_of_h[i] = 0;
            }
            // ��� ������׷� ����
            for (int i = 0; i < (height % grid); i++) {
                for (int j = 0; j < grid; j++) {
                    offset = x + j + (y + i) * width;

                    value = inimg[offset];
                    histogram[value] += 1;
                    outimg[offset] = inimg[offset];
                }
            }

            // ��� grid ������׷� ������ ���� ��
            for (int i = 0, sum = 0; i < 65536; i++) {
                sum += histogram[i];
                sum_of_h[i] = sum;
            }

            /* ����ȭ = new # of gray levels div by area */
            for (int i = 0; i < (height % grid); i++) {
                for (int j = 0; j < grid; j++) {
                    offset = x + j + (y + i) * width;
                    k = outimg[offset];
                    outimg[offset] = LIMIT_UBYTE(sum_of_h[k] * constant);
                }
            }
        };
    };

    if ((outfp = fopen(outfile, "wb")) == NULL) {
        fprintf(stderr, "cannot open this file");
        return 0;
    }

    fwrite(outimg, sizeof(float) * imageSize, 1, outfp);

    free(inimg);
    free(outimg);

    fclose(outfp);
    printf("\n 3000x1628 32bit Real\n");
    printf("Success processing !!\n");
    return 0;
}
