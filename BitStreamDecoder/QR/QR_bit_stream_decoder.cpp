#include <string.h>
#include "../Reed_solomon/head/reed_solomon.h"
#include "./head/QRdecode.h"

int TransArray(unsigned char *rx,int MRT_V,int MRT_R,int DataCount ,int ErrorCount,unsigned char QRStructTable[40][32]);
unsigned char *RS2binary(unsigned char *rx,int Datacount);
unsigned char *GetFormatInformation(unsigned char datamap[177][177],int XORmask[],unsigned char QRFormatTable[32][15],int ModulesNum);

char *NumericMode(unsigned char DataArray[],int CharacterCountNum,int CharacterCount ,char NumericTable[],int BadPT,int ModeIndicatorLength);
char *AlphanumericMode(unsigned char DataArray[],int CharacterCountNum,int CharacterCount ,char AlphanumericTable[],int BadPT,int ModeIndicatorLength);              
char *BabitModeTicket(unsigned char DataArray[],int CharacterCountNum,int CharacterCount,int BadPT,int ModeIndicatorLength);
char *KanjiMode(unsigned char DataArray[],int CharacterCountNum,int CharacterCount ,int BadPT,int ModeIndicatorLength);

void *GetCharacterCountIndicator(unsigned char DataArray[],int Version,int BadPT,int *IMnum,int *CharacterCount,int *CharacterCountNum);  
int MicroQRTransArray(unsigned char *rx,int MRT_V,int MRT_R,int DataCount ,int ErrorCount);                                                   

unsigned char *GetFormatInformationMQR(unsigned char datamap[177][177],int XORmask[]);                                                                                
void *GetCharacterCountIndicatorMQR(unsigned char DataArray[],int Version,int BadPT,int *IMnum,int *CharacterCount,int *CharacterCountNum,int *ModeIndicatorLength);


//ref 2005 QR spec..

	//FormatInformation XOR mask, ref. Annex C.2, QR and MircoQR is fixed bit stream, but position of LSB is inverse with SPE
    int XORmask[15]={0,1,0,0,1, 0,0,0,0,0, 1,0,1,0,1};

	//FormatInformation XOR遮罩 [MicroQR]
    int MQR_XORmask[15]={1,0,1,0,0, 0,1,0,0,0, 1,0,0,0,1};
    
	//字數統計符計算用
    int CharacterCountTemp[16]={0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,0};                        
    
	//Alphanumeric編碼表, spec. 6.4.4 table 5, 編排順序和ascii table 不同無法省略
    char AlphanumericTable[45]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
						        ' ','$','%','*','+','-','.','/',':'};  
    
	//Numeric編碼表, 目前省略掉, 直接加上 0x30輸出
    char NumericTable[10]={'0','1','2','3','4','5','6','7','8','9'};

    //各等級資料容量記錄表  Ver  L    M   Q   H
	//Ver:表示每個Version的total code word數目(data + error correct)
	//接下來的四個數字表示L~H的格式下, data 佔了多少code word(byte), 可參考table 7和table 9
    int datacapacity[200]={ 26 ,19 ,16 ,13,  9,
		                     44 ,34 ,28 ,22, 16,
                             70 ,55 ,44 ,34, 26,
						    100 ,80 ,64 ,48 ,36,
                            134,108, 86, 62 ,46,     //Version 5
						    172,136,108, 76 ,60,  
                            196,156,124, 88 ,66,
						    242,194,154, 110,86,
                            292,232,182, 132,100,
						    346,274,216, 154,122,    //Version 10
                            404,324,254, 180,140,
						    466,370,290, 206,158,
						    532,428,334, 244,180,
						    581,461,365, 261,197,
						    655,523,415, 295,223,     //Version 15
						    733,589,453, 325,253,     
						    815,647,507, 367,283,     
						    901,721,563, 397,313,     //0429修正
						    991,795,627, 445,341,     
						   1085,861,669, 485,385,     //Version 20
						   1156, 932, 714, 512,406,
						   1258,1006, 782, 568,442,
						   1364,1094, 860, 614,464,
						   1474,1174, 914, 664,514,
						   1588,1276,1000, 718,538,   //Version 25
						   1706,1370,1062, 754,596,
						   1828,1468,1128, 808,628,
						   1921,1531,1193, 871,661,
						   2051,1631,1267, 911,701,
						   2185,1735,1373, 985,745,   //30
						   2323,1843,1455,1033,793, 
						   2465,1955,1541,1115,845,
						   2611,2071,1631,1171,901,
						   2761,2191,1725,1231,961,
						   2876,2306,1812,1286,986,   //35
						   3034,2434,1914,1354,1054,
						   3196,2566,1992,1426,1096,
						   3362,2702,2102,1502,1142,
						   3532,2812,2216,1582,1222,
						   3706,2956,2334,1666,1276  //Version 40  
                       };   
        //版本區塊各等級對照表, 參考Annex D, table D.1, 用查表比對來取代BCH錯誤檢查
		unsigned char QRVersionTable[34][18]={{0,0, 0,1,1,1, 1,1,0,0, 1,0,0,1 ,0,1,0,0},
		                        {0,0, 1,0,0,0, 0,1,0,1, 1,0,1,1 ,1,1,0,0},
								{0,0, 1,0,0,1, 1,0,1,0, 1,0,0,1 ,1,0,0,1},
								{0,0, 1,0,1,0, 0,1,0,0, 1,1,0,1 ,0,0,1,1},
								{0,0, 1,0,1,1, 1,0,1,1, 1,1,1,1 ,0,1,1,0}, //Ver.11
								{0,0, 1,1,0,0, 0,1,1,1, 0,1,1,0 ,0,0,1,0},
								{0,0, 1,1,0,1, 1,0,0,0, 0,1,0,0 ,0,1,1,1},
								{0,0, 1,1,1,0, 0,1,1,0, 0,0,0,0 ,1,1,0,1},
								{0,0, 1,1,1,1, 1,0,0,1, 0,0,1,0 ,1,0,0,0},
								{0,1, 0,0,0,0, 1,0,1,1, 0,1,1,1 ,1,0,0,0},
								{0,1, 0,0,0,1, 0,1,0,0, 0,1,0,1 ,1,1,0,1},
								{0,1, 0,0,1,0, 1,0,1,0, 0,0,0,1 ,0,1,1,1}, //Ver.18
								{0,1, 0,0,1,1, 0,1,0,1, 0,0,1,1 ,0,0,1,0},
								{0,1, 0,1,0,0, 1,0,0,1, 1,0,1,0 ,0,1,1,0},
								{0,1, 0,1,0,1, 0,1,1,0, 1,0,0,0 ,0,0,1,1},
								{0,1, 0,1,1,0, 1,0,0,0, 1,1,0,0 ,1,0,0,1},
								{0,1, 0,1,1,1, 0,1,1,1, 1,1,1,0 ,1,1,0,0},
								{0,1, 1,0,0,0, 1,1,1,0, 1,1,0,0 ,0,1,0,0},
								{0,1, 1,0,0,1, 0,0,0,1, 1,1,1,0 ,0,0,0,1},
								{0,1, 1,0,1,0, 1,1,1,1, 1,0,1,0 ,1,0,1,1}, //ver.26
								{0,1, 1,0,1,1, 0,0,0,0, 1,0,0,0 ,1,1,1,0},
								{0,1, 1,1,0,0, 1,1,0,0, 0,0,0,1 ,1,0,1,0},
								{0,1, 1,1,0,1, 0,0,1,1, 0,0,1,1 ,1,1,1,1},
								{0,1, 1,1,1,0, 1,1,0,1, 0,1,1,1 ,0,1,0,1}, //ver.30
								{0,1, 1,1,1,1, 0,0,1,0, 0,1,0,1 ,0,0,0,0},
								{1,0, 0,0,0,0, 1,0,0,1, 1,1,0,1 ,0,1,0,1},
								{1,0, 0,0,0,1, 0,1,1,0, 1,1,1,1 ,0,0,0,0},
								{1,0, 0,0,1,0, 1,0,0,0, 1,0,1,1 ,1,0,1,0},
								{1,0, 0,0,1,1, 0,1,1,1, 1,0,0,1 ,1,1,1,1},
								{1,0, 0,1,0,0, 1,0,1,1, 0,0,0,0 ,1,0,1,1}, //36
								{1,0, 0,1,0,1, 0,1,0,0, 0,0,1,0 ,1,1,1,0},
								{1,0, 0,1,1,0, 1,0,1,0, 0,1,1,0 ,0,1,0,0},
								{1,0, 0,1,1,1, 0,1,0,1, 0,1,0,0 ,0,0,0,1},
								{1,0, 1,0,0,0, 1,1,0,0, 0,1,1,0 ,1,0,0,1}		                       
		};

	//Format的檢查表,參考Annex C, table C.1, 用查表比對來取代BCH錯誤檢查, 目前只有QR, 缺MircoQR的部分
	unsigned char QRFormatTable[32][15]={{1,0,1,0,1,0,0,0,0,0,1,0,0,1,0},
		                       {1,0,1,0,0,0,1,0,0,1,0,0,1,0,1},
		                       {1,0,1,1,1,1,0,0,1,1,1,1,1,0,0},
		                       {1,0,1,1,0,1,1,0,1,0,0,1,0,1,1},
		                       {1,0,0,0,1,0,1,1,1,1,1,1,0,0,1},
		                       {1,0,0,0,0,0,0,1,1,0,0,1,1,1,0},
							   {1,0,0,1,1,1,1,1,0,0,1,0,1,1,1},
							   {1,0,0,1,0,1,0,1,0,1,0,0,0,0,0},
							   {1,1,1,0,1,1,1,1,1,0,0,0,1,0,0},
							   {1,1,1,0,0,1,0,1,1,1,1,0,0,1,1},
							   {1,1,1,1,1,0,1,1,0,1,0,1,0,1,0},
							   {1,1,1,1,0,0,0,1,0,0,1,1,1,0,1},
							   {1,1,0,0,1,1,0,0,0,1,0,1,1,1,1},
							   {1,1,0,0,0,1,1,0,0,0,1,1,0,0,0},
							   {1,1,0,1,1,0,0,0,1,0,0,0,0,0,1},
							   {1,1,0,1,0,0,1,0,1,1,1,0,1,1,0},
							   {0,0,1,0,1,1,0,1,0,0,0,1,0,0,1},
							   {0,0,1,0,0,1,1,1,0,1,1,1,1,1,0},
							   {0,0,1,1,1,0,0,1,1,1,0,0,1,1,1},
							   {0,0,1,1,0,0,1,1,1,0,1,0,0,0,0},
							   {0,0,0,0,1,1,1,0,1,1,0,0,0,1,0},
							   {0,0,0,0,0,1,0,0,1,0,1,0,1,0,1},
							   {0,0,0,1,1,0,1,0,0,0,0,1,1,0,0},
							   {0,0,0,1,0,0,0,0,0,1,1,1,0,1,1},
							   {0,1,1,0,1,0,1,0,1,0,1,1,1,1,1},
							   {0,1,1,0,0,0,0,0,1,1,0,1,0,0,0},
							   {0,1,1,1,1,1,1,0,0,1,1,0,0,0,1},
							   {0,1,1,1,0,1,0,0,0,0,0,0,1,1,0},
							   {0,1,0,0,1,0,0,1,0,1,1,0,1,0,0},   //p.79
							   {0,1,0,0,0,0,1,1,0,0,0,0,0,1,1},
							   {0,1,0,1,1,1,0,1,1,0,1,1,0,1,0},
							   {0,1,0,1,0,1,1,1,1,1,0,1,1,0,1}
	};
 	unsigned char uQR_FormatTable[32][15]={
		{1,0,0,0,1,0,0,0,1,0,0,0,1,0,1},
		{1,0,0,0,0,0,1,0,1,1,1,0,0,1,0},
		{1,0,0,1,1,1,0,0,0,1,0,1,0,1,1},
		{1,0,0,1,0,1,1,0,0,0,1,1,1,0,0},
		{1,0,1,0,1,0,1,1,0,1,0,1,1,1,0},
		{1,0,1,0,0,0,0,1,0,0,1,1,0,0,1},
		{1,0,1,1,1,1,1,1,1,0,0,0,0,0,0},
		{1,0,1,1,0,1,0,1,1,1,1,0,1,1,1},
		{1,1,0,0,1,1,1,1,0,0,1,0,0,1,1},
		{1,1,0,0,0,1,0,1,0,1,0,0,1,0,0},
		{1,1,0,1,1,0,1,1,1,1,1,1,1,0,1},
		{1,1,0,1,0,0,0,1,1,0,0,1,0,1,0},
        {1,1,1,0,1,1,0,0,1,1,1,1,0,0,0},
		{1,1,1,0,0,1,1,0,1,0,0,1,1,1,1},
		{1,1,1,1,1,0,0,0,0,0,1,0,1,1,0},
		{1,1,1,1,0,0,1,0,0,1,0,0,0,0,1},
		{0,0,0,0,1,1,0,1,1,0,1,1,1,1,0},
		{0,0,0,0,0,1,1,1,1,1,0,1,0,0,1},
		{0,0,0,1,1,0,0,1,0,1,1,0,0,0,0},
		{0,0,0,1,0,0,1,1,0,0,0,0,1,1,1},
		{0,0,1,0,1,1,1,0,0,1,1,0,1,0,1},
		{0,0,1,0,0,1,0,0,0,0,0,0,0,1,0},
		{0,0,1,1,1,0,1,0,1,0,1,1,0,1,1},
		{0,0,1,1,0,0,0,0,1,1,0,1,1,0,0},
		{0,1,0,0,1,0,1,0,0,0,0,1,0,0,0},
		{0,1,0,0,0,0,0,0,0,1,1,1,1,1,1},
		{0,1,0,1,1,1,1,0,1,1,0,0,1,1,0},
		{0,1,0,1,0,1,0,0,1,0,1,0,0,0,1},
		{0,1,1,0,1,0,0,1,1,1,0,0,0,1,1},
		{0,1,1,0,0,0,1,1,1,0,1,0,1,0,0},
		{0,1,1,1,1,1,0,1,0,0,0,1,1,0,1},
		{0,1,1,1,0,1,1,1,0,1,1,1,0,1,0}
	};


	//RS的分組表格, 參考Table 9, 每個格式最多有兩組RS的樣式,
	//以ver 11,為例, L:4,101, 81,10,0,0,0,0  , 表示有4組 101 byte的RS碼要檢查, data 佔81 byte, error correct佔10 byte
    //M:1,80,50,15,4,81,51,15 ,表示有1組 80 byte(data:50 byte, EC:15 byte)和4組81 byte(data:51 byte, EC:15 byte)的RS碼要檢查, 總數量一樣是404 byte
    unsigned char QRStructTable[40][32]={{1, 26, 19, 2,0,0,0,0      ,1,26,16,4,0, 0, 0, 0  ,1,26,13, 6,0, 0, 0,0  ,  1,26,9,8,0,0,0,0},         //Version.1
                           {1, 44, 34,14,0,0,0,0      ,1,44,28,8,0, 0, 0, 0  ,1,44,22,11,0, 0, 0, 0 ,  1,44,16,14,0,0,0,0},
                           {1, 70, 55, 7,0,0,0,0      ,1,70,44,13,0, 0, 0, 0 ,2,35,17, 9,0, 0, 0, 0 ,  2,35,13,11,0,0,0,0},
						   {1,100, 80,10,0,0,0,0      ,2,50,32,9,0, 0, 0, 0  ,2,50,24,13,0, 0, 0, 0 ,  4,25,9,8,0,0,0,0},
						   {1,134,108,13,0,0,0,0      ,2,67,43,12, 0,0, 0, 0 ,2,33,15, 9,2,34,16, 9 ,  2,33,11,11,2,34,12,11},    //Version.5
						   {2,86 , 68,9,0,0,0, 0      ,4,43,27,8,0, 0, 0, 0  ,4,43,19,12,0,0,0, 0   ,  4,43,15,14,0,0,0,0   },
						   {2,98 , 78,10,0,0,0,0      ,4,49,31,9,0, 0, 0, 0  ,2,32,14,9,4,33,15, 9  ,  4,39,13,13,1,40,14,13},
						   {2,121, 97,12,0,0,0,0      ,2,60,38,11,2,61,39,11 ,4,40,18,11,2,41,19,11 ,  4,40,14,13,2,41,15,13},
						   {2,146,116,15,0,0,0,0      ,3,58,36,11,2,59,37,11 ,4,36,16,10,4,37,17,10 ,  4,36,12,12,4,37,13,12},
						   {2,86 , 68, 9,2,87,69,9    ,4,69,43,13,1,70,44,13 ,6,43,19,12,2,44,20,12 ,  6,43,15,14,2,44,16,14},    //Version.10
                           {4,101, 81,10,0,0,0,0      ,1,80,50,15,4,81,51,15 ,4,50,22,14,4,51,23,14 ,  3,36,12,12,8,37,13,12}, 
                           {2,116, 92,12,2,117,93,12  ,6,58,36,11,2,59,37,11 ,4,46,20,13,6,47,21,13 ,  7,42,14,14,4,43,15,14}, 
                           {4,133,107,13,0,0,0,0      ,8,59,37,11,1,60,38,11 ,8,44,20,12,4,45,21,12 , 12,33,11,11,4,34,12,11},    //Version.13

                           {3,145,115,15,1,146,116,15      , 4,64,40,12, 5,65,41,12   ,11,36,16,10, 5,37,17,10  , 11,36,12,12, 5,37,13,12}, 
						   {5,109, 87,11,1,110, 88,11      , 5,65,41,12, 5,66,42,12   , 5,54,24,15, 7,55,25,15  , 11,36,12,12, 7,37,13,12},     //Version.15 
						   {5,122, 98,12,1,123, 99,12      , 7,73,45,14, 3,74,46,14   ,15,43,19,12, 2,44,20,12  ,  3,45,15,15,13,46,16,15},
						   {1,135,107,14,5,136,108,14      ,10,74,46,14, 1,75,47,14   , 1,50,22,14,15,51,23,14  ,  2,42,14,14,17,43,15,14}, 
						   {5,150,120,15,1,151,121,15      , 9,69,43,13, 4,70,44,13   ,17,50,22,14, 1,51,23,14  ,  2,42,14,14,19,43,15,14}, 
						   {3,141,113,14,4,142,114,14      , 3,70,44,13,11,71,45,13   ,17,47,21,13, 4,48,22,13  ,  9,39,13,13,16,40,14,13}, 
						   {3,135,107,14,5,136,108,14      , 3,67,41,13,13,68,42,13   ,15,54,24,15, 5,55,25,15  , 15,43,15,14,10,44,16,14},     //Version.20

						   {4,144,116,14,4,145,117,14      ,17,68,42,13, 0, 0, 0, 0   ,17,50,22,14, 6,51,23,14  , 19,46,16,15, 6,47,17,15},     //Version.21
						   {2,139,111,14,7,140,112,14      ,17,74,46,14, 0, 0, 0, 0   , 7,54,24,15,16,55,25,15  , 34,37,13,12, 0, 0, 0, 0},    
						   {4,151,121,15,5,152,122,15      , 4,75,47,14,14,76,48,14   ,11,54,24,15,14,55,25,15  , 16,45,15,15,14,46,16,15},    
						   {6,147,117,15,4,148,118,15      , 6,73,45,14,14,74,46,14   ,11,54,24,15,16,55,25,15  , 30,46,16,15, 2,47,17,15},     
						   {8,132,106,13,4,133,107,13      , 8,75,47,14,13,76,48,14   , 7,54,24,15,22,55,25,15  , 22,45,15,15,13,46,16,15},     //Ver.25
						  {10,142,114,14,2,143,115,14      ,19,74,46,14, 4,75,47,14   ,28,50,22,14, 6,51,23,14  , 33,46,16,15, 4,47,17,15},     
						  { 8,152,122,15,4,153,123,15      ,22,73,45,14, 3,74,46,14   , 8,53,23,15,26,54,24,15  , 12,45,15,15,28,46,16,15},     //27
						  { 3,147,117,15,10,148,118,15     , 3,73,45,14,23,74,46,14   , 4,54,24,15,31,55,25,15  , 11,45,15,15,31,46,16,15},     
						  { 7,146,116,15, 7,147,117,15     ,21,73,45,14, 7,74,46,14   , 1,53,23,15,37,54,24,15  , 19,45,15,15,26,46,16,15},     
						  { 5,145,115,15,10,146,116,15     ,19,75,47,14,10,76,48,14   ,15,54,24,15,25,55,25,15  , 23,45,15,15,25,46,16,15},     //Ver.30

						  {13,145,115,15, 3,146,116,15     , 2,74,46,14,29,75,47,14   ,42,54,24,15, 1,55,25,15  , 23,45,15,15,28,46,16,15},     //31
						  {17,145,115,15, 0,  0,  0, 0     ,10,74,46,14,23,75,47,14   ,10,54,24,15,35,55,25,15  , 19,45,15,15,35,46,16,15},
						  {17,145,115,15, 1,146,116,15     ,14,74,46,14,21,75,47,14   ,29,54,24,15,19,55,25,15  , 11,45,15,15,46,46,16,15},     //33
						  {13,145,115,15, 6,146,116,15     ,14,74,46,14,23,75,47,14   ,44,54,24,15, 7,55,25,15  , 59,46,16,15, 1,47,17,15},
						  {12,151,121,15, 7,152,122,15     ,12,75,47,14,26,76,48,14   ,39,54,24,15,14,55,25,15  , 22,45,15,15,41,46,16,15},     //35
						  { 6,151,121,15,14,152,122,15     , 6,75,47,14,34,76,48,14   ,46,54,24,15,10,55,25,15  ,  2,45,15,15,64,46,16,15},     						  
						  {17,152,122,15, 4,153,123,15     ,29,74,46,14,14,75,47,14   ,49,54,24,15,10,55,25,15  , 24,45,15,15,46,46,16,15},     //37    
						  { 4,152,122,15,18,153,123,15     ,13,74,46,14,32,75,47,14   ,48,54,24,15,14,55,25,15  , 42,45,15,15,32,46,16,15},     
						  {20,147,117,15, 4,148,118,15     ,40,75,47,14, 7,76,48,14   ,43,54,24,15,22,55,25,15  , 10,45,15,15,67,46,16,15},     
						  {19,148,118,15, 6,149,119,15     ,18,75,47,14,31,76,48,14   ,34,54,24,15,34,55,25,15  , 20,45,15,15,61,46,16,15}      //40
    };  


