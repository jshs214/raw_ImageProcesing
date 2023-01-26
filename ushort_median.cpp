#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

typedef unsigned short ushort;

void insertion(ushort a[], int n)
{
    int i, j;
    for (i = 1; i < n; i++) {
        int tmp = a[i];
        for (j = i; j > 0 && a[j - 1] > tmp; j--)
            a[j] = a[j - 1];
        a[j] = tmp;
    }
}

int main()
{
    FILE* infp, * outfp;
    char inputfile[] = "./postData230124_/Ceph_Sample_16_3000x2400.raw";
    char savefile[] = "./postProcessing/Ceph_PostSample_16_3000x2400.raw";

    ushort* inimg, * outimg;

    int width = 3000;
    int height = 2400;
    int imageSize = width * height;
    int rowSize = width;
    int cnt = 0;
    double sum = 0.0;

    ushort arr[9] = { 0, };

    inimg = (ushort*)malloc(sizeof(ushort) * imageSize);
    outimg = (ushort*)malloc(sizeof(ushort) * imageSize);

    memset(inimg, 0, sizeof(ushort) * imageSize);
    memset(outimg, 0, sizeof(ushort) * imageSize);

    if ((infp = fopen(inputfile, "rb")) == NULL) {
        printf("No such file or folder\n");
        return -1;
    }

    fread(inimg, sizeof(ushort) * imageSize, 1, infp);
    fclose(infp);

    int widthCnt = 0, heightCnt = -1;

    for (int i = 0; i < imageSize; i++) {
        widthCnt = i % width;
        if (i % width == 0) heightCnt++;

        int offset = widthCnt + (heightCnt * width);
        if (widthCnt == 0) {
            //좌측 상단 Vertex
            if (heightCnt == 0) {
                arr[0] = arr[1] = arr[3] = arr[4] = inimg[widthCnt + (heightCnt * rowSize)];
                arr[2] = arr[5] = inimg[widthCnt + 1 + (heightCnt * rowSize)];
                arr[6] = arr[7] = inimg[widthCnt + ((heightCnt + 1) * rowSize)];
                arr[8] = inimg[widthCnt + 1 + ((heightCnt + 1) * rowSize)];
            }
            //좌측 하단 Vertex
            else if (heightCnt == height - 1) {
                arr[0] = arr[1] = inimg[widthCnt + ((heightCnt - 1) * rowSize)];
                arr[2] = inimg[widthCnt + 1 + ((heightCnt - 1) * rowSize)];
                arr[3] = arr[6] = arr[7] = arr[4] = inimg[widthCnt + ((heightCnt * rowSize))];
                arr[8] = arr[5] = inimg[widthCnt + 1 + (heightCnt * rowSize)];
            }
            else {
                arr[0] = arr[1] = inimg[widthCnt + ((heightCnt - 1) * rowSize)];
                arr[2] = inimg[widthCnt + 1 + ((heightCnt - 1) * rowSize)];
                arr[3] = arr[4] = inimg[widthCnt + (heightCnt * rowSize)];
                arr[5] = inimg[widthCnt + 1 + (heightCnt * rowSize)];
                arr[6] = arr[7] = inimg[widthCnt + ((heightCnt + 1) * rowSize)];
                arr[8] = inimg[widthCnt + 1 + ((heightCnt + 1) * rowSize)];
            }

            insertion(arr, 9);
            outimg[(widthCnt + heightCnt * rowSize)] = arr[4];
        }
        else if (widthCnt == (rowSize - 1)) {
            //우측 상단 Vertex
            if (heightCnt == 0) {
                arr[0] = arr[3] = inimg[widthCnt - 1 + (heightCnt * rowSize)];
                arr[1] = arr[2] = arr[5] = arr[4] = inimg[widthCnt + (heightCnt * rowSize)];
                arr[6] = inimg[widthCnt - 1 + ((heightCnt - 1) * rowSize)];
                arr[7] = arr[8] = inimg[widthCnt + ((heightCnt + 1) * rowSize)];
            }
            //우측 하단 Vertex
            else if (heightCnt == height - 1) {
                arr[0] = inimg[widthCnt - 1 + ((heightCnt - 1) * rowSize)];
                arr[1] = arr[2] = inimg[widthCnt - 1 + ((heightCnt - 1) * rowSize)];
                arr[3] = arr[6] = inimg[widthCnt - 1 + (heightCnt * rowSize)];
                arr[4] = arr[5] = arr[7] = arr[8] = inimg[widthCnt + (heightCnt * rowSize)];
            }
            else {
                arr[0] = inimg[widthCnt - 1 + ((heightCnt - 1) * rowSize)];
                arr[2] = arr[1] = inimg[widthCnt + ((heightCnt - 1) * rowSize)];
                arr[3] = inimg[widthCnt - 1 + (heightCnt * rowSize)];
                arr[5] = arr[4] = inimg[widthCnt + (heightCnt * rowSize)];
                arr[6] = inimg[widthCnt - 1 + ((heightCnt + 1) * rowSize)];
                arr[8] = arr[7] = inimg[widthCnt + ((heightCnt + 1) * rowSize)];
            }

            insertion(arr, 9);
            outimg[(widthCnt + heightCnt * rowSize)] = arr[4];
        }
        else if (heightCnt == 0) {
            if (widthCnt != 1 && widthCnt != rowSize - 1) {
                arr[0] = arr[3] = inimg[widthCnt - 1 + (heightCnt * rowSize)];
                arr[1] = arr[4] = inimg[widthCnt + (heightCnt * rowSize)];
                arr[2] = arr[5] = inimg[widthCnt + 1 + (heightCnt * rowSize)];
                arr[6] = inimg[widthCnt - 1 + ((heightCnt + 1) * rowSize)];
                arr[7] = inimg[widthCnt + ((heightCnt + 1) * rowSize)];
                arr[8] = inimg[widthCnt + 1 + ((heightCnt + 1) * rowSize)];
            }

            insertion(arr, 9);
            outimg[(widthCnt + heightCnt * rowSize)] = arr[4];
        }
        else if (heightCnt == (height - 1)) {
            if (widthCnt != 1 && widthCnt != rowSize - 1) {
                arr[0] = inimg[widthCnt - 1 + ((heightCnt - 1) * rowSize)];
                arr[1] = inimg[widthCnt + ((heightCnt - 1) * rowSize)];
                arr[2] = inimg[widthCnt + 1 + ((heightCnt - 1) * rowSize)];
                arr[3] = arr[6] = inimg[widthCnt - 1 + (heightCnt * rowSize)];
                arr[4] = arr[7] = inimg[widthCnt + (heightCnt * rowSize)];
                arr[5] = arr[8] = inimg[widthCnt + 1 + (heightCnt * rowSize)];
            }

            insertion(arr, 9);
            outimg[(widthCnt + heightCnt * rowSize)] = arr[4];
        }
        else {
            cnt = 0;
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    arr[cnt++] = inimg[((widthCnt + i) + (heightCnt + j) * width)];
                }
            }
            insertion(arr, 9);
            outimg[(widthCnt + heightCnt * rowSize)] = arr[4];
        }

    }

    if ((outfp = fopen(savefile, "wb")) == NULL) {
        printf("%d No such file or folder\n", __LINE__);
        return -1;
    }
    fwrite(outimg, sizeof(ushort) * imageSize, 1, outfp);

    free(inimg);
    free(outimg);

    fclose(outfp);

    printf("Success processing !!\n");
    return 0;
}