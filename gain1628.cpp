
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
	char output[100] = "Pre_2.raw";
	char averageData[] = "GainMap.raw";

	unsigned short* inimg, * outimg;
	unsigned short* averageimg, * gainAverageimg;

	//디렉토리 내 파일 찾기
	dirfile();

	int width = 1628;
	int height = 1628;
	int imageSize = width * height;

	inimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	outimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	averageimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);
	gainAverageimg = (unsigned short*)malloc(sizeof(unsigned short) * imageSize);

	memset(averageimg, 0, sizeof(unsigned short) * imageSize);
	memset(gainAverageimg, 0, sizeof(unsigned short) * imageSize);

	vector<string>::iterator iter;
	iter = fn.begin();
	int cnt = 0, divcnt = 0;
	for (iter = fn.begin(); iter != fn.end(); iter++) {
		memset(input, 0, sizeof(char) * 100);
		memset(inimg, 0, sizeof(unsigned short) * imageSize);

		char path[100] = "./S1_1628x1628/Gain/";
		string file = path + *iter;
		strcpy(input, file.c_str());

		fp = fopen(input, "rb");
		//cout << input << endl;	// 파일 fopen 확인

		fread(inimg, sizeof(unsigned short) * imageSize, 1, fp);

		if (cnt == 8) {
			for (int i = 0; i < imageSize; i++) {
				*(averageimg + i) += inimg[i];
				*(gainAverageimg + i) += *(averageimg + i) / cnt;
			}
			divcnt++;
			cnt = 0;
			memset(averageimg, 0, sizeof(unsigned short) * imageSize);
		}
		else {
			for (int i = 0; i < imageSize; i++) {
				*(averageimg + i) += inimg[i];
			}
		}
		cnt++;

		fclose(fp);
	}

	if (cnt > 0) {
		divcnt++;
		for (int i = 0; i < imageSize; i++) {
			*(gainAverageimg + i) += *(averageimg + i) / cnt;
			*(gainAverageimg + i) = *(gainAverageimg + i) / divcnt;
		}
	}

	outfp = fopen(averageData, "wb");

	fwrite(gainAverageimg, sizeof(unsigned short) * imageSize, 1, outfp);
	fclose(outfp);

	// MTF_V.raw - DarkMap
	memset(inimg, 0, sizeof(unsigned short) * imageSize);

	char MTF_V[100] = "./S1_1628x1628/MTF_V.raw";

	fp = fopen(MTF_V, "rb");

	fread(inimg, sizeof(unsigned short) * imageSize, 1, fp);

	for (int i = 0; i < imageSize; i++) {
		if (inimg[i] - *(gainAverageimg + i) >= 0)
			outimg[i] = inimg[i] - *(gainAverageimg + i);
		else
			outimg[i] = *(gainAverageimg + i) - inimg[i];
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
	handle = _findfirst("./S1_1628x1628/Gain/*.raw", &fd);  //현재 폴더 내 모든 파일을 찾는다.

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