//from decoder
//T4 : 29648 = ver40的module數扣掉AP, finder pattern, version, format等等所剩下的code word(data + EC)
unsigned char ErrorArray[T4];   //儲存錯誤更正碼[T4]
unsigned char DataArray[T4];
//from TransArray
//T2: 3706, 最大data code word的大小, 這幾個大型陣列有機會減少/重複使用
unsigned short Fout[T2];  //排序表用陣列
unsigned char MemberOut[T2];

//modify by Joseph@20131210, move r[T2] and Decodeword[T2] to global
unsigned char r[T2];  
unsigned char Decodeword[T2]; 

int CheckArray[160]; //add by Joseph@20131213, move RS checkArray to global

//旋轉flag, 缺少一個回字作二次解碼時用
int second_rotate_flag;
unsigned char qr_mask[177][177]={0};
unsigned char datamap[177][177]={0};
extern UINT PPswMode; //add by Joseph@20131205, counter for mode switch

//save correct RS result and re-try
unsigned char fail_frame_cnt=0,last_version = 0, last_ec=0,last_masknum=0; 
unsigned char group1_pass_cnt[70], group2_pass_cnt[70]; //record each RS result

unsigned char structured_total_cnt;
unsigned char structured_current_cnt;
unsigned char structured_parity;

extern unsigned char mode1_BW[73*73];
//extern SCANNERSETTING *pSetting;
#ifdef REAL_TIME
#define MAX_FRAME_CNT 7
#else 
#define MAX_FRAME_CNT 1
#endif

char* parse_bitstream(int start_pos,int Version,int ErrorLevel,int *QRENCODEmode);
void get_structured_append_info(unsigned char DataArray[],int ModeIndicatorLength,int BadPT);

//////////////////////////////////////////////////////////////////////////////
//QRcode解碼主程式
//輸入: rx(unsigned char):儲存QR條碼讀取結果的資料陣列
//      QRversion   (int):該張QR條碼的版本等級(Version)
//
//輸出: ans       (*char):回傳解碼結果
//
//說明:QRcode Decode的主要程式，包含 取出Codeword ， ReedSolomon檢查 與 資料解碼 等三大步驟

//////////////////////////////////////////////////////////////////////////////
char *QR_bit_stream_decoder(unsigned char *rx,int ModulesNum,int *QRENCODEmode)  //輸入:讀取結果
{  
	int sum=0;

	//[0425備註] T2,T4 是否能改為使用時再分配 ,方法再修改
	//unsigned char ErrorArray[T4];   //儲存錯誤更正碼[T4]
	//unsigned char r[T2];    //mark by Joseph@20131210
	unsigned char *DataCorrect = NULL;
	int RSanswer=0;
	//int DataArray[T4];
    memset(DataArray, 0, T4);
	memset(ErrorArray, 0, T4);

    memset(r, 0, T2);   //add by Joseph@20131210, reset r[] and Decodeword[]
    //memset(Decodeword, 0, T2);
   
	char *tempans=NULL;              //回傳片斷解碼結果
    char *ans=NULL;                  //回傳最終解碼結果
    //char Decodeword[T2] = {""};      //Version 10最高支援到652個codeword
    int EncodeMode =0;               //檢查用旗標: [1]Numberic, [2]AlphaNum, [3]8bitBinary ,[0]ErrorWord

    
    //計算Version等級，最小21 每級相差4, MicroQR:Version 41~44
    int Version=0;
	if(ModulesNum==21)                                                    
		Version=1;                                 
	else if((ModulesNum-21)%4==0 && (ModulesNum-21)>0)                    
		Version= (ModulesNum-21)/4 +1;
	else if(ModulesNum==11)                                               
		Version=41;
	else if(ModulesNum<=17 && (ModulesNum-11)%2==0 && (ModulesNum-11)>0)  
		Version= (ModulesNum-11)/2 +41;
	else 
		Version=99;             //錯誤標記
	 
	int MQVersion=0;             //MicorQR用

	if(Version==99)
		return ans;

    memset(datamap,0,sizeof(datamap));
    memset(qr_mask,0,sizeof(qr_mask));
    
 

	//將資料置入datamap[X][Y]
	for(int i=0;i<ModulesNum;i++)
		for(int j=0;j<ModulesNum;j++)
		{
			if(rx[i*ModulesNum+j])
				datamap[i][j]=0;  
			else
				datamap[i][j]=255;  
		}
//////////////////////////////////////////////////////////////////////////////
//3.0 讀取Version Block
//
//說明: 讀取Version Block(左下與右上)區塊的資料，與前處理所取得的版本辨識結果做交叉比對
//      若是兩個Version Block區塊的結果相同= 使用Version Block的版本資訊
//      若是兩個Version Block區塊的結果相異= 條碼破損,結束解碼
//
//3.1 讀取Format Information
//
//說明: 讀取QR條碼Format Information區塊的資料
//
//////////////////////////////////////////////////////////////////////////////
	unsigned char *QRFormatInformation = NULL;
	if(Version<=40)      
		QRFormatInformation = GetFormatInformation(datamap,XORmask,QRFormatTable,ModulesNum);
	else if(Version>40) 
		QRFormatInformation = GetFormatInformationMQR(datamap,MQR_XORmask);

	for(int a=0;a<ModulesNum;a++)
		for(int b=0;b<ModulesNum;b++)
		{
			  if(datamap[a][b]==255)    
				  datamap[a][b]=0;       //c=255,白色區塊                     
			  else                      
				  datamap[a][b]=1;                              
		}
//////////////////////////////////////////////////////////////////////////////
//3.2 去遮罩
//
///說明: 根據前步驟讀取Format Information區塊所得到的遮罩代號
//       建立相對應類型的遮罩，與原始資料陣列進行XOR運算，進行解遮罩處理
//
///RemoveHerMask(&mask,ModulesNum,QRFormation);
//////////////////////////////////////////////////////////////////////////////
	int masknum;

//MQR
//======================================================================
	if(Version>40){
		masknum=2*QRFormatInformation[11]+QRFormatInformation[10];
		//printf(",masknum=%d",masknum);
		switch(masknum){
		case 0:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{sum=i % 2;                         //Pattern 001 :i mod 2 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //若符合該式,則為1 (黑點)
			  else
			   qr_mask[i][j]=0;    }
			 break;

		case 1:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=((i/2)+(j/3))% 2;              //Pattern 100 :((i div 2) + (j div 3)) mod 2 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //若符合該式,則為1 (黑點)
			  else
			   qr_mask[i][j]=0;    }
			break;

		case 2:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=((i*j)% 2 + (i*j)% 3)%2;        //Pattern 110 :((i j) mod 2 + (i j) mod 3) mod 2 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //若符合該式,則為1 (黑點)
			  else
			   qr_mask[i][j]=0;    }
			break;

		case 3:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=((i*j)% 3 + (i+j)% 2)% 2;       //Pattern 111 :((i j) mod 3 + (i+j) mod 2) mod 2 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //若符合該式,則為1 (黑點)
			  else
			   qr_mask[i][j]=0;    }
			break;
		}
	}else
	{
		masknum=4*QRFormatInformation[12]+2*QRFormatInformation[11]+QRFormatInformation[10];
		//--------------------------------------------------------------------
		switch(masknum){
		case 0:
		   for(int i=0;i<ModulesNum;i++)
		   for(int j=0;j<ModulesNum;j++)
			 { sum=(i+j) % 2;                     //Pattern 000 :(i+j) mod 2 = 0
			   if(sum==0)
				qr_mask[i][j]=1;                        //若符合該式,則為1 (黑點)  !!!!!!!!
			  else
				qr_mask[i][j]=0;  }
			break;
		case 1:
   			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{sum=i % 2;                         //Pattern 001 :i mod 2 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //若符合該式,則為1 (黑點)
			  else
			   qr_mask[i][j]=0;    }
			 break;

		case 2:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=j % 3;                         //Pattern 010 :j mod 3 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //若符合該式,則為1 (黑點)
			  else
			   qr_mask[i][j]=0;    }
			break;

		case 3:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=(i+j) % 3;                     //Pattern 011 :(i + j) mod 3 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //若符合該式,則為1 (黑點)
			  else
			   qr_mask[i][j]=0;    }
			break;

		case 4:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=((i/2)+(j/3))% 2;              //Pattern 100 :((i div 2) + (j div 3)) mod 2 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //若符合該式,則為1 (黑點)
			  else
			   qr_mask[i][j]=0;    }
			break;

		case 5:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=(i*j)% 2 + (i*j)% 3;               //Pattern 101 :(i j) mod 2 + (i j) mod 3 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //若符合該式,則為1 (黑點)   大驚!!
			  else
			   qr_mask[i][j]=0;    }
		   break;

		case 6:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=((i*j)% 2 + (i*j)% 3)%2;        //Pattern 110 :((i j) mod 2 + (i j) mod 3) mod 2 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //若符合該式,則為1 (黑點)
			  else
			   qr_mask[i][j]=0;    }
			break;

		case 7:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=((i*j)% 3 + (i+j)% 2)% 2;       //Pattern 111 :((i j) mod 3 + (i+j) mod 2) mod 2 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //若符合該式,則為1 (黑點)
			  else
			   qr_mask[i][j]=0;    }
			break;
		}         
	}

