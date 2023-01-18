#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <io.h>
#include <filesystem>
typedef unsigned short ushort;

using namespace std;
using std::filesystem::directory_iterator;

void dirCephfile();
void dirPanofile();
void Ceph1pixel();
void Pano1pixel();
void Ceph2pixel();
void Pano2pixel();

vector<string> cephfn;
vector<string> panofn;

int main()
{
	dirCephfile();
	dirPanofile();
	Ceph1pixel();
	Ceph2pixel();
	Pano1pixel();
	Pano2pixel();
	printf("\nSuccess processing !!\n");

	return 0;
}

void dirCephfile()
{
	string path = "./stitchingData_230117/Ceph_48x2400/";

	for (const auto& file : directory_iterator(path)) {
		std::string filepath{ file.path().u8string() };
		cephfn.push_back(filepath);
	}

	return;
}
void dirPanofile()
{
	string path = "./stitchingData_230117/Pano_80x1628/";

	for (const auto& file : directory_iterator(path)) {
		std::string filepath{ file.path().u8string() };
		panofn.push_back(filepath);
	}

	return;
}

void Ceph1pixel()
{
	FILE* infp, * outfp;
	char savefile[] = "./stitching/Ceph1pixel.raw";

	ushort* inimg, * outimg;

	int width = 48;
	int height = 2400;
	int imageSize = width * height;
	int fileCount = cephfn.size();

	inimg = (ushort*)malloc(sizeof(ushort) * imageSize);
	outimg = (ushort*)malloc(sizeof(ushort) * (imageSize * fileCount));
	memset(outimg, 0, sizeof(ushort) * (imageSize * fileCount));

	vector<string>::iterator iter;
	iter = cephfn.begin();

	int cnt = 0;
	for (iter = cephfn.begin(); iter != cephfn.end(); iter++) {
		string file = *iter;
		//cout << file << endl;
		memset(inimg, 0, sizeof(ushort) * imageSize);

		if ((infp = fopen(file.c_str(), "rb")) == NULL) {
			printf("No such file or folder\n");
			return;
		}
		fread(inimg, sizeof(ushort) * imageSize, 1, infp);
		fclose(infp);


		/* 이미지의 우측 1픽셀만 */
		for (int y = 0; y < height; y++)
		{
			for (int x = width/2 ; x < width /2 +1 ; x++)
			{
				outimg[cnt + (y * fileCount)] = ~inimg[x + (y * width)];
			}
		}

		cnt++;
	}

	printf("Ceph1pixel.raw :  %dx%d\n", cnt,height);


	if ((outfp = fopen(savefile, "wb")) == NULL) {
		printf("%d No such file or folder\n", __LINE__);
		return;
	}

	fwrite(outimg, sizeof(ushort) * imageSize * fileCount, 1, outfp);
	fclose(outfp);


	free(inimg);
	free(outimg);
}
void Ceph2pixel()
{
	FILE* infp, * outfp;
	char savefile[] = "./stitching/Ceph2pixel.raw";

	ushort* inimg, * outimg;

	int width = 48;
	int height = 2400;
	int imageSize = width * height;
	int fileCount = cephfn.size();

	inimg = (ushort*)malloc(sizeof(ushort) * imageSize);
	outimg = (ushort*)malloc(sizeof(ushort) * (imageSize * fileCount));
	memset(outimg, 0, sizeof(ushort) * (imageSize * fileCount));

	vector<string>::iterator iter;
	iter = cephfn.begin();

	int cnt = 0;
	for (iter = cephfn.begin(); iter != cephfn.end(); iter++) {
		string file = *iter;
		//cout << file << endl;
		memset(inimg, 0, sizeof(ushort) * imageSize);

		if ((infp = fopen(file.c_str(), "rb")) == NULL) {
			printf("No such file or folder\n");
			return;
		}
		fread(inimg, sizeof(ushort) * imageSize, 1, infp);
		fclose(infp);

		for (int y = 0; y < height; y++)
		{
			for (int x = width / 2; x < width / 2 + 2; x++)
			{
				if (x == width / 2)
					outimg[cnt + (y * fileCount * 2)] = ~inimg[x + (y * width)];
				if (x == width / 2 + 1)
					outimg[cnt + 1 + (y * fileCount * 2)] = ~inimg[x + (y * width)];
			}
		}

		cnt += 2;
	}

	printf("Ceph2pixel.raw :  %dx%d\n", cnt, height);


	if ((outfp = fopen(savefile, "wb")) == NULL) {
		printf("%d No such file or folder\n", __LINE__);
		return;
	}

	fwrite(outimg, sizeof(ushort) * imageSize * fileCount, 1, outfp);
	fclose(outfp);

	free(inimg);
	free(outimg);
}

