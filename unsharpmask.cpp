#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define LIMIT_UBYTE(n) (n > USHRT_MAX) ? USHRT_MAX : (n < 0) ? 0 : n

int main()
{
    FILE* infp, * outfp;
    char inputfile[] = "./postData230124_/Pano_Sample_float_3000x1628.raw";
    char savefile[] = "./postProcessing/Pano_test_float_3000x1628.raw";

    float* inimg, * outimg, *imgDif;

    int width = 3000;
    int height = 1628;
    int imageSize = width * height;
    int rowSize = width;
    double sum = 0.0;

    float arr[9] = { 0, };
    float kernel[3][3] = { {1/9.0 , 1/9.0, 1/9.0},
                        {1/9.0, 1/9.0, 1/9.0},
                        { 1/9.0, 1/9.0, 1/9.0} };

    inimg = (float*)malloc(sizeof(float) * imageSize);
    outimg = (float*)malloc(sizeof(float) * imageSize);
    imgDif = (float*)malloc(sizeof(float) * imageSize);

    memset(inimg, 0, sizeof(float) * imageSize);
    memset(outimg, 0, sizeof(float) * imageSize);
    memset(imgDif, 0, sizeof(float) * imageSize);

    if ((infp = fopen(inputfile, "rb")) == NULL) {
        printf("No such file or folder\n");
        return -1;
    }

    fread(inimg, sizeof(float) * imageSize, 1, infp);
    fclose(infp);

    int widthCnt = 0, heightCnt = -1, cnt = 0;
    for (int i = 0; i < imageSize; i++) {
        widthCnt = i % width;
        if (i % width == 0) heightCnt++;

        int offset = widthCnt + (heightCnt * width);
        if (widthCnt == 0) {
            //좌측 상단 Vertex
            if (heightCnt == 0) {
                arr[0] = arr[1] = arr[3] = arr[4] = inimg[widthCnt + (heightCnt * rowSize) ];
                arr[2] = arr[5] = inimg[widthCnt + 1 + (heightCnt * rowSize) ];
                arr[6] = arr[7] = inimg[widthCnt + ((heightCnt + 1) * rowSize) ];
                arr[8] = inimg[widthCnt + 1 + ((heightCnt + 1) * rowSize) ];
            }
            //좌측 하단 Vertex
            else if (heightCnt == height - 1) {
                arr[0] = arr[1] = inimg[widthCnt + ((heightCnt - 1) * rowSize) ];
                arr[2] = inimg[widthCnt + 1 + ((heightCnt - 1) * rowSize) ];
                arr[3] = arr[6] = arr[7] = arr[4] = inimg[widthCnt + ((heightCnt * rowSize)) ];
                arr[8] = arr[5] = inimg[widthCnt + 1 + (heightCnt * rowSize) ];
            }
            else {
                arr[0] = arr[1] = inimg[widthCnt + ((heightCnt - 1) * rowSize) ];
                arr[2] = inimg[widthCnt + 1 + ((heightCnt - 1) * rowSize) ];
                arr[3] = arr[4] = inimg[widthCnt + (heightCnt * rowSize) ];
                arr[5] = inimg[widthCnt + 1 + (heightCnt * rowSize) ];
                arr[6] = arr[7] = inimg[widthCnt + ((heightCnt + 1) * rowSize) ];
                arr[8] = inimg[widthCnt + 1 + ((heightCnt + 1) * rowSize) ];
            }
            cnt = 0;
            sum = 0.0;
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    sum += kernel[i + 1][j + 1] * arr[cnt++];
                }
            }
            outimg[(widthCnt + heightCnt * rowSize)] = LIMIT_UBYTE(sum);

        }
        else if (widthCnt == (rowSize - 1)) {
            //우측 상단 Vertex
            if (heightCnt == 0) {
                arr[0] = arr[3] = inimg[widthCnt - 1 + (heightCnt * rowSize) ];
                arr[1] = arr[2] = arr[5] = arr[4] = inimg[widthCnt + (heightCnt * rowSize) ];
                arr[6] = inimg[widthCnt - 1 + ((heightCnt - 1) * rowSize) ];
                arr[7] = arr[8] = inimg[widthCnt + ((heightCnt + 1) * rowSize) ];
            }
            //우측 하단 Vertex
            else if (heightCnt == height - 1) {
                arr[0] = inimg[widthCnt - 1 + ((heightCnt - 1) * rowSize) ];
                arr[1] = arr[2] = inimg[widthCnt - 1 + ((heightCnt - 1) * rowSize) ];
                arr[3] = arr[6] = inimg[widthCnt - 1 + (heightCnt * rowSize) ];
                arr[4] = arr[5] = arr[7] = arr[8] = inimg[widthCnt + (heightCnt * rowSize) ];
            }
            else {
                arr[0] = inimg[widthCnt - 1 + ((heightCnt - 1) * rowSize) ];
                arr[2] = arr[1] = inimg[widthCnt + ((heightCnt - 1) * rowSize) ];
                arr[3] = inimg[widthCnt - 1 + (heightCnt * rowSize) ];
                arr[5] = arr[4] = inimg[widthCnt + (heightCnt * rowSize) ];
                arr[6] = inimg[widthCnt - 1 + ((heightCnt + 1) * rowSize) ];
                arr[8] = arr[7] = inimg[widthCnt + ((heightCnt + 1) * rowSize) ];
            }
            cnt = 0;
            sum = 0.0;
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    sum += kernel[i + 1][j + 1] * arr[cnt++];
                }
            }
            outimg[(widthCnt + heightCnt * rowSize)] = LIMIT_UBYTE(sum);

        }
        else if (heightCnt == 0) {
            if (widthCnt != 1 && widthCnt != rowSize - 1) {
                arr[0] = arr[3] = inimg[widthCnt - 1 + (heightCnt * rowSize) ];
                arr[1] = arr[4] = inimg[widthCnt + (heightCnt * rowSize) ];
                arr[2] = arr[5] = inimg[widthCnt + 1 + (heightCnt * rowSize) ];
                arr[6] = inimg[widthCnt - 1 + ((heightCnt + 1) * rowSize) ];
                arr[7] = inimg[widthCnt + ((heightCnt + 1) * rowSize) ];
                arr[8] = inimg[widthCnt + 1 + ((heightCnt + 1) * rowSize) ];
            }
            cnt = 0;
            sum = 0.0;
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    sum += kernel[i + 1][j + 1] * arr[cnt++];
                }
            }
            outimg[(widthCnt + heightCnt * rowSize) ] = LIMIT_UBYTE(sum);
        }
        else if (heightCnt == (height - 1)) {
            if (widthCnt != 1 && widthCnt != rowSize - 1) {
                arr[0] = inimg[widthCnt - 1 + ((heightCnt - 1) * rowSize) ];
                arr[1] = inimg[widthCnt + ((heightCnt - 1) * rowSize) ];
                arr[2] = inimg[widthCnt + 1 + ((heightCnt - 1) * rowSize) ];
                arr[3] = arr[6] = inimg[widthCnt - 1 + (heightCnt * rowSize) ];
                arr[4] = arr[7] = inimg[widthCnt + (heightCnt * rowSize) ];
                arr[5] = arr[8] = inimg[widthCnt + 1 + (heightCnt * rowSize) ];
            }
            cnt = 0;
            sum = 0.0;
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    sum += kernel[i + 1][j + 1] * arr[cnt++];
                }
            }
            outimg[(widthCnt + heightCnt * rowSize) ] = LIMIT_UBYTE(sum);
        }
        else {
            double sum = 0.0;
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    sum += kernel[i + 1][j + 1] * inimg[((widthCnt + i ) + (heightCnt + j) * rowSize) ];
                }
            }
            outimg[(widthCnt + heightCnt * rowSize) ] = LIMIT_UBYTE(sum);
        }

        *(imgDif + i) = *(inimg + i) - *(outimg + i);

        *(outimg + i) = LIMIT_UBYTE ( *(inimg + i) +  *(imgDif + i) );

       // printf("inimg : %f outimg : %f imgDif : %f\n", *(inimg + i), *(outimg + i), *(imgDif + i) );
    }

    if ((outfp = fopen(savefile, "wb")) == NULL) {
        printf("%d No such file or folder\n", __LINE__);
        return -1;
    }
    fwrite(outimg, sizeof(float) * imageSize, 1, outfp);

    free(inimg);
    free(outimg);

    fclose(outfp);


    printf("Success processing !!\n");
    return 0;
}