//--------------------------------------------------------------------------
//區分功能區塊與資料區塊:
//根據Version的不同，標籤功能區塊的大小位置也隨之改變，故在此進行分類
//基本功能區塊:回字定位區塊(Position Detection Patterns)X3 ,護城河(Separate for Position Detection Pattern)X3 ,斑馬線(Timing Patterns)X2 ,Format Information區塊
//特殊功能區塊:Version Information區塊[v7以後出現] 小回字輔助定位區塊(Alignment Patterns)[v2-v6(1) ,v7~v13(3x3) ,v14~v20(4x4) ,v21~v39(5x5) ,v40(7x7)]
//--------------------------------------------------------------------------
	int ThePoint=ModulesNum-7;  //邊角點位置
	int NewPoint=0;     //計算用:新的邊角點(中心點)與間距

//MicroQR部分處理
//======================================================================
	if(Version>40)
	{
		for(int i=0;i<9;i++)       
			for(int j=0;j<9;j++)
				qr_mask[i][j]=99; //左上PDP

		for(int i=0;i<ModulesNum;i++) 
			qr_mask[0][i]=99;  //兩條Timing Patten
		for(int i=0;i<ModulesNum;i++) 
			qr_mask[i][0]=99;
	}

//======================================================================
	else
	{
		for(int i=0;i<9;i++)       
			for(int j=0;j<9;j++)
				qr_mask[i][j]=99;            //左上PDP

		for(int i=ThePoint-1;i<ModulesNum;i++)
			for(int j=0;j<9;j++)       
				qr_mask[i][j]=99;            //左下PDP

		for(int i=0;i<9;i++)
			for(int j=ThePoint-1;j<ModulesNum;j++)
				qr_mask[i][j]=99;            //右上PDP

		for(int k=6;k<=ThePoint;k++)
		{
			qr_mask[k][6]=99;            //直Timing Patterns
			qr_mask[6][k]=99;            //橫Timing Patterns
		}            




		//mask[ModulesNum-7][ModulesNum-7] 第四PDP區塊邊角點
		//v2~v6 當只有一個AP時適用
		if(Version >= 2) 
		{
			for(int i=ThePoint-2;i<=ThePoint+2;i++)
				for(int j=ThePoint-2;j<=ThePoint+2;j++)
					qr_mask[i][j]=99;}



	//-------------------------------------------------------------------------- 
	//Version 7~13 輔助定位區塊
			int AP_X,AP_Y;
			if(Version >= 7 && Version <=13)
			{
				 //A,B區
				 AP_Y=6,AP_X=(6+ThePoint)/2;
				 for(int i=AP_X-2;i<=AP_X+2;i++)
					 for(int j=AP_Y-2;j<=AP_Y+2;j++)
					 {
						 qr_mask[j][i]=99;
						 qr_mask[i][j]=99;
					 }

				 //D,E區
				 AP_Y=ThePoint,AP_X=(6+ThePoint)/2;
				 for(int i=AP_X-2;i<=AP_X+2;i++)
					 for(int j=AP_Y-2;j<=AP_Y+2;j++)
					 {
						 qr_mask[j][i]=99;
						 qr_mask[i][j]=99;
					 }

				 //C區
				 AP_Y=AP_X=(6+ThePoint)/2;
				 for(int i=AP_X-2;i<=AP_X+2;i++)
					for(int j=AP_Y-2;j<=AP_Y+2;j++)
						qr_mask[i][j]=99;
			}

    //-------------------------------------------------------------------------- 
	//Version 14~20 輔助定位區塊
			int AP[8];                              //8來源?
			if(Version >= 14 && Version <=20)
			{
				AP[0]=6;
				AP[1]=6+(ThePoint-6)/3;                 //如果除不盡會發生大問題!!!!
				AP[2]=6+((ThePoint-6)/3)*2;    
				AP[3]=ThePoint;

			//特例:Version 15,18
			if(Version==15 || Version==18)  
				AP[1]=AP[1]-1;

			//特例:Version 19
			if(Version==19 || Version==16)  
				AP[1]=AP[1]-2;

			//第一排:僅有23
			for(int b=1;b<3;b++)
			{
				AP_X=AP[0];
				AP_Y=AP[b];
				for(int i=AP_X-2;i<=AP_X+2;i++)
					for(int j=AP_Y-2;j<=AP_Y+2;j++)
						qr_mask[i][j]=99;
			}

			//第二三排:有1234
			for(int a=1;a<=2;a++)
				for(int b=0;b<4;b++)
				{
					AP_X=AP[a];
					AP_Y=AP[b];
					for(int i=AP_X-2;i<=AP_X+2;i++)
					{
						for(int j=AP_Y-2;j<=AP_Y+2;j++)
							qr_mask[i][j]=99;
					}
				}


			//第四排:僅有234
			for(int b=1;b<4;b++)
			{
				AP_X=AP[3];
				AP_Y=AP[b];
				for(int i=AP_X-2;i<=AP_X+2;i++)
				{
					for(int j=AP_Y-2;j<=AP_Y+2;j++)
						qr_mask[i][j]=99;
				}
			}

		} 

	//--------------------------------------------------------------------------
		//Version 21~27 輔助定位區塊
		if(Version >= 21 && Version <=27)
		{
			AP[0]=6;
			AP[1]=6+(ThePoint-6)/4;
			AP[2]=6+((ThePoint-6)/4)*2;
			AP[3]=6+((ThePoint-6)/4)*3;
			AP[4]=ThePoint;

			//特例:Version 22,24,26
			if(Version ==22 || Version ==24 || Version ==26)
			{
				AP[1]=AP[1]-3;
				AP[2]=AP[2]-2;
				AP[3]=AP[3]-1;
			}

		//第一排:僅有234
			for(int b=1;b<=3;b++)
			{
				AP_X=AP[0];
				AP_Y=AP[b];
				for(int i=AP_X-2;i<=AP_X+2;i++)
				{
					for(int j=AP_Y-2;j<=AP_Y+2;j++)
						qr_mask[i][j]=99;
				}
			}

		//第二三四排:有12345
			for(int a=1;a<=3;a++)
				for(int b=0;b<5;b++)
				 {
					AP_X=AP[a];
					AP_Y=AP[b];
					for(int i=AP_X-2;i<=AP_X+2;i++)
					{
						for(int j=AP_Y-2;j<=AP_Y+2;j++)
							qr_mask[i][j]=99;
					}
				}


		//第五排:僅有2345
			for(int b=1;b<=4;b++)
			 {
				 AP_X=AP[4];
				AP_Y=AP[b];
				for(int i=AP_X-2;i<=AP_X+2;i++)
				{
					for(int j=AP_Y-2;j<=AP_Y+2;j++)
						qr_mask[i][j]=99;
				}
			}
		}
		//--------------------------------------------------------------------------
		//Version 28~34 輔助定位區塊[6x6]
		if(Version >= 28 && Version <=34)
		{
			AP[0]=6;
			AP[1]=6+(ThePoint-6)/5;
			AP[2]=6+((ThePoint-6)/5)*2;
			AP[3]=6+((ThePoint-6)/5)*3;
			AP[4]=6+((ThePoint-6)/5)*4;
			AP[5]=ThePoint;

			//特例:Version 28,33
			if(Version ==28 || Version ==33 )
			{
				AP[1]=AP[1]-3;
				AP[2]=AP[2]-2;
				AP[3]=AP[3]-1;
			}

			//特例:Version 31
			if(Version ==31 )
			{
				AP[1]=AP[1]-1;
				AP[3]=AP[3]+1;
				AP[4]=AP[4]+2;
			}

			//特例:Version 32
			if(Version ==32 )
			{
				AP[1]=AP[1]+2;
				AP[2]=AP[2]+2;
				AP[3]=AP[3]+2;
				AP[4]=AP[4]+2;
			}

			//特例:Version 30
			if(Version ==30 )
			{
				AP[1]=AP[1]-4;
				AP[2]=AP[2]-2;
				AP[4]=AP[4]+2;
			}

			//第一排:僅有2345
			for(int b=1;b<=4;b++)
			{
				 AP_X=AP[0];
				AP_Y=AP[b];
				for(int i=AP_X-2;i<=AP_X+2;i++)
				{
					for(int j=AP_Y-2;j<=AP_Y+2;j++)
						qr_mask[i][j]=99;
				}
			}

			//第二三四五排:有123456
			for(int a=1;a<=4;a++)
			{
				for(int b=0;b<=5;b++)
				{
					AP_X=AP[a];
					AP_Y=AP[b];
					for(int i=AP_X-2;i<=AP_X+2;i++)
					{
						for(int j=AP_Y-2;j<=AP_Y+2;j++)
							qr_mask[i][j]=99;
					}
				}
			}

			//第五排:僅有23456
			for(int b=1;b<=5;b++)
			{
				AP_X=AP[5];
				AP_Y=AP[b];
				for(int i=AP_X-2;i<=AP_X+2;i++)
				{
					for(int j=AP_Y-2;j<=AP_Y+2;j++)
						qr_mask[i][j]=99;
				}
			}
		}

		//--------------------------------------------------------------------------
		//Version 35~40 輔助定位區塊[7X7]
		if(Version >= 35 && Version <=40)
		{
			AP[0]=6;
			AP[1]=6+(ThePoint-6)/6;
			AP[2]=6+((ThePoint-6)/6)*2;
			AP[3]=6+((ThePoint-6)/6)*3;
			AP[4]=6+((ThePoint-6)/6)*4;
			AP[5]=6+((ThePoint-6)/6)*5;
			AP[6]=ThePoint;

			//特例:Version 36,39
			if(Version ==36 || Version ==39 )
			{
				AP[1]=AP[1]-6;
				AP[2]=AP[2]-4;
				AP[3]=AP[3]-2;
				AP[5]=AP[5]+2;
			}    

			//特例:Version 37,40
			if(Version ==37 || Version ==40 )
			{
				AP[1]=AP[1]-3;
				AP[2]=AP[2]-2;
				AP[3]=AP[3]-1;
				AP[5]=AP[5]+1;
			}    



			//第1排:僅有23456
			for(int b=1;b<=5;b++)
			{
				AP_X=AP[0];
				AP_Y=AP[b];
				for(int i=AP_X-2;i<=AP_X+2;i++)
				{
					for(int j=AP_Y-2;j<=AP_Y+2;j++)
						qr_mask[i][j]=99;
				}
			}

			//第2~6排:有123456
			for(int a=1;a<=5;a++)
				for(int b=0;b<=6;b++)
				 {
					AP_X=AP[a];
					AP_Y=AP[b];
					for(int i=AP_X-2;i<=AP_X+2;i++)
					{
						for(int j=AP_Y-2;j<=AP_Y+2;j++)
							qr_mask[i][j]=99;
					}
				}


			//第7排:僅有234567
			for(int b=1;b<=6;b++)
			{
				AP_X=AP[6];
				AP_Y=AP[b];
				for(int i=AP_X-2;i<=AP_X+2;i++)
				{
					for(int j=AP_Y-2;j<=AP_Y+2;j++)
						qr_mask[i][j]=99;
				}
			}
		}
		//--------------------------------------------------------------------------
		//Version7以上 版本訊息區塊
		if(Version >= 7)
		{
			 for(int i=0;i<6;i++)
				 for(int j=0;j<3;j++)
				 { 
					 qr_mask[i][ModulesNum-11+j]=99;  //右上區塊
					 qr_mask[ModulesNum-11+j][i]=99;  //左下區塊
				 }
		}      
	}


	//使用遮罩對原始條碼進行XOR運算
	for(int a=0;a<ModulesNum;a++)
		for(int b=0;b<ModulesNum;b++)
		{
			if(qr_mask[a][b]!=99)
			{
				if(qr_mask[a][b]==datamap[a][b])
					qr_mask[a][b]=0;
				else
					qr_mask[a][b]=1;
			}
		}
	if(Version<=40) 
		for(int k=0;k<ModulesNum;k++)   
			qr_mask[k][6]=55;            //[QR限定]直Timing Patterns會對解碼產生障礙，排除



