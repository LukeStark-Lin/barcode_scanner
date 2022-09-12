#ifndef H_ECC200
#define H_ECC200
#include <stdlib.h>
#include <stdio.h>
#include "../../stdType.h"

typedef struct ECC200
{
	int iSymbolSize_Row;
	int iSymbolSize_Col;

	int iDataRegionSize_Row;
	int iDataRegionSize_Col;
	int iDataRegionNumber;

	int iMappingMatrixSize_Row;
	int iMappingMatrixSize_Col;

	int iCodeWordsData;
	int iCodeWordsError;

	/*2012.12.10*/
	//進行Reed-Solomon解碼時使用多少組Block
	int  i_RS_Block_Number;
	/**/
}ECC200;

void ECC200_MappingMatrix(int iGridRow,int iGridColumn,BYTE *_byteBinaryData,BYTE *_byteFormatTempData,BYTE *_byteFormatData);

void module(int row, int col, int chr, int bit,int iGridRow,int iGridColumn,ECC200 _ecc200,BYTE *_byteBinaryData,BYTE *_byteFormatTempData,BYTE *_byteFormatData);
void utah(int row, int col, int chr,int iGridRow,int iGridColumn,BYTE *_byteBinaryData,BYTE *_byteFormatTempData,BYTE *_byteFormatData);
void corner1(int chr,int iGridRow,int iGridColumn,BYTE *_byteBinaryData,BYTE *_byteFormatTempData,BYTE *_byteFormatData);
void corner2(int chr,int iGridRow,int iGridColumn,BYTE *_byteBinaryData,BYTE *_byteFormatTempData,BYTE *_byteFormatData);
void corner3(int chr,int iGridRow,int iGridColumn,BYTE *_byteBinaryData,BYTE *_byteFormatTempData,BYTE *_byteFormatData);
void corner4(int chr,int iGridRow,int iGridColumn,BYTE *_byteBinaryData,BYTE *_byteFormatTempData,BYTE *_byteFormatData);

int GetECC200SymbolAttributes(int iSymbolSize_Row,int iSymbolSize_Col,ECC200 *_ecc200,BYTE *_byteFormatData);
#endif
