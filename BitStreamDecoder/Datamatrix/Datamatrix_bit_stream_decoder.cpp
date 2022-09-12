#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "../Reed_solomon/head/reed_solomon.h"
#include "./head/Datamatrix.h"
#include "./head/ECC200.h"
#include "./head/DataMatrix_ExtendedMode.h"


BYTE tmpSource[MAX_SYMBOL];   // Temp source
BYTE byteOSource[MAX_MATRIX]; // Binary output source
BYTE byteFormatData[MAX_MATRIX];
int Cw_buff[MAX_frame][MAX__cw];

int Frame_cnt = 0;
bool fnc1 = false;
bool fnc_position = false;
bool fnc_position4 = false;
bool fnc_position1 = false;
bool fnc_position5 = false;

int GetMappingMatrix(BYTE *inSource, BYTE *outSource, int X, int Y, int *mapppingMatrixX, int *mapppingMatrixY);
char *DecodeBinary(int iGridX, int iGridY, BYTE *ucSource, ECC200 *_ecc200, int iDataLength);
int power(int number, int iPowFlag);
void repair_cw_dm(int *codewords, int N);
int RegroupRSBlock(int *iNumCodeWord, int *iNumCodeOrderedBlock, int i_CodeWordLength, int i_BlockNumber, int i_CodeWordDataLength, int i_CodeWordECCLength, int icheck);
int RecoveryCodeWord(int *iNumCodeWord, int *iNumCodeOrderedBlock, int i_CodeWordLength, int i_BlockNumber, int i_CodeWordDataLength, int i_CodeWordECCLength);
int DecodeText(s_DataMatrix_Result *result, ECC200 *_ecc200, char *resultout);

// C40的表格
int C40_Basic_Set[] = {C40_SHIFT_1, C40_SHIFT_2, C40_SHIFT_3, 32, 48, 49, 50, 51, 52, 53,
					   54, 55, 56, 57, 65, 66, 67, 68, 69, 70,
					   71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
					   81, 82, 83, 84, 85, 86, 87, 88, 89, 90};

int C40_Shift_1_Set[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
						 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
						 30, 31, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE};

int C40_Shift_2_Set[] = {33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
						 44, 45, 46, 47, 58, 59, 60, 61, 62, 63, 64,
						 91, 92, 93, 94, 95, C40_FNC_1, C40_NO_DEFINE, C40_NO_DEFINE, C40_UPPER_SHIFT, C40_NO_DEFINE,
						 C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE};

int C40_Shift_3_Set[] = {96, 97, 98, 99, 100, 101, 102, 103, 104, 105,
						 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
						 116, 117, 118, 119, 120, 121, 122, 123, 124, 125,
						 126, 127, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE, C40_NO_DEFINE};

// Text的表格
int Text_Basic_Set[] = {TEXT_SHIFT_1, TEXT_SHIFT_2, TEXT_SHIFT_3, 32, 48, 49, 50, 51, 52, 53,
						54, 55, 56, 57, 97, 98, 99, 100, 101, 102,
						103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
						113, 114, 115, 116, 117, 118, 119, 120, 121, 122};

int Text_Shift_1_Set[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
						  20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
						  30, 31, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE,
						  TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE};

int Text_Shift_2_Set[] = {33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
						  43, 44, 45, 46, 47, 58, 59, 60, 61, 62,
						  63, 64, 91, 92, 93, 94, 95, TEXT_FNC_1, TEXT_NO_DEFINE, TEXT_NO_DEFINE,
						  TEXT_UPPER_SHIFT, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE};

int Text_Shift_3_Set[] = {96, 65, 66, 67, 68, 69, 70, 71, 72, 73,
						  74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
						  84, 85, 86, 87, 88, 89, 90, 123, 124, 125,
						  126, 127, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE, TEXT_NO_DEFINE};

//EDIFACT的表格
int EDIFACT_Set[] = {64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
					 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
					 84, 85, 86, 87, 88, 89, 90, 91, 92, 93,
					 94, EDIFACT_UNLATCH, 32, 33, 34, 35, 36, 37, 38, 39,
					 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
					 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
					 60, 61, 62, 63};

//X12的表格
int X12_To_ASCII_Table[] = {13, 42, 62, 32, 48, 49, 50, 51, 52, 53,
							54, 55, 56, 57, 65, 66, 67, 68, 69, 70,
							71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
							81, 82, 83, 84, 85, 86, 87, 88, 89, 90};


char *Datamatrix_bit_stream_decoder(unsigned char *input,int iGridX,int iGridY)
{
	int test_iGridX = 14, test_iGridY = 14;
	int iMappingMatrixX,iMappingMatrixY;
	int iDataLength = 0;
	char *result = NULL;
    ECC200 ecc200;

	memcpy(tmpSource,input,sizeof(BYTE)*iGridX*iGridY);
    if (GetMappingMatrix(tmpSource, byteOSource, iGridX, iGridY, &iMappingMatrixX, &iMappingMatrixY) != 0)
    {
         iDataLength = (iMappingMatrixX * iMappingMatrixY + 7) / 8; // 因10x10, 14x14...無法被8整除, 需採無條件進位
         memset(byteFormatData, 0, MAX_MATRIX);
         memset(tmpSource, 0, MAX_SYMBOL);
	}else
		return NULL;

    ECC200_MappingMatrix(iMappingMatrixY, iMappingMatrixX, byteOSource, tmpSource, byteFormatData);
	if (GetECC200SymbolAttributes(iGridY, iGridX, &ecc200, byteFormatData) == 0)
	{
		return NULL;
	}

	result = DecodeBinary(iMappingMatrixX, iMappingMatrixY, byteFormatData, &ecc200, iDataLength);

	if(result != NULL)
		return result;
	else
		return NULL;
}


int GetMappingMatrix(BYTE *inSource, BYTE *outSource, int X, int Y, int *mapppingMatrixX, int *mapppingMatrixY)
{
    int i = 0, j = 0;
    int outSourceCounter = 0;
    int tempDataBlockSize = 0;
    int inX = 0, inY = 0;
    *outSource = 0;
    *mapppingMatrixX = 0;
    *mapppingMatrixY = 0;

    inX = X;
    inY = Y;
    if (inX < 10 || inY < 8 || inX > 144 || inY > 144) //檢查有沒有符?Xsymbol size
    {
        return 0;
    }
    //正方形
    if (inX == inY)
    {
        //Data Region 為1
        if (inX >= 10 && inX <= 26 && inX % 2 == 0) //symbol size 10~26 之間
        {
            *mapppingMatrixX = inX - 2;
            *mapppingMatrixY = inY - 2;
            outSourceCounter = 0;
            memset(outSource, 0, sizeof(BYTE) * (*mapppingMatrixX) * (*mapppingMatrixY));
            for (i = 0; i < inY; i++)
            {
                if (i == 0 || i == (inY - 1))
                    continue;
                for (j = 0; j < inX; j++)
                {
                    if (j == 0 || j == (inX - 1))
                        continue;
                    else
                    {
                        memcpy(outSource + outSourceCounter, inSource + inX * i + j, sizeof(BYTE));
                        outSourceCounter++;
                    }
                }
            }
            return 1;
        }
        //Data Region 為4
        if (inX >= 32 && inX <= 52 && ((inX - 32) % 4 == 0))
        {
            //DataRegion為4(2*2)時，每一塊DataBlock的大小
            tempDataBlockSize = inX / 2;

            *mapppingMatrixX = inX - 4;
            *mapppingMatrixY = inY - 4;
            outSourceCounter = 0;
            memset(outSource, 0, sizeof(BYTE) * (*mapppingMatrixX) * (*mapppingMatrixY));
            for (i = 0; i < inY; i++)
            {
                //EX : 0 15 16 31
                //EX : 0 25 26 51
                if ((i + 1) % tempDataBlockSize == 0 || i % tempDataBlockSize == 0)
                    continue;
                for (j = 0; j < inX; j++)
                {
                    if ((j + 1) % tempDataBlockSize == 0 || j % tempDataBlockSize == 0)
                        continue;
                    else
                    {
                        memcpy(outSource + outSourceCounter, inSource + inX * i + j, sizeof(BYTE));
                        outSourceCounter++;
                    }
                }
            }
            return 1;
        }
        //Data Region 為16
        if (inX >= 64 && inX <= 104 && ((inX - 64) % 8 == 0))
        {
            //DataRegion為16(4*4)時，每一塊DataBlock的大小
            tempDataBlockSize = inX / 4;

            *mapppingMatrixX = inX - 8;
            *mapppingMatrixY = inY - 8;
            outSourceCounter = 0;
            memset(outSource, 0, sizeof(BYTE) * (*mapppingMatrixX) * (*mapppingMatrixY));
            for (i = 0; i < inY; i++)
            {
                //EX: 0 15 16 31 32 47 48 63
                if ((i + 1) % tempDataBlockSize == 0 || i % tempDataBlockSize == 0)
                    continue;
                for (j = 0; j < inX; j++)
                {
                    if ((j + 1) % tempDataBlockSize == 0 || j % tempDataBlockSize == 0)
                        continue;
                    else
                    {
                        memcpy(outSource + outSourceCounter, inSource + inX * i + j, sizeof(BYTE));
                        outSourceCounter++;
                    }
                }
            }
            return 1;
        }
        //Data Region 為36
        if (inX == 120 || inX == 132 || inX == 144)
        {
            //DataRegion為36(6*6)?氶A?C一塊DataBlock的大小
            tempDataBlockSize = inX / 6;

            *mapppingMatrixX = inX - 12;
            *mapppingMatrixY = inY - 12;
            outSourceCounter = 0;
            memset(outSource, 0, sizeof(BYTE) * (*mapppingMatrixX) * (*mapppingMatrixY));
            //強制不縮
            memcpy(outSource, inSource, 18 * 18 * 36);
            return 1;
            /*for(i = 0; i < inY; i++)
            {
                //EX:0 19 20 39 40 59 60 79 80 99  100 119
                //EX:0 23 24 47 48 71 72 95 96 119 120 143
                if ((i + 1) % tempDataBlockSize == 0 || i % tempDataBlockSize == 0)
                    continue;
                for (j = 0; j < inX; j++)
                {
                    if ((j + 1) % tempDataBlockSize == 0 || j % tempDataBlockSize == 0)
                        continue;
                    else
                    {
                        memcpy(outSource + outSourceCounter, inSource + inX * i + j, sizeof(BYTE));
                        outSourceCounter++;
                    }
                }
            }
            return 1;*/
        }
        else
            return 0;
    }
    else //長方形
    {
        //
        if (inX == 18 && inY == 8)
        {
            *mapppingMatrixX = inX - 2;
            *mapppingMatrixY = inY - 2;
            outSourceCounter = 0;
            memset(outSource, 0, sizeof(BYTE) * (*mapppingMatrixX) * (*mapppingMatrixY));
            for (i = 0; i < inY; i++)
            {
                if (i == 0 || i == 7)
                    continue;
                for (j = 0; j < inX; j++)
                {
                    if (j == 0 || j == 17)
                        continue;
                    else
                    {
                        memcpy(outSource + outSourceCounter, inSource + inX * i + j, sizeof(BYTE));
                        outSourceCounter++;
                    }
                }
            }
            return 1;
        }
        //
        if (inX == 32 && inY == 8)
        {
            *mapppingMatrixX = inX - 4;
            *mapppingMatrixY = inY - 2;
            outSourceCounter = 0;
            memset(outSource, 0, sizeof(BYTE) * (*mapppingMatrixX) * (*mapppingMatrixY));
            for (i = 0; i < inY; i++)
            {
                if (i == 0 || i == 7)
                    continue;
                for (j = 0; j < inX; j++)
                {
                    if (j == 0 || j == 15 || j == 16 || j == 31)
                        continue;
                    else
                    {
                        memcpy(outSource + outSourceCounter, inSource + inX * i + j, sizeof(BYTE));
                        outSourceCounter++;
                    }
                }
            }
            return 1;
        }
        //
        if (inX == 26 && inY == 12)
        {
            *mapppingMatrixX = inX - 2;
            *mapppingMatrixY = inY - 2;
            outSourceCounter = 0;
            memset(outSource, 0, sizeof(BYTE) * (*mapppingMatrixX) * (*mapppingMatrixY));
            for (i = 0; i < inY; i++)
            {
                if (i == 0 || i == 11)
                    continue;
                for (j = 0; j < inX; j++)
                {
                    if (j == 0 || j == 25)
                        continue;
                    else
                    {
                        memcpy(outSource + outSourceCounter, inSource + inX * i + j, sizeof(BYTE));
                        outSourceCounter++;
                    }
                }
            }
            return 1;
        }
        //
        if (inX == 36 && inY == 12)
        {
            *mapppingMatrixX = inX - 4;
            *mapppingMatrixY = inY - 2;
            outSourceCounter = 0;
            memset(outSource, 0, sizeof(BYTE) * (*mapppingMatrixX) * (*mapppingMatrixY));
            for (i = 0; i < inY; i++)
            {
                if (i == 0 || i == 11)
                    continue;
                for (j = 0; j < inX; j++)
                {
                    if (j == 0 || j == 17 || j == 18 || j == 31)
                        continue;
                    else
                    {
                        memcpy(outSource + outSourceCounter, inSource + inX * i + j, sizeof(BYTE));
                        outSourceCounter++;
                    }
                }
            }
            return 1;
        }
        //
        if (inX == 36 && inY == 16)
        {
            *mapppingMatrixX = inX - 4;
            *mapppingMatrixY = inY - 2;
            outSourceCounter = 0;
            memset(outSource, 0, sizeof(BYTE) * (*mapppingMatrixX) * (*mapppingMatrixY));
            for (i = 0; i < inY; i++)
            {
                if (i == 0 || i == 15)
                    continue;
                for (j = 0; j < inX; j++)
                {
                    if (j == 0 || j == 17 || j == 18 || j == 33)
                        continue;
                    else
                    {
                        memcpy(outSource + outSourceCounter, inSource + inX * i + j, sizeof(BYTE));
                        outSourceCounter++;
                    }
                }
            }

            return 1;
        }
        //
        if (inX == 48 && inY == 16)
        {
            *mapppingMatrixX = inX - 4;
            *mapppingMatrixY = inY - 2;
            outSourceCounter = 0;
            memset(outSource, 0, sizeof(BYTE) * (*mapppingMatrixX) * (*mapppingMatrixY));
            for (i = 0; i < inY; i++)
            {
                if (i == 0 || i == 15)
                    continue;
                for (j = 0; j < inX; j++)
                {
                    if (j == 0 || j == 23 || j == 24 || j == 47)
                        continue;
                    else
                    {
                        memcpy(outSource + outSourceCounter, inSource + inX * i + j, sizeof(BYTE));
                        outSourceCounter++;
                    }
                }
            }
            return 1;
        }
        else
            return 0;
    }
}

