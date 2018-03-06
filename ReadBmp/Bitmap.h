#pragma once

#include "vector"

#define FILEHEADERSIZE 14					//�t�@�C���w�b�_�̃T�C�Y
#define INFOHEADERSIZE 40					//���w�b�_�̃T�C�Y
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

	//�擾�ɐ�������΃|�C���^�����s�����NULL��Ԃ�
	Image *Read_Bmp(char *filename);

	//�������݂ɐ��������0�����s�����1��Ԃ�
	int Write_Bmp(char *filename, Image *img);

	//Image���쐬���ARGB����width*height���������I�Ɏ擾����B
	//��������΃|�C���^���A���s�����NULL��Ԃ�
	Image *Create_Image(int width, int height);
	//Image����@����
	void Free_Image(Image *img);
};
