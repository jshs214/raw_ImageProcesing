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
	char output[100] = "Pre_1(1536).raw";
	char averageData[] = "DarkMap1536.raw";

	unsigned short* inimg, * outimg, *averageimg;
	float* f_averageimg;

	//���丮 �� ���� ã��
	dirfile();

	int width = 1536;
	int height = 1536;
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

		char path[100] = "./S2_1536x1536/Dark/";
		string file = path + *iter;
		strcpy(input, file.c_str());

		fp = fopen(input, "rb");
		//cout << input << endl;	// ���� fopen Ȯ��

		fread(inimg, sizeof(unsigned short) * imageSize, 1, fp);

		for (int i = 0; i < imageSize; i++) {
			*(f_averageimg + i) += inimg[i];
		}

		fclose(fp);
	}

	/* float������ ������ ��հ��� unsigned short ������ ����ȯ*/
	for (int i = 0; i < imageSize; i++) {
		*(f_averageimg + i) = *(f_averageimg + i) / 101;
		*(averageimg + i) = (unsigned short)*(f_averageimg + i);
	}

	outfp = fopen(averageData, "wb");

	fwrite(averageimg, sizeof(unsigned short) * imageSize, 1, outfp);
	fclose(outfp);

	// MTF_H.raw - DarkMap
	memset(inimg, 0, sizeof(unsigned short) * imageSize);

	fp = fopen("./S2_1536x1536/MTF_H.raw", "rb");
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
	free(f_averageimg);
	free(outimg);

	return 0;
}

//���丮 �� ���� ã�� �Լ�
void dirfile()
{
	_finddata_t fd;
	long handle;
	int out = 1;
	handle = _findfirst("./S2_1536x1536/Dark/*.raw", &fd);  //���� ���� �� ��� ������ ã�´�.

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