int power(int number, int iPowFlag)
{
    int sum = 1;
    for (int i = 0; i < iPowFlag; i++)
    {
        sum *= number;
    }
    return sum;
}

char *DecodeBinary(int iGridX, int iGridY, BYTE *ucSource, ECC200 *_ecc200, int iDataLength)
{

    //int iNumDecoderInf = 0;
    int i = 0, x = 0, y = 0;
    int iRS_Block_Length = 0;
    int iNumCodeOrderedBlock[MAX_CODEWORD]; // 重新排列後的CodeWord內容
    int data_size = 0;
    int iTempRSResult = 0;
    int iPowFlag = 0;
    int tempNumCodeWord[MAX_RS_BLOCK_L];
    int icount = 0;

    char *resultout = NULL;
    resultout = (char *)calloc(MAX_CHARACTER_LENGTH, sizeof(char)); //MAX_CHARACTER_LENGTH is 3116
    s_DataMatrix_Result result_ext;                                 // Output codeword info
    result_ext.iCodeWordLength = iDataLength;
    result_ext.iCodeWordDataLength = _ecc200->iCodeWordsData;
    memset(result_ext.iCodeWord, 0, sizeof(int) * iDataLength);
    memset(iNumCodeOrderedBlock, 0, sizeof(int) * iDataLength);

    icount = 0;
    for (y = 0; y < iDataLength; y++)
    {

        iPowFlag = 7;
        //iResultNum = 0;
        result_ext.iCodeWord[y] = 0;
        for (x = 0; x < 8; x++)
        {
            result_ext.iCodeWord[y] += ((int)ucSource[y * 8 + x]) * power(2, iPowFlag);

            if (result_ext.iCodeWord[y] == 0)
            {
                icount = icount + 1;
            }
            iPowFlag -= 1;
        }
    }

    switch (iDataLength)
    {

    case 8:
        if (icount > 40)
        {
            return 0;
        }
        break;

    case 13:
        if (icount > 58)
        {
            return 0;
        }
        break;

    case 18:
        if (icount > 80)
        {
            return 0;
        }
        break;

    case 25:
        if (icount > 98)
        {
            return 0;
        }
        break;

    case 32:
        if (icount > 112)
        {
            return 0;
        }
        break;

    case 41:
        if (icount > 146)
        {
            return 0;
        }
        break;

    case 50:
        if (icount > 160)
        {
            return 0;
        }
        break;

    case 61:
        if (icount > 194)
        {
            return 0;
        }
        break;

    case 72:
        if (icount > 224)
        {
            return 0;
        }
        break;

    case 98:
        if (icount > 288)
        {
            return 0;
        }
        break;

    case 128:
        if (icount > 336)
        {
            return 0;
        }
        break;

    case 162:
        if (icount > 384)
        {
            return 0;
        }
        break;

    case 200:
        if (icount > 448)
        {
            return 0;
        }
        break;

    case 242:
        if (icount > 544)
        {
            return 0;
        }
        break;

    case 288:
        if (icount > 673)
        {
            return 0;
        }
        break;

    case 392:
        if (icount > 897)
        {
        }
        break;

    case 512:
        if (icount > 1151)
        {
            return 0;
        }
        break;

    case 648:
        if (icount > 1534)
        {
            return 0;
        }
        break;

    case 800:
        if (icount > 1792)
        {
            return 0;
        }
        break;

    case 968:
        if (icount > 2176)
        {
            return 0;
        }
        break;

    case 1152:
        if (icount > 2691)
        {
            return 0;
        }
        break;

    case 1458:
        if (icount > 3266)
        {
            return 0;
        }
        break;

    case 1800:
        if (icount > 3974)
        {
            return 0;
        }
        break;

    case 2178:
        if (icount > 4966)
        {
            return 0;
        }
        break;

        //Rectangular Symbols//

    case 12:

        if (icount > 56)
        {
            return 0;
        }
        break;

    case 21:
        if (icount > 88)
        {
            return 0;
        }
        break;

    case 30:
        if (icount > 112)
        {
            return 0;
        }
        break;

    case 40:
        if (icount > 144)
        {
            return 0;
        }
        break;

    case 56:
        if (icount > 192)
        {
            return 0;
        }
        break;

    case 77:
        if (icount > 224)
        {
            return 0;
        }
        break;
    }

    /*=============================spec Annex A p.40================================*/
    //printf("\n%d\n",_ecc200->i_RS_Block_Number);
    int icheck = 0;
    if (_ecc200->i_RS_Block_Number == 10) //使用10組block
    {
        iRS_Block_Length = 218;
    }
    else
    {
        iRS_Block_Length = iDataLength / _ecc200->i_RS_Block_Number;
    }
    repair_cw_dm(result_ext.iCodeWord, iDataLength);
    icheck = 1;
    if (RegroupRSBlock(result_ext.iCodeWord, iNumCodeOrderedBlock, iDataLength, _ecc200->i_RS_Block_Number, _ecc200->iCodeWordsData, _ecc200->iCodeWordsError, icheck) == 0)
    {
        return 0;
    }

    for (i = 0; i < _ecc200->i_RS_Block_Number; i++)
    {
        if (_ecc200->iCodeWordsData == 1558 && (i == 8 || i == 9))
        {
            memcpy(tempNumCodeWord, iNumCodeOrderedBlock + 1744 + (i - 8) * 217, 217 * sizeof(int));
            iTempRSResult = reed_solomon(tempNumCodeWord, 217, 155, DATA_MATRIX);
            if ((iTempRSResult == 0))
            {
                if ((iTempRSResult == 0) && (icheck == 0))
                {
                    return 0;
                }
                icheck = 0;

                RegroupRSBlock(result_ext.iCodeWord, iNumCodeOrderedBlock, iDataLength, _ecc200->i_RS_Block_Number, _ecc200->iCodeWordsData, _ecc200->iCodeWordsError, icheck);
                i = -1;
            }
            memcpy(iNumCodeOrderedBlock + 1744 + (i - 8) * 217, tempNumCodeWord, 217 * sizeof(int));
        }
        else if (_ecc200->iCodeWordsData == 1558)
        {
            memcpy(tempNumCodeWord, iNumCodeOrderedBlock + i * 218, 218 * sizeof(int));
            iTempRSResult = reed_solomon(tempNumCodeWord, 218, 156, DATA_MATRIX);
            if ((iTempRSResult == 0))
            {
                if ((iTempRSResult == 0) && (icheck == 0))
                {
                    return 0;
                }
                icheck = 0;

                RegroupRSBlock(result_ext.iCodeWord, iNumCodeOrderedBlock, iDataLength, _ecc200->i_RS_Block_Number, _ecc200->iCodeWordsData, _ecc200->iCodeWordsError, icheck);
                i = -1;
            }
            memcpy(iNumCodeOrderedBlock + iRS_Block_Length * i, tempNumCodeWord, iRS_Block_Length * sizeof(int));
        }
        else
        {
            memcpy(tempNumCodeWord, iNumCodeOrderedBlock + iRS_Block_Length * i, iRS_Block_Length * sizeof(int));

            iTempRSResult = reed_solomon(tempNumCodeWord, (_ecc200->iCodeWordsError + _ecc200->iCodeWordsData) / _ecc200->i_RS_Block_Number, _ecc200->iCodeWordsData / _ecc200->i_RS_Block_Number, DATA_MATRIX);
            //printf("\n%d\n",iTempRSResult );
            //printf("n=%d,k=%d\n",(_ecc200->iCodeWordsError + _ecc200->iCodeWordsData)/_ecc200->i_RS_Block_Number, _ecc200->iCodeWordsData/_ecc200->i_RS_Block_Number);
            if (iTempRSResult == 0)
            {
                return 0;
            }
            memcpy(iNumCodeOrderedBlock + iRS_Block_Length * i, tempNumCodeWord, iRS_Block_Length * sizeof(int));
        }
    }
    //for(int q = 0;q < iRS_Block_Length;q++) printf("%d,",tempNumCodeWord[q]);
    //printf("\n");

    RecoveryCodeWord(result_ext.iCodeWord, iNumCodeOrderedBlock, iDataLength, _ecc200->i_RS_Block_Number, _ecc200->iCodeWordsData, _ecc200->iCodeWordsError);

    if (DecodeText(&result_ext, _ecc200, resultout))
    {

        //printf("\n \n" );

        //data_size = strlen(result_ext.wcContent);
        //printf("%d,",data_size);
        //result = (char *)calloc(MAX_CHARACTER_LENGTH, sizeof(char));	//MAX_CHARACTER_LENGTH is 3116
        //wcstombs( result, result_ext.wcContent, origsize);
        // for(int i = 0 ; i < data_size ; i++) {
        // result[i] = result_ext.wcContent[i];
        // printf("%c",result[i] );
        //}
        // free(result_ext.wcContent);
        return resultout;
    }
    else
    {
        return 0;
    }
}


