//////////////////////////////////////////////////////////////////////////
//		2011/03/17	nishiokozo
//////////////////////////////////////////////////////////////////////////
//#include	<iostream>
//using std::cout;
/*
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<math.h>
#include	<time.h>
#include <sys/time.h>
#include <limits.h>
*/

#include	<stdio.h>
#include	<stdlib.h>
#include "loaderbmp.h"

#define	printErrorf(str)	({ printf("\nERROR!! %s::%s():(%d)\n>",__FILE__,__func__,__LINE__); printf str ;printf("\n");})
#define	_line	printf("--%s(%d)\n",__FILE__,__LINE__)
//#include <libtype.h>
//#include <libgraphics.h>
//#include <libsys.h>


#define _ALLIGN32(x)	((x) & (~0x1F))
#define _UPALLIGN32(x)	_ALLIGN32((x) + 0x1F)


#define OPENGL_IMAGE_COORDINATE 1
/* ファイルヘッダ */
typedef struct _tmpBMPFileHeader {
	unsigned short bfType;			/* ファイルタイプ */
	unsigned int bfSize;			/* ファイルサイズ */
	unsigned short bfReserved1;		/* 予約領域 */
	unsigned short bfReserved2;		/* 予約領域 */
	unsigned int bfOffBits;			/* ファイル先頭から画像データまでのオフセット */
} BMPFileHeader;

/* BMP情報ヘッダ */
typedef struct _tmpBMPInfoHeader {
	unsigned int biSize;			/* 情報ヘッダーのサイズ */
	int biWidth;					/* 幅 */
	int biHeight;					/* 高さ(正ならば下から上、負ならば上から下) */
	unsigned short biPlanes;		/* プレーン数(常に1) */
	unsigned short biBitCount;		/* 1画素あたりのビット数(1,4,8,24,32) */
	unsigned int biCompression;
	unsigned int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	unsigned int biClrUsed;			/* パレットの色数 */
	unsigned int biClrImportant;
} BMPInfoHeader;

/* パレットデータ */
typedef struct _tmpRGBQuad {
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
} RGBQuad;

#define MAX_COLOR_PALETTE		(256)

