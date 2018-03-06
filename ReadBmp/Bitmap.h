#pragma once

#include "vector"

#define FILEHEADERSIZE 14					//ファイルヘッダのサイズ
#define INFOHEADERSIZE 40					//情報ヘッダのサイズ
#define HEADERSIZE (FILEHEADERSIZE+INFOHEADERSIZE)

typedef struct {
	unsigned char b;
	unsigned char g;
	unsigned char r;
}Rgb;

typedef struct {
	int height;
	int width;
	std::vector<Rgb*> dataV;
}Image;

class Bitmap
{
public:
	Bitmap();
	virtual ~Bitmap();

	//取得に成功すればポインタを失敗すればNULLを返す
	Image *Read_Bmp(char *filename);

	//書き込みに成功すれば0を失敗すれば1を返す
	int Write_Bmp(char *filename, Image *img);

	//Imageを作成し、RGB情報もwidth*height分だけ動的に取得する。
	//成功すればポインタを、失敗すればNULLを返す
	Image *Create_Image(int width, int height);
	//Imageを解法する
	void Free_Image(Image *img);
};