void repair_cw_dm(int *codewords, int N)
{
    int i, j, k;
    //int N = codewords_w*codewords_h;
    int max, max_idx;
    int weight[MAX_frame];
    int diff_cnt = 0; // 兩個frame中不相同的codeword個數有多少

#ifdef CW_WEIGHT_DEBUG
    for (i = 0; i < MAX__cw; i++)
        for (j = 0; j < MAX_frame; j++)
            tmp_weight[j][i] = 0;
#endif

    // 比較兩張frame中codewords差異性
    if (Frame_cnt != 0)
    {
        for (i = 0; i < N; i++)
        {
            if (codewords[i] != Cw_buff[Frame_cnt - 1][i])
                diff_cnt++;
        }
        //printf("diff_cnt = %d, N = %d, ratio = %d%%\n", diff_cnt, N, 100*diff_cnt/N);
        if (diff_cnt * 100 > N * 55) // 如果相異的點數超過總codewords的60%,則視為另一個新的barcode
            Frame_cnt = 0;
    }

    // 藉由比較每個frame的權重來填補正確的codewords
    for (i = 0; i < N; i++)
    {
        max = 0;
        max_idx = -1;
        for (j = 0; j <= Frame_cnt; j++)
            weight[j] = 0;
        Cw_buff[Frame_cnt][i] = codewords[i];

        for (j = Frame_cnt; j > 0; j--)
        {
            if (Cw_buff[j][i] == -1)
                continue;
            for (k = j - 1; k >= 0; k--)
            {
                if (Cw_buff[j][i] == Cw_buff[k][i])
                {
                    weight[j]++;
#ifdef CW_WEIGHT_DEBUG
                    if (j == frame_cnt)
                        tmp_weight[j][i]++;
#endif
                }
            }
            if (weight[j] >= (Frame_cnt + 1) >> 1)
            {
                max = weight[j];
                codewords[i] = Cw_buff[j][i];
                break;
            }
        }

        if (max)
            continue;
        for (j = Frame_cnt; j >= 0; j--)
        {
            if (weight[j] > max)
            {
                max = weight[j];
                max_idx = j;
            }
        }
        if (max_idx != -1) // 至少有兩張frame的codeword相同
            codewords[i] = Cw_buff[max_idx][i];
        else if (codewords[i] == -1)
        { // 表示沒任何一個frame的codeword相同, 直接填入前一個frame的值
            for (j = Frame_cnt - 1; j >= 0; j--)
            {
                if (Cw_buff[j][i] != -1)
                {
                    codewords[i] = Cw_buff[j][i];
                    break;
                }
            }
        }
    }
#ifdef CW_WEIGHT_DEBUG
    printf("frame_cnt = %d\n", frame_cnt);
    FILE *file1;
    if (frame_cnt == 0)
        file1 = fopen("\\My Documents\\cw0.txt", "w");
    else if (frame_cnt == 1)
        file1 = fopen("\\My Documents\\cw1.txt", "w");
    else if (frame_cnt == 2)
        file1 = fopen("\\My Documents\\cw2.txt", "w");
    else if (frame_cnt == 3)
        file1 = fopen("\\My Documents\\cw3.txt", "w");
    else if (frame_cnt == 4)
        file1 = fopen("\\My Documents\\cw4.txt", "w");
    else if (frame_cnt == 5)
        file1 = fopen("\\My Documents\\cw5.txt", "w");
    else if (frame_cnt == 6)
        file1 = fopen("\\My Documents\\cw6.txt", "w");
    else if (frame_cnt == 7)
        file1 = fopen("\\My Documents\\cw7.txt", "w");
    else if (frame_cnt == 8)
        file1 = fopen("\\My Documents\\cw8.txt", "w");
    else if (frame_cnt == 9)
        file1 = fopen("\\My Documents\\cw9.txt", "w");
    fprintf(file1, "frame_cnt = %d, codewords_h = %d, codewords_w = %d\n", frame_cnt, codewords_h, codewords_w);
    // 每個frame的暫存codewords和weight
    for (int i = 0; i < codewords_h; ++i)
    {
        for (int j = 0; j < codewords_w; ++j)
        {
            fprintf(file1, "%3d(%d) ", cw_buff[frame_cnt][i * codewords_w + j], tmp_weight[frame_cnt][i * codewords_w + j]);
        }
        fprintf(file1, "\n\n");
    }
    // 實際輸出 codewords
    for (int i = 0; i < codewords_h; ++i)
    {
        for (int j = 0; j < codewords_w; ++j)
        {
            fprintf(file1, "%3d ", codewords[i * codewords_w + j]);
        }
        fprintf(file1, "\n");
    }
    fclose(file1);
#endif

    Frame_cnt = (Frame_cnt + 1) % MAX_frame;
}


