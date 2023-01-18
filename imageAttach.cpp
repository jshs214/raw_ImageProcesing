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

vector<string> cephfn;

int main()
{
	dirCephfile();
	FILE* infp, * outfp;
	char savefile[] = "./stitching/Ceph.raw";

	ushort* inimg, * outimg;

	int width = 48;
	int height = 2400;
	int imageSize = width * height;
	int fileCount = cephfn.size();
	printf("vector size : %d \n", fileCount);

	inimg = (ushort*)malloc(sizeof(ushort) * imageSize);
	outimg = (ushort*)malloc(sizeof(ushort) * (imageSize * fileCount));
	memset(outimg, 0, sizeof(ushort) * (imageSize * fileCount));

	vector<string>::iterator iter;
	iter = cephfn.begin();

	int cnt = 0;
	for (iter = cephfn.begin(); iter != cephfn.end(); iter++) {
		string file = *iter;
		cout << file << endl;
		memset(inimg, 0, sizeof(ushort) * imageSize);

		if ((infp = fopen(file.c_str(), "rb")) == NULL) {
			printf("No such file or folder\n");
			return -1;
		}
		fread(inimg, sizeof(ushort) * imageSize, 1, infp);
		fclose(infp);

		/* 이미지 단순 이어 붙이기 */
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				outimg[x + (y * width * fileCount) + (cnt * width)] = inimg[x + (y * width)];
			}
		}


		cnt++;
	}

	printf("Image Width : %d\n", 48 * fileCount);
	printf("cnt : %d\n", cnt);


	if ((outfp = fopen(savefile, "wb")) == NULL) {
		printf("%d No such file or folder\n", __LINE__);
		return -1;
	}

	fwrite(outimg, sizeof(ushort) * imageSize * fileCount, 1, outfp);
	fclose(outfp);


	free(inimg);
	free(outimg);

	printf("\nSuccess processing !!\n");

	return 0;
}

// dark 디렉토리 내 파일 찾기 함수
void dirCephfile()
{
	string path = "./stitchingData_230117/Ceph_48x2400/";

	for (const auto& file : directory_iterator(path)) {
		std::string filepath{ file.path().u8string() };
		cephfn.push_back(filepath);
	}

	return;
}