//-----------------------------------------------------------------------------
LoaderBmp::~LoaderBmp()
//-----------------------------------------------------------------------------
{
	if (m_pBuf)		free(m_pBuf);
}
//-----------------------------------------------------------------------------
LoaderBmp::LoaderBmp( const char *name )
//-----------------------------------------------------------------------------
{
	m_flgActive = true;

	FILE *fp;
	int i,j;
	int bpp, width, height;
	int lineBits, lineSize, linePos;
	unsigned char *lpImg, *lpImgArea;
	int index;
	unsigned char bfType[2];
	unsigned int bfOffBits;
	BMPInfoHeader bmpInfo;
	RGBQuad palette[MAX_COLOR_PALETTE];

	fp = fopen( name, "rb");

	if (fp == 0)
	{
//		fprintf(stderr, "foen error %s\n", name);
		printErrorf(( "File error [%s]\n", name ));
		m_flgActive = false;
	}

	if ( m_flgActive )
	{

		fseek(fp, 0, SEEK_END);
		if ((int)ftell(fp) < (int)(sizeof(BMPFileHeader) + sizeof(BMPInfoHeader)))
		{
			fclose(fp);
//				fprintf(stderr, "fclose error %s\n", name);
			printErrorf(( "File error [%s]\n", name ));
			m_flgActive = false;
		}
	}

	if ( m_flgActive )
	{
		fseek(fp, 0, SEEK_SET);
		fread(&bfType[0], 1, 2, fp);
		if (bfType[0] != 'B' || bfType[1] != 'M')
		{
			fclose(fp);
			printErrorf(( "File error [%s]\n", name ));
//				fprintf(stderr, "fread error %s\n", name);
			m_flgActive = false;
		}
	}

	if ( m_flgActive )
	{
		/* 画像データへのオフセット */
		fseek(fp, 10, SEEK_SET);
		fread(&bfOffBits, 4, 1, fp);

		/* BMP情報ヘッダ読み込み */
		fseek(fp, 14, SEEK_SET);
		fread(&bmpInfo, sizeof(BMPInfoHeader), 1, fp);
		if (bmpInfo.biCompression != 0 || bmpInfo.biSize != 40)
		{
			/* ランレングス圧縮は未サポート */
			fclose(fp);
				fprintf(stderr, "fclose error %s\n", name);
			m_flgActive = false;
		}
	}

	if ( m_flgActive )
	{

		/* 1画素当たりのビット数、幅、高さ */
		bpp = bmpInfo.biBitCount;
		width = bmpInfo.biWidth;
		height = bmpInfo.biHeight;

		/* パッドも含めたラインサイズ（バイト） */
		lineBits = _UPALLIGN32(bpp * width);
		lineSize = lineBits / 8;

		/* RGBA領域確保 */
	 	lpImgArea = (unsigned char *)malloc(sizeof(unsigned int) * width * height);
		if (lpImgArea == NULL)
		{
			fclose(fp);
				fprintf(stderr, "rgba assign error %s\n", name);
			m_flgActive = false;
		} 
	}

	if ( m_flgActive )
	{

		/* RGB読み込み */
		lpImg = lpImgArea;
	//printf("----------%s bpp %d\n", name, bpp );
		switch(bpp)
		{
		case 8:	/* 8 bits/pixel */
			/* カラーマップ読み込み */
			fseek(fp,54,SEEK_SET);
			fread(&palette[0], sizeof(RGBQuad), 256, fp);

	#if OPENGL_IMAGE_COORDINATE != 0
			for (i = 0; i < height; i++)
	#else
			for (i = height - 1; i >= 0; i--)
	#endif
			{
				linePos = bfOffBits + lineSize * i;
				fseek(fp, linePos, SEEK_SET);
				for (j = 0; j < width; j++)
				{
					index = fgetc(fp);
					*lpImg++ = palette[index].rgbRed;
					*lpImg++ = palette[index].rgbGreen;
					*lpImg++ = palette[index].rgbBlue;
					*lpImg++ = 255;
				}
			}
			break;

		case 24: /* 24 bits/pixel */
	#if OPENGL_IMAGE_COORDINATE != 0
			for (i = 0; i < height; i++)
	#else
			for (i = height - 1; i >= 0; i--)
	#endif
			{
				linePos = bfOffBits + lineSize * i;
				fseek(fp, linePos, SEEK_SET);	
				for (j = 0; j < width; j++)
				{
					*(lpImg + 2) = (unsigned char)fgetc(fp);	/* B */
					*(lpImg + 1) = (unsigned char)fgetc(fp);	/* G */
					*(lpImg + 0) = (unsigned char)fgetc(fp);	/* R */
					*(lpImg + 3) = 255;							/* A */
					lpImg += 4;
				}
			}
			break;

		case 32: /* 32 bits/pixel */
	#if OPENGL_IMAGE_COORDINATE != 0
			for (i = 0; i < height; i++)
	#else
			for (i = height - 1; i >= 0; i--)
	#endif
			{
				linePos = bfOffBits + lineSize * i;
				fseek(fp, linePos, SEEK_SET);	
				for (j = 0; j < width; j++)
				{
					*(lpImg + 2) = (unsigned char)fgetc(fp);	/* B */
					*(lpImg + 1) = (unsigned char)fgetc(fp);	/* G */
					*(lpImg + 0) = (unsigned char)fgetc(fp);	/* R */
	//							   (unsigned char)fgetc(fp);	/* reserved */
	//				*(lpImg + 3) = 255;							/* A */
					*(lpImg + 3) = (unsigned char)fgetc(fp);							/* A */

					lpImg += 4;
				}
			}
			break;

		default:
			free(lpImg);
			fclose(fp);
				fprintf(stderr, "invalid bpp type error %s\n", name);
			m_flgActive = false;
		}

	}

	if ( m_flgActive )
	{
		m_pBuf = lpImgArea;
		m_width = width;
		m_height = height;
	}

	if (fp)
	{
		fclose(fp);
	}
}