int RegroupRSBlock(int *iNumCodeWord, int *iNumCodeOrderedBlock, int i_CodeWordLength, int i_BlockNumber, int i_CodeWordDataLength, int i_CodeWordECCLength, int icheck)
{
    int temp_Pointer[MAX_RS_BLOCK_N];
    int i = 0;

    switch (i_CodeWordDataLength)
    {

    case 204:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 2;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeOrderedBlock[temp_Pointer[0]] = iNumCodeWord[i];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeOrderedBlock[temp_Pointer[1]] = iNumCodeWord[i];
                temp_Pointer[1]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 280:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 2;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeOrderedBlock[temp_Pointer[0]] = iNumCodeWord[i];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeOrderedBlock[temp_Pointer[1]] = iNumCodeWord[i];
                temp_Pointer[1]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 368:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 4;
        temp_Pointer[2] = i_CodeWordLength / 2;
        temp_Pointer[3] = (i_CodeWordLength / 4) * 3;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeOrderedBlock[temp_Pointer[0]] = iNumCodeWord[i];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeOrderedBlock[temp_Pointer[1]] = iNumCodeWord[i];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeOrderedBlock[temp_Pointer[2]] = iNumCodeWord[i];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeOrderedBlock[temp_Pointer[3]] = iNumCodeWord[i];
                temp_Pointer[3]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 456:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 4;
        temp_Pointer[2] = i_CodeWordLength / 2;
        temp_Pointer[3] = (i_CodeWordLength / 4) * 3;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeOrderedBlock[temp_Pointer[0]] = iNumCodeWord[i];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeOrderedBlock[temp_Pointer[1]] = iNumCodeWord[i];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeOrderedBlock[temp_Pointer[2]] = iNumCodeWord[i];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeOrderedBlock[temp_Pointer[3]] = iNumCodeWord[i];
                temp_Pointer[3]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 576:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 4;
        temp_Pointer[2] = i_CodeWordLength / 2;
        temp_Pointer[3] = (i_CodeWordLength / 4) * 3;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeOrderedBlock[temp_Pointer[0]] = iNumCodeWord[i];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeOrderedBlock[temp_Pointer[1]] = iNumCodeWord[i];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeOrderedBlock[temp_Pointer[2]] = iNumCodeWord[i];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeOrderedBlock[temp_Pointer[3]] = iNumCodeWord[i];
                temp_Pointer[3]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 696:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 4;
        temp_Pointer[2] = i_CodeWordLength / 2;
        temp_Pointer[3] = (i_CodeWordLength / 4) * 3;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeOrderedBlock[temp_Pointer[0]] = iNumCodeWord[i];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeOrderedBlock[temp_Pointer[1]] = iNumCodeWord[i];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeOrderedBlock[temp_Pointer[2]] = iNumCodeWord[i];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeOrderedBlock[temp_Pointer[3]] = iNumCodeWord[i];
                temp_Pointer[3]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 816:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 6;
        temp_Pointer[2] = i_CodeWordLength / 3;
        temp_Pointer[3] = i_CodeWordLength / 2;
        temp_Pointer[4] = i_CodeWordLength / 3 * 2;
        temp_Pointer[5] = i_CodeWordLength / 6 * 5;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeOrderedBlock[temp_Pointer[0]] = iNumCodeWord[i];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeOrderedBlock[temp_Pointer[1]] = iNumCodeWord[i];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeOrderedBlock[temp_Pointer[2]] = iNumCodeWord[i];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeOrderedBlock[temp_Pointer[3]] = iNumCodeWord[i];
                temp_Pointer[3]++;
                break;
            case 4:
                iNumCodeOrderedBlock[temp_Pointer[4]] = iNumCodeWord[i];
                temp_Pointer[4]++;
                break;
            case 5:
                iNumCodeOrderedBlock[temp_Pointer[5]] = iNumCodeWord[i];
                temp_Pointer[5]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 1050:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 6;
        temp_Pointer[2] = i_CodeWordLength / 3;
        temp_Pointer[3] = i_CodeWordLength / 2;
        temp_Pointer[4] = i_CodeWordLength / 3 * 2;
        temp_Pointer[5] = i_CodeWordLength / 6 * 5;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeOrderedBlock[temp_Pointer[0]] = iNumCodeWord[i];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeOrderedBlock[temp_Pointer[1]] = iNumCodeWord[i];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeOrderedBlock[temp_Pointer[2]] = iNumCodeWord[i];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeOrderedBlock[temp_Pointer[3]] = iNumCodeWord[i];
                temp_Pointer[3]++;
                break;
            case 4:
                iNumCodeOrderedBlock[temp_Pointer[4]] = iNumCodeWord[i];
                temp_Pointer[4]++;
                break;
            case 5:
                iNumCodeOrderedBlock[temp_Pointer[5]] = iNumCodeWord[i];
                temp_Pointer[5]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 1304:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 8;
        temp_Pointer[2] = i_CodeWordLength / 4;
        temp_Pointer[3] = i_CodeWordLength / 8 * 3;
        temp_Pointer[4] = i_CodeWordLength / 2;
        temp_Pointer[5] = i_CodeWordLength / 8 * 5;
        temp_Pointer[6] = i_CodeWordLength / 4 * 3;
        temp_Pointer[7] = i_CodeWordLength / 8 * 7;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeOrderedBlock[temp_Pointer[0]] = iNumCodeWord[i];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeOrderedBlock[temp_Pointer[1]] = iNumCodeWord[i];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeOrderedBlock[temp_Pointer[2]] = iNumCodeWord[i];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeOrderedBlock[temp_Pointer[3]] = iNumCodeWord[i];
                temp_Pointer[3]++;
                break;
            case 4:
                iNumCodeOrderedBlock[temp_Pointer[4]] = iNumCodeWord[i];
                temp_Pointer[4]++;
                break;
            case 5:
                iNumCodeOrderedBlock[temp_Pointer[5]] = iNumCodeWord[i];
                temp_Pointer[5]++;
                break;
            case 6:
                iNumCodeOrderedBlock[temp_Pointer[6]] = iNumCodeWord[i];
                temp_Pointer[6]++;
                break;
            case 7:
                iNumCodeOrderedBlock[temp_Pointer[7]] = iNumCodeWord[i];
                temp_Pointer[7]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 1558:
        temp_Pointer[1] = 0;
        temp_Pointer[2] = 218;
        temp_Pointer[3] = 436;
        temp_Pointer[4] = 654;
        temp_Pointer[5] = 872;
        temp_Pointer[6] = 1090;
        temp_Pointer[7] = 1308;
        temp_Pointer[8] = 1526;
        temp_Pointer[9] = 1744;
        temp_Pointer[0] = 1961;
        for (i = 0; i < i_CodeWordDataLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeOrderedBlock[temp_Pointer[0]] = iNumCodeWord[i];
                temp_Pointer[0]++;
                break;
            case 1:
                iNumCodeOrderedBlock[temp_Pointer[1]] = iNumCodeWord[i];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeOrderedBlock[temp_Pointer[2]] = iNumCodeWord[i];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeOrderedBlock[temp_Pointer[3]] = iNumCodeWord[i];
                temp_Pointer[3]++;
                break;
            case 4:
                iNumCodeOrderedBlock[temp_Pointer[4]] = iNumCodeWord[i];
                temp_Pointer[4]++;
                break;
            case 5:
                iNumCodeOrderedBlock[temp_Pointer[5]] = iNumCodeWord[i];
                temp_Pointer[5]++;
                break;
            case 6:
                iNumCodeOrderedBlock[temp_Pointer[6]] = iNumCodeWord[i];
                temp_Pointer[6]++;
                break;
            case 7:
                iNumCodeOrderedBlock[temp_Pointer[7]] = iNumCodeWord[i];
                temp_Pointer[7]++;
                break;
            case 8:
                iNumCodeOrderedBlock[temp_Pointer[8]] = iNumCodeWord[i];
                temp_Pointer[8]++;
                break;
            case 9:
                iNumCodeOrderedBlock[temp_Pointer[9]] = iNumCodeWord[i];
                temp_Pointer[9]++;
                break;

            default:
                return 0;
            }
        }
        if (icheck == 0)
        {

            for (i = 0; i < i_CodeWordECCLength; i++)
            {
                switch ((i + 1) % i_BlockNumber)
                {

                case 0:
                    iNumCodeOrderedBlock[temp_Pointer[0]] = iNumCodeWord[1558 + i];
                    temp_Pointer[0]++;
                    break;
                case 1:
                    iNumCodeOrderedBlock[temp_Pointer[1]] = iNumCodeWord[1558 + i];
                    temp_Pointer[1]++;
                    break;
                case 2:
                    iNumCodeOrderedBlock[temp_Pointer[2]] = iNumCodeWord[1558 + i];
                    temp_Pointer[2]++;
                    break;
                case 3:
                    iNumCodeOrderedBlock[temp_Pointer[3]] = iNumCodeWord[1558 + i];
                    temp_Pointer[3]++;
                    break;
                case 4:
                    iNumCodeOrderedBlock[temp_Pointer[4]] = iNumCodeWord[1558 + i];
                    temp_Pointer[4]++;
                    break;
                case 5:
                    iNumCodeOrderedBlock[temp_Pointer[5]] = iNumCodeWord[1558 + i];
                    temp_Pointer[5]++;
                    break;
                case 6:
                    iNumCodeOrderedBlock[temp_Pointer[6]] = iNumCodeWord[1558 + i];
                    temp_Pointer[6]++;
                    break;
                case 7:
                    iNumCodeOrderedBlock[temp_Pointer[7]] = iNumCodeWord[1558 + i];
                    temp_Pointer[7]++;
                    break;
                case 8:
                    iNumCodeOrderedBlock[temp_Pointer[8]] = iNumCodeWord[1558 + i];
                    temp_Pointer[8]++;
                    break;
                case 9:
                    iNumCodeOrderedBlock[temp_Pointer[9]] = iNumCodeWord[1558 + i];
                    temp_Pointer[9]++;
                    break;

                default:
                    return 0;
                }
            }
        }
        if (icheck == 1)
        {

            for (i = 1558; i < i_CodeWordLength; i++)
            {
                switch ((i + 1) % i_BlockNumber)
                {

                case 0:
                    iNumCodeOrderedBlock[temp_Pointer[0]] = iNumCodeWord[i];
                    temp_Pointer[0]++;
                    break;
                case 1:
                    iNumCodeOrderedBlock[temp_Pointer[1]] = iNumCodeWord[i];
                    temp_Pointer[1]++;
                    break;
                case 2:
                    iNumCodeOrderedBlock[temp_Pointer[2]] = iNumCodeWord[i];
                    temp_Pointer[2]++;
                    break;
                case 3:
                    iNumCodeOrderedBlock[temp_Pointer[3]] = iNumCodeWord[i];
                    temp_Pointer[3]++;
                    break;
                case 4:
                    iNumCodeOrderedBlock[temp_Pointer[4]] = iNumCodeWord[i];
                    temp_Pointer[4]++;
                    break;
                case 5:
                    iNumCodeOrderedBlock[temp_Pointer[5]] = iNumCodeWord[i];
                    temp_Pointer[5]++;
                    break;
                case 6:
                    iNumCodeOrderedBlock[temp_Pointer[6]] = iNumCodeWord[i];
                    temp_Pointer[6]++;
                    break;
                case 7:
                    iNumCodeOrderedBlock[temp_Pointer[7]] = iNumCodeWord[i];
                    temp_Pointer[7]++;
                    break;
                case 8:
                    iNumCodeOrderedBlock[temp_Pointer[8]] = iNumCodeWord[i];
                    temp_Pointer[8]++;
                    break;
                case 9:
                    iNumCodeOrderedBlock[temp_Pointer[9]] = iNumCodeWord[i];
                    temp_Pointer[9]++;
                    break;

                default:
                    return 0;
                }
            }
        }
        break;
    default:
        memcpy(iNumCodeOrderedBlock, iNumCodeWord, i_CodeWordLength * sizeof(int));
        break;
    }
    return 1;
}

int RecoveryCodeWord(int *iNumCodeWord, int *iNumCodeOrderedBlock, int i_CodeWordLength, int i_BlockNumber, int i_CodeWordDataLength, int i_CodeWordECCLength)
{
    int temp_Pointer[MAX_RS_BLOCK_N];
    int i = 0;
    switch (i_CodeWordDataLength)
    {

    case 204:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 2;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[0]];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[1]];
                temp_Pointer[1]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 280:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 2;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[0]];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[1]];
                temp_Pointer[1]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 368:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 4;
        temp_Pointer[2] = i_CodeWordLength / 2;
        temp_Pointer[3] = i_CodeWordLength / 4 * 3;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[0]];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[1]];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[2]];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[3]];
                temp_Pointer[3]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 456:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 4;
        temp_Pointer[2] = i_CodeWordLength / 2;
        temp_Pointer[3] = i_CodeWordLength / 4 * 3;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[0]];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[1]];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[2]];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[3]];
                temp_Pointer[3]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 576:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 4;
        temp_Pointer[2] = i_CodeWordLength / 2;
        temp_Pointer[3] = i_CodeWordLength / 4 * 3;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[0]];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[1]];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[2]];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[3]];
                temp_Pointer[3]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 696:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 4;
        temp_Pointer[2] = i_CodeWordLength / 2;
        temp_Pointer[3] = i_CodeWordLength / 4 * 3;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[0]];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[1]];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[2]];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[3]];
                temp_Pointer[3]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 816:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 6;
        temp_Pointer[2] = i_CodeWordLength / 3;
        temp_Pointer[3] = i_CodeWordLength / 2;
        temp_Pointer[4] = i_CodeWordLength / 3 * 2;
        temp_Pointer[5] = i_CodeWordLength / 6 * 5;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[0]];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[1]];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[2]];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[3]];
                temp_Pointer[3]++;
                break;
            case 4:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[4]];
                temp_Pointer[4]++;
                break;
            case 5:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[5]];
                temp_Pointer[5]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 1050:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 6;
        temp_Pointer[2] = i_CodeWordLength / 3;
        temp_Pointer[3] = i_CodeWordLength / 2;
        temp_Pointer[4] = i_CodeWordLength / 3 * 2;
        temp_Pointer[5] = i_CodeWordLength / 6 * 5;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[0]];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[1]];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[2]];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[3]];
                temp_Pointer[3]++;
                break;
            case 4:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[4]];
                temp_Pointer[4]++;
                break;
            case 5:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[5]];
                temp_Pointer[5]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 1304:
        temp_Pointer[0] = 0;
        temp_Pointer[1] = i_CodeWordLength / 8;
        temp_Pointer[2] = i_CodeWordLength / 4;
        temp_Pointer[3] = i_CodeWordLength / 8 * 3;
        temp_Pointer[4] = i_CodeWordLength / 2;
        temp_Pointer[5] = i_CodeWordLength / 8 * 5;
        temp_Pointer[6] = i_CodeWordLength / 4 * 3;
        temp_Pointer[7] = i_CodeWordLength / 8 * 7;
        for (i = 0; i < i_CodeWordLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[0]];
                temp_Pointer[0]++;
                break;

            case 1:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[1]];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[2]];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[3]];
                temp_Pointer[3]++;
                break;
            case 4:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[4]];
                temp_Pointer[4]++;
                break;
            case 5:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[5]];
                temp_Pointer[5]++;
                break;
            case 6:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[6]];
                temp_Pointer[6]++;
                break;
            case 7:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[7]];
                temp_Pointer[7]++;
                break;

            default:
                return 0;
            }
        }
        break;
    case 1558:
        temp_Pointer[1] = 0;
        temp_Pointer[2] = 218;
        temp_Pointer[3] = 436;
        temp_Pointer[4] = 654;
        temp_Pointer[5] = 872;
        temp_Pointer[6] = 1090;
        temp_Pointer[7] = 1308;
        temp_Pointer[8] = 1526;
        temp_Pointer[9] = 1744;
        temp_Pointer[0] = 1961;
        for (i = 0; i < i_CodeWordDataLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[0]];
                temp_Pointer[0]++;
                break;
            case 1:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[1]];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[2]];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[3]];
                temp_Pointer[3]++;
                break;
            case 4:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[4]];
                temp_Pointer[4]++;
                break;
            case 5:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[5]];
                temp_Pointer[5]++;
                break;
            case 6:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[6]];
                temp_Pointer[6]++;
                break;
            case 7:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[7]];
                temp_Pointer[7]++;
                break;
            case 8:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[8]];
                temp_Pointer[8]++;
                break;
            case 9:
                iNumCodeWord[i] = iNumCodeOrderedBlock[temp_Pointer[9]];
                temp_Pointer[9]++;
                break;

            default:
                return 0;
            }
        }

        for (i = 0; i < i_CodeWordECCLength; i++)
        {
            switch ((i + 1) % i_BlockNumber)
            {
            case 0:
                iNumCodeWord[1558 + i] = iNumCodeOrderedBlock[temp_Pointer[0]];
                temp_Pointer[0]++;
                break;
            case 1:
                iNumCodeWord[1558 + i] = iNumCodeOrderedBlock[temp_Pointer[1]];
                temp_Pointer[1]++;
                break;
            case 2:
                iNumCodeWord[1558 + i] = iNumCodeOrderedBlock[temp_Pointer[2]];
                temp_Pointer[2]++;
                break;
            case 3:
                iNumCodeWord[1558 + i] = iNumCodeOrderedBlock[temp_Pointer[3]];
                temp_Pointer[3]++;
                break;
            case 4:
                iNumCodeWord[1558 + i] = iNumCodeOrderedBlock[temp_Pointer[4]];
                temp_Pointer[4]++;
                break;
            case 5:
                iNumCodeWord[1558 + i] = iNumCodeOrderedBlock[temp_Pointer[5]];
                temp_Pointer[5]++;
                break;
            case 6:
                iNumCodeWord[1558 + i] = iNumCodeOrderedBlock[temp_Pointer[6]];
                temp_Pointer[6]++;
                break;
            case 7:
                iNumCodeWord[1558 + i] = iNumCodeOrderedBlock[temp_Pointer[7]];
                temp_Pointer[7]++;
                break;
            case 8:
                iNumCodeWord[1558 + i] = iNumCodeOrderedBlock[temp_Pointer[8]];
                temp_Pointer[8]++;
                break;
            case 9:
                iNumCodeWord[1558 + i] = iNumCodeOrderedBlock[temp_Pointer[9]];
                temp_Pointer[9]++;
                break;

            default:
                return 0;
            }
        }
        break;
    default:
        memcpy(iNumCodeWord, iNumCodeOrderedBlock, i_CodeWordLength * sizeof(int));
        break;
    }
    return 1;
}

