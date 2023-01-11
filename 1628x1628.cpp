#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <io.h>
#include <conio.h>

using namespace std;

void dirDarkfile();
void dirGainfile();
void darkMap();
void darkProcessing();
void gainMap();
void gainProcessing();

vector<string> darkfn;
vector<string> gainfn;

void callibration();

int main()
{
	dirDarkfile();
	dirGainfile();
	darkMap();
	darkProcessing();
	gainMap();
	gainProcessing();

	callibration();

	printf("Success processing !!\n");
	return 0;
}

void darkMap() {
	FILE* infp, * outfp;
	char savefile[] = "./output(1628x1628)/DarkMap(1628).raw";

	unsigned short* inimg, * outimg;
	float* f_averageimg;

	int width = 1628;
	int height = 1628;
	int imageSize = width * height;

	inimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	outimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	f_averageimg = (float*)malloc(sizeof(float) * imageSize);

	memset(inimg, 0, sizeof(unsigned short) * imageSize);
	memset(outimg, 0, sizeof(unsigned short) * imageSize);
	memset(f_averageimg, 0, sizeof(float) * imageSize);

	vector<string>::iterator iter;
	iter = darkfn.begin();
	for (iter = darkfn.begin(); iter != darkfn.end(); iter++) {
		memset(inimg, 0, sizeof(unsigned short) * imageSize);
		char path[100] = "./S1_1628x1628/Dark/";
		string file = path + *iter;

		//cout << file << endl;	// 파일 fopen 확인

		if ((infp = fopen(file.c_str(), "rb")) == NULL) {
			printf("No such file or folder\n");
			return;
		}

		fread(inimg, sizeof(unsigned short) * imageSize, 1, infp);

		for (int i = 0; i < imageSize; i++) {
			*(f_averageimg + i) += inimg[i];
		}

		fclose(infp);
	}

	for (int i = 0; i < imageSize; i++) {
		* (outimg + i) = *(f_averageimg + i) / 101;
	}

	if ((outfp = fopen(savefile, "wb")) == NULL) {
		printf("%d No such file or folder\n", __LINE__);
		return;
	}
	fwrite(outimg, sizeof(unsigned short) * imageSize, 1, outfp);

	free(inimg);
	free(outimg);
	free(f_averageimg);
	fclose(outfp);
}

void darkProcessing() {
	FILE* darkMapFp, * MTF_VFp, *outfp;
	int width = 1628;
	int height = 1628;
	int imageSize = width * height;
	char savefile[] = "./output(1628x1628)/DarkProcessing(1628).raw";

	unsigned short* darkMapImg, * MTF_VImg, *outimg;

	darkMapImg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	MTF_VImg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	outimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	
	memset(darkMapImg, 0, sizeof(unsigned short) * imageSize);
	memset(MTF_VImg, 0, sizeof(unsigned short) * imageSize);
	memset(outimg, 0, sizeof(unsigned short) * imageSize);

	if ((darkMapFp = fopen("./output(1628x1628)/DarkMap(1628).raw", "rb")) == NULL) {
		printf("No such file or folder\n");
		return;
	}
	if ((MTF_VFp = fopen("./S1_1628x1628/MTF_V.raw", "rb")) == NULL) {
		printf("No such file or folder\n");
		return;
	}

	fread(darkMapImg, sizeof(unsigned short) * imageSize, 1, darkMapFp);
	fread(MTF_VImg, sizeof(unsigned short) * imageSize, 1, MTF_VFp);
	fclose(darkMapFp);
	fclose(MTF_VFp);

	for (int i = 0; i < imageSize; i++) {
		if(*(MTF_VImg + i) - *(darkMapImg + i) >=0 )
			*(outimg + i) = *(MTF_VImg + i) - *(darkMapImg + i);
		else
			*(outimg + i) = *(darkMapImg + i) - *(MTF_VImg + i);
	}

	if ((outfp = fopen(savefile, "wb")) == NULL) {
		printf("%d No such file or folder\n", __LINE__);
		return;
	}
	fwrite(outimg, sizeof(unsigned short) * imageSize, 1, outfp);

	fclose(outfp);

	free(darkMapImg);
	free(MTF_VImg);
	free(outimg);
}