//////////////////////////////////////////////////////////////////////////////
//3.3 取得資料串流長度
//
//說明:根據該QR條碼的版本等級(Version)與容錯等級(L,M,Q,H),進行查表
//     找出對應的資料區塊與容錯區塊的數量
//     使用表格: datacapacity[200]
//     *會用到格式區塊資訊(QRFormatInformation)
//////////////////////////////////////////////////////////////////////////////
	int Xmove,Ymove;
	int Datamove=0,Errormove=0,Datapoint=0;
	int LR=1,UD=0;              //起始位置右(1) 上行(0)
	int TopWeight=0;

	int DataCount=0;   //資料區數量
	int ErrorCount=0;  //容錯區數量
	int ErrorLevel=0;

//======================================================================

	if(Version>40)
	{
		
		ErrorLevel=4*QRFormatInformation[14]+2*QRFormatInformation[13]+1*QRFormatInformation[12];
		MQVersion=ErrorLevel+1;
		//printf(",EC=%d",ErrorLevel);
		switch(ErrorLevel){ //對照Table 9和Table 13, 設定data 和EC code word數目
		case 0:
		  DataCount=3;
		  ErrorCount=2;
		  ErrorLevel=0;
		  //printf("這是一張 M1_X的MicroQR條碼\n");
		  break;

		case 1:
		  DataCount=5;
		  ErrorCount=5; //joseph test
		  ErrorLevel=1;
		  //printf("這是一張 M2_L的MicroQR條碼\n");
		  break;

		case 2:
		  DataCount=4;
		  ErrorCount=6;
		  ErrorLevel=2;
		  //printf("這是一張 M2_M的MicroQR條碼\n");
		  break;

		case 3:
		  DataCount=11;
		  ErrorCount=6;
		  ErrorLevel=3;
		  //printf("這是一張 M3_L的MicroQR條碼\n");
		  break;

		case 4:
		  DataCount=9;
		  ErrorCount=8;
		  ErrorLevel=4;
		  //printf("這是一張 M3_M的MicroQR條碼\n");
		  break;

		case 5:
		  DataCount=16;
		  ErrorCount=8;
		  ErrorLevel=5;
		  //printf("這是一張 M4_L的MicroQR條碼\n");
		  break;

		case 6:
		  DataCount=14;
		  ErrorCount=10;
		  ErrorLevel=6;
		  //printf("這是一張 M4_M的MicroQR條碼\n");
		  break;

		case 7:
		  DataCount=10;
		  ErrorCount=14;
		  ErrorLevel=7;
		  //printf("這是一張 M4_Q的MicroQR條碼\n");
		  break;
		}

	}
	else
	{
		if(QRFormatInformation[14]==0)
		{
			DataCount =datacapacity[5*(Version-1)+(2-QRFormatInformation[13])];
			ErrorCount=datacapacity[5*(Version-1)]-DataCount;                   //容錯區數量
			if(QRFormatInformation[13]==1)  
				ErrorLevel=0;		  //L等級
			else	                          
				ErrorLevel=1;       //M等級
			
		 }else
		 {
			 DataCount =datacapacity[5*(Version-1)+(4-QRFormatInformation[13])];
			 ErrorCount=datacapacity[5*(Version-1)]-DataCount;                   //容錯區數量
			 if(QRFormatInformation[13]==1)   
				 ErrorLevel=2;      //Q等級
			 else                             
				 ErrorLevel=3;	  //H等級
		 }
	}

	//add by Joseph@20131213, free QRFormatInformation, maybe modify format checking method later
	if (QRFormatInformation != NULL) 
	{
		free(QRFormatInformation);
		QRFormatInformation = NULL;
	}

	//skip illegal MicroQR pair
	switch (Version)
	{
		case 41:
			if (ErrorLevel !=0) return ans;
			break;
		case 42:
			if ((ErrorLevel !=1) && (ErrorLevel !=2)) return ans;
			break;
		case 43:
			if ((ErrorLevel !=3) && (ErrorLevel !=4)) return ans;
			break;
		case 44:
			if ((ErrorLevel !=5) && (ErrorLevel !=6) && (ErrorLevel !=7)) return ans;
			break;
	}

	//////////////////////////////////////////////////////////////////////////////
	//3.4 讀取對應數量的Data碼與RS碼
	//
	//說明:根據前步驟取得的資訊:資料區塊數量(DataCount) 與 容錯區塊數量(ErrorDataCount)
	//     在QR條碼中讀取相對應數量的資料欄位，並進行Codeword的計算
	//
	//////////////////////////////////////////////////////////////////////////////
	//開始讀入Codeword，起始點座標為最右下角點(ModulesNum-1,ModulesNum-1)，擷取數量:DataCount
	Xmove= ModulesNum -1;
	Ymove= ModulesNum -1;


	for(int a=1 ; a <= DataCount+ErrorCount; a++)  //第一層:讀取總資料區塊數
	{ 
		for(;;)                                      //第二層:無窮 資料讀取指針的移動
		{ 
			if(LR==1)  //當LR走到右方，結束後指針左移(x-1 y)       
			{ 
				if(qr_mask[Ymove][Xmove]==55)        
					Xmove=Xmove-1;                   //修正用:當碰到特殊區域※時

				if(qr_mask[Ymove][Xmove]!=99 && qr_mask[Ymove][Xmove]!=55)  //當指針所指位置不等於功能區塊時，讀取該格data
				{
					if(a>DataCount)
					{
						ErrorArray[Errormove]=qr_mask[Ymove][Xmove];
						Errormove++;
					}
					else
					{
						 DataArray[Datamove]=qr_mask[Ymove][Xmove];
						  ErrorArray[Errormove]=qr_mask[Ymove][Xmove];
						  Errormove++;
					}

     				Datamove++;
					Datapoint++;
				}
		  
				if(UD==0 && Ymove==0)  //當上行觸底時 權重+1
					TopWeight+=1;
				if(UD==1 && Ymove==ModulesNum-1)  //當下行觸底時 權重+1
					TopWeight+=1;

				 Xmove=Xmove-1;    //左偏移
				 LR=0;             //切換指針
			}//------------------------------2.1 LR==1 End


			if(LR==0 && Datapoint<8)  //當LR走到左方時，追加判斷目前是上行/下行   
			{ 
				if(qr_mask[Ymove][Xmove]!=99 && qr_mask[Ymove][Xmove]!=55)  //當指針所指位置不等於功能區塊時，讀取該格data
				{
					if(a>DataCount)
					{
						 ErrorArray[Errormove]=qr_mask[Ymove][Xmove];
						 Errormove++;
					}
				    else
					{
						 DataArray[Datamove]=qr_mask[Ymove][Xmove];
						 ErrorArray[Errormove]=qr_mask[Ymove][Xmove];
						 Errormove++;}

     					Datamove++;
						Datapoint++;
				    }

				    if(UD==0 && Ymove==0)             //當上行觸底時 權重+1
						TopWeight+=1;
					if(UD==1 && Ymove==ModulesNum-1)  //當下行觸底時 權重+1
						TopWeight+=1;

					if(TopWeight!=2)                  //只有在非觸底情況下，才進行右上/右下偏移 
					{
						if(UD==0)//當UD=0[上行]時，結束後指針往右上移(x+1 y-1)  
						{
							Xmove=Xmove+1;    //右上偏移
							Ymove=Ymove-1;
						} 
					    else     //當UD=1[下行]時，結束後指針往右下移(x+1 y+1)
						{
							Xmove=Xmove+1;    //右下偏移
							Ymove=Ymove+1;
						}
				   }

					LR=1;               //切換指針
				}//------------------------------2.2 LR==0 End
	   

				if(TopWeight==2  &&  LR==1) //到達上下邊緣時 進行轉向並平移起始點[條件:TopWeight=2,LR=0]
				{    
					Xmove=Xmove-1;      //起始點統一左平移一格  
					if(UD==0) //當UD=0[上行]到邊界時，更改UD=1 LR=1[右方]
						UD=1;
					else      //當UD=1[下行]到邊界時，更改UD=0 LR=1[右方]
						UD=0;
						LR=1;
						TopWeight=0;    //完成轉向後歸零
				}//---------------2.3 ReturnBack End


				 if(Datapoint==8) //累積讀取滿8筆資料 跳出無窮迴圈
				 {     Datapoint=0;    //計數指針歸零
					   break;
				 }
			}//------------------------------2nd End
		}//------------------------------1st End





if(ErrorCount<0 || DataCount < 0 || Version>45 || Version==0)   EncodeMode=99;             //驗證:若格式資料有誤,跳出

if(EncodeMode !=99){

	if(Version==41 || Version==43)  	    //特例:MicroQR _M1,M3
	for(int a=0 ; a<DataCount+ErrorCount ; a++){
		if(a<DataCount-1 )  r[a]= 128*ErrorArray[8*a]+64*ErrorArray[(8*a)+1]+32*ErrorArray[(8*a)+2]+16*ErrorArray[(8*a)+3]+8*ErrorArray[(8*a)+4]+4*ErrorArray[(8*a)+5]+2*ErrorArray[(8*a)+6]+1*ErrorArray[(8*a)+7]; 
		if(a==DataCount-1)  r[a]= 8*ErrorArray[(8*a)]+4*ErrorArray[(8*a)+1]+2*ErrorArray[(8*a)+2]+1*ErrorArray[(8*a)+3]; 
		if(a>DataCount-1 )  r[a]= 128*ErrorArray[8*(a-1)+4]+64*ErrorArray[(8*(a-1))+5]+32*ErrorArray[(8*(a-1))+6]+16*ErrorArray[(8*(a-1))+7]+8*ErrorArray[(8*(a-1))+8]+4*ErrorArray[(8*(a-1))+9]+2*ErrorArray[(8*(a-1))+10]+1*ErrorArray[(8*(a-1))+11]; 
	}
	else
	for(int a=0 ; a<DataCount+ErrorCount ; a++){
	r[a]= 128*ErrorArray[8*a]+64*ErrorArray[(8*a)+1]+32*ErrorArray[(8*a)+2]+16*ErrorArray[(8*a)+3]+8*ErrorArray[(8*a)+4]+4*ErrorArray[(8*a)+5]+2*ErrorArray[(8*a)+6]+1*ErrorArray[(8*a)+7];
	}

#ifdef DEV_MODE
  printf("原始條碼的Bitstream  :");
  for(int a=0;a<DataCount*8;a++)  printf(" %d",DataArray[a]);
  printf("\n");

  printf("RSanswer is %d\n",RSanswer);
#endif


	//資料的重新排序與修正
	if(Version>40) {
	    RSanswer=MicroQRTransArray(r,MQVersion,ErrorLevel,DataCount,ErrorCount);     //檢查r值是否修正並回傳: 13-資料輸入正確  666-資料無法修正 1-資料錯誤已修正
	    DataCorrect=RS2binary(r,DataCount);                            //將修正結果r[] 轉回8bit碼
	} else {
	    if ((Version != last_version) || (ErrorLevel != last_ec) || (fail_frame_cnt > MAX_FRAME_CNT) ||(masknum != last_masknum)) {			
		memset(Decodeword,0,T2);
		memset(group1_pass_cnt,0,70);
		memset(group2_pass_cnt,0,70);
		last_version = Version;
		last_ec = ErrorLevel;
		last_masknum = masknum;
		fail_frame_cnt=0;
	    }
	    RSanswer=TransArray(r,Version,ErrorLevel,DataCount,ErrorCount,QRStructTable);     //檢查r值是否修正並回傳: 13-資料輸入正確  666-資料無法修正 1-資料錯誤已修正
	    DataCorrect=RS2binary(Decodeword,DataCount);
	}
		
	for(int a=0;a<DataCount*8;a++)  DataArray[a]=DataCorrect[a];   //將一個codeword變成八個二進制, 4=>0000 0100
	free(DataCorrect);

#ifdef DEV_MODE
  printf("重新排列後的原始條碼Bitstream:");
  for(int a=0;a<DataCount*8;a++)  printf(" %d",DataArray[a]);
  printf("\n");

  printf("RSanswer is %d\n",RSanswer);
#endif
}

//add by Joseph@20131119, dump bitstream, will be used for ECI implement
#ifdef DEV_MODE
//dump_bitstream start
int mode[4];
int assignnent[8];
int mode_in_ECI[4];
int char_counter[8];
int content[8];

FILE *fp;
fp = fopen("\\Storage Card\\dumpQR_bitstream.txt", "w");

fprintf(fp,"mode:");
for(int a=0;a<4;a++){ 
	mode[a]=DataArray[a];
	fprintf(fp,"%d",mode[a]);
}
fprintf(fp,"\nassigment:");
for(int a=4;a<12;a++) {
	assignnent[a-4]=DataArray[a];
	fprintf(fp,"%d",assignnent[a-4]);
}
fprintf(fp,"\nmode in ECI:");
for(int a=12;a<16;a++){
	mode_in_ECI[a-12]=DataArray[a];
	fprintf(fp,"%d",mode_in_ECI[a-12]);
}
fprintf(fp,"\nchar counter:");
for(int a=16;a<24;a++) {
	char_counter[a-16]=DataArray[a];
	fprintf(fp,"%d",char_counter[a-16]);
}
fprintf(fp,"\ncontent:\n");
for (int a=24;a<DataCount*8;a=a+8){
    for(int i=0;i<8;i++) {
	    content[i]=DataArray[a+i];
	    fprintf(fp,"%d",content[i]);
    }
	fprintf(fp,"\n");
}
fclose(fp);
//dump_bitstream end
#endif
//add end@20131119

if(RSanswer >= 666)  EncodeMode=99;              //驗證:若錯誤太多無法修復,跳出




//////////////////////////////////////////////////////////////////////////////
//釋放記憶體[遮罩用]
//for(int i=0;i<ModulesNum;i++) free(mask[i]);
//free(mask);
//for(int i=0;i<ModulesNum;i++) free(datamap[i]);
//free(datamap);


//////////////////////////////////////////////////////////////////////////////
//5.3 輸出結果
//
//說明:檢查偵測旗標(EncodeMode)是否有異常值
//     若否，則回傳解碼結果ans
//////////////////////////////////////////////////////////////////////////////        
if(EncodeMode==99) {	
    //ans=NULL;
	fail_frame_cnt++;
	//if (segmation_mode == QR_PATTERN_MODE) 
	//	fail_frame_cnt++;
} else {
	ans = parse_bitstream(0,Version,ErrorLevel,QRENCODEmode);
    memset(Decodeword,0,T2);
    memset(group1_pass_cnt,0,70);
    memset(group2_pass_cnt,0,70);
    last_version = 0;
    last_ec = 0;
	last_masknum = 0;
    fail_frame_cnt=0;    
}

#ifdef DEV_MODE
printf("Output:  %s\n\n",ans);
if(IMnum==8)
for(int a=0;a<CharacterCountNum*4;a++) printf("%c",ans[a]);