int DecodeText(s_DataMatrix_Result *result, ECC200 *_ecc200, char *resultout)
{
    int i;
    int iNumDecoderInf;
    char tempCharValue;
    char wc_tempArray[20];
    //fnc_position = false;
    //fnc_position4 = false;
    //fnc_position1 = false;
    // fnc_position5 = false;

    //	char wc_temp_Value_1;

    s_ExtendedMode_Struct sExtendedMode_Parameters;

    sExtendedMode_Parameters._iC40_Current_Set = C40_BASIC_SET;
    sExtendedMode_Parameters._iC40_Last_Set = C40_BASIC_SET;
    sExtendedMode_Parameters._iCurrent_Encoding_Mode = ASCII;
    sExtendedMode_Parameters._iLast_Encoding_Mode = ASCII;
    sExtendedMode_Parameters._iText_Current_Set = TEXT_BASIC_SET;
    sExtendedMode_Parameters._iText_Last_Set = TEXT_BASIC_SET;
    sExtendedMode_Parameters._iGoingUnlath = 0;
    //

    //printf("\n");

    //

    //
    result->iContentIndex = 0;
    //result->wcContent = (char *)malloc(sizeof(char) * 1);
    //result->wcContent[0] = '\0';
    i = result->iContentIndex;
    //
    int macro_flag = 0;
    for (i = 0; i < result->iCodeWordDataLength; i++)
    {
        /*//test
        resultout[result->iContentIndex] = 49;
        result->iContentIndex++;
        break;*/
        if (i == (result->iCodeWordDataLength - 1))
        {

            sExtendedMode_Parameters._iCurrent_Encoding_Mode = ASCII;
        }
        if ((result->iCodeWord[i] == 232) && (i != 0))
        {
            result->iCodeWord[i] = 29;
            tempCharValue = (char)(result->iCodeWord[i]);
            resultout[result->iContentIndex] = tempCharValue;
            result->iContentIndex++;
            continue;
        }

        //ASCII Code Value ASCII查表
        if ((result->iCodeWord[i] <= 229) && (sExtendedMode_Parameters._iCurrent_Encoding_Mode == ASCII))
        {
            if (result->iCodeWord[i] == 129)
            {
                if (macro_flag == 1)
                {
                    resultout[result->iContentIndex] = 30;
                    result->iContentIndex++;
                    resultout[result->iContentIndex] = 4;
                    result->iContentIndex++;
                }
                break;
            }
            else if ((result->iCodeWord[i] >= 130) && (result->iCodeWord[i] <= 229))
            {

                iNumDecoderInf = result->iCodeWord[i] - 130;

                if ((iNumDecoderInf >= 0) && (iNumDecoderInf <= 9))
                {
                    //swprintf(wc_tempArray, L"%d", iNumDecoderInf);
                    wc_tempArray[0] = (char)(iNumDecoderInf + 48);
                    resultout[result->iContentIndex] = L'0';
                    result->iContentIndex++;
                    //free(result->wcContent);
                    //result->wcContent = (char *)malloc(sizeof(char) * (result->iContentIndex + 1));
                    resultout[result->iContentIndex] = wc_tempArray[0];
                    //printf("\n%c\n",result->wcContent[result->iContentIndex]);
                    result->iContentIndex++;
                    //free(result->wcContent);
                    // result->wcContent = (char *)malloc(sizeof(char) * (result->iContentIndex + 1));
                }
                else
                {
                    if (iNumDecoderInf >= 10 && iNumDecoderInf < 100)
                    {
                        //swprintf(wc_tempArray, L"%d", iNumDecoderInf);
                        wc_tempArray[0] = (char)((iNumDecoderInf / 10) + 48);
                        resultout[result->iContentIndex] = wc_tempArray[0];
                        //printf("\n%c\n",result->wcContent[result->iContentIndex]);
                        result->iContentIndex++;
                        // free(result->wcContent);
                        //result->wcContent = (char *)malloc(sizeof(char) * (result->iContentIndex + 1));
                        wc_tempArray[1] = (char)((iNumDecoderInf % 10) + 48);
                        resultout[result->iContentIndex] = wc_tempArray[1];
                        //printf("\n%c\n",result->wcContent[result->iContentIndex]);
                        result->iContentIndex++;
                        //free(result->wcContent);
                        //result->wcContent = (char *)malloc(sizeof(char) * (result->iContentIndex + 1));
                    }
                }
            }
            else if (result->iCodeWord[i] < 129)
            {
                if (result->iCodeWord[i - 1] == 235)
                {

                    tempCharValue = (char)(result->iCodeWord[i] + 127);
                    //mbtowc(&wc_temp_Value_1,&tempCharValue,1);
                    resultout[result->iContentIndex] = tempCharValue;
                    //printf("\n%c\n",result->wcContent[result->iContentIndex]);
                    result->iContentIndex++;
                }
                else
                {
                    tempCharValue = (char)(result->iCodeWord[i] - 1);
                    //mbtowc(&wc_temp_Value_1,&tempCharValue,1);
                    resultout[result->iContentIndex] = tempCharValue;
                    //printf("\n%c\n",result->wcContent[result->iContentIndex]);
                    result->iContentIndex++;
                }
            }
            else // Normal ASCII Value
            {
                tempCharValue = (char)(result->iCodeWord[i] - 1);
                //mbtowc(&wc_temp_Value_1,&tempCharValue,1);
                resultout[result->iContentIndex] = tempCharValue;
                //printf("\n%c\n",result->wcContent[result->iContentIndex]);
                result->iContentIndex++;
                //free(result->wcContent);
                // result->wcContent = (char *)malloc(sizeof(char) * (result->iContentIndex + 1));
            }
        }
        if (result->iCodeWord[i] == 254)
        {
            sExtendedMode_Parameters._iCurrent_Encoding_Mode = ASCII;
        }

        else if (result->iCodeWord[i] > 229 && sExtendedMode_Parameters._iCurrent_Encoding_Mode == ASCII) // ASCII 跳其它模式
        {

            switch (result->iCodeWord[i])
            {

            case C40:
                sExtendedMode_Parameters._iCurrent_Encoding_Mode = C40;
                sExtendedMode_Parameters._iLast_Encoding_Mode = ASCII;
                i++;
                result->iCodeWordIndex = i;
                if (!GetC40(result, &sExtendedMode_Parameters, resultout))
                {
                    resultout[result->iContentIndex] = L'\0';
                    return 0;
                }
                i = result->iCodeWordIndex;
                break;
            case Base256:
                sExtendedMode_Parameters._iCurrent_Encoding_Mode = Base256;
                sExtendedMode_Parameters._iLast_Encoding_Mode = ASCII;
                i++;
                result->iCodeWordIndex = i;
                if (!GetBase256(result, &sExtendedMode_Parameters, resultout))
                {
                    resultout[result->iContentIndex] = L'\0';
                    return 1;
                }
                i = result->iCodeWordIndex;
                break;
            case FNC1:
                if ((result->iCodeWord[0] == 232) || (result->iCodeWord[4] == 232) || (result->iCodeWord[1] == 232) || (result->iCodeWord[5] == 232))
                {
                    fnc1 = true;
                    if (result->iCodeWord[0] == 232)
                        fnc_position = true;
                    if (result->iCodeWord[4] == 232)
                        fnc_position4 = true;
                    if (result->iCodeWord[1] == 232)
                        fnc_position1 = true;
                    if (result->iCodeWord[5] == 232)
                        fnc_position5 = true;
                    //if(result->iCodeWord[19] == 232)
                    //fnc_position6 = true;
                    break;
                }
                //if((result->iCodeWord[i] == 232) && (i != 0 )&&(jj==0))
                // {
                // jj++;
                // i--;
                // break;
                //}

            case Structured_Append:
                break;
            case Reader_Programming:
                break;
            case Upper_Shift:
                break;
            case _05_Macro:
                resultout[result->iContentIndex] = 91;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 41;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 62;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 30;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 48;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 53;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 29;
                result->iContentIndex++;
                macro_flag = 1;
                break;
            case _06_Macro:
                resultout[result->iContentIndex] = 91;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 41;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 62;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 30;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 48;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 54;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 29;
                result->iContentIndex++;
                macro_flag = 1;
                break;
            case X12:
                sExtendedMode_Parameters._iCurrent_Encoding_Mode = X12;
                sExtendedMode_Parameters._iLast_Encoding_Mode = ASCII;
                i++;
                result->iCodeWordIndex = i;
                if (!GetX12(result, &sExtendedMode_Parameters, resultout))
                {
                    resultout[result->iContentIndex] = L'\0';
                    return 0;
                }
                i = result->iCodeWordIndex;
                break;
            case Text:
                sExtendedMode_Parameters._iCurrent_Encoding_Mode = Text;
                sExtendedMode_Parameters._iLast_Encoding_Mode = ASCII;
                i++;
                result->iCodeWordIndex = i;
                if (!GetText(result, &sExtendedMode_Parameters, resultout))
                {
                    resultout[result->iContentIndex] = L'\0';
                    return 0;
                }
                i = result->iCodeWordIndex;
                break;
            case EDIFACT:
                sExtendedMode_Parameters._iCurrent_Encoding_Mode = EDIFACT;
                sExtendedMode_Parameters._iLast_Encoding_Mode = ASCII;
                i++;
                result->iCodeWordIndex = i;
                if (!GetEDIFACT(result, &sExtendedMode_Parameters, resultout))
                {
                    resultout[result->iContentIndex] = L'\0';
                    return 0;
                }
                i = result->iCodeWordIndex;
                break;
            case ECI:
                i++;
                break;
            //case 254:
            //sExtendedMode_Parameters._iCurrent_Encoding_Mode = ASCII;
            //break;
            default:
                break;
            }
        }
        else // 原本就在其它模式下，解碼
        {
            switch (sExtendedMode_Parameters._iCurrent_Encoding_Mode)
            {
            case C40:
                result->iCodeWordIndex = i;
                if (!GetC40(result, &sExtendedMode_Parameters, resultout))
                {
                    resultout[result->iContentIndex] = L'\0';
                    return 1;
                }
                i = result->iCodeWordIndex;
                break;
            case Base256:
                result->iCodeWordIndex = i;
                if (!GetBase256(result, &sExtendedMode_Parameters, resultout))
                {
                    resultout[result->iContentIndex] = L'\0';
                    return 1;
                }
                i = result->iCodeWordIndex;
                break;
            case FNC1:
                if ((result->iCodeWord[0] == 232) || (result->iCodeWord[4] == 232) || (result->iCodeWord[1] == 232) || (result->iCodeWord[5] == 232))
                {
                    fnc1 = true;
                    if (result->iCodeWord[0] == 232)
                        fnc_position = true;
                    if (result->iCodeWord[4] == 232)
                        fnc_position4 = true;
                    if (result->iCodeWord[1] == 232)
                        fnc_position1 = true;
                    if (result->iCodeWord[5] == 232)
                        fnc_position5 = true;
                    //if(result->iCodeWord[19] == 232)
                    //fnc_position6 = true;
                    break;
                }
                //if((result->iCodeWord[i] == 232) && (i != 0 )&&(jj==0))
                //{
                // jj++;
                // break;
                // }

            case Structured_Append:
                break;
            case Reader_Programming:
                break;
            case Upper_Shift:
                break;
            case _05_Macro:
                resultout[result->iContentIndex] = 91;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 41;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 62;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 30;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 48;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 53;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 29;
                result->iContentIndex++;
                macro_flag = 1;
                break;
            case _06_Macro:
                resultout[result->iContentIndex] = 91;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 41;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 62;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 30;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 48;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 54;
                result->iContentIndex++;
                resultout[result->iContentIndex] = 29;
                result->iContentIndex++;
                macro_flag = 1;
                break;
            case X12:
                result->iCodeWordIndex = i;
                if (!GetX12(result, &sExtendedMode_Parameters, resultout))
                {
                    resultout[result->iContentIndex] = L'\0';
                    return 1;
                }
                i = result->iCodeWordIndex;
                break;
            case Text:
                result->iCodeWordIndex = i;
                if (!GetText(result, &sExtendedMode_Parameters, resultout))
                {
                    resultout[result->iContentIndex] = L'\0';
                    return 1;
                }
                i = result->iCodeWordIndex;
                break;
            case EDIFACT:
                result->iCodeWordIndex = i;
                if (!GetEDIFACT(result, &sExtendedMode_Parameters, resultout))
                {
                    resultout[result->iContentIndex] = L'\0';
                    return 1;
                }
                i = result->iCodeWordIndex;

                break;
            case ECI:
                i++;
                break;
            default:
                break;
            }
        }
    }
    resultout[result->iContentIndex] = L'\0';
    return 1;
}