void gainMap() {
	FILE* infp, * outfp;
	char savefile[] = "./output(1628x1628)/GainMap(1628).raw";

	unsigned short* inimg, * outimg;
	float* f_averageimg;

	int width = 1628;
	int height = 1628;
	int imageSize = width * height;

	inimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	outimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	f_averageimg = (float*)malloc(sizeof(float) * imageSize);

	memset(inimg, 0, sizeof(unsigned short) * imageSize);
	memset(outimg, 0, sizeof(unsigned short) * imageSize);
	memset(f_averageimg, 0, sizeof(float) * imageSize);

	vector<string>::iterator iter;
	iter = gainfn.begin();
	for (iter = gainfn.begin(); iter != gainfn.end(); iter++) {
		memset(inimg, 0, sizeof(unsigned short) * imageSize);
		char path[100] = "./S1_1628x1628/Gain/";
		string file = path + *iter;

		//cout << file << endl;	// 파일 fopen 확인

		if ((infp = fopen(file.c_str(), "rb")) == NULL) {
			printf("No such file or folder\n");
			return;
		}

		fread(inimg, sizeof(unsigned short) * imageSize, 1, infp);

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
	fwrite(outimg, sizeof(unsigned short) * imageSize, 1, outfp);

	free(inimg);
	free(outimg);
	free(f_averageimg);
	fclose(outfp);
}

void gainProcessing() {
	FILE* gainMapFp, * MTF_VFp, * outfp;
	int width = 1628;
	int height = 1628;
	int imageSize = width * height;
	char savefile[] = "./output(1628x1628)/GainProcessing(1628).raw";

	unsigned short* gainMapImg, * MTF_VImg, * outimg;

	gainMapImg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	MTF_VImg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	outimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);

	memset(gainMapImg, 0, sizeof(unsigned short) * imageSize);
	memset(MTF_VImg, 0, sizeof(unsigned short) * imageSize);
	memset(outimg, 0, sizeof(unsigned short) * imageSize);

	if ((gainMapFp = fopen("./output(1628x1628)/GainMap(1628).raw", "rb")) == NULL) {
		printf("No such file or folder\n");
		return;
	}
	if ((MTF_VFp = fopen("./S1_1628x1628/MTF_V.raw", "rb")) == NULL) {
		printf("No such file or folder\n");
		return;
	}

	fread(gainMapImg, sizeof(unsigned short) * imageSize, 1, gainMapFp);
	fread(MTF_VImg, sizeof(unsigned short) * imageSize, 1, MTF_VFp);
	fclose(gainMapFp);
	fclose(MTF_VFp);

	for (int i = 0; i < imageSize; i++) {
		if (*(MTF_VImg + i) - *(gainMapImg + i) >= 0)
			*(outimg + i) = *(MTF_VImg + i) - *(gainMapImg + i);
		else
			*(outimg + i) = *(gainMapImg + i) - *(MTF_VImg + i);
	}

	if ((outfp = fopen(savefile, "wb")) == NULL) {
		printf("%d No such file or folder\n", __LINE__);
		return;
	}
	fwrite(outimg, sizeof(unsigned short) * imageSize, 1, outfp);

	fclose(outfp);

	free(gainMapImg);
	free(MTF_VImg);
	free(outimg);
}

void callibration() {
	FILE* gainMapFp, * darkMapFp, * MTF_VFp, *outfp;

	int width = 1628;
	int height = 1628;
	int imageSize = width * height;
	char savefile[] = "./output(1628x1628)/callibration(1628).raw";

	unsigned short * darkMapImg, * gainMapImg,* MTF_VImg, *outimg;

	darkMapImg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	gainMapImg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	MTF_VImg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	outimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);

	memset(darkMapImg, 0, sizeof(unsigned short) * imageSize);
	memset(gainMapImg, 0, sizeof(unsigned short) * imageSize);
	memset(MTF_VImg, 0, sizeof(unsigned short) * imageSize);
	memset(outimg, 0, sizeof(unsigned short) * imageSize);

	if ((darkMapFp = fopen("./output(1628x1628)/DarkMap(1628).raw", "rb")) == NULL) {
		printf("No such file or folder\n");
		return;
	}
	if ((gainMapFp = fopen("./output(1628x1628)/GainMap(1628).raw", "rb")) == NULL) {
		printf("No such file or folder\n");
		return;
	}
	if ((MTF_VFp = fopen("./S1_1628x1628/MTF_V.raw", "rb")) == NULL) {
		printf("No such file or folder\n");
		return;
	}

	fread(darkMapImg, sizeof(unsigned short) * imageSize, 1, darkMapFp);
	fread(gainMapImg, sizeof(unsigned short) * imageSize, 1, gainMapFp);
	fread(MTF_VImg, sizeof(unsigned short) * imageSize, 1, MTF_VFp);

	fclose(darkMapFp);
	fclose(gainMapFp);
	fclose(MTF_VFp);

	for (int i = 0; i < imageSize; i++) {
		*(outimg + i) = 
		 (unsigned short) (abs( *(MTF_VImg + i) - ( *(darkMapImg + i) ) ) / (float)( *(gainMapImg + i) )  * ( *(gainMapImg + i) ) );
	}


	if ((outfp = fopen(savefile, "wb")) == NULL) {
		printf("%d No such file or folder\n", __LINE__);
		return;
	}

	fwrite(outimg, sizeof(unsigned short) * imageSize, 1, outfp);

	fclose(outfp);

	free(gainMapImg);
	free(darkMapImg);
	free(MTF_VImg);
	free(outimg);
}

// dark 디렉토리 내 파일 찾기 함수
void dirDarkfile()
{
	_finddata_t fd;
	long handle;
	int out = 1;
	handle = _findfirst("./S1_1628x1628/Dark/*.raw", &fd);  //현재 폴더 내 모든 파일을 찾는다.

	if (handle == -1)
	{
		printf("There were no files.\n");
	}

	while (out != -1)
	{
		darkfn.push_back(fd.name);
		out = _findnext(handle, &fd);
	}
	_findclose(handle);
}

// Gain 디렉토리 내 파일 찾기 함수
void dirGainfile()
{
	_finddata_t fd;
	long handle;
	int out = 1;
	handle = _findfirst("./S1_1628x1628/Gain/*.raw", &fd);  //현재 폴더 내 모든 파일을 찾는다.

	if (handle == -1)
	{
		printf("There were no files.\n");
	}

	while (out != -1)
	{
		gainfn.push_back(fd.name);
		out = _findnext(handle, &fd);
	}
	_findclose(handle);
}