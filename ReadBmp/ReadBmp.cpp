// ReadBmp.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Bitmap.h"

int main()
{
	//if (argc != 3) {
	//	fprintf(stderr, "Usage: program <inputfile> <outputfile>\n");
	//	exit(1);
	//}

	Bitmap main;

	Image *colorimg;

	char bmp[] = "C:\\proj\\ReadBitmap\\aaa.bmp";
	if ((colorimg = main.Read_Bmp(bmp)) == NULL) {
		return 1;
	}

	//if (Write_Bmp(argv[2], colorimg)) {
	//	exit(1);
	//}

	main.Free_Image(colorimg);

	return 0;
}