//Text
int GetText(s_DataMatrix_Result *result, s_ExtendedMode_Struct *_sExtModePara, char *resultout)
{
	int i, tempIntValue;
	char tempCharValue;
	//wchar_t wc_temp_Value_1;
	int iCodeWordTrans[3];
	long ltemp;
	tempCharValue = '\0';
	//wc_temp_Value_1 = 0;
	//result->wcContent[200];
	//result->wcContent = (char*)malloc(sizeof(char)*1);
	ltemp = 256 * result->iCodeWord[result->iCodeWordIndex] + result->iCodeWord[result->iCodeWordIndex + 1];
	if (ltemp < 1600)
		iCodeWordTrans[0] = 0;
	else
	{
		iCodeWordTrans[0] = (ltemp - ltemp % 1600) / 1600;
		ltemp = ltemp - iCodeWordTrans[0] * 1600;
		if (ltemp == 0)
		{
			iCodeWordTrans[0] = iCodeWordTrans[0] - 1;
		}
	}
	if (ltemp < 40)
		iCodeWordTrans[1] = 0;
	else
	{
		iCodeWordTrans[1] = (ltemp - ltemp % 40) / 40;
		ltemp = ltemp - iCodeWordTrans[1] * 40;
		if (ltemp == 0)
		{
			iCodeWordTrans[1] = iCodeWordTrans[1] - 1;
		}
	}
	if (ltemp == 1)
	{
		iCodeWordTrans[2] = 0;
	}
	if (ltemp == 0)
	{
		iCodeWordTrans[2] = 39;
	}
	else
	{
		iCodeWordTrans[2] = ltemp - 1;
		ltemp = 0;
	}
	for (i = 0; i < 3; i++)
	{
		switch (_sExtModePara->_iText_Current_Set)
		{
		case TEXT_BASIC_SET:
			if (iCodeWordTrans[i] == 254)
			{
				_sExtModePara->_iGoingUnlath = 1;
			}
			else if (iCodeWordTrans[i] >= TEXT_VALUE_RANGE_UPBOUND || iCodeWordTrans[i] < 0)
				return 0;
			tempIntValue = Text_Basic_Set[iCodeWordTrans[i]];
			if (tempIntValue == TEXT_SHIFT_1)
			{
				_sExtModePara->_iText_Current_Set = TEXT_SHIFT_1_SET;
				_sExtModePara->_iText_ShiftCount = 1;
				//_sExtModePara->_iText_Last_Set = TEXT_BASIC_SET;
				continue;
			}
			else if (tempIntValue == TEXT_SHIFT_2)
			{
				_sExtModePara->_iText_Current_Set = TEXT_SHIFT_2_SET;
				_sExtModePara->_iText_ShiftCount = 1;
				//_sExtModePara->_iText_Last_Set = TEXT_BASIC_SET;
				continue;
			}
			else if (tempIntValue == TEXT_SHIFT_3)
			{
				_sExtModePara->_iText_Current_Set = TEXT_SHIFT_3_SET;
				_sExtModePara->_iText_ShiftCount = 1;
				//_sExtModePara->_iText_Last_Set = TEXT_BASIC_SET;
				continue;
			}
			else
			{
				if (_sExtModePara->_iText_Last_Set == TEXT_SHIFT_2_SET && _sExtModePara->_iText_Last_Shift == TEXT_UPPER_SHIFT)
				{
					tempCharValue = (char)(tempIntValue + 128);
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
					//_sExtModePara->_iText_Current_Set = TEXT_BASIC_SET;
					//_sExtModePara->_iText_ShiftCount = 0;
					_sExtModePara->_iText_Last_Set = TEXT_BASIC_SET;
					continue;
				}
				else
				{
					tempCharValue = (char)tempIntValue;
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					//printf("%c",result->wcContent[result->iContentIndex]);
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
				}
			}
			break;
		case TEXT_SHIFT_1_SET:
			if (iCodeWordTrans[i] >= TEXT_VALUE_RANGE_UPBOUND || iCodeWordTrans[i] < 0)
				return 0;
			if (iCodeWordTrans[i] == 254)
			{
				_sExtModePara->_iGoingUnlath = 1;
			}
			tempIntValue = Text_Shift_1_Set[iCodeWordTrans[i]];
			if (tempIntValue == TEXT_NO_DEFINE)
			{
				_sExtModePara->_iText_Current_Set = TEXT_BASIC_SET;
				_sExtModePara->_iText_ShiftCount = 0;
				_sExtModePara->_iText_Last_Set = TEXT_SHIFT_1_SET;
				continue;
			}
			else
			{
				if (_sExtModePara->_iText_Last_Set == TEXT_SHIFT_2_SET && _sExtModePara->_iText_Last_Shift == TEXT_UPPER_SHIFT)
				{
					tempCharValue = (char)(tempIntValue + 128);
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
					_sExtModePara->_iText_Current_Set = TEXT_BASIC_SET;
					_sExtModePara->_iText_ShiftCount = 0;
					_sExtModePara->_iText_Last_Set = TEXT_SHIFT_1_SET;
					continue;
				}
				else
				{
					tempCharValue = (char)tempIntValue;
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
					_sExtModePara->_iText_Current_Set = TEXT_BASIC_SET;
					_sExtModePara->_iText_ShiftCount = 0;
					_sExtModePara->_iText_Last_Set = TEXT_SHIFT_1_SET;
				}
			}
			break;
		case TEXT_SHIFT_2_SET:
			if (iCodeWordTrans[i] >= TEXT_VALUE_RANGE_UPBOUND || iCodeWordTrans[i] < 0)
				return 0;
			if (iCodeWordTrans[i] == 254)
			{
				_sExtModePara->_iGoingUnlath = 1;
			}
			tempIntValue = Text_Shift_2_Set[iCodeWordTrans[i]];
			if (tempIntValue == TEXT_NO_DEFINE)
			{
				_sExtModePara->_iText_Current_Set = TEXT_BASIC_SET;
				_sExtModePara->_iText_ShiftCount = 0;
				_sExtModePara->_iText_Last_Set = TEXT_SHIFT_2_SET;
				continue;
			}
			else if (tempIntValue == TEXT_UPPER_SHIFT)
			{
				_sExtModePara->_iText_Current_Set = TEXT_BASIC_SET;
				_sExtModePara->_iText_ShiftCount = 0;
				_sExtModePara->_iText_Last_Set = TEXT_SHIFT_2_SET;
				_sExtModePara->_iText_Last_Shift = TEXT_UPPER_SHIFT;
				continue;
			}
			else if (tempIntValue == TEXT_FNC_1)
			{
				_sExtModePara->_iText_Current_Set = TEXT_BASIC_SET;
				_sExtModePara->_iText_ShiftCount = 0;
				_sExtModePara->_iText_Last_Set = TEXT_SHIFT_2_SET;
				result->iCodeWord[result->iCodeWordIndex - 1] = 29;
				tempCharValue = (char)(result->iCodeWord[result->iCodeWordIndex - 1]);
				//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
				resultout[result->iContentIndex] = tempCharValue;
				result->iContentIndex++;
				continue;
			}
			else
			{
				if (_sExtModePara->_iText_Last_Set == TEXT_SHIFT_2_SET && _sExtModePara->_iText_Last_Shift == TEXT_UPPER_SHIFT)
				{
					tempCharValue = (char)(tempIntValue + 128);
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
					_sExtModePara->_iText_Current_Set = TEXT_BASIC_SET;
					_sExtModePara->_iText_ShiftCount = 0;
					_sExtModePara->_iText_Last_Set = TEXT_SHIFT_3_SET;
					continue;
				}
				else
				{
					tempCharValue = (char)tempIntValue;
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
					_sExtModePara->_iText_Current_Set = TEXT_BASIC_SET;
					_sExtModePara->_iText_ShiftCount = 0;
					_sExtModePara->_iText_Last_Set = TEXT_SHIFT_2_SET;
					continue;
				}
			}
			break;
		case TEXT_SHIFT_3_SET:
			if (iCodeWordTrans[i] == 254)
			{
				_sExtModePara->_iGoingUnlath = 1;
			}
			else if (iCodeWordTrans[i] >= TEXT_VALUE_RANGE_UPBOUND || iCodeWordTrans[i] < 0)
				return 0;
			tempIntValue = Text_Shift_3_Set[iCodeWordTrans[i]];
			if (tempIntValue == TEXT_NO_DEFINE)
			{
				_sExtModePara->_iText_Current_Set = TEXT_BASIC_SET;
				_sExtModePara->_iText_ShiftCount = 0;
				_sExtModePara->_iText_Last_Set = TEXT_SHIFT_3_SET;
				continue;
			}
			else
			{
				if (_sExtModePara->_iText_Last_Set == TEXT_SHIFT_2_SET && _sExtModePara->_iText_Last_Shift == TEXT_UPPER_SHIFT)
				{
					tempCharValue = (char)(tempIntValue + 128);
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
					_sExtModePara->_iText_Current_Set = TEXT_BASIC_SET;
					_sExtModePara->_iText_ShiftCount = 0;
					_sExtModePara->_iText_Last_Set = TEXT_SHIFT_3_SET;
					continue;
				}
				else
				{
					tempCharValue = (char)tempIntValue;
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
					_sExtModePara->_iText_Current_Set = TEXT_BASIC_SET;
					_sExtModePara->_iText_ShiftCount = 0;
					_sExtModePara->_iText_Last_Set = TEXT_SHIFT_3_SET;
					continue;
				}
			}
			break;
		default:
			break;
		}
	}
	result->iCodeWordIndex++;
	return 1;
}

