#include "stdafx.h"
#include "Bitmap.h"

#pragma warning(disable:4996)

#include <stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include"bitmap.h"

Bitmap::Bitmap()
{
}

Bitmap::~Bitmap()
{
}

//filename��Bitmap�t�@�C����ǂݍ��݁A�����ƕ��ARGB����img�\���̂ɓ����
Image* Bitmap::Read_Bmp(char *filename)
{
	int i, j;
	int real_width;					//�f�[�^���1�s���̃o�C�g��
	unsigned int width, height;			//�摜�̉��Əc�̃s�N�Z����
	unsigned int color;			//��bit��Bitmap�t�@�C���ł��邩
	FILE *fp;
	char header_buf[HEADERSIZE];	//�w�b�_������荞��
	//unsigned char header_buf[HEADERSIZE];	//�w�b�_������荞��
	unsigned char *bmp_line_data;  //�摜�f�[�^1�s��
	Image *img;

	if ((fp = fopen(filename, "rb")) == NULL) {
		fprintf(stderr, "Error: %s could not read.", filename);
		return NULL;
	}

	fread(header_buf, sizeof(unsigned char), HEADERSIZE, fp); //�w�b�_�����S�Ă���荞��

															  //�ŏ���2�o�C�g��BM(Bitmap�t�@�C���̈�)�ł��邩
	if (strncmp(header_buf, "BM", 2)) {
		fprintf(stderr, "Error: %s is not Bitmap file.", filename);
		return NULL;
	}

	memcpy(&width, header_buf + 18, sizeof(width)); //�摜�̌����ڏ�̕����擾
	memcpy(&height, header_buf + 22, sizeof(height)); //�摜�̍������擾
	memcpy(&color, header_buf + 28, sizeof(unsigned int)); //��bit��Bitmap�ł��邩���擾

														   //24bit�Ŗ�����ΏI��
	if (color != 24) {
		fprintf(stderr, "Error: %s is not 24bit color image", filename);
		return NULL;
	}

	//RGB���͉摜��1�s����4byte�̔{���Ŗ�����΂Ȃ�Ȃ����߂���ɍ��킹�Ă���
	real_width = width * 3 + width % 4;

	//�摜��1�s����RGB��������Ă��邽�߂̃o�b�t�@�𓮓I�Ɏ擾
	if ((bmp_line_data = (unsigned char *)malloc(sizeof(unsigned char)*real_width)) == NULL) {
		fprintf(stderr, "Error: Allocation error.\n");
		return NULL;
	}

	//RGB������荞�ނ��߂̃o�b�t�@�𓮓I�Ɏ擾
	if ((img = Create_Image(width, height)) == NULL) {
		free(bmp_line_data);
		fclose(fp);
		return NULL;
	}

	img->dataV.reserve(width);

	//Bitmap�t�@�C����RGB���͍�������E�ցA�������ɕ���ł���
	for (i = 0; i < height; i++) {
		fread(bmp_line_data, 1, real_width, fp);
		for (j = 0; j < width; j++) {
			Rgb* rgb = new Rgb;
			img->dataV.push_back(rgb);

			rgb->b = bmp_line_data[j * 3];
			rgb->g = bmp_line_data[j * 3 + 1];
			rgb->r = bmp_line_data[j * 3 + 2];
			int a = 10;
		}
	}

	free(bmp_line_data);

	fclose(fp);

	return img;
}

int Bitmap::Write_Bmp(char *filename, Image *img)
{
	int i, j;
	FILE *fp;
	int real_width;
	unsigned char *bmp_line_data; //�摜1�s����RGB�����i�[����
	unsigned char header_buf[HEADERSIZE]; //�w�b�_���i�[����
	unsigned int file_size;
	unsigned int offset_to_data;
	unsigned long info_header_size;
	unsigned int planes;
	unsigned int color;
	unsigned long compress;
	unsigned long data_size;
	long xppm;
	long yppm;

	if ((fp = fopen(filename, "wb")) == NULL) {
		fprintf(stderr, "Error: %s could not open.", filename);
		return 1;
	}

	real_width = img->width * 3 + img->width % 4;

	//��������w�b�_�쐬
	file_size = img->height * real_width + HEADERSIZE;
	offset_to_data = HEADERSIZE;
	info_header_size = INFOHEADERSIZE;
	planes = 1;
	color = 24;
	compress = 0;
	data_size = img->height * real_width;
	xppm = 1;
	yppm = 1;

	header_buf[0] = 'B';
	header_buf[1] = 'M';
	memcpy(header_buf + 2, &file_size, sizeof(file_size));
	header_buf[6] = 0;
	header_buf[7] = 0;
	header_buf[8] = 0;
	header_buf[9] = 0;
	memcpy(header_buf + 10, &offset_to_data, sizeof(file_size));
	header_buf[11] = 0;
	header_buf[12] = 0;
	header_buf[13] = 0;

	memcpy(header_buf + 14, &info_header_size, sizeof(info_header_size));
	header_buf[15] = 0;
	header_buf[16] = 0;
	header_buf[17] = 0;
	memcpy(header_buf + 18, &img->width, sizeof(img->width));
	memcpy(header_buf + 22, &img->height, sizeof(img->height));
	memcpy(header_buf + 26, &planes, sizeof(planes));
	memcpy(header_buf + 28, &color, sizeof(color));
	memcpy(header_buf + 30, &compress, sizeof(compress));
	memcpy(header_buf + 34, &data_size, sizeof(data_size));
	memcpy(header_buf + 38, &xppm, sizeof(xppm));
	memcpy(header_buf + 42, &yppm, sizeof(yppm));
	header_buf[46] = 0;
	header_buf[47] = 0;
	header_buf[48] = 0;
	header_buf[49] = 0;
	header_buf[50] = 0;
	header_buf[51] = 0;
	header_buf[52] = 0;
	header_buf[53] = 0;

	//�w�b�_�̏�������
	fwrite(header_buf, sizeof(unsigned char), HEADERSIZE, fp);

	if ((bmp_line_data = (unsigned char *)malloc(sizeof(unsigned char)*real_width)) == NULL) {
		fprintf(stderr, "Error: Allocation error.\n");
		fclose(fp);
		return 1;
	}

	//RGB���̏�������
	for (i = 0; i < img->height; i++) {
		for (j = 0; j < img->width; j++) {
			Rgb* rgb = img->dataV.at((img->height - i - 1)*img->width + j);

			bmp_line_data[j * 3] = rgb->b;
			bmp_line_data[j * 3 + 1] = rgb->g;
			bmp_line_data[j * 3 + 2] = rgb->r;
		}
		//RGB����4�o�C�g�̔{���ɍ��킹�Ă���
		for (j = img->width * 3; j<real_width; j++) {
			bmp_line_data[j] = 0;
		}
		fwrite(bmp_line_data, sizeof(unsigned char), real_width, fp);
	}

	free(bmp_line_data);

	fclose(fp);

	return 0;
}

Image* Bitmap::Create_Image(int width, int height)
{
	Image* img=new Image;

	//if ((img = (Image *)malloc(sizeof(Image))) == NULL) {
	//	fprintf(stderr, "Allocation error\n");
	//	return NULL;
	//}

	//if ((img->data = (Rgb*)malloc(sizeof(Rgb)*width*height)) == NULL) {
	//	fprintf(stderr, "Allocation error\n");
	//	free(img);
	//	return NULL;
	//}

	img->width = width;
	img->height = height;

	return img;
}

//���I�Ɏ擾����RGB���̊J��
void Bitmap::Free_Image(Image *img)
{
	//free(img->data);
	free(img);
}
