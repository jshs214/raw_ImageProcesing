#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <io.h>
#include <conio.h>

using namespace std;
void dirfile();

vector<string> fn;

int main(int argc, char** argv)
{
	FILE* fp, * outfp;
	char input[100];
	char output[100] = "Pre_1.raw";
	char averageData[] = "DarkMap.raw";

	unsigned short* inimg, * outimg;
	unsigned short* averageimg;

	//디렉토리 내 파일 찾기
	dirfile();

	int width = 1628;
	int height = 1628;
	int imageSize = width * height;

	inimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	outimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	averageimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);

	memset(averageimg, 0, sizeof(unsigned short) * imageSize);
	vector<string>::iterator iter;
	iter = fn.begin();
	for (iter = fn.begin(); iter != fn.end(); iter++) {
		memset(input, 0, sizeof(char) * 100);
		memset(inimg, 0, sizeof(unsigned short) * imageSize);

		char path[100] = "./S1_1628x1628/Dark/";
		string file = path + *iter;
		strcpy(input, file.c_str());

		fp = fopen(input, "rb");
		cout << input << endl;	// 파일 fopen 확인

		fread(inimg, sizeof(unsigned short) * imageSize, 1, fp);

		for (int i = 0; i < imageSize; i++) {
			*(averageimg + i) += inimg[i];
		}

		fclose(fp);
	}


	for (int i = 0; i < imageSize; i++) {
		*(averageimg + i) = *(averageimg + i) / 101;
	}

	outfp = fopen(averageData, "wb");

	fwrite(averageimg, sizeof(unsigned short) * imageSize, 1, outfp);
	fclose(outfp);

	// MTF_V.raw - DarkMap
	memset(inimg, 0, sizeof(unsigned short) * imageSize);


	fp = fopen("./S1_1628x1628/MTF_V.raw", "rb");

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
	handle = _findfirst("./S1_1628x1628/Dark/*.raw", &fd);  //현재 폴더 내 모든 파일을 찾는다.

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