void Pano1pixel()
{
	FILE* infp, * outfp;
	char savefile[] = "./stitching/Pano1pixel.raw";

	ushort* inimg, * outimg;

	int width = 80;
	int height = 1628;
	int imageSize = width * height;
	int fileCount = panofn.size();

	inimg = (ushort*)malloc(sizeof(ushort) * imageSize);
	outimg = (ushort*)malloc(sizeof(ushort) * (imageSize * fileCount));
	memset(outimg, 0, sizeof(ushort) * (imageSize * fileCount));

	vector<string>::iterator iter;
	iter = panofn.begin();

	int cnt = 0;
	for (iter = panofn.begin(); iter != panofn.end(); iter++) {
		string file = *iter;
		//cout << file << endl;
		memset(inimg, 0, sizeof(ushort) * imageSize);

		if ((infp = fopen(file.c_str(), "rb")) == NULL) {
			printf("No such file or folder\n");
			return;
		}
		fread(inimg, sizeof(ushort) * imageSize, 1, infp);
		fclose(infp);


		/* 이미지의 우측 1픽셀만 */
		for (int y = 0; y < height; y++)
		{
			for (int x = width/2; x < width/2 +1; x++)
			{
				outimg[cnt + (y * fileCount)] = ~inimg[x + (y * width)];
			}
		}

		cnt++;
	}

	printf("Pano1pixel.raw :  %dx%d\n", cnt, height);


	if ((outfp = fopen(savefile, "wb")) == NULL) {
		printf("%d No such file or folder\n", __LINE__);
		return;
	}

	fwrite(outimg, sizeof(ushort) * imageSize * fileCount, 1, outfp);
	fclose(outfp);


	free(inimg);
	free(outimg);
}

void Pano2pixel()
{
	FILE* infp, * outfp;
	char savefile[] = "./stitching/Pano2pixel.raw";

	ushort* inimg, * outimg;

	int width = 80;
	int height = 1628;
	int imageSize = width * height;
	int fileCount = panofn.size();

	inimg = (ushort*)malloc(sizeof(ushort) * imageSize);
	outimg = (ushort*)malloc(sizeof(ushort) * (imageSize * fileCount));
	memset(outimg, 0, sizeof(ushort) * (imageSize * fileCount));

	vector<string>::iterator iter;
	iter = panofn.begin();

	int cnt = 0;
	for (iter = panofn.begin(); iter != panofn.end(); iter++) {
		string file = *iter;
		//cout << file << endl;
		memset(inimg, 0, sizeof(ushort) * imageSize);

		if ((infp = fopen(file.c_str(), "rb")) == NULL) {
			printf("No such file or folder\n");
			return;
		}
		fread(inimg, sizeof(ushort) * imageSize, 1, infp);
		fclose(infp);


		for (int y = 0; y < height; y++)
		{
			for (int x = width / 2; x < width / 2 + 2; x++)
			{
				if (x == width / 2)
					outimg[cnt + (y * fileCount * 2)] = ~inimg[x + (y * width)];
				if (x == width / 2 + 1)
					outimg[cnt + 1 + (y * fileCount * 2)] = ~inimg[x + (y * width)];
			}
		}

		cnt += 2;
	}

	printf("Pano2pixel.raw :  %dx%d\n", cnt, height);


	if ((outfp = fopen(savefile, "wb")) == NULL) {
		printf("%d No such file or folder\n", __LINE__);
		return;
	}

	fwrite(outimg, sizeof(ushort) * imageSize * fileCount, 1, outfp);
	fclose(outfp);


	free(inimg);
	free(outimg);
}