#endif

return ans;
}

bool check_uQR_ending(int ErrorLevel,int offset)
{
	bool result = false;
	switch (ErrorLevel)
	{
		case 0:
			if ((DataArray[offset]==0) && (DataArray[offset+1]==0) && (DataArray[offset+2]==0))  result=true;
			break;
		case 1:
		case 2:
			if ((DataArray[offset]==0) && (DataArray[offset+1]==0) && (DataArray[offset+2]==0)&& (DataArray[offset+3]==0) && (DataArray[offset+4]==0))  result=true;
			break;
		case 3:
		case 4:
			if ((DataArray[offset]==0) && (DataArray[offset+1]==0) && (DataArray[offset+2]==0)&& (DataArray[offset+3]==0) && (DataArray[offset+4]==0)) {
				if ((DataArray[offset+5]==0) && (DataArray[offset+6]==0)) {
				    result=true;
				}
			}
			break;
		case 5:
		case 6:
		case 7:
			if ((DataArray[offset]==0) && (DataArray[offset+1]==0) && (DataArray[offset+2]==0)&& (DataArray[offset+3]==0) && (DataArray[offset+4]==0)) {
				if ((DataArray[offset+5]==0) && (DataArray[offset+6]==0) && (DataArray[offset+7]==0) && (DataArray[offset+8]==0)) {
				    result=true;
				}
			}
			break;

	}
	return result;
}


char* parse_bitstream(int start_pos,int Version,int ErrorLevel,int *QRENCODEmode)
{
	//////////////////////////////////////////////////////////////////////////////
	//5.1取得模式指示符與字數統計符
	//
	//說明:取得該QR條碼的編碼模式,字數統計符位數 與 條碼資料字元數量 等資訊
	//
	//////////////////////////////////////////////////////////////////////////////
	int IMnum=0;                        //編碼模式
	int CharacterCount=0;               //標頭檔長度
	int CharacterCountNum=0;            //字元數量
	int R=0;                            //計算用變數
	int DatabitsLong=0;

	int BadPT=start_pos;                //指針:記錄目前處理到的資料數
	int EndPT=0;                        //while迴圈的判斷參數:檢查是否到達終止符，結束解碼

	int ModeIndicatorLength=0;
	char* ans=NULL;
	char* tempans=NULL;
	int EncodeMode=0;

	ans=(char*)malloc(sizeof(char)*7089+2);                    //暫定:最大解為100的字元 ,7089=Ver.40_L Numeric最大字數, add by Joseph@20131210, plus 2(same buffer size of numeric mode)
	memset(ans,0,7091);

	while(EndPT==0)
	{
		//根據輸入編碼模式等資訊，計算出本條碼的編碼模式,字數統計符位數與條碼內資料字元數量
		if(Version<=40)       
			GetCharacterCountIndicator(DataArray,Version,BadPT,&IMnum,&CharacterCount,&CharacterCountNum);
		else                  
			GetCharacterCountIndicatorMQR(DataArray,ErrorLevel,BadPT,&IMnum,&CharacterCount,&CharacterCountNum,&ModeIndicatorLength);



		if(Version<=40) 
			ModeIndicatorLength=4;     //QR條碼的模式指示符長度固定為4

		//////////////////////////////////////////////////////////////////////////////
		//5.2 資料解碼
		//
		//說明:根據不同的資料編碼模式，進行各自的解碼處理
		//
		////////////////////////////////////////////////////////////////////////////// 

		switch(IMnum)
		{
		case 1:
			(*QRENCODEmode)=1;
			tempans=NumericMode(DataArray,CharacterCountNum,CharacterCount,NumericTable,BadPT,ModeIndicatorLength);
			EncodeMode=1;                                             //標記為:Numberic
			break;

		case 2:
			(*QRENCODEmode)=2;
			tempans=AlphanumericMode(DataArray,CharacterCountNum,CharacterCount,AlphanumericTable,BadPT,ModeIndicatorLength);
			EncodeMode=2;                                             //標記為:AlphaNumberic
			break;

		case 4:  
			(*QRENCODEmode)=3;
			//tempans=BabitMode(DataArray,CharacterCountNum,CharacterCount);                  //8bitMode顯示十六進位碼
			tempans=BabitModeTicket(DataArray,CharacterCountNum,CharacterCount,BadPT,ModeIndicatorLength);    //8bitMode顯示ASCII碼
			EncodeMode=3;                                             //標記為:8bitBinary
			break;

		case 8:  
			(*QRENCODEmode)=4;                                        //標記為:Kanji Mode 漢字模式
			tempans=KanjiMode(DataArray,CharacterCountNum,CharacterCount,BadPT,ModeIndicatorLength);
			EncodeMode=4;  
			break;

		case 3:  
			(*QRENCODEmode)=5;                                        //標記為:Structured Append
			//EncodeMode=99;                                            
			//EndPT=2;
			EncodeMode=5;                                                
			get_structured_append_info(DataArray,ModeIndicatorLength,BadPT);
			break;

		case 5:  
			(*QRENCODEmode)=6;                                        //標記為:FNC1 mode(1st position)
			//EncodeMode=99;                                            
			//EndPT=2; 
			EncodeMode=6;  
			break;
		case 7:  
			(*QRENCODEmode)=7;                                        //標記為:ECI mode
			//EncodeMode=99;                                            
			//EndPT=2;
			EncodeMode=7;                                                
			break;
		case 9:  
			(*QRENCODEmode)=9;                                        //標記為:FNC1 mode(2nd position)
			EncodeMode=99;                                            
			EndPT=2;  
			break;

		default:
			EncodeMode=99;                                            //標記為:ErrorMode
			EndPT=2;  
			break;
		}

		if (EncodeMode==99) 
			continue;  //add by Joseph@20131119, if we meet error mode, skip the following process

		//////////////////////////////////////////////////////////////////////////////
		//檢查是否為Mixing Mode
		//是:更新BadPT,將解碼結果做字串相加並丟入暫存區,繼續下步驟
		//否:解碼結果做字串相加並輸出結果,結束迴圈
		switch(IMnum)
		{
		case 1:
			if(CharacterCountNum%3==2)        R=7;
			else if(CharacterCountNum%3==1)   R=4;  
			else R=0;
			DatabitsLong= ModeIndicatorLength + CharacterCount + 10*(CharacterCountNum / 3) + R;
			break;
		case 2:
			DatabitsLong=ModeIndicatorLength + CharacterCount + 11*(CharacterCountNum / 2)+6*(CharacterCountNum % 2);
			break;
		case 4:
			DatabitsLong=ModeIndicatorLength + CharacterCount + (8 * CharacterCountNum);
			break;
		case 8:
			DatabitsLong=ModeIndicatorLength + CharacterCount + (13 * CharacterCountNum);
			break;
		case 3:
			DatabitsLong=4 + 16;
			break;
		case 7:
			DatabitsLong=4 + 8;
			break;
		case 5:
			DatabitsLong=4;
			break;
		default:
			EncodeMode=99;                                            
			break;
		}

	/***************************** TBD ***********************************/
		//檢查是否到達終點 [QR用]    
		if(Version<=40)
		{
			if(DataArray[BadPT+DatabitsLong+0]==0 && DataArray[BadPT+DatabitsLong+1]==0 && DataArray[BadPT+DatabitsLong+2]==0 && DataArray[BadPT+DatabitsLong+3]==0)
			{//非MixingMode
				 if(BadPT==0) strcpy(ans,tempans);
				 else         strcat(ans,tempans);
				 EndPT=2;
			}
			else if(8*DataArray[BadPT+DatabitsLong+0]+ 4*DataArray[BadPT+DatabitsLong+1]+ 2*DataArray[BadPT+DatabitsLong+2]+ 1*DataArray[BadPT+DatabitsLong+3] <=8)
			{//此為MixingMode
				 if(BadPT==0){
					if (tempans != NULL) {
						strcpy(ans,tempans);
					}
				 } else  {
					 if (tempans != NULL) {
						strcat(ans,tempans);
					 }
				 }
			}
			else 
			{
				//printf("Next DecodeMode: ");
				 if(BadPT==0){
					 if (tempans != NULL) {
						 strcpy(ans,tempans);
					 }
				 }
				 else{
					 if (tempans != NULL) {
						 strcat(ans,tempans);
					 }
				 }
			}
			BadPT=BadPT+DatabitsLong;
			//add by Joseph@20131213, free tempans 
			if (tempans != NULL) {
				free(tempans);
				tempans = NULL;
			}
		}

	/***************************** TBD ***********************************/
		//檢查是否到達終點 [MQR用]
		if(Version>40)
		{  
			//add by Joseph@20131213, free tempans
			//ans=tempans;	
			if (check_uQR_ending(ErrorLevel,BadPT+DatabitsLong))
			{
				 if(BadPT==0) strcpy(ans,tempans);
				 else         strcat(ans,tempans);
				 EndPT=2;
			} else 
			{
				 if(BadPT==0)
				 {
					 if (tempans != NULL) 
					 {
						 strcpy(ans,tempans);
					 }
				 }
				 else{
					 if (tempans != NULL)
					 {
						 strcat(ans,tempans);
					 }
				 }	    
			}
			BadPT=BadPT+DatabitsLong;
			if (tempans != NULL) 
			{
				free(tempans);
				tempans = NULL;
			}
		}
	}//解碼結束 while(EndPT==0) END
	return ans;
}



//////////////////////////////////////////////////////////////////////////////
//函式: RS2binary
//輸入: rx            (unsigned char):原始資料陣列
//      Datacount               (int):資料欄位的數量
//
//輸出: DataArray   (unsigned char *):處理完成的資料陣列
//
//說明:將原本以Codeeord儲存的資料陣列，轉回去2進制陣列的方式儲存
//     例如: 4,1,2,0 ... => 00000100 00000001 00000010 00000000 ...
//
//////////////////////////////////////////////////////////////////////////////
unsigned char *RS2binary(unsigned char *rx,int Datacount) 
{  
	unsigned char *DataArray = (unsigned char *)malloc(Datacount*8*sizeof(unsigned char));  //回傳標籤讀取結果
	int num=0;
	int temparray[8];

	for(int a=0;a<Datacount;a++)
	{
		num=rx[a];
		temparray[0]=num/128;
		temparray[1]=(num-temparray[0]*128)/64;
		temparray[2]=(num-temparray[0]*128-temparray[1]*64)/32;
		temparray[3]=(num-temparray[0]*128-temparray[1]*64-temparray[2]*32)/16;
		temparray[4]=(num-temparray[0]*128-temparray[1]*64-temparray[2]*32-temparray[3]*16)/8;
		temparray[5]=(num-temparray[0]*128-temparray[1]*64-temparray[2]*32-temparray[3]*16-temparray[4]*8)/4;
		temparray[6]=(num-temparray[0]*128-temparray[1]*64-temparray[2]*32-temparray[3]*16-temparray[4]*8-temparray[5]*4)/2;
		temparray[7]=(num-temparray[0]*128-temparray[1]*64-temparray[2]*32-temparray[3]*16-temparray[4]*8-temparray[5]*4-temparray[6]*2)/1;  

		for(int b=0;b<8;b++)
		DataArray[a*8+b]=temparray[b];
	}
	return DataArray;
}




