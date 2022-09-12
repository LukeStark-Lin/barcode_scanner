#ifndef H_DATA_MATRIX_EXTENDED_MODE
#define H_DATA_MATRIX_EXTENDED_MODE

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

/*編碼格式*/
#define ASCII				0	 
#define C40					230
#define Text				239
#define EDIFACT				240
#define Base256				231
#define X12					238
#define ECI					241
#define Structured_Append	233
#define FNC1				232
#define Reader_Programming	234
#define Upper_Shift			235
#define _05_Macro			236
#define _06_Macro			237

//C40 
#define C40_SHIFT_1 300
#define C40_SHIFT_2 301
#define C40_SHIFT_3 302
#define C40_FNC_1 303
#define C40_UPPER_SHIFT 303
#define C40_NO_DEFINE 305

#define C40_BASIC_SET 0
#define C40_SHIFT_1_SET 1
#define C40_SHIFT_2_SET 2
#define C40_SHIFT_3_SET 3

#define C40_VALUE_RANGE_UPBOUND 40

//EDIFACT
#define EDIFACT_UNLATCH 306
#define EDIFACT_NO_DEFINE 307

#define EDIFACT_VALUE_RANGE_UPBOUND 64

//Text
#define TEXT_SHIFT_1 308
#define TEXT_SHIFT_2 309
#define TEXT_SHIFT_3 310
#define TEXT_FNC_1 311
#define TEXT_UPPER_SHIFT 312
#define TEXT_NO_DEFINE 314

#define TEXT_BASIC_SET 0
#define TEXT_SHIFT_1_SET 1
#define TEXT_SHIFT_2_SET 2
#define TEXT_SHIFT_3_SET 3

#define TEXT_VALUE_RANGE_UPBOUND 40
#define MAX_CODEWORD 2178 // Maximum codeword capacity = 132*132/8 = 2178

//X12
#define X12_VALUE_RANGE_UPBOUND 40

#pragma region Result_Structer
//解的結構
typedef struct
{
	//char *wcContent;		// CodeWord 轉出來的結果(寬字元結果)
	int iContentIndex;		// CodeWord 轉出來的結果_陣列索引值
	int iCodeWordDataLength;// CodeWord 實際上有資料非Error Correction 部份的長度
	int iCodeWordIndex;		// CodeWord 陣列索引值
	int iCodeWordLength;	// CodeWord 的數目量
//	int *iUsingCodeSet;		// 所使用的字符集
	int iCodeWord[MAX_CODEWORD];	// CodeWord內容 (int陣列)
//	char *SymbolSize;		// Bit Stream Map Module的格式 (ex: 10x10 20x20 ... ...) 
}s_DataMatrix_Result;

#pragma endregion


#pragma region ExtendedMode_Struct

typedef struct
{
	//Text
	int _iText_Current_Set;
	int _iText_ShiftCount;
	int _iText_Last_Set;
	int _iText_Last_Shift;

	//C40
	int _iC40_Current_Set;
	int _iC40_ShiftCount;
	int _iC40_Last_Set;
	int _iC40_Last_Shift;
	//Encoding mode
	int _iCurrent_Encoding_Mode ; 
	//Last Encoding mode
	int _iLast_Encoding_Mode;
	//Ready to Unlatch
	int _iGoingUnlath;

}s_ExtendedMode_Struct;

#pragma endregion


//Text
int GetText(s_DataMatrix_Result *result,s_ExtendedMode_Struct *_sExtModePara,char *resultout);

//EDIFACT
int GetEDIFACT(s_DataMatrix_Result *result,s_ExtendedMode_Struct *_sExtModePara,char *resultout);

// C40
int GetC40(s_DataMatrix_Result *result,s_ExtendedMode_Struct *_sExtModePara,char *resultout);

//Base256
int GetBase256(s_DataMatrix_Result *result,s_ExtendedMode_Struct *_sExtModePara,char *resultout);

//X12
int GetX12(s_DataMatrix_Result *result,s_ExtendedMode_Struct *_sExtModePara,char *resultout);
#endif