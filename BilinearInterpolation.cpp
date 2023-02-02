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
    char inputfile[] = "./grid10CLAHE.raw";
    char outfile[] = "./CLAHE_Interpolation_.raw";

    float* inimg, * outimg;

    int width = 3000;
    int height = 1628;
    int imageSize = width * height;

    int offset;

    int grid = 10, cnt = 0;

    inimg = (float*)malloc(sizeof(float) * imageSize);
    outimg = (float*)malloc(sizeof(float) * imageSize);

    memset(inimg, 0, sizeof(float) * imageSize);
    memset(outimg, 0, sizeof(float) * imageSize);
    

    if ((infp = fopen(inputfile, "rb")) == NULL) {
        printf("No such file or folder\n");
        return;
    }
    fread(inimg, sizeof(float) * imageSize, 1, infp);
    fclose(infp);

    float value = 0.0;
    float z, t, s, w ,ul, ur, bl, br;

    for (int y = grid; y < height - grid; y +=grid) {
        for (int x = grid; x < width - grid; x +=grid) {
            //int offset = x + (y * width);
            value = 0.0;
            
            for (int i = 0; i < grid; i++) {
                for (int j = 0; j < grid; j++) {
                    int offset = (x+j) + ((y+i) * width);

                    z = inimg[(x - grid +j) + (y * width)];
                    t = inimg[(grid - j -1) + (y * width)];
                    w = inimg[x + ( (y - grid + i) * width)];
                    s = inimg[x + ( (grid - i + 1) * width)];

                    ul = inimg[x + grid/2 + ((y+grid/2) * width)];
                    ur = inimg[x + grid / 2 + grid + ((y + grid / 2) * width)];

                    bl = inimg[x + grid / 2 + ((y + grid / 2 + grid) * width)];
                    br = inimg[x + grid / 2 + grid + ((y + grid / 2 + grid) * width)];


                    value = (s / (s + w)) * ((t / (z + t)) * ul + (z / (z + t)) * bl)
                        + (w / (s + w)) * ((t / (z + t)) * ur + (z / (z + t)) * br);

                    outimg[offset] = LIMIT_UBYTE(value);
                }
            }


        }
    }

    if ((outfp = fopen(outfile, "wb")) == NULL) {
        fprintf(stderr, "cannot open this file");
        return;
    }

    fwrite(outimg, sizeof(float) * imageSize, 1, outfp);

    free(inimg);
    free(outimg);

    fclose(outfp);
}