//EDIFACT
int GetEDIFACT(s_DataMatrix_Result *result, s_ExtendedMode_Struct *_sExtModePara, char *resultout)
{
	int tempIntValue, i;
	char tempCharValue;
	int iEDIFACT_Value[12];
	int iTrans[4];
	//wchar_t wc_temp_Value_1;
	tempCharValue = '\0';
	//wc_temp_Value_1 = 0;
	//result->wcContent[200];
	//#pragma region Trans
	//
	for (i = 0; i < 3; i++)
	{
		tempIntValue = result->iCodeWord[result->iCodeWordIndex + i];
		if (tempIntValue < 64)
		{
			iEDIFACT_Value[i * 4 + 0] = 0;
		}
		else
		{
			iEDIFACT_Value[i * 4 + 0] = (tempIntValue - tempIntValue % 64) / 64;
			tempIntValue = tempIntValue - iEDIFACT_Value[i * 4 + 0] * 64;
		}
		if (tempIntValue < 16)
		{
			iEDIFACT_Value[i * 4 + 1] = 0;
		}
		else
		{
			iEDIFACT_Value[i * 4 + 1] = (tempIntValue - tempIntValue % 16) / 16;
			tempIntValue = tempIntValue - iEDIFACT_Value[i * 4 + 1] * 16;
		}
		if (tempIntValue < 4)
		{
			iEDIFACT_Value[i * 4 + 2] = 0;
		}
		else
		{
			iEDIFACT_Value[i * 4 + 2] = (tempIntValue - tempIntValue % 4) / 4;
			tempIntValue = tempIntValue - iEDIFACT_Value[i * 4 + 2] * 4;
		}
		if (tempIntValue < 1)
		{
			iEDIFACT_Value[i * 4 + 3] = 0;
		}
		else
		{
			iEDIFACT_Value[i * 4 + 3] = tempIntValue;
		}
		tempIntValue = 0;
	}

	for (i = 0; i < 4; i++)
	{
		tempIntValue = 0;
		tempIntValue += 16 * iEDIFACT_Value[i * 3 + 0];
		tempIntValue += 4 * iEDIFACT_Value[i * 3 + 1];
		tempIntValue += 1 * iEDIFACT_Value[i * 3 + 2];
		iTrans[i] = tempIntValue;
	}
	//
	result->iCodeWordIndex++;

	int i_tag;
	for (i = 0; i < 4; i++)
	{
		if (_sExtModePara->_iCurrent_Encoding_Mode != EDIFACT)
		{
			return 0;
		}
		//if((result->iCodeWord[result->iCodeWordIndex]==240))
		//{
		//	return 0;
		//}

		tempIntValue = (EDIFACT_Set[iTrans[i]]);
		if (tempIntValue != EDIFACT_UNLATCH)
		{
			tempCharValue = (char)tempIntValue;
		}
		else
		{
			_sExtModePara->_iGoingUnlath = 1;
			break;
		}
		//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
		resultout[result->iContentIndex] = tempCharValue;
		result->iContentIndex++;
		//free(result->wcContent);
		//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));

		/*
		if((result->iCodeWord[result->iCodeWordIndex]==240))
		{
			return 0;
		}*/
		i_tag = i;
	}

	if (_sExtModePara->_iGoingUnlath)
	{
		_sExtModePara->_iCurrent_Encoding_Mode = ASCII;
		_sExtModePara->_iLast_Encoding_Mode = EDIFACT;
		_sExtModePara->_iGoingUnlath = 0;
		if (i_tag == 0)
			result->iCodeWordIndex = result->iCodeWordIndex - 2;
	}
	result->iCodeWordIndex++;
	return 1;
}

// C40
int GetC40(s_DataMatrix_Result *result, s_ExtendedMode_Struct *_sExtModePara, char *resultout)
{
	int i, tempIntValue;
	char tempCharValue;
	//wchar_t wc_temp_Value_1;
	int iCodeWordTrans[3];
	long ltemp;
	tempCharValue = '\0';
	//wc_temp_Value_1 = 0;
	//result->wcContent[200];
	ltemp = 256 * result->iCodeWord[result->iCodeWordIndex] + result->iCodeWord[result->iCodeWordIndex + 1];
	if (ltemp < 1600)
		iCodeWordTrans[0] = 0;
	else
	{
		iCodeWordTrans[0] = (ltemp - ltemp % 1600) / 1600;
		ltemp = ltemp - iCodeWordTrans[0] * 1600;
		if (ltemp == 0)
		{
			iCodeWordTrans[0] = iCodeWordTrans[0] - 1;
			ltemp = 1600;
		}
	}
	if (ltemp < 40)
		iCodeWordTrans[1] = 0;
	else
	{
		iCodeWordTrans[1] = (ltemp - ltemp % 40) / 40;
		ltemp = ltemp - iCodeWordTrans[1] * 40;
		if (ltemp == 0)
		{
			iCodeWordTrans[1] = iCodeWordTrans[1] - 1;
			ltemp = 40;
		}
	}
	iCodeWordTrans[2] = ltemp - 1;

	for (i = 0; i < 3; i++)
	{
		switch (_sExtModePara->_iC40_Current_Set)
		{
		case C40_BASIC_SET:
			if (iCodeWordTrans[i] == 254)
			{
				_sExtModePara->_iGoingUnlath = 1;
			}
			else if (iCodeWordTrans[i] >= C40_VALUE_RANGE_UPBOUND || iCodeWordTrans[i] < 0)
				return 0;
			tempIntValue = C40_Basic_Set[iCodeWordTrans[i]];
			if (tempIntValue == C40_SHIFT_1)
			{
				_sExtModePara->_iC40_Current_Set = C40_SHIFT_1_SET;
				_sExtModePara->_iC40_ShiftCount = 1;
				//_sExtModePara->_iC40_Last_Set = C40_BASIC_SET;
				continue;
			}
			else if (tempIntValue == C40_SHIFT_2)
			{
				_sExtModePara->_iC40_Current_Set = C40_SHIFT_2_SET;
				_sExtModePara->_iC40_ShiftCount = 1;
				//_sExtModePara->_iC40_Last_Set = C40_BASIC_SET;
				continue;
			}
			else if (tempIntValue == C40_SHIFT_3)
			{
				_sExtModePara->_iC40_Current_Set = C40_SHIFT_3_SET;
				_sExtModePara->_iC40_ShiftCount = 1;
				//_sExtModePara->_iC40_Last_Set = C40_BASIC_SET;
				continue;
			}
			else
			{
				if (_sExtModePara->_iC40_Last_Set == C40_SHIFT_2_SET && _sExtModePara->_iC40_Last_Shift == C40_UPPER_SHIFT)
				{
					tempCharValue = (char)(tempIntValue + 128);
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
					//_sExtModePara->_iC40_ShiftCount =0;
					//_sExtModePara->_iC40_Current_Set = C40_BASIC_SET;
					_sExtModePara->_iC40_Last_Set = C40_BASIC_SET;
				}
				else
				{
					tempCharValue = (char)tempIntValue;
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
				}
			}
			break;
		case C40_SHIFT_1_SET:
			if (iCodeWordTrans[i] == 254)
			{
				_sExtModePara->_iGoingUnlath = 1;
			}
			else if (iCodeWordTrans[i] >= C40_VALUE_RANGE_UPBOUND || iCodeWordTrans[i] < 0)
				return 0;
			if (iCodeWordTrans[i] == 254)
			{
				_sExtModePara->_iGoingUnlath = 1;
			}
			tempIntValue = C40_Shift_1_Set[iCodeWordTrans[i]];
			if (tempIntValue == C40_NO_DEFINE)
			{
				_sExtModePara->_iC40_ShiftCount = 0;
				_sExtModePara->_iC40_Current_Set = C40_BASIC_SET;
				_sExtModePara->_iC40_Last_Set = C40_SHIFT_1_SET;
				continue;
			}
			else
			{
				if (_sExtModePara->_iC40_Last_Set == C40_SHIFT_2_SET && _sExtModePara->_iC40_Last_Shift == C40_UPPER_SHIFT)
				{
					tempCharValue = (char)(tempIntValue + 128);
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
					_sExtModePara->_iC40_ShiftCount = 0;
					_sExtModePara->_iC40_Current_Set = C40_BASIC_SET;
					_sExtModePara->_iC40_Last_Set = C40_SHIFT_1_SET;
				}
				else
				{
					tempCharValue = (char)tempIntValue;
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
					_sExtModePara->_iC40_ShiftCount = 0;
					_sExtModePara->_iC40_Current_Set = C40_BASIC_SET;
					_sExtModePara->_iC40_Last_Set = C40_SHIFT_1_SET;
				}
			}
			break;
		case C40_SHIFT_2_SET:
			if (iCodeWordTrans[i] == 254)
			{
				_sExtModePara->_iGoingUnlath = 1;
			}
			else if (iCodeWordTrans[i] >= C40_VALUE_RANGE_UPBOUND || iCodeWordTrans[i] < 0)
				return 0;
			if (iCodeWordTrans[i] == 254)
			{
				_sExtModePara->_iGoingUnlath = 1;
			}
			tempIntValue = C40_Shift_2_Set[iCodeWordTrans[i]];
			if (tempIntValue == C40_NO_DEFINE)
			{
				_sExtModePara->_iC40_ShiftCount = 0;
				_sExtModePara->_iC40_Current_Set = C40_BASIC_SET;
				_sExtModePara->_iC40_Last_Set = C40_SHIFT_2_SET;
				continue;
			}
			else if (tempIntValue == C40_UPPER_SHIFT)
			{
				_sExtModePara->_iC40_ShiftCount = 0;
				_sExtModePara->_iC40_Current_Set = C40_BASIC_SET;
				_sExtModePara->_iC40_Last_Set = C40_SHIFT_2_SET;
				_sExtModePara->_iC40_Last_Shift = C40_UPPER_SHIFT;
				continue;
			}
			else if (tempIntValue == C40_FNC_1)
			{
				_sExtModePara->_iC40_ShiftCount = 0;
				_sExtModePara->_iC40_Current_Set = C40_BASIC_SET;
				_sExtModePara->_iC40_Last_Set = C40_SHIFT_2_SET;
				result->iCodeWord[result->iCodeWordIndex - 1] = 29;
				tempCharValue = (char)(result->iCodeWord[result->iCodeWordIndex - 1]);
				//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
				resultout[result->iContentIndex] = tempCharValue;
				result->iContentIndex++;
				continue;
			}
			else
			{
				if (_sExtModePara->_iC40_Last_Set == C40_SHIFT_2_SET && _sExtModePara->_iC40_Last_Shift == C40_UPPER_SHIFT)
				{
					tempCharValue = (char)(tempIntValue + 128);
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
					_sExtModePara->_iC40_ShiftCount = 0;
					_sExtModePara->_iC40_Current_Set = C40_BASIC_SET;
					_sExtModePara->_iC40_Last_Set = C40_SHIFT_2_SET;
				}
				else
				{
					tempCharValue = (char)tempIntValue;
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
					_sExtModePara->_iC40_ShiftCount = 0;
					_sExtModePara->_iC40_Current_Set = C40_BASIC_SET;
					_sExtModePara->_iC40_Last_Set = C40_SHIFT_2_SET;
				}
			}
			break;
		case C40_SHIFT_3_SET:
			if (iCodeWordTrans[i] == 254)
			{
				_sExtModePara->_iGoingUnlath = 1;
			}
			else if (iCodeWordTrans[i] >= C40_VALUE_RANGE_UPBOUND || iCodeWordTrans[i] < 0)
				return 0;
			if (iCodeWordTrans[i] == 254)
			{
				_sExtModePara->_iGoingUnlath = 1;
			}
			tempIntValue = C40_Shift_3_Set[iCodeWordTrans[i]];
			if (tempIntValue == C40_NO_DEFINE)
			{
				_sExtModePara->_iC40_ShiftCount = 0;
				_sExtModePara->_iC40_Current_Set = C40_BASIC_SET;
				_sExtModePara->_iC40_Last_Set = C40_SHIFT_3_SET;
				continue;
			}
			else
			{
				if (_sExtModePara->_iC40_Last_Set == C40_SHIFT_2_SET && _sExtModePara->_iC40_Last_Shift == C40_UPPER_SHIFT)
				{
					tempCharValue = (char)(tempIntValue + 128);
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
					_sExtModePara->_iC40_ShiftCount = 0;
					_sExtModePara->_iC40_Current_Set = C40_BASIC_SET;
					_sExtModePara->_iC40_Last_Set = C40_SHIFT_3_SET;
				}
				else
				{
					tempCharValue = (char)tempIntValue;
					//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
					resultout[result->iContentIndex] = tempCharValue;
					result->iContentIndex++;
					//free(result->wcContent);
					//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
					_sExtModePara->_iC40_ShiftCount = 0;
					_sExtModePara->_iC40_Current_Set = C40_BASIC_SET;
					_sExtModePara->_iC40_Last_Set = C40_SHIFT_3_SET;
				}
			}
			break;
		default:
			break;
		}
	}
	result->iCodeWordIndex++;
	return 1;
}

