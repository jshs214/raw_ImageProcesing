#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <io.h>
#include <conio.h>

using namespace std;
void dirfile();

vector<string> fn;	//디렉토리 내 파일명을 저장할 벡터

int main(int argc, char** argv)
{
	FILE* fp, * outfp;
	char input[100];
	char output[100] = "Pre_2(1792).raw";
	char averageData[] = "GainMap(1792).raw";

	unsigned short* inimg, * outimg;
	unsigned short* averageimg;
	float* f_averageimg;
	//디렉토리 내 파일 찾기
	dirfile();

	int width = 1792;
	int height = 1792;
	int imageSize = width * height;

	inimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	outimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	averageimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	f_averageimg = (float*)malloc(sizeof(float) * imageSize);

	memset(averageimg, 0, sizeof(unsigned short) * imageSize);
	memset(f_averageimg, 0, sizeof(float) * imageSize);
	vector<string>::iterator iter;
	iter = fn.begin();

	for (iter = fn.begin(); iter != fn.end(); iter++) {	
		memset(input, 0, sizeof(char) * 100);
		memset(inimg, 0, sizeof(unsigned short) * imageSize);

		char path[100] = "./S3_1792x1792/Gain/";
		string file = path + *iter;

		if ((fp = fopen(file.c_str(), "rb")) == NULL) {
			printf("Not Exist folder or file");
			return -1;
		}
		//cout << file << endl;	// 파일 fopen 확인

		fread(inimg, sizeof(unsigned short) * imageSize, 1, fp);

		for (int i = 0; i < imageSize; i++) {
			*(f_averageimg + i) += inimg[i];
		}

		fclose(fp);
	}

	for (int i = 0; i < imageSize; i++) {
		*(f_averageimg + i) = *(f_averageimg + i) / 101;
		 *(averageimg + i) = *(f_averageimg + i);
	}

	outfp = fopen(averageData, "wb");

	fwrite(averageimg, sizeof(unsigned short) * imageSize, 1, outfp);	// GainMap write.
	fclose(outfp);

	// MTF_H.raw - GainMap
	memset(inimg, 0, sizeof(unsigned short) * imageSize);

	fp = fopen("./S3_1792x1792/MTF_H.raw", "rb");
	fread(inimg, sizeof(unsigned short) * imageSize, 1, fp);

	for (int i = 0; i < imageSize; i++) {
		if (inimg[i] - *(averageimg + i) >= 0)
			outimg[i] = inimg[i] - *(averageimg + i);
		else
			outimg[i] = *(averageimg + i) - inimg[i];
	}

	outfp = fopen(output, "wb");

	fwrite(outimg, sizeof(unsigned short) * imageSize, 1, outfp);

	fclose(fp);
	fclose(outfp);
	free(inimg);
	free(averageimg);
	free(outimg);

	return 0;
}

//디렉토리 내 파일 찾기 함수
void dirfile()
{
	_finddata_t fd;
	long handle;
	int out = 1;
	handle = _findfirst("./S3_1792x1792/Gain/*.raw", &fd);  //현재 폴더 내 모든 파일을 찾는다.

	if (handle == -1)
	{
		printf("There were no files.\n");
	}

	while (out != -1)
	{
		fn.push_back(fd.name);

		out = _findnext(handle, &fd);
	}

	_findclose(handle);
}