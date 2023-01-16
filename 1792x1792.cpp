#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <string>
#include <filesystem>

using namespace std;

using std::filesystem::directory_iterator;

typedef unsigned short ushort;

void dirDarkfile();		// ������ ����� Dark ���丮 �� ��� raw������ darkfn ���Ϳ� pushback �ϴ� �Լ� 
void dirGainfile();		// ������ ����� Gain ���丮 �� ��� raw������ gainfn ���Ϳ� pushback �ϴ� �Լ� 
void darkMap();			// darkfn�� ��հ����� darkMap.raw ���� �Լ�
void gainMap();			// gainfn�� ��հ����� gainMap.raw ���� �Լ�


vector<string> darkfn;	// Dark ���丮 �� raw���ϵ��� �����ϰ� �ִ� ����
vector<string> gainfn;	// Gain ���丮 �� raw���ϵ��� �����ϰ� �ִ� ����

void callibration();	// ���� �Լ�

int main()
{
	dirDarkfile();
	dirGainfile();
	darkMap();
	gainMap();

	callibration();

	printf("Success processing !!\n");
	return 0;
}

void darkMap() {
	FILE* infp, * outfp;
	char savefile[] = "./output(1792x1792)/DarkMap(1792).raw";

	ushort* inimg, * outimg;
	float* f_averageimg;

	int width = 1792;
	int height = 1792;
	int imageSize = width * height;

	inimg = (ushort*)malloc(sizeof(ushort) * imageSize);
	outimg = (ushort*)malloc(sizeof(ushort) * imageSize);
	f_averageimg = (float*)malloc(sizeof(float) * imageSize);

	memset(inimg, 0, sizeof(ushort) * imageSize);
	memset(outimg, 0, sizeof(ushort) * imageSize);
	memset(f_averageimg, 0, sizeof(float) * imageSize);

	vector<string>::iterator iter;
	iter = darkfn.begin();

	for (iter = darkfn.begin(); iter != darkfn.end(); iter++) {
		memset(inimg, 0, sizeof(ushort) * imageSize);
		string file = *iter;

		//cout << file << endl;	// ���� fopen Ȯ��
		if ((infp = fopen(file.c_str(), "rb")) == NULL) {
			printf("No such file or folder\n");
			return;
		}

		fread(inimg, sizeof(ushort) * imageSize, 1, infp);

		for (int i = 0; i < imageSize; i++) {
			*(f_averageimg + i) += inimg[i];
		}

		fclose(infp);
	}

	for (int i = 0; i < imageSize; i++) {
		*(outimg + i) = *(f_averageimg + i) / 101;
	}

	if ((outfp = fopen(savefile, "wb")) == NULL) {
		printf("%d No such file or folder\n", __LINE__);
		return;
	}
	fwrite(outimg, sizeof(ushort) * imageSize, 1, outfp);

	free(inimg);
	free(outimg);
	free(f_averageimg);
	fclose(outfp);
}

void gainMap() {
	FILE* infp, * outfp;
	char savefile[] = "./output(1792x1792)/GainMap(1792).raw";

	ushort* inimg, * outimg;
	float* f_averageimg;

	int width = 1792;
	int height = 1792;
	int imageSize = width * height;

	inimg = (ushort*)malloc(sizeof(ushort) * imageSize);
	outimg = (ushort*)malloc(sizeof(ushort) * imageSize);
	f_averageimg = (float*)malloc(sizeof(float) * imageSize);

	memset(inimg, 0, sizeof(ushort) * imageSize);
	memset(outimg, 0, sizeof(ushort) * imageSize);
	memset(f_averageimg, 0, sizeof(float) * imageSize);

	vector<string>::iterator iter;
	iter = gainfn.begin();
	for (iter = gainfn.begin(); iter != gainfn.end(); iter++) {
		memset(inimg, 0, sizeof(ushort) * imageSize);

		string file = *iter;
		//cout << file << endl;	// ���� fopen Ȯ��

		if ((infp = fopen(file.c_str(), "rb")) == NULL) {
			printf("No such file or folder\n");
			return;
		}

		fread(inimg, sizeof(ushort) * imageSize, 1, infp);

		for (int i = 0; i < imageSize; i++) {
			*(f_averageimg + i) += inimg[i];
		}

		fclose(infp);
	}


	for (int i = 0; i < imageSize; i++) {
		*(outimg + i) = *(f_averageimg + i) / 101;
	}

	if ((outfp = fopen(savefile, "wb")) == NULL) {
		printf("%d No such file or folder\n", __LINE__);
		return;
	}
	fwrite(outimg, sizeof(ushort) * imageSize, 1, outfp);

	free(inimg);
	free(outimg);
	free(f_averageimg);
	fclose(outfp);
}

