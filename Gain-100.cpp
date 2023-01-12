#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <io.h>
#include <conio.h>

using namespace std;

int main(int argc, char** argv)
{
	FILE* infp, * outfp;
	char savefile[] = "./output(1628x1628)/subGainMap(1628).raw";

	int width = 1628, height = 1628;
	int imageSize = width * height;
	int subImageSize = (width - 200) * (height - 200);
	int j = 0;
	int widthcnt = 0;

	unsigned short* inimg, * outimg;

	inimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	outimg = (unsigned short*)malloc(sizeof(unsigned short) * subImageSize);
	//outimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);

	memset(inimg, 0, sizeof(unsigned short) * imageSize);
	memset(outimg, 0, sizeof(unsigned short) * subImageSize);
	//memset(outimg, 0, sizeof(unsigned short) * imageSize);

	if ((infp = fopen("./output(1628x1628)/GainMap(1628).raw", "rb")) == NULL) {
		printf("No such file or folder\n");
		return -1;
	}
	fread(inimg, sizeof(unsigned short) * imageSize, 1, infp);
	fclose(infp);

	for (int i = 0; i < imageSize; i++) {
		widthcnt++;
		if(widthcnt == width) widthcnt = 0;

		if(width * 100 > i || width * (height - 100) < i) continue;
		if(widthcnt <= 100 || widthcnt > 1528) continue;

		outimg[j] = inimg[i];
		j++;
	}
	printf("%d", j);

	if ((outfp = fopen(savefile, "wb")) == NULL) {
		printf("%d No such file or folder\n", __LINE__);
		return -1;
	}
	fwrite(outimg, sizeof(unsigned short) * subImageSize, 1, outfp);

	fclose(outfp);

	free(inimg);
	free(outimg);

	return 0;
}