//////////////////////////////////////////////////////////////////////////////
//3.5 DataArea的重新排序
//輸入: rx(unsigned char):原始資料陣列
//      MRT_V       (int):條碼等級(Version)
//      MRT_R       (int):容錯等級(L,M,Q,H)
//      DataCount   (int):資料區塊數量
//      ErrorCount  (int):容錯區塊數量
//
//輸出:TRA_t        (int):
//
//說明:將原始資料陣列進行重新排列，並進行RS碼的檢查，確認資料的正確
//
//    10/05: 已更新 Ver.20排列規則
//////////////////////////////////////////////////////////////////////////////
int TransArray(unsigned char *rx,int MRT_V,int MRT_R,int DataCount ,int ErrorCount ,unsigned char QRStructTable[40][32]) 
{  


	int RQ[3]={26,44,70};      //Version1,2,3 條碼資料區塊總數
	int pass_cnt1=0,pass_cnt2=0;
	int gba=0;      //計數用
	//int Fout[T2];  //排序表用陣列
	int type=0;     //處理模式
	memset(Fout, 0, T2);

	//根據版本與容錯等級，取得分組資訊:a(b,c,d),e(f,g,h) 
	int MRT_menu[8];
	for(int i=0;i<8;i++) 
		MRT_menu[i]=QRStructTable[MRT_V-1][8*MRT_R+i];   

	//特例: V1 V2l V3L, weight: V1 +2 ,L +1 ,V3,V2 +1 ,若Weight>=2 需特別處理
	int SpecialWei=0;
	if(MRT_V==1) 
		SpecialWei=SpecialWei+2;
	if(MRT_V==2 || MRT_V==3 ) 
		SpecialWei=SpecialWei++;
	if(MRT_R==0) 
		SpecialWei=SpecialWei++;



	//狀況0:最佳情況，不需更動順序
	if(MRT_menu[0]==1 && MRT_menu[4]==0)
		type=1;


	//狀況1:僅有一種有(Datacodeword,RScodeword)組合
	if(MRT_menu[0]>=2 && MRT_menu[4]==0)
	{
		 for(int i=1;i<=MRT_menu[2];i++)
		 {     
			for(int j=0;j<MRT_menu[0];j++)
			{
				Fout[gba]=i+j*MRT_menu[2];
				gba++;
			}
		 }
		  
		 for(int i=1;i<=MRT_menu[3]*2;i++)
		 {    
			 for(int j=0;j<MRT_menu[0];j++)
			 {
			     Fout[gba]=i+j*MRT_menu[3]*2;
			     gba++;
			 }
		  }
	}


	//狀況2: 有兩組以上的(Datacodeword,RScodeword)組合
	if(MRT_menu[0]!=0 && MRT_menu[4]!=0) //MRT_menu[6] 必大於 MRT_menu[2]
	{                 
		for(int i=1;i<=MRT_menu[6];i++)//取兩組Datacodeword長度的最大值 =MRT_menu[6]
		{                      
		     for(int j=0;j<MRT_menu[0]+MRT_menu[4];j++)
			 {       
		   
				 if(j<MRT_menu[0]){                               //第1組Datacodeword長度_MRT_menu[0]
				   if(i==MRT_menu[6] && j<MRT_menu[0]) {           //突出部分不處理
						//printf("");
						 i = i;
						 j = j;
				   }
				   else
				   {
						Fout[gba]=i+j*MRT_menu[2];         
					    gba++;
				   }
				 } 
			   
				 if(j>=MRT_menu[0]){                               //第1組Datacodeword長度_MRT_menu[4]
					   Fout[gba]=i+ MRT_menu[0]*MRT_menu[2]+ (j-MRT_menu[0])*MRT_menu[6];
					   gba++;
				 }  
			}
		}

	    for(int i=1;i<=MRT_menu[3]*2;i++) //RS碼
		{       
			for(int j=0;j<MRT_menu[0]+MRT_menu[4];j++)
			{
				 Fout[gba]=i+j*MRT_menu[3]*2;
				 gba++;
			}
		}

	}

	//----順序表建立完成,開始重新配置---
	//Fout[134]:排序表  rx[]:資料存放區塊
	//int MemberOut[T2];
	memset(MemberOut, 0, T2);


	if(type!=1)
	{
		//Data區位置重新排序
		for(int i=0;i<DataCount;i++)  
			MemberOut[Fout[i]-1]=rx[i];

		//RS區位置重新排序
		for(int i=DataCount;i<DataCount+ErrorCount;i++)  
			MemberOut[DataCount+Fout[i]-1]=rx[i];

		//資料置入rx[] ,回傳
		for(int i=0;i<DataCount + ErrorCount;i++)        
			rx[i]=MemberOut[i];
	}


	//////////////////////////////////////////////////////////////////////////////
	//4.1 確認資料完整性與正確性
	//
	//
	//////////////////////////////////////////////////////////////////////////////
	//int *CheckArray = (int *)malloc(160*sizeof(int));  //每組RS碼的最大容量
	memset(CheckArray,0,160);  //add by Joseph@20131213, move RS checkArray to global
	int Pt=0;                       //計數器
	int TRA=0;                       
	int TRA_t=0;                    //檢查RS是否修正
	int SavedataFlag=0;             //儲存資料用的指針



	//組1檢測:普通
	if(SpecialWei<2)
	for(int i=0;i<MRT_menu[0];i++)
	{

		//置入Datacode
		for(int a=0;a<MRT_menu[2];a++)
		{
		  CheckArray[Pt]=rx[a +i*MRT_menu[2]];
		  Pt++;
		}
	    
		//置入Rscode
		for(int b=0;b<MRT_menu[3]*2;b++)
		{
		  CheckArray[Pt]=rx[b +MRT_menu[0]*MRT_menu[2]+MRT_menu[4]*MRT_menu[6] +i*(2*MRT_menu[3]) ]; //MRT_menu[0]*MRT_menu[2]+MRT_menu[4]*MRT_menu[6] =RScode起始點位置 =正樣本總數  
		  Pt++;                                                                                      //2*MRT_menu[3]=每組RScode長度
		}
	    
		//RS檢查
		TRA=reed_solomon(CheckArray, MRT_menu[1],MRT_menu[2],QR_CODE);  //N全長 K訊息
		for(int a=0;a<MRT_menu[2];a++)
		{
			//rx[SavedataFlag]=CheckArray[a];
			if(TRA != 0) {
				//rx[SavedataFlag]=CheckArray[a];
	  		Decodeword[SavedataFlag]=CheckArray[a];
			}     		
			SavedataFlag++;
		}	
		if(TRA==0) 
		{
			TRA=667;
			//printf("\ng1 error serial=%d",i);
		} else {
			group1_pass_cnt[i]=1;
		}	
		TRA_t=TRA_t+TRA;

		//清空陣列
		memset(CheckArray,0,160);
		//for(int d=0;d<160;d++) CheckArray[d]=0;
		Pt=0; 
	}



	//組1檢測:特殊情況
	if(SpecialWei>=2)
	for(int i=0;i<MRT_menu[0];i++)
	{
		//置入Datacode
		for(int a=0;a<MRT_menu[2];a++){
		  CheckArray[Pt]=rx[a +i*MRT_menu[2]];
		  Pt++;
		}
	    
		//置入Rscode
		for(int b=0;b<RQ[MRT_V-1]-MRT_menu[2];b++){
		  CheckArray[Pt]=rx[b +MRT_menu[0]*MRT_menu[2]+MRT_menu[4]*MRT_menu[6] +i*(RQ[MRT_V-1]-MRT_menu[2]) ]; //MRT_menu[0]*MRT_menu[2]+MRT_menu[4]*MRT_menu[6] =RScode起始點位置 =正樣本總數  
		  Pt++;                                                                                                //RQ[MRT_V-1]-MRT_menu[2]=每組RScode長度
		}
	    
		//RS檢查
		TRA=reed_solomon(CheckArray, MRT_menu[1],MRT_menu[2],QR_CODE);
		for(int a=0;a<MRT_menu[2];a++){
			if (TRA != 0) {
				//rx[SavedataFlag]=CheckArray[a];
				Decodeword[SavedataFlag]=CheckArray[a];
		}
		SavedataFlag++;
		}
		if(TRA==0) {
		TRA=667;
		//printf("\ng1 error serial=%d",i);
		} else {
			group1_pass_cnt[i]=1;
		}
		TRA_t=TRA_t+TRA;

		//清空陣列
		memset(CheckArray,0,160);
		//for(int d=0;d<160;d++) CheckArray[d]=0;
		Pt=0;
	}



	//組2檢測:取後四位,若MRT_menu[4]=0則不做
	for(int i=0;i<MRT_menu[4];i++){
		//置入Datacode
		for(int a=0;a<MRT_menu[6];a++){
		  CheckArray[Pt]=rx[a +MRT_menu[0]*MRT_menu[2] +i*MRT_menu[6]];   //起始點:第一組Datacode數據之後
		  Pt++;
		}
	    
		//置入Rscode
		for(int b=0;b<MRT_menu[7]*2;b++){
		  CheckArray[Pt]=rx[b +MRT_menu[0]*MRT_menu[2]+MRT_menu[4]*MRT_menu[6]+2*MRT_menu[0]*MRT_menu[3] +i*(2*MRT_menu[7]) ];  	  //起始點:第一組RScode數據之後
		  Pt++;                                                                                                                       //第2段RScode起始點位置 =正樣本總數 + 第1段RScode=MRT_menu[0]*MRT_menu[2] + MRT_menu[4]*MRT_menu[6] + 2*MRT_menu[0]*MRT_menu[3]
		}

		//RS檢查
		TRA=reed_solomon(CheckArray, MRT_menu[5],MRT_menu[6],QR_CODE);
		for(int a=0;a<MRT_menu[6];a++){
			//rx[SavedataFlag]=CheckArray[a];
			if (TRA != 0) {
				//rx[SavedataFlag]=CheckArray[a];
				Decodeword[SavedataFlag]=CheckArray[a];
			}		
				SavedataFlag++;
		}
		if(TRA==0) {
			TRA=667;
			//printf("\ng1 error serial=%d",i);
		} else {
			group2_pass_cnt[i]=1;
		}
		TRA_t=TRA_t+TRA;
	    
		//清空陣列
		memset(CheckArray,0,160);
		//for(int d=0;d<160;d++) CheckArray[d]=0;
		Pt=0;

	}

	for (int i=0;i<70;i++)
	{
		pass_cnt1+=group1_pass_cnt[i];
		pass_cnt2+=group2_pass_cnt[i];
	}

	if ((MRT_menu[0]==pass_cnt1) && ( MRT_menu[4]==pass_cnt2) && (fail_frame_cnt >= 1)) 
	{	
		if (TRA_t >= 667) 
		{
			TRA_t = 555;
		}	
	}
	    
	return TRA_t;
}

//////////////////////////////////////////////////////////////////////////////
//函式: GetFormatInformation    Ver.2
//輸入: rx(unsigned char) :原始資料陣列
//      XORmask[]         :XOR使用的遮罩
//      ModulesNum        :module size
//輸出: ans(unsigned char):固定長度15,該條碼格式區塊的資訊
//說明:
//
//    
//////////////////////////////////////////////////////////////////////////////
unsigned char *GetFormatInformation(unsigned char datamap[177][177],int XORmask[],unsigned char QRFormatTable[32][15],int ModulesNum)
{
	unsigned char *ans = (unsigned char *)malloc(15*sizeof(unsigned char));
    int temp=0;

	for(int i=0;i<=8;i++)   //讀取左上直線(0~7)
	{
		 if(i<6)         
			 ans[i]=datamap[i][8];
			 else if(i==6)   
				 temp=0;
			 else if(i>=7)  
				 ans[i-1]=datamap[i][8];
	}


	for(int j=0	;j<=8;j++)   //讀取左上橫線(7~14)
	{
		 if(j==7)   	   
			 ans[j+1]=datamap[8][j];
			 else if(j==6)     
				 temp=0;
			 else if(j<=5)    
				  ans[14-j]=datamap[8][j];
	}


	for(int i=0;i<=14;i++){  
		 if(ans[i]==255) 	
			ans[i]=0;
		  else             	
			ans[i]=1;
	}


    //錯誤糾正
	int ErrorN=0,ErrorLM=99,GoodPT=0;
	for(int a=0;a<32;a++)
	{
		  for(int b=0;b<15;b++)  
			  if(ans[14-b]!=QRFormatTable[a][b])  
				  ErrorN++;
		  if(ErrorN<ErrorLM)
		  { 
				  GoodPT=a;
				  ErrorLM=ErrorN;
	      }
		  ErrorN=0;
	}

	if(ErrorLM<=3) 	
		for(int a=0;a<15;a++) 
			ans[14-a]=QRFormatTable[GoodPT][a];
	else            
		for(int a=0;a<15;a++) 
			ans[a]=0;                             //若錯誤太多 則清空陣列
	  
	if (ErrorLM >3) 
	{
		for(int i=0;i<=7;i++) 
		{  //讀取右上橫線(0~7)
            ans[i]=datamap[8][ModulesNum-i-1];
		}
		for(int i=0;i<=6;i++)
		{ //讀取左下橫線(8~14)
            ans[8+i]=datamap[ModulesNum-7+i][8];
		}
	    for(int i=0;i<=14;i++)
		{  
	        if(ans[i]==255) 	 
				ans[i]=0;
	         else             	 
				ans[i]=1;
	    }
	    ErrorN=0,ErrorLM=99,GoodPT=0;
	    for(int a=0;a<32;a++)
		{
			for(int b=0;b<15;b++)
			{
				if(ans[14-b]!=QRFormatTable[a][b])  
					ErrorN++;
			}
	        if(ErrorN<ErrorLM)
			{ 
                GoodPT=a;
		        ErrorLM=ErrorN;
	        }
	        ErrorN=0;
		}
	    if(ErrorLM<=3) 	
			for(int a=0;a<15;a++) 
				ans[14-a]=QRFormatTable[GoodPT][a];
	    else            
			for(int a=0;a<15;a++)
				ans[a]=0;                             //若錯誤太多 則清空陣列
	}

    //將資訊區塊進行XOR運算
	for(int k=0;k<=14;k++){                             
		if(ans[k]==XORmask[k])  
			ans[k]=0;
		else                   
			ans[k]=1;
	}

    return ans;

}


void get_structured_append_info(unsigned char DataArray[],int ModeIndicatorLength,int BadPT)
{
	int pos=ModeIndicatorLength+BadPT;
	structured_current_cnt = (DataArray[pos]<<3)+(DataArray[pos+1]<<2)+(DataArray[pos+2]<<1)+DataArray[pos+3]+1;
    structured_total_cnt = (DataArray[pos+4]<<3)+(DataArray[pos+5]<<2)+(DataArray[pos+6]<<1)+DataArray[pos+7]+1;
	pos+=8;	
	structured_parity = (DataArray[pos]<<7)+(DataArray[pos+1]<<6)+(DataArray[pos+2]<<5)+(DataArray[pos+3]<<4)+
		                (DataArray[pos+4]<<3)+(DataArray[pos+5]<<2)+(DataArray[pos+6]<<1)+(DataArray[pos+7]);
}

//////////////////////////////////////////////////////////////////////////////
//函式:NumericMode
//輸入: DataArray        (原始Bitstream)
//      CharacterCountNum(字元數量)
//      CharacterCount   (標頭檔長度)
//      NumericTable     (Numeric字元表) 
//     
//輸出: char *ans
//
//說明: 解碼_數字模式, 三個digit編成10bit, 餘下一個digit: 4bit, 餘下兩個digit:7bit
//    
//////////////////////////////////////////////////////////////////////////////
char *NumericMode(unsigned char DataArray[],int CharacterCountNum,int CharacterCount ,char NumericTable[],int BadPT,int ModeIndicatorLength)
{

	int R=0; 
    int Blocknum;
    //int DataString[800];  //mark by Joseph@20131210, use ans2 array 	
	int n1,n2,n3;
	int DatabitsLong;
    char *ans2=NULL;   

	//printf("[Numeric mode]  \n");
	//printf("此處的資料區塊總位元長度計算公式:B=4+C+10(D % 3)+R \n");
	//printf("B:資料總位元長度  C:字數統計符位數  D:輸入字元的數量     \n");

	if(CharacterCountNum%3==2)        R=7;                                       //計算R值:若無法整除，餘兩位，R=7 
    else if(CharacterCountNum%3==1)   R=4;                                       //計算R值:若無法整除，餘一位
	DatabitsLong= ModeIndicatorLength + CharacterCount + 10*(CharacterCountNum / 3) + R;           //計算資料區塊總長度為 n 個bit
	if (CharacterCountNum==0) return NULL;
	ans2=(char *)malloc((CharacterCountNum+2)*sizeof(char));                      //對ans指標設定記憶體區塊大小 [備註:外部回傳用] 
    int Blockbits = DatabitsLong-CharacterCount-ModeIndicatorLength;  
    int PT=CharacterCount+ModeIndicatorLength+BadPT;
   
   //開始解碼
   int Flag1=0;


   Blockbits=(Blockbits/10);

   for(int a=0;a<Blockbits;a++)
   {
	   Blocknum= 512*DataArray[PT+0+a*10]+256*DataArray[PT+1+(a*10)]
             +128*DataArray[PT+2+(a*10)]+64*DataArray[PT+3+(a*10)]
			 +32*DataArray[PT+4+(a*10)]+16*DataArray[PT+5+(a*10)]
			 +8*DataArray[PT+6+(a*10)]+4*DataArray[PT+7+(a*10)]
			 +2*DataArray[PT+8+(a*10)]+1*DataArray[PT+9+(a*10)];

		n1=Blocknum/100;
		//DataString[Flag1]=n1;
		ans2[Flag1] = n1;
		Flag1++;

		n2=(Blocknum-(100*n1))/10;
		//DataString[Flag1]=n2;
		ans2[Flag1] = n2;
		Flag1++;

		n3=Blocknum-(100*n1)-(10*n2);
		//DataString[Flag1]=n3;
		ans2[Flag1] = n3;
		Flag1++;
   }
    
   if(R==4)
   {
		Blockbits=Blockbits*10;
		Blocknum= 8*DataArray[PT+Blockbits]  +4*DataArray[PT+Blockbits+1]+2*DataArray[PT+Blockbits+2]+1*DataArray[PT+Blockbits+3];
  	 
		//DataString[Flag1]=Blocknum;
		ans2[Flag1] = Blocknum;
		Flag1++;   	//0426修正
   }
   else if(R==7)
   {
	   Blockbits=Blockbits*10;
	   Blocknum = 64*DataArray[PT+Blockbits]  +32*DataArray[PT+Blockbits+1]+16*DataArray[PT+Blockbits+2] + 8*DataArray[PT+Blockbits+3]+ 4*DataArray[PT+Blockbits+4]+ 2*DataArray[PT+Blockbits+5] + 1*DataArray[PT+Blockbits+6];
     
	   //DataString[Flag1]=Blocknum/10;   	 //0426修正
	   ans2[Flag1]=Blocknum/10;
	   Flag1++;
	   //DataString[Flag1]=Blocknum%10;
	   ans2[Flag1]=Blocknum%10;
	   Flag1++;
   }    
  
   //char temp;   //mark by Joseph@20131210, use ans2 array, digit -> ascii
   for(int a=0;a<Flag1;a++){
       ans2[a] = ans2[a] + 0x30;
   }   

   ans2[Flag1]='\0';

 return ans2;
}

//////////////////////////////////////////////////////////////////////////////
//函式:AlphanumericMode
//輸入: DataArray            (原始Bitstream)
//      CharacterCountNum    (字元數量)
//      CharacterCount       (標頭檔長度)
//      AlphanumericTable    (AlphaNumeric字元表) 
//     
//輸出: char *ans
//
//說明: 解碼_數字符號模式, 解碼成數字(0~44)之後再查出對應的字元(0~9,A~Z, others)
//    
//////////////////////////////////////////////////////////////////////////////
char *AlphanumericMode(unsigned char DataArray[],int CharacterCountNum,int CharacterCount ,char AlphanumericTable[],int BadPT,int ModeIndicatorLength)
{
    
	int R=0; //資料分組,記數用
    int Blocknum;
    //int DataString[800]; //mark by Joseph@20131210, dynamic allocate temp array       
    int* DataString = NULL;
	int n1,n2,Flag1=0;
	int DatabitsLong;
    char *ans2=NULL;   

	//printf("[Alphanumeric Mode]  \n");
	//printf("此處的資料區塊總位元長度計算公式:B=4+C+11(D / 2)+6(D % 2)  \n");
	//printf("B:資料總位元長度  C:字數統計符位數  D:輸入字元的數量     \n");

	DatabitsLong=ModeIndicatorLength + CharacterCount + 11*(CharacterCountNum / 2)+6*(CharacterCountNum % 2);	
	//int Blockbits = DatabitsLong-CharacterCount-4;  //更新! 模式指示符長度(ModeIndicatorLength)並非全部皆為4,MicroQR的長度為動態
	int Blockbits = DatabitsLong-CharacterCount-ModeIndicatorLength;
	int PT=CharacterCount+ModeIndicatorLength+BadPT;
    if (CharacterCountNum==0) return NULL;
    ans2=(char *)malloc((CharacterCountNum+2)*sizeof(char));    //對ans指標設定記憶體區塊大小 [AlphaNumeric]
    DataString=(int *)malloc((CharacterCountNum+2)*sizeof(int));  //add by Joseph@20131210, dynamic allocate temp array
    R=(Blockbits/11);                                  
   
    for(int a=0;a<R;a++)
	{
		Blocknum= 1024*DataArray[PT+0+(a*11)]+512*DataArray[PT+1+(a*11)]+256*DataArray[PT+2+(a*11)]
             + 128*DataArray[PT+3+(a*11)]+ 64*DataArray[PT+4+(a*11)]+ 32*DataArray[PT+5+(a*11)]
			 +  16*DataArray[PT+6+(a*11)]+  8*DataArray[PT+7+(a*11)]+  4*DataArray[PT+8+(a*11)]
			 +   2*DataArray[PT+9+(a*11)]+  1*DataArray[PT+10+(a*11)];

		if(Blocknum<0 || Blocknum>2024) 
			Blocknum=0;  //範圍介於0 ~ 2^11
		
		n1=Blocknum/45;
		n2=Blocknum%45;
	    

		if(n1<45 && n2<45 )
		{
			DataString[Flag1]  =AlphanumericTable[n1];
			DataString[Flag1+1]=AlphanumericTable[n2];
			Flag1=Flag1+2;	
		}

   }

   if(Blockbits%11 !=0)                          //若無法整除，建立[6]
   {
	   R=R*11;
	   Blocknum=  32*DataArray[PT+R]  +16*DataArray[PT+R+1]+8*DataArray[PT+R+2] + 4*DataArray[PT+R+3]+ 2*DataArray[PT+R+4]+1*DataArray[PT+R+5];  
    
	   if(Blocknum<0 || Blocknum>64) 
		   Blocknum=0;   //範圍介於0 ~ 2^11
	   DataString[Flag1]=AlphanumericTable[Blocknum];
   }
   else
		Flag1--;


   for(int a=0;a<=Flag1;a++)    
	   ans2[a]=DataString[a];
   
   ans2[Flag1+1]='\0';

   //add by Joseph@20131210, free the temp array
   if (DataString != NULL) 
   {
       free(DataString);
       DataString = NULL;
   }

	return ans2;
}


//////////////////////////////////////////////////////////////////////////////
//函式: 8bitMode 電子發票用
//輸入: DataArray            (原始Bitstream)
//      CharacterCountNum    (字元數量)
//      CharacterCount       (標頭檔長度)
//     
//輸出: char *ans 
//
//說明: 解碼_8bitMode模式 ,全部顯示為ASCII碼 
//    
//////////////////////////////////////////////////////////////////////////////
char *BabitModeTicket(unsigned char DataArray[],int CharacterCountNum,int CharacterCount,int BadPT,int ModeIndicatorLength){

    int R=0; //資料分組,記數用
    int Blocknum=0,Blocknum2=0;  
	int n1=0,n2=0,Flag1=0;
	int DatabitsLong=0;
    char *ans=NULL;   

	//printf("[8bitsBinary Mode]  \n");
	//printf("此處的資料區塊總位元長度計算公式:B=4+C+8D  \n");
	//printf("B:資料總位元長度  C:字數統計符位數  D:輸入字元的數量     \n");

    DatabitsLong=ModeIndicatorLength + CharacterCount + (8 * CharacterCountNum);	//8bitMode用
	int Blockbits = DatabitsLong-CharacterCount-ModeIndicatorLength;
	int PT=ModeIndicatorLength+CharacterCount+BadPT;
    if (CharacterCountNum==0) return NULL;
	ans=(char*)malloc(sizeof(char)*CharacterCountNum+2);      //對ans指標設定記憶體區塊大小 

	//解碼                         
	R=(Blockbits/8);  
	for(int a=0;a<=R-1;a++)
	{   Blocknum = 8*DataArray[PT+0+(a*8)]+4*DataArray[PT+1+(a*8)]+2*DataArray[PT+2+(a*8)]+1*DataArray[PT+3+(a*8)];
        Blocknum2= 8*DataArray[PT+4+(a*8)]+4*DataArray[PT+5+(a*8)]+2*DataArray[PT+6+(a*8)]+1*DataArray[PT+7+(a*8)];

        if ((Blocknum == 0) && (Blocknum2 == 0)) {
        }else {
		    ans[Flag1]=Blocknum*16+Blocknum2;
		    Flag1++;	
		}
		
	}  
    ans[Flag1]='\0';
	return ans;
}

//////////////////////////////////////////////////////////////////////////////
//函式: GetCharacterCountIndicator 取得字數統計符長度
//輸入: DataArray            (原始Bitstream)
//      CharacterCountNum    *(字元數量)
//      CharacterCount       *(標頭檔長度)
//      Version              (條碼版本等級) 
//      BadPT                (位移指針)
//      IMnum                *(編碼模式) 
//輸出:  *IMnum, *CharacterCount,int ,*CharacterCountNum
//
//說明:   [0731更新]加入KanjiMode相關資料
//    
//////////////////////////////////////////////////////////////////////////////
void *GetCharacterCountIndicator(unsigned char DataArray[],int Version,int BadPT,int *IMnum,int *CharacterCount,int *CharacterCountNum)
{

	int IM[4];  //IndicatorsMode 資料編碼代號
	int CharacterCountTable[3][4]={{10, 9, 8,8},
								   {12,11,16,10},
								   {14,13,16,12}};
	for(int a=0;a<4;a++) 
		IM[a]=DataArray[BadPT+a];

	*IMnum=8*IM[0]+4*IM[1]+2*IM[2]+IM[3];
	//檢查標籤版本與編碼模式，決定字數統計符位數
	//step.1 根據Version與資料編碼模式，決定字數統計符位數
	int tempA,tempB;
	switch(*IMnum)
	{
	case 1:
		 tempB=0;
		 if(Version<=9)
		   tempA=0;
		 else if(Version>=27)
		   tempA=2;
		 else
		   tempA=1;
		 break;

	case 2:
		 tempB=1;
		 if(Version<=9)
		   tempA=0;
		 else if(Version>=27)
		   tempA=2;
		 else
		   tempA=1;
		 break;

	case 4:
		 tempB=2;
		 if(Version<=9)
		   tempA=0;
		 else if(Version>=27)
		   tempA=2;
		 else
		   tempA=1;
		 break;

	case 8: //KanjiMode
		 tempB=3;
		 if(Version<=9)
		   tempA=0;
		 else if(Version>=27)
		   tempA=2;
		 else
		   tempA=1;
		 break;
	case 3:
		return 0;
	case 7: //ECI mode
		return 0;
	case 5:
		return 0;
	default: 
	   tempA=0;
	   tempB=0;
	   *IMnum=99;   //驗證:若為非正規值 => Error 
	   return 0;    //add by Joseph@20131119, if we meet unsupported mode, skip following process
	}

	//if(EncodeMode==99)  IMnum=99;  //驗證:排除違法項目,重點

	//取得字數統計符(CharacterCount)長度
	*CharacterCount = CharacterCountTable[tempA][tempB];   

	//計算該條碼的字元數量
	*CharacterCountNum = 3 + *CharacterCount;
	for(int a=0;a<16;a++) 
		CharacterCountTemp[a]=0;  //使用前陣列先清空


	for(int a=0;a < *CharacterCount ;a++)
		CharacterCountTemp[a] = DataArray[BadPT+*CharacterCountNum-a];  
	*CharacterCountNum=(1*CharacterCountTemp[0])+(2*CharacterCountTemp[1])+(4*CharacterCountTemp[2])+(8*CharacterCountTemp[3])+(16*CharacterCountTemp[4])+(32*CharacterCountTemp[5])
					  +(64*CharacterCountTemp[6])+(128*CharacterCountTemp[7])+(256*CharacterCountTemp[8])+(512*CharacterCountTemp[9])+(1024*CharacterCountTemp[10])+(2048*CharacterCountTemp[11])+(4096*CharacterCountTemp[12]); //add by Joseph@20131210, fixed bug when chars over 2047

	return 0;
}


//////////////////////////////////////////////////////////////////////////////
//3.5 DataArea的重新排序(MicroQR)
//輸入: rx(unsigned char):原始資料陣列
//      MRT_V       (int):條碼等級(Version)
//      MRT_R       (int):MicroQR的分組編號
//
//輸出:TRA_t        (int):
//////////////////////////////////////////////////////////////////////////////
int MicroQRTransArray(unsigned char *rx,int MRT_V,int MRT_R,int DataCount ,int ErrorCount) 
{  

	//MQVersion範圍:1~8 =MRT_V
	int MicroQRStructTable[9][8]={{0,  0,  0, 0,0,0,0,0 },   //留白區
								  {1,  5,  3, 0,0,0,0,0 },   //M1
								  {1, 10,  5, 1,0,0,0,0 },   //M2_L
								  {1, 10,  4, 2,0,0,0,0 },   //M2_M
								  {1, 17, 11, 2,0,0,0,0 },   //M3_L
								  {1, 17,  9, 4,0,0,0,0 },   //M3_M
								  {1, 24, 16, 3,0,0,0,0 },   //M4_L
								  {1, 24, 14, 5,0,0,0,0 },   //M4_M
								  {1, 24, 10, 7,0,0,0,0 }};  //M4_Q
	//M1,M2,M3,M4 條碼資料區塊總數
	int RQ[9]={0,5,10,10,17,17,24,24,24};      

	int gba=0;      //計數用
	//int Fout[T2];  //排序表用陣列
	int type=0;     //處理模式




	//根據版本與容錯等級，取得分組資訊:a(b,c,d),e(f,g,h) 
	int MRT_menu[8];
	for(int i=0;i<8;i++) 
		MRT_menu[i]=MicroQRStructTable[MRT_V][i];   


	//狀況0:最佳情況，不需更動順序
	if(MRT_menu[0]==1 && MRT_menu[4]==0)
		type=1;

	//////////////////////////////////////////////////////////////////////////////
	//4.1 確認資料完整性與正確性
	//////////////////////////////////////////////////////////////////////////////
	
	memset(CheckArray,0,160*4); //add by Joseph@20131213, move RS checkArray to global
	int Pt=0;                       //計數器
	int TRA=0;                       
	int TRA_t=0;                    //檢查RS是否修正
	int SavedataFlag=0;             //儲存資料用的指針



	//組1檢測:特殊情況
	//if(SpecialWei>=2)
	for(int i=0;i<MRT_menu[0];i++)
	{
		//置入Datacode
		for(int a=0;a<MRT_menu[2];a++)
		{
			CheckArray[Pt]=rx[a +i*MRT_menu[2]];
			Pt++;
		}
	    
		//置入Rscode

		for(int b=0;b<RQ[MRT_V]-MRT_menu[2];b++)
		{
			CheckArray[Pt]=rx[b +MRT_menu[0]*MRT_menu[2]+MRT_menu[4]*MRT_menu[6] +i*(RQ[MRT_V]-MRT_menu[2]) ]; //MRT_menu[0]*MRT_menu[2]+MRT_menu[4]*MRT_menu[6] =RScode起始點位置 =正樣本總數  
			Pt++;                                                                                                //RQ[MRT_V-1]-MRT_menu[2]=每組RScode長度
		}
	    
		//RS檢查
		TRA=reed_solomon(CheckArray, MRT_menu[1],MRT_menu[2],QR_CODE);
		//擋掉micro QR M1完全不能修卻會解碼的部分
		if(MRT_V == 1 && TRA != 2)
			TRA = 0;

		for(int a=0;a<MRT_menu[2];a++)
		{
			rx[SavedataFlag]=CheckArray[a];
			SavedataFlag++;
		}
		
		if(TRA==0) 
			TRA=667;	
		TRA_t=TRA_t+TRA;

		//清空陣列
		for(int d=0;d<160;d++) 
			CheckArray[d]=0;
		Pt=0;
	}

	return TRA_t;
}