void callibration() {
	FILE* GainMapFp, * darkMapFp, * MTF_HFp, * outfp;
	char savefile[] = "./output(1792x1792)/callibration(1792).raw";

	int width = 1792, height = 1792;
	int imageSize = width * height;
	int subImageSize = (width - 200) * (height - 200);
	double subGainSum = 0, subGainAvg = 0;

	ushort* darkMapImg, * GainMapImg, * MTF_HImg, * outimg;

	darkMapImg = (ushort*)malloc(sizeof(ushort) * imageSize);
	GainMapImg = (ushort*)malloc(sizeof(ushort) * imageSize);
	MTF_HImg = (ushort*)malloc(sizeof(ushort) * imageSize);
	outimg = (ushort*)malloc(sizeof(ushort) * imageSize);


	memset(darkMapImg, 0, sizeof(ushort) * imageSize);
	memset(GainMapImg, 0, sizeof(ushort) * imageSize);
	memset(MTF_HImg, 0, sizeof(ushort) * imageSize);
	memset(outimg, 0, sizeof(ushort) * imageSize);

	if ((darkMapFp = fopen("./output(1792x1792)/DarkMap(1792).raw", "rb")) == NULL) {
		printf("No such file or folder\n");
		return;
	}
	if ((GainMapFp = fopen("./output(1792x1792)/GainMap(1792).raw", "rb")) == NULL) {
		printf("No such file or folder\n");
		return;
	}
	if ((MTF_HFp = fopen("./S3_1792x1792/MTF_H.raw", "rb")) == NULL) {
		printf("No such file or folder\n");
		return;
	}

	fread(darkMapImg, sizeof(ushort) * imageSize, 1, darkMapFp);
	fread(GainMapImg, sizeof(ushort) * imageSize, 1, GainMapFp);
	fread(MTF_HImg, sizeof(ushort) * imageSize, 1, MTF_HFp);

	fclose(darkMapFp);
	fclose(GainMapFp);
	fclose(MTF_HFp);

	int j = 0;

	/* subGainSum Calculation */
	int widthcnt = 0;
	for (int i = 0; i < imageSize; i++) {
		widthcnt = i % width;
		if (widthcnt == width - 1) widthcnt = 0;

		if (width * 100 > i || width * (height - 100) < i) continue;
		if (widthcnt <= 100 || widthcnt > 1692) continue;

		subGainSum += GainMapImg[i];
		j++;
	}

	subGainAvg = subGainSum / subImageSize;
	printf("pixel count : %d\n", j);
	printf("Sum : %f \n", subGainSum);
	printf("mean : %f\n", subGainAvg);

	/* ���� for ��*/
	for (int i = 0; i < imageSize; i++) {
		*(outimg + i) =
			(ushort)(abs(*(MTF_HImg + i) - (*(darkMapImg + i))) / (float)(*(GainMapImg + i)) * subGainAvg);
	}


	if ((outfp = fopen(savefile, "wb")) == NULL) {
		printf("%d No such file or folder\n", __LINE__);
		return;
	}

	fwrite(outimg, sizeof(ushort) * imageSize, 1, outfp);

	fclose(outfp);

	free(GainMapImg);
	free(darkMapImg);
	free(MTF_HImg);
	free(outimg);
}

// dark ���丮 �� ���� ã�� �Լ�
void dirDarkfile()
{
	string path = "./S3_1792x1792/Dark/";

	for (const auto& file : directory_iterator(path)) {
		std::string filepath{ file.path().u8string() };
		darkfn.push_back(filepath);
	}

	return;
}

// Gain ���丮 �� ���� ã�� �Լ�
void dirGainfile()
{
	string path = "./S3_1792x1792/Gain/";

	for (const auto& file : directory_iterator(path)) {
		std::string filepath{ file.path().u8string() };
		gainfn.push_back(filepath);
	}

	return;
}