//Base256
int GetBase256(s_DataMatrix_Result *result, s_ExtendedMode_Struct *_sExtModePara, char *resultout)
{
	long _i_FieldLength, _iRandom;
	int i, _iPosition;
	int _iTempCodeWord_1, _iTempCodeWord_2, _iTempCodeWord_3, _iTempCodeWord_4;
	char tempCharValue;
//	wchar_t wc_temp_Value_1;
//result->wcContent [200];

	_iTempCodeWord_1 = result->iCodeWord[result->iCodeWordIndex];
	_iPosition = result->iCodeWordIndex + 1;
	_iRandom = (_iPosition * 149) % 255 + 1;
	//fix ECI bug
	int length_message = result->iCodeWordDataLength - result->iCodeWordIndex - 1;
	if ((_iTempCodeWord_1 - _iRandom) <= 0)
	{
		_iTempCodeWord_2 = 256 + _iTempCodeWord_1 - _iRandom;
	}
	else
	{
		_iTempCodeWord_2 = _iTempCodeWord_1 - _iRandom;
	}
	if (_iTempCodeWord_2 <= 249)
	{

		if (_iTempCodeWord_2 < 0)
			return 0;
		_i_FieldLength = _iTempCodeWord_2;
	}
	else
	{
		_iTempCodeWord_3 = result->iCodeWord[result->iCodeWordIndex + 1];
		_iPosition = result->iCodeWordIndex + 2;
		_iRandom = (_iPosition * 149) % 255 + 1;
		if ((_iTempCodeWord_3 - _iRandom) <= 0)
		{
			_iTempCodeWord_4 = 256 + _iTempCodeWord_3 - _iRandom;
		}
		else
		{
			_iTempCodeWord_4 = _iTempCodeWord_3 - _iRandom;
		}
		_i_FieldLength = (_iTempCodeWord_2 - 249) * 250 + _iTempCodeWord_4;
		//_i_FieldLength = _i_FieldLength - 63;
		result->iCodeWordIndex++;
	}
	result->iCodeWordIndex++;

	//fix ECI bug
	if (_i_FieldLength != (length_message - 7) && length_message < 249)
	{
		result->iCodeWordIndex--;
		_i_FieldLength = length_message;
	}

	for (i = 0; i < _i_FieldLength; i++)
	{
		_iTempCodeWord_1 = result->iCodeWord[result->iCodeWordIndex];
		_iPosition = result->iCodeWordIndex + 1;
		_iRandom = (_iPosition * 149) % 255 + 1;
		if ((_iTempCodeWord_1 - _iRandom) <= 0)
		{
			_iTempCodeWord_2 = 256 + _iTempCodeWord_1 - _iRandom;
		}
		else
		{
			_iTempCodeWord_2 = _iTempCodeWord_1 - _iRandom;
		}
		tempCharValue = (char)_iTempCodeWord_2;
		//mbstowcs(&wc_temp_Value_1,&tempCharValue,1);
		resultout[result->iContentIndex] = tempCharValue;
		result->iContentIndex++;
		//free(result->wcContent);
		//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
		result->iCodeWordIndex++;
	}

	result->iCodeWordIndex--;
	_sExtModePara->_iCurrent_Encoding_Mode = ASCII;
	_sExtModePara->_iLast_Encoding_Mode = Base256;
	return 1;
}

//X12
int GetX12(s_DataMatrix_Result *result, s_ExtendedMode_Struct *_sExtModePara, char *resultout)
{
	//int tempIntValue;
	char tempCharValue;
	//wchar_t wc_temp_Value_1;
	tempCharValue = '\0';
	//wc_temp_Value_1 = 0;
	int i, tempIntValue;
	//char tempCharValue;
	//wchar_t wc_temp_Value_1;
	int iCodeWordTrans[3];
	long ltemp;
	//tempCharValue = '\0';
	//wc_temp_Value_1 = 0;
	//result->wcContent [200];
	ltemp = 256 * result->iCodeWord[result->iCodeWordIndex] + result->iCodeWord[result->iCodeWordIndex + 1];
	//printf("\n%d,%d",result->iCodeWord[result->iCodeWordIndex],result->iCodeWord[result->iCodeWordIndex+1]);

	if (_sExtModePara->_iCurrent_Encoding_Mode != X12)
	{
		return 0;
	}
	for (i = 0; i < 3; i++)
	{
		iCodeWordTrans[i] = 0;
	}
	if (ltemp < 1600)
		iCodeWordTrans[0] = 0;
	else
	{
		iCodeWordTrans[0] = (ltemp - ltemp % 1600) / 1600;

		ltemp = ltemp - iCodeWordTrans[0] * 1600;
		if (ltemp == 0)
		{
			iCodeWordTrans[0] = iCodeWordTrans[0] - 1;
		}
	}
	if (ltemp < 40)
		iCodeWordTrans[1] = 0;
	else
	{
		iCodeWordTrans[1] = (ltemp - ltemp % 40) / 40;

		ltemp = ltemp - iCodeWordTrans[1] * 40;
		if (ltemp == 0)
		{
			iCodeWordTrans[1] = iCodeWordTrans[1] - 1;
		}
	}
	if (ltemp <= 1)
		iCodeWordTrans[2] = 0;
	else
	{
		iCodeWordTrans[2] = ltemp - 1;
		ltemp = 0;
	}

	for (i = 0; i < 3; i++)
	{
		//printf("%d,",result->iCodeWord[result->iCodeWordIndex]);
		if (iCodeWordTrans[i] > 63 || iCodeWordTrans[i] <= 0)
		{
			return 0;
		}
		if (result->iCodeWord[result->iCodeWordIndex] == 254)
		{
			_sExtModePara->_iCurrent_Encoding_Mode = _sExtModePara->_iLast_Encoding_Mode;
			result->iCodeWordIndex++;
			return 1;
		}

		tempIntValue = (X12_To_ASCII_Table[iCodeWordTrans[i]]);

		if (tempIntValue != EDIFACT_UNLATCH)
		{
			tempCharValue = (char)tempIntValue;
			//printf("\n%c,",tempCharValue);
		}

		//mbtowc(&wc_temp_Value_1,&tempCharValue,1);
		resultout[result->iContentIndex] = tempCharValue;
		result->iContentIndex++;
		//free(result->wcContent);
		//result->wcContent = (char*)malloc(sizeof(char)*(result->iContentIndex+1));
	}
	result->iCodeWordIndex++;
	return 1;
}