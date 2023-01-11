#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>

using namespace std;
void dirfile();
vector<string> fn;

int main(int argc, char** argv)
{
	FILE* fp, * result;

	unsigned short* inimg;
	unsigned short* outimg;

	int width = 1536;
	int height = 1536;
	int imageSize = width * height;

	fp = fopen("0000.raw", "rb");
	result = fopen("copy1536.raw", "wb");

	inimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	outimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);

	fread(inimg, sizeof(unsigned short) * imageSize, 1, fp);

	fclose(fp);


	// 1 Dimensional Array
	for (int i = 0; i < imageSize; i++) {
		outimg[i] = inimg[i];
	}

	for (int i = 0; i < 100; i++) {
		printf("%d ", inimg[i]);
	}


	fwrite(outimg, sizeof(unsigned short) * imageSize, 1, result);

	fclose(result);

	return 0;
}