//////////////////////////////////////////////////////////////////////////////
//函式: GetFormatInformation [MicroQR用]
//輸入: rx(unsigned char) :原始資料陣列
//      XORmask[]         :XOR使用的遮罩
//輸出: ans(unsigned char):固定長度15,該條碼格式區塊的資訊
//說明:
//
//    
//////////////////////////////////////////////////////////////////////////////
unsigned char *GetFormatInformationMQR(unsigned char datamap[177][177],int XORmask[])
{
	unsigned char *ans = (unsigned char *)malloc(15*sizeof(unsigned char));

/*
	if (segmation_mode == QR_PATTERN_MODE) 
	{	
		for(int a=0;a<15;a++) 
			ans[14-a]=uQR_FormatTable[last_version][a];
		for(int k=0;k<=14;k++)
		{                             //將資訊區塊進行XOR運算
			if(ans[k]==XORmask[k])  
				ans[k]=0;
			else                   
				ans[k]=1;
		}
		return ans;
	}
*/
	for(int i=1;i<=8;i++)  
		ans[i-1]=datamap[i][8];//讀取左上直線(1~8)

	for(int j=1;j<=8;j++)  
		ans[15-j]=datamap[8][j]; //讀取左上橫線(8~14)



	for(int i=0;i<=14;i++)
	{  
		 if(ans[i]==255) 	 
			 ans[i]=0;
		 else             	 
			 ans[i]=1;
	}
    //BCH error correct
	int ErrorN=0,ErrorLM=99,GoodPT=0;
	for(int a=0;a<32;a++)
	{
	  for(int b=0;b<15;b++)  
		  if(ans[14-b]!=uQR_FormatTable[a][b])  
			  ErrorN++;
		  if(ErrorN<ErrorLM){ 
			  GoodPT=a;
			  ErrorLM=ErrorN;
	  }
	  ErrorN=0;
	}
	if(ErrorLM<=3) 	
		for(int a=0;a<15;a++) 
			ans[14-a]=uQR_FormatTable[GoodPT][a];
	else            
		for(int a=0;a<15;a++) 
			ans[a]=0;  

	for(int k=0;k<=14;k++)
	{                           
		if(ans[k]==XORmask[k])  
			ans[k]=0;
		else                   
			ans[k]=1;
	}

    return ans;
}

//////////////////////////////////////////////////////////////////////////////
//函式: GetCharacterCountIndicatorMQR 取得字數統計符長度[MicroQR用]
//輸入: DataArray            (原始Bitstream)
//      CharacterCountNum    *(字元數量)
//      CharacterCount       *(標頭檔長度)
//      IMnum                *(編碼模式) 
//      BadPT                (位移指針)
//      ErrorLevel           (MicroQR條碼等級 ,重點)
//      ModeIndicatorLength  *(模式指示符長度)
//輸出:  *IMnum, *CharacterCount,int ,*CharacterCountNum
//
//說明: M1僅有Numeric , M2僅有Numeric與AlphaNumeric，M3與M4則有完整的4種編碼模式
//    
//////////////////////////////////////////////////////////////////////////////
void *GetCharacterCountIndicatorMQR(unsigned char DataArray[],int ErrorLevel,int BadPT,int *IMnum,int *CharacterCount,int *CharacterCountNum,int *ModeIndicatorLength)
{
	int tempN=0;                        //判斷該條碼為何種編碼模式
	switch(ErrorLevel)
	{
	case 0://M1_X
		 *ModeIndicatorLength=0;
		 *CharacterCount=3;
		 *IMnum=1;                          //M1僅有Numeric模式
	break;

	case 1: //M2_L
		*ModeIndicatorLength=1;
		if(DataArray[BadPT]==0){ 
			 *CharacterCount=4;
			 *IMnum=1;
		}
		 if(DataArray[BadPT]==1){ 
			 *CharacterCount=3;
			 *IMnum=2;
		 }
	break;

	case 2: //M2_m
		*ModeIndicatorLength=1;
		 if(DataArray[BadPT]==0)
		 { 
			 *CharacterCount=4;
			 *IMnum=1;
		 }
		 if(DataArray[BadPT]==1)
		 { 
			 *CharacterCount=3;
			 *IMnum=2;
		 }
	 break;

	case 3: //M3_L
		 *ModeIndicatorLength=2;
		 tempN=2*DataArray[BadPT]+1*DataArray[BadPT+1];
		 switch(tempN)
		 {
		 case 0:
  			 *CharacterCount=5;
			 *IMnum=1;
			break;
		 case 1:
  			 *CharacterCount=4;
			 *IMnum=2;
			break;
		 case 2:
  			 *CharacterCount=4;
			 *IMnum=4;
			break;
		 case 3:
  			 *CharacterCount=3;
			 *IMnum=8;
			break;
		 }
	 break;

	case 4: //M3_M
		 *ModeIndicatorLength=2;
		 tempN=2*DataArray[BadPT]+1*DataArray[BadPT+1];
		 switch(tempN)
		 {
		 case 0:
  			 *CharacterCount=5;
			 *IMnum=1;
			 break;
		 case 1:
  			 *CharacterCount=4;
			 *IMnum=2;
			 break;
		 case 2:
  			 *CharacterCount=4;
			 *IMnum=4;
			 break;
		 case 3:
  			 *CharacterCount=3;
			 *IMnum=8;
			 break;
		 }
	 break;

	case 5: //M4_L
		 *ModeIndicatorLength=3;
		 tempN=4*DataArray[BadPT]+2*DataArray[BadPT+1]+1*DataArray[BadPT+2];
		 switch(tempN)
		 {
		 case 0:
  			 *CharacterCount=6;
			 *IMnum=1;
			 break;
		 case 1:
  			 *CharacterCount=5;
			 *IMnum=2;
			 break;
		 case 2:
  			 *CharacterCount=5;
			 *IMnum=4;
			 break;
		 case 3:
  			 *CharacterCount=4;
			 *IMnum=8;
			 break;
		 }
	break;

	case 6: //M4_m
		 *ModeIndicatorLength=3;
		 tempN=4*DataArray[BadPT]+2*DataArray[BadPT+1]+1*DataArray[BadPT+2];
		 switch(tempN)
		 {
		 case 0:
  			 *CharacterCount=6;
			 *IMnum=1;
			 break;
		 case 1:
  			 *CharacterCount=5;
			 *IMnum=2;
			 break;
		 case 2:
  			 *CharacterCount=5;
			 *IMnum=4;
			 break;
		 case 3:
  			 *CharacterCount=4;
			 *IMnum=8;
			 break;
		 }
	 break;

	case 7:  //M4_Q
		 *ModeIndicatorLength=3;
		 tempN=4*DataArray[BadPT]+2*DataArray[BadPT+1]+1*DataArray[BadPT+2];
		 switch(tempN)
		 {
		 case 0:
  			 *CharacterCount=6;
			 *IMnum=1;
			 break;
		 case 1:
  			 *CharacterCount=5;
			 *IMnum=2;
			 break;
		 case 2:
  			 *CharacterCount=5;
			 *IMnum=4;
			 break;
		 case 3:
  			 *CharacterCount=4;
			 *IMnum=8;
			 break;
		 }
	 break;
	}


	//if(*IMnum==1) printf("Encode Mode: Numeric Mode\n");
	//if(*IMnum==2) printf("Encode Mode: AlphaNumeric Mode\n");
	//if(*IMnum==4) printf("Encode Mode: Byte Mode\n");
	//if(*IMnum==8) printf("Encode Mode: Kanji Mode\n");

	//if(EncodeMode==99)  IMnum=99;  //驗證:排除違法項目,重點

	//取得字數統計符(CharacterCount)長度
	//*CharacterCount = CharacterCountTable[tempA][tempB];   

	//計算該條碼的字元數量
	*CharacterCountNum = (*ModeIndicatorLength-1) + *CharacterCount;
	for(int a=0;a<16;a++) 
		CharacterCountTemp[a]=0;  //使用前陣列先清空


	for(int a=0;a < *CharacterCount ;a++)
		CharacterCountTemp[a] = DataArray[BadPT+*CharacterCountNum-a];  

	*CharacterCountNum=(1*CharacterCountTemp[0])+(2*CharacterCountTemp[1])+(4*CharacterCountTemp[2])+(8*CharacterCountTemp[3])+(16*CharacterCountTemp[4])+(32*CharacterCountTemp[5])
					  +(64*CharacterCountTemp[6])+(128*CharacterCountTemp[7])+(256*CharacterCountTemp[8])+(512*CharacterCountTemp[9])+(1024*CharacterCountTemp[10]);

	return 0;
}


//////////////////////////////////////////////////////////////////////////////
//函式: KanjiMode             日文漢字模式
//輸入: DataArray            (原始Bitstream)
//      CharacterCountNum    (字元數量)
//      CharacterCount       (標頭檔長度)
//      ModeIndicatorLength  (模式指示符長度)
//
//輸出: char *ans
//
//說明: 解碼_漢字符號模式
//    
//////////////////////////////////////////////////////////////////////////////
char *KanjiMode(unsigned char DataArray[],int CharacterCountNum,int CharacterCount ,int BadPT,int ModeIndicatorLength)
{
    int Blocknum;
	int Blocknum2;
	int Flag1=0,ALC=0;
	int temp[4];  
	int  *ans =NULL;
	char *ans2=NULL;   
	//char TransBuf[2];

	//printf("[Kanji Mode]  \n");
	//printf("此處的資料區塊總位元長度計算公式:B = 4(模式指示符) + C + 13D  \n");
	//printf("B:資料總位元長度  C:字數統計符位數  D:輸入字元的數量     \n");
	//DatabitsLong=ModeIndicatorLength + CharacterCount + 13*CharacterCountNum;	
	int PT=CharacterCount+ModeIndicatorLength+BadPT;                                 //處理指針
    if (CharacterCountNum==0) return NULL;
   
    /*
	    use Windows-1252 to display, need double size, AACC => AA CC,
		if display correct number one by one, it need 4 times size, AACC=> A A C C
	*/ 
    ans2 =(char *)malloc((2*CharacterCountNum+1)*sizeof(char)); 
	for(int a=0;a<CharacterCountNum;a++)
	{
		//step.1 共13位數的二進制碼
		Blocknum=   4096* DataArray[PT+0+(a*13)]                                                                                  //13_1
			      +  256*(8*DataArray[PT+1+(a*13)]+4*DataArray[PT+2+(a*13)]+2*DataArray[PT+3+(a*13)]+DataArray[PT+4+(a*13)])      //13_2~5
				  +   16*(8*DataArray[PT+5+(a*13)]+4*DataArray[PT+6+(a*13)]+2*DataArray[PT+7+(a*13)]+DataArray[PT+8+(a*13)])      //13_6~9
				  +    1*(8*DataArray[PT+9+(a*13)]+4*DataArray[PT+10+(a*13)]+2*DataArray[PT+11+(a*13)]+DataArray[PT+12+(a*13)]);  //13_10~13
	
		//step.2 同除以 C0(Hex) = 192 取得A,B值後計算總和。此處使用temp[4]來暫存結果
		temp[0]= (Blocknum/192)/16;
		temp[1]= (Blocknum/192)%16;
		temp[2]= (Blocknum%192)/16;
		temp[3]= (Blocknum%192)%16;
		Blocknum2= 4096*temp[0]+256*temp[1]+16*temp[2]+1*temp[3];

		//step.3 檢查該字元是屬於JIS X 0208表格的哪個區域
		ALC=Blocknum2+49472;
		if(57408<=ALC && ALC<=60351)      
			Blocknum2=ALC;
		else  Blocknum2=Blocknum2+33088;
	}
    ans2[2*CharacterCountNum]='\0';
	return ans2;

}



//////////////////////////////////////////////////////////////////////////////
//函式: 8bitMode 一般模式 
//輸入: DataArray            (原始Bitstream)
//      CharacterCountNum    (字元數量)
//      CharacterCount       (標頭檔長度)
//     
//輸出: char *ans 
//
//說明: 解碼_8bitMode模式 ,全部顯示為UTF-8的16進制碼, 目前未使用
//    
//////////////////////////////////////////////////////////////////////////////
//char *BabitMode(int DataArray[],int CharacterCountNum,int CharacterCount){
//
//    int R=0; //資料分組,記數用
//    int Blocknum=0,Blocknum2=0;
//	int n1=0,n2=0,Flag1=0;
//	int DatabitsLong;
//    char *ans2=NULL;   
//
//	//printf("[8bitsBinary Mode]  \n");
//	//printf("此處的資料區塊總位元長度計算公式:B=4+C+8D  \n");
//	//printf("B:資料總位元長度  C:字數統計符位數  D:輸入字元的數量     \n");
//
//    DatabitsLong=4 + CharacterCount + (8 * CharacterCountNum);	//8bitMode用
//	int Blockbits = DatabitsLong-CharacterCount-4;
//	int PT=CharacterCount+4;
//
//    ans2=(char *)malloc((CharacterCountNum+2)*sizeof(char));    //對ans指標設定記憶體區塊大小 [AlphaNumeric]
//
//	//解碼
//	char utf8buf[2];                           
//	
//	 
//	R=(Blockbits/8);  
//	for(int a=0;a<=R-1;a++)
//	{   Blocknum = 8*DataArray[PT+0+(a*8)]+4*DataArray[PT+1+(a*8)]+2*DataArray[PT+2+(a*8)]+1*DataArray[PT+3+(a*8)];
//        _itoa(Blocknum,utf8buf,16);   
//		ans2[Flag1]=utf8buf[0];
//        Flag1++;
//
//		Blocknum2= 8*DataArray[PT+4+(a*8)]+4*DataArray[PT+5+(a*8)]+2*DataArray[PT+6+(a*8)]+1*DataArray[PT+7+(a*8)];
//        _itoa(Blocknum2,utf8buf,16);
//		ans2[Flag1]=utf8buf[0];
//        Flag1++;
//		
//		ans2[Flag1]=' ';
//		Flag1++;
//	}  
//    ans2[Flag1]='\0';
//
//	//printf("\n8bitMode FunctionTest: ");
//    //  for(int a=0;a<Flag1;a++) printf("%c",ans2[a]);
//	//printf("\n");
//
//
//	return ans2;
//
//
//}