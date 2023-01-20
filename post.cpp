#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

typedef unsigned short ushort;

int main()
{
	FILE* infp, * outfp;
	char inputfile[] = "./postData230124_/Ceph_Sample_16_3000x2400.raw";
	char savefile[] = "./postProcessing/Ceph_postdata_16_3000x2400.raw";


	ushort* inimg, * outimg;

	int width = 3000;
	int height = 2400;
	int imageSize = width * height;

	inimg = (ushort*)malloc(sizeof(ushort) * imageSize);
	outimg = (ushort*)malloc(sizeof(ushort) * imageSize);

	memset(inimg, 0, sizeof(ushort) * imageSize);
	memset(outimg, 0, sizeof(ushort) * imageSize);


	//cout << file << endl;	// 파일 fopen 확인
	if ((infp = fopen(inputfile, "rb")) == NULL) {
		printf("No such file or folder\n");
		return -1;
	}

	fread(inimg, sizeof(ushort) * imageSize, 1, infp);
	fclose(infp);

	for (int i = 0; i < imageSize; i++) {
		*(outimg + i) = *(inimg + i);
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