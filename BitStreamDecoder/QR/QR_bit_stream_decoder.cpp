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

	//FormatInformation XOR�B�n [MicroQR]
    int MQR_XORmask[15]={1,0,1,0,0, 0,1,0,0,0, 1,0,0,0,1};
    
	//�r�Ʋέp�ŭp���
    int CharacterCountTemp[16]={0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,0};                        
    
	//Alphanumeric�s�X��, spec. 6.4.4 table 5, �s�ƶ��ǩMascii table ���P�L�k�ٲ�
    char AlphanumericTable[45]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
						        ' ','$','%','*','+','-','.','/',':'};  
    
	//Numeric�s�X��, �ثe�ٲ���, �����[�W 0x30��X
    char NumericTable[10]={'0','1','2','3','4','5','6','7','8','9'};

    //�U���Ÿ�Ʈe�q�O����  Ver  L    M   Q   H
	//Ver:��ܨC��Version��total code word�ƥ�(data + error correct)
	//���U�Ӫ��|�ӼƦr���L~H���榡�U, data ���F�h��code word(byte), �i�Ѧ�table 7�Mtable 9
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
						    901,721,563, 397,313,     //0429�ץ�
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
        //�����϶��U���Ź�Ӫ�, �Ѧ�Annex D, table D.1, �άd����Ө��NBCH���~�ˬd
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

	//Format���ˬd��,�Ѧ�Annex C, table C.1, �άd����Ө��NBCH���~�ˬd, �ثe�u��QR, ��MircoQR������
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


	//RS�����ժ��, �Ѧ�Table 9, �C�Ӯ榡�̦h�����RS���˦�,
	//�Hver 11,����, L:4,101, 81,10,0,0,0,0  , ��ܦ�4�� 101 byte��RS�X�n�ˬd, data ��81 byte, error correct��10 byte
    //M:1,80,50,15,4,81,51,15 ,��ܦ�1�� 80 byte(data:50 byte, EC:15 byte)�M4��81 byte(data:51 byte, EC:15 byte)��RS�X�n�ˬd, �`�ƶq�@�ˬO404 byte
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
//T4 : 29648 = ver40��module�Ʀ���AP, finder pattern, version, format�����ҳѤU��code word(data + EC)
unsigned char ErrorArray[T4];   //�x�s���~�󥿽X[T4]
unsigned char DataArray[T4];
//from TransArray
//T2: 3706, �̤jdata code word���j�p, �o�X�Ӥj���}�C�����|���/���ƨϥ�
unsigned short Fout[T2];  //�ƧǪ�ΰ}�C
unsigned char MemberOut[T2];

//modify by Joseph@20131210, move r[T2] and Decodeword[T2] to global
unsigned char r[T2];  
unsigned char Decodeword[T2]; 

int CheckArray[160]; //add by Joseph@20131213, move RS checkArray to global

//����flag, �ʤ֤@�Ӧ^�r�@�G���ѽX�ɥ�
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
//QRcode�ѽX�D�{��
//��J: rx(unsigned char):�x�sQR���XŪ�����G����ư}�C
//      QRversion   (int):�ӱiQR���X����������(Version)
//
//��X: ans       (*char):�^�ǸѽX���G
//
//����:QRcode Decode���D�n�{���A�]�t ���XCodeword �A ReedSolomon�ˬd �P ��ƸѽX ���T�j�B�J

//////////////////////////////////////////////////////////////////////////////
char *QR_bit_stream_decoder(unsigned char *rx,int ModulesNum,int *QRENCODEmode)  //��J:Ū�����G
{  
	int sum=0;

	//[0425�Ƶ�] T2,T4 �O�_��אּ�ϥήɦA���t ,��k�A�ק�
	//unsigned char ErrorArray[T4];   //�x�s���~�󥿽X[T4]
	//unsigned char r[T2];    //mark by Joseph@20131210
	unsigned char *DataCorrect = NULL;
	int RSanswer=0;
	//int DataArray[T4];
    memset(DataArray, 0, T4);
	memset(ErrorArray, 0, T4);

    memset(r, 0, T2);   //add by Joseph@20131210, reset r[] and Decodeword[]
    //memset(Decodeword, 0, T2);
   
	char *tempans=NULL;              //�^�Ǥ��_�ѽX���G
    char *ans=NULL;                  //�^�ǳ̲׸ѽX���G
    //char Decodeword[T2] = {""};      //Version 10�̰��䴩��652��codeword
    int EncodeMode =0;               //�ˬd�κX��: [1]Numberic, [2]AlphaNum, [3]8bitBinary ,[0]ErrorWord

    
    //�p��Version���šA�̤p21 �C�Ŭۮt4, MicroQR:Version 41~44
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
		Version=99;             //���~�аO
	 
	int MQVersion=0;             //MicorQR��

	if(Version==99)
		return ans;

    memset(datamap,0,sizeof(datamap));
    memset(qr_mask,0,sizeof(qr_mask));
    
 

	//�N��Ƹm�Jdatamap[X][Y]
	for(int i=0;i<ModulesNum;i++)
		for(int j=0;j<ModulesNum;j++)
		{
			if(rx[i*ModulesNum+j])
				datamap[i][j]=0;  
			else
				datamap[i][j]=255;  
		}
//////////////////////////////////////////////////////////////////////////////
//3.0 Ū��Version Block
//
//����: Ū��Version Block(���U�P�k�W)�϶�����ơA�P�e�B�z�Ҩ��o���������ѵ��G����e���
//      �Y�O���Version Block�϶������G�ۦP= �ϥ�Version Block��������T
//      �Y�O���Version Block�϶������G�۲�= ���X�}�l,�����ѽX
//
//3.1 Ū��Format Information
//
//����: Ū��QR���XFormat Information�϶������
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
				  datamap[a][b]=0;       //c=255,�զ�϶�                     
			  else                      
				  datamap[a][b]=1;                              
		}
//////////////////////////////////////////////////////////////////////////////
//3.2 �h�B�n
//
///����: �ھګe�B�JŪ��Format Information�϶��ұo�쪺�B�n�N��
//       �إ߬۹����������B�n�A�P��l��ư}�C�i��XOR�B��A�i��ѾB�n�B�z
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
			   qr_mask[i][j]=1;                        //�Y�ŦX�Ӧ�,�h��1 (���I)
			  else
			   qr_mask[i][j]=0;    }
			 break;

		case 1:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=((i/2)+(j/3))% 2;              //Pattern 100 :((i div 2) + (j div 3)) mod 2 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //�Y�ŦX�Ӧ�,�h��1 (���I)
			  else
			   qr_mask[i][j]=0;    }
			break;

		case 2:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=((i*j)% 2 + (i*j)% 3)%2;        //Pattern 110 :((i j) mod 2 + (i j) mod 3) mod 2 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //�Y�ŦX�Ӧ�,�h��1 (���I)
			  else
			   qr_mask[i][j]=0;    }
			break;

		case 3:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=((i*j)% 3 + (i+j)% 2)% 2;       //Pattern 111 :((i j) mod 3 + (i+j) mod 2) mod 2 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //�Y�ŦX�Ӧ�,�h��1 (���I)
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
				qr_mask[i][j]=1;                        //�Y�ŦX�Ӧ�,�h��1 (���I)  !!!!!!!!
			  else
				qr_mask[i][j]=0;  }
			break;
		case 1:
   			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{sum=i % 2;                         //Pattern 001 :i mod 2 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //�Y�ŦX�Ӧ�,�h��1 (���I)
			  else
			   qr_mask[i][j]=0;    }
			 break;

		case 2:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=j % 3;                         //Pattern 010 :j mod 3 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //�Y�ŦX�Ӧ�,�h��1 (���I)
			  else
			   qr_mask[i][j]=0;    }
			break;

		case 3:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=(i+j) % 3;                     //Pattern 011 :(i + j) mod 3 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //�Y�ŦX�Ӧ�,�h��1 (���I)
			  else
			   qr_mask[i][j]=0;    }
			break;

		case 4:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=((i/2)+(j/3))% 2;              //Pattern 100 :((i div 2) + (j div 3)) mod 2 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //�Y�ŦX�Ӧ�,�h��1 (���I)
			  else
			   qr_mask[i][j]=0;    }
			break;

		case 5:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=(i*j)% 2 + (i*j)% 3;               //Pattern 101 :(i j) mod 2 + (i j) mod 3 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //�Y�ŦX�Ӧ�,�h��1 (���I)   �j��!!
			  else
			   qr_mask[i][j]=0;    }
		   break;

		case 6:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=((i*j)% 2 + (i*j)% 3)%2;        //Pattern 110 :((i j) mod 2 + (i j) mod 3) mod 2 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //�Y�ŦX�Ӧ�,�h��1 (���I)
			  else
			   qr_mask[i][j]=0;    }
			break;

		case 7:
			for(int i=0;i<ModulesNum;i++)
			for(int j=0;j<ModulesNum;j++)
			{ sum=((i*j)% 3 + (i+j)% 2)% 2;       //Pattern 111 :((i j) mod 3 + (i+j) mod 2) mod 2 = 0
			  if(sum==0)
			   qr_mask[i][j]=1;                        //�Y�ŦX�Ӧ�,�h��1 (���I)
			  else
			   qr_mask[i][j]=0;    }
			break;
		}         
	}

//--------------------------------------------------------------------------
//�Ϥ��\��϶��P��ư϶�:
//�ھ�Version�����P�A���ҥ\��϶����j�p��m�]�H�����ܡA�G�b���i�����
//�򥻥\��϶�:�^�r�w��϶�(Position Detection Patterns)X3 ,�@���e(Separate for Position Detection Pattern)X3 ,�����u(Timing Patterns)X2 ,Format Information�϶�
//�S��\��϶�:Version Information�϶�[v7�H��X�{] �p�^�r���U�w��϶�(Alignment Patterns)[v2-v6(1) ,v7~v13(3x3) ,v14~v20(4x4) ,v21~v39(5x5) ,v40(7x7)]
//--------------------------------------------------------------------------
	int ThePoint=ModulesNum-7;  //�䨤�I��m
	int NewPoint=0;     //�p���:�s���䨤�I(�����I)�P���Z

//MicroQR�����B�z
//======================================================================
	if(Version>40)
	{
		for(int i=0;i<9;i++)       
			for(int j=0;j<9;j++)
				qr_mask[i][j]=99; //���WPDP

		for(int i=0;i<ModulesNum;i++) 
			qr_mask[0][i]=99;  //���Timing Patten
		for(int i=0;i<ModulesNum;i++) 
			qr_mask[i][0]=99;
	}

//======================================================================
	else
	{
		for(int i=0;i<9;i++)       
			for(int j=0;j<9;j++)
				qr_mask[i][j]=99;            //���WPDP

		for(int i=ThePoint-1;i<ModulesNum;i++)
			for(int j=0;j<9;j++)       
				qr_mask[i][j]=99;            //���UPDP

		for(int i=0;i<9;i++)
			for(int j=ThePoint-1;j<ModulesNum;j++)
				qr_mask[i][j]=99;            //�k�WPDP

		for(int k=6;k<=ThePoint;k++)
		{
			qr_mask[k][6]=99;            //��Timing Patterns
			qr_mask[6][k]=99;            //��Timing Patterns
		}            




		//mask[ModulesNum-7][ModulesNum-7] �ĥ|PDP�϶��䨤�I
		//v2~v6 ��u���@��AP�ɾA��
		if(Version >= 2) 
		{
			for(int i=ThePoint-2;i<=ThePoint+2;i++)
				for(int j=ThePoint-2;j<=ThePoint+2;j++)
					qr_mask[i][j]=99;}



	//-------------------------------------------------------------------------- 
	//Version 7~13 ���U�w��϶�
			int AP_X,AP_Y;
			if(Version >= 7 && Version <=13)
			{
				 //A,B��
				 AP_Y=6,AP_X=(6+ThePoint)/2;
				 for(int i=AP_X-2;i<=AP_X+2;i++)
					 for(int j=AP_Y-2;j<=AP_Y+2;j++)
					 {
						 qr_mask[j][i]=99;
						 qr_mask[i][j]=99;
					 }

				 //D,E��
				 AP_Y=ThePoint,AP_X=(6+ThePoint)/2;
				 for(int i=AP_X-2;i<=AP_X+2;i++)
					 for(int j=AP_Y-2;j<=AP_Y+2;j++)
					 {
						 qr_mask[j][i]=99;
						 qr_mask[i][j]=99;
					 }

				 //C��
				 AP_Y=AP_X=(6+ThePoint)/2;
				 for(int i=AP_X-2;i<=AP_X+2;i++)
					for(int j=AP_Y-2;j<=AP_Y+2;j++)
						qr_mask[i][j]=99;
			}

    //-------------------------------------------------------------------------- 
	//Version 14~20 ���U�w��϶�
			int AP[8];                              //8�ӷ�?
			if(Version >= 14 && Version <=20)
			{
				AP[0]=6;
				AP[1]=6+(ThePoint-6)/3;                 //�p�G�����ɷ|�o�ͤj���D!!!!
				AP[2]=6+((ThePoint-6)/3)*2;    
				AP[3]=ThePoint;

			//�S��:Version 15,18
			if(Version==15 || Version==18)  
				AP[1]=AP[1]-1;

			//�S��:Version 19
			if(Version==19 || Version==16)  
				AP[1]=AP[1]-2;

			//�Ĥ@��:�Ȧ�23
			for(int b=1;b<3;b++)
			{
				AP_X=AP[0];
				AP_Y=AP[b];
				for(int i=AP_X-2;i<=AP_X+2;i++)
					for(int j=AP_Y-2;j<=AP_Y+2;j++)
						qr_mask[i][j]=99;
			}

			//�ĤG�T��:��1234
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


			//�ĥ|��:�Ȧ�234
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
		//Version 21~27 ���U�w��϶�
		if(Version >= 21 && Version <=27)
		{
			AP[0]=6;
			AP[1]=6+(ThePoint-6)/4;
			AP[2]=6+((ThePoint-6)/4)*2;
			AP[3]=6+((ThePoint-6)/4)*3;
			AP[4]=ThePoint;

			//�S��:Version 22,24,26
			if(Version ==22 || Version ==24 || Version ==26)
			{
				AP[1]=AP[1]-3;
				AP[2]=AP[2]-2;
				AP[3]=AP[3]-1;
			}

		//�Ĥ@��:�Ȧ�234
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

		//�ĤG�T�|��:��12345
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


		//�Ĥ���:�Ȧ�2345
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
		//Version 28~34 ���U�w��϶�[6x6]
		if(Version >= 28 && Version <=34)
		{
			AP[0]=6;
			AP[1]=6+(ThePoint-6)/5;
			AP[2]=6+((ThePoint-6)/5)*2;
			AP[3]=6+((ThePoint-6)/5)*3;
			AP[4]=6+((ThePoint-6)/5)*4;
			AP[5]=ThePoint;

			//�S��:Version 28,33
			if(Version ==28 || Version ==33 )
			{
				AP[1]=AP[1]-3;
				AP[2]=AP[2]-2;
				AP[3]=AP[3]-1;
			}

			//�S��:Version 31
			if(Version ==31 )
			{
				AP[1]=AP[1]-1;
				AP[3]=AP[3]+1;
				AP[4]=AP[4]+2;
			}

			//�S��:Version 32
			if(Version ==32 )
			{
				AP[1]=AP[1]+2;
				AP[2]=AP[2]+2;
				AP[3]=AP[3]+2;
				AP[4]=AP[4]+2;
			}

			//�S��:Version 30
			if(Version ==30 )
			{
				AP[1]=AP[1]-4;
				AP[2]=AP[2]-2;
				AP[4]=AP[4]+2;
			}

			//�Ĥ@��:�Ȧ�2345
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

			//�ĤG�T�|����:��123456
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

			//�Ĥ���:�Ȧ�23456
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
		//Version 35~40 ���U�w��϶�[7X7]
		if(Version >= 35 && Version <=40)
		{
			AP[0]=6;
			AP[1]=6+(ThePoint-6)/6;
			AP[2]=6+((ThePoint-6)/6)*2;
			AP[3]=6+((ThePoint-6)/6)*3;
			AP[4]=6+((ThePoint-6)/6)*4;
			AP[5]=6+((ThePoint-6)/6)*5;
			AP[6]=ThePoint;

			//�S��:Version 36,39
			if(Version ==36 || Version ==39 )
			{
				AP[1]=AP[1]-6;
				AP[2]=AP[2]-4;
				AP[3]=AP[3]-2;
				AP[5]=AP[5]+2;
			}    

			//�S��:Version 37,40
			if(Version ==37 || Version ==40 )
			{
				AP[1]=AP[1]-3;
				AP[2]=AP[2]-2;
				AP[3]=AP[3]-1;
				AP[5]=AP[5]+1;
			}    



			//��1��:�Ȧ�23456
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

			//��2~6��:��123456
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


			//��7��:�Ȧ�234567
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
		//Version7�H�W �����T���϶�
		if(Version >= 7)
		{
			 for(int i=0;i<6;i++)
				 for(int j=0;j<3;j++)
				 { 
					 qr_mask[i][ModulesNum-11+j]=99;  //�k�W�϶�
					 qr_mask[ModulesNum-11+j][i]=99;  //���U�϶�
				 }
		}      
	}


	//�ϥξB�n���l���X�i��XOR�B��
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
			qr_mask[k][6]=55;            //[QR���w]��Timing Patterns�|��ѽX���ͻ�ê�A�ư�



//////////////////////////////////////////////////////////////////////////////
//3.3 ���o��Ʀ�y����
//
//����:�ھڸ�QR���X����������(Version)�P�e������(L,M,Q,H),�i��d��
//     ��X��������ư϶��P�e���϶����ƶq
//     �ϥΪ��: datacapacity[200]
//     *�|�Ψ�榡�϶���T(QRFormatInformation)
//////////////////////////////////////////////////////////////////////////////
	int Xmove,Ymove;
	int Datamove=0,Errormove=0,Datapoint=0;
	int LR=1,UD=0;              //�_�l��m�k(1) �W��(0)
	int TopWeight=0;

	int DataCount=0;   //��ưϼƶq
	int ErrorCount=0;  //�e���ϼƶq
	int ErrorLevel=0;

//======================================================================

	if(Version>40)
	{
		
		ErrorLevel=4*QRFormatInformation[14]+2*QRFormatInformation[13]+1*QRFormatInformation[12];
		MQVersion=ErrorLevel+1;
		//printf(",EC=%d",ErrorLevel);
		switch(ErrorLevel){ //���Table 9�MTable 13, �]�wdata �MEC code word�ƥ�
		case 0:
		  DataCount=3;
		  ErrorCount=2;
		  ErrorLevel=0;
		  //printf("�o�O�@�i M1_X��MicroQR���X\n");
		  break;

		case 1:
		  DataCount=5;
		  ErrorCount=5; //joseph test
		  ErrorLevel=1;
		  //printf("�o�O�@�i M2_L��MicroQR���X\n");
		  break;

		case 2:
		  DataCount=4;
		  ErrorCount=6;
		  ErrorLevel=2;
		  //printf("�o�O�@�i M2_M��MicroQR���X\n");
		  break;

		case 3:
		  DataCount=11;
		  ErrorCount=6;
		  ErrorLevel=3;
		  //printf("�o�O�@�i M3_L��MicroQR���X\n");
		  break;

		case 4:
		  DataCount=9;
		  ErrorCount=8;
		  ErrorLevel=4;
		  //printf("�o�O�@�i M3_M��MicroQR���X\n");
		  break;

		case 5:
		  DataCount=16;
		  ErrorCount=8;
		  ErrorLevel=5;
		  //printf("�o�O�@�i M4_L��MicroQR���X\n");
		  break;

		case 6:
		  DataCount=14;
		  ErrorCount=10;
		  ErrorLevel=6;
		  //printf("�o�O�@�i M4_M��MicroQR���X\n");
		  break;

		case 7:
		  DataCount=10;
		  ErrorCount=14;
		  ErrorLevel=7;
		  //printf("�o�O�@�i M4_Q��MicroQR���X\n");
		  break;
		}

	}
	else
	{
		if(QRFormatInformation[14]==0)
		{
			DataCount =datacapacity[5*(Version-1)+(2-QRFormatInformation[13])];
			ErrorCount=datacapacity[5*(Version-1)]-DataCount;                   //�e���ϼƶq
			if(QRFormatInformation[13]==1)  
				ErrorLevel=0;		  //L����
			else	                          
				ErrorLevel=1;       //M����
			
		 }else
		 {
			 DataCount =datacapacity[5*(Version-1)+(4-QRFormatInformation[13])];
			 ErrorCount=datacapacity[5*(Version-1)]-DataCount;                   //�e���ϼƶq
			 if(QRFormatInformation[13]==1)   
				 ErrorLevel=2;      //Q����
			 else                             
				 ErrorLevel=3;	  //H����
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
	//3.4 Ū�������ƶq��Data�X�PRS�X
	//
	//����:�ھګe�B�J���o����T:��ư϶��ƶq(DataCount) �P �e���϶��ƶq(ErrorDataCount)
	//     �bQR���X��Ū���۹����ƶq��������A�öi��Codeword���p��
	//
	//////////////////////////////////////////////////////////////////////////////
	//�}�lŪ�JCodeword�A�_�l�I�y�Ь��̥k�U���I(ModulesNum-1,ModulesNum-1)�A�^���ƶq:DataCount
	Xmove= ModulesNum -1;
	Ymove= ModulesNum -1;


	for(int a=1 ; a <= DataCount+ErrorCount; a++)  //�Ĥ@�h:Ū���`��ư϶���
	{ 
		for(;;)                                      //�ĤG�h:�L�a ���Ū�����w������
		{ 
			if(LR==1)  //��LR����k��A��������w����(x-1 y)       
			{ 
				if(qr_mask[Ymove][Xmove]==55)        
					Xmove=Xmove-1;                   //�ץ���:��I��S��ϰ조��

				if(qr_mask[Ymove][Xmove]!=99 && qr_mask[Ymove][Xmove]!=55)  //����w�ҫ���m������\��϶��ɡAŪ���Ӯ�data
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
		  
				if(UD==0 && Ymove==0)  //��W��Ĳ���� �v��+1
					TopWeight+=1;
				if(UD==1 && Ymove==ModulesNum-1)  //��U��Ĳ���� �v��+1
					TopWeight+=1;

				 Xmove=Xmove-1;    //������
				 LR=0;             //�������w
			}//------------------------------2.1 LR==1 End


			if(LR==0 && Datapoint<8)  //��LR���쥪��ɡA�l�[�P�_�ثe�O�W��/�U��   
			{ 
				if(qr_mask[Ymove][Xmove]!=99 && qr_mask[Ymove][Xmove]!=55)  //����w�ҫ���m������\��϶��ɡAŪ���Ӯ�data
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

				    if(UD==0 && Ymove==0)             //��W��Ĳ���� �v��+1
						TopWeight+=1;
					if(UD==1 && Ymove==ModulesNum-1)  //��U��Ĳ���� �v��+1
						TopWeight+=1;

					if(TopWeight!=2)                  //�u���b�DĲ�����p�U�A�~�i��k�W/�k�U���� 
					{
						if(UD==0)//��UD=0[�W��]�ɡA��������w���k�W��(x+1 y-1)  
						{
							Xmove=Xmove+1;    //�k�W����
							Ymove=Ymove-1;
						} 
					    else     //��UD=1[�U��]�ɡA��������w���k�U��(x+1 y+1)
						{
							Xmove=Xmove+1;    //�k�U����
							Ymove=Ymove+1;
						}
				   }

					LR=1;               //�������w
				}//------------------------------2.2 LR==0 End
	   

				if(TopWeight==2  &&  LR==1) //��F�W�U��t�� �i����V�å����_�l�I[����:TopWeight=2,LR=0]
				{    
					Xmove=Xmove-1;      //�_�l�I�Τ@�������@��  
					if(UD==0) //��UD=0[�W��]����ɮɡA���UD=1 LR=1[�k��]
						UD=1;
					else      //��UD=1[�U��]����ɮɡA���UD=0 LR=1[�k��]
						UD=0;
						LR=1;
						TopWeight=0;    //������V���k�s
				}//---------------2.3 ReturnBack End


				 if(Datapoint==8) //�ֿnŪ����8����� ���X�L�a�j��
				 {     Datapoint=0;    //�p�ƫ��w�k�s
					   break;
				 }
			}//------------------------------2nd End
		}//------------------------------1st End





if(ErrorCount<0 || DataCount < 0 || Version>45 || Version==0)   EncodeMode=99;             //����:�Y�榡��Ʀ��~,���X

if(EncodeMode !=99){

	if(Version==41 || Version==43)  	    //�S��:MicroQR _M1,M3
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
  printf("��l���X��Bitstream  :");
  for(int a=0;a<DataCount*8;a++)  printf(" %d",DataArray[a]);
  printf("\n");

  printf("RSanswer is %d\n",RSanswer);
#endif


	//��ƪ����s�ƧǻP�ץ�
	if(Version>40) {
	    RSanswer=MicroQRTransArray(r,MQVersion,ErrorLevel,DataCount,ErrorCount);     //�ˬdr�ȬO�_�ץ��æ^��: 13-��ƿ�J���T  666-��ƵL�k�ץ� 1-��ƿ��~�w�ץ�
	    DataCorrect=RS2binary(r,DataCount);                            //�N�ץ����Gr[] ��^8bit�X
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
	    RSanswer=TransArray(r,Version,ErrorLevel,DataCount,ErrorCount,QRStructTable);     //�ˬdr�ȬO�_�ץ��æ^��: 13-��ƿ�J���T  666-��ƵL�k�ץ� 1-��ƿ��~�w�ץ�
	    DataCorrect=RS2binary(Decodeword,DataCount);
	}
		
	for(int a=0;a<DataCount*8;a++)  DataArray[a]=DataCorrect[a];   //�N�@��codeword�ܦ��K�ӤG�i��, 4=>0000 0100
	free(DataCorrect);

#ifdef DEV_MODE
  printf("���s�ƦC�᪺��l���XBitstream:");
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

if(RSanswer >= 666)  EncodeMode=99;              //����:�Y���~�Ӧh�L�k�״_,���X




//////////////////////////////////////////////////////////////////////////////
//����O����[�B�n��]
//for(int i=0;i<ModulesNum;i++) free(mask[i]);
//free(mask);
//for(int i=0;i<ModulesNum;i++) free(datamap[i]);
//free(datamap);


//////////////////////////////////////////////////////////////////////////////
//5.3 ��X���G
//
//����:�ˬd�����X��(EncodeMode)�O�_�����`��
//     �Y�_�A�h�^�ǸѽX���Gans
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
	//5.1���o�Ҧ����ܲŻP�r�Ʋέp��
	//
	//����:���o��QR���X���s�X�Ҧ�,�r�Ʋέp�Ŧ�� �P ���X��Ʀr���ƶq ����T
	//
	//////////////////////////////////////////////////////////////////////////////
	int IMnum=0;                        //�s�X�Ҧ�
	int CharacterCount=0;               //���Y�ɪ���
	int CharacterCountNum=0;            //�r���ƶq
	int R=0;                            //�p����ܼ�
	int DatabitsLong=0;

	int BadPT=start_pos;                //���w:�O���ثe�B�z�쪺��Ƽ�
	int EndPT=0;                        //while�j�骺�P�_�Ѽ�:�ˬd�O�_��F�פ�šA�����ѽX

	int ModeIndicatorLength=0;
	char* ans=NULL;
	char* tempans=NULL;
	int EncodeMode=0;

	ans=(char*)malloc(sizeof(char)*7089+2);                    //�ȩw:�̤j�Ѭ�100���r�� ,7089=Ver.40_L Numeric�̤j�r��, add by Joseph@20131210, plus 2(same buffer size of numeric mode)
	memset(ans,0,7091);

	while(EndPT==0)
	{
		//�ھڿ�J�s�X�Ҧ�����T�A�p��X�����X���s�X�Ҧ�,�r�Ʋέp�Ŧ�ƻP���X����Ʀr���ƶq
		if(Version<=40)       
			GetCharacterCountIndicator(DataArray,Version,BadPT,&IMnum,&CharacterCount,&CharacterCountNum);
		else                  
			GetCharacterCountIndicatorMQR(DataArray,ErrorLevel,BadPT,&IMnum,&CharacterCount,&CharacterCountNum,&ModeIndicatorLength);



		if(Version<=40) 
			ModeIndicatorLength=4;     //QR���X���Ҧ����ܲŪ��שT�w��4

		//////////////////////////////////////////////////////////////////////////////
		//5.2 ��ƸѽX
		//
		//����:�ھڤ��P����ƽs�X�Ҧ��A�i��U�۪��ѽX�B�z
		//
		////////////////////////////////////////////////////////////////////////////// 

		switch(IMnum)
		{
		case 1:
			(*QRENCODEmode)=1;
			tempans=NumericMode(DataArray,CharacterCountNum,CharacterCount,NumericTable,BadPT,ModeIndicatorLength);
			EncodeMode=1;                                             //�аO��:Numberic
			break;

		case 2:
			(*QRENCODEmode)=2;
			tempans=AlphanumericMode(DataArray,CharacterCountNum,CharacterCount,AlphanumericTable,BadPT,ModeIndicatorLength);
			EncodeMode=2;                                             //�аO��:AlphaNumberic
			break;

		case 4:  
			(*QRENCODEmode)=3;
			//tempans=BabitMode(DataArray,CharacterCountNum,CharacterCount);                  //8bitMode��ܤQ���i��X
			tempans=BabitModeTicket(DataArray,CharacterCountNum,CharacterCount,BadPT,ModeIndicatorLength);    //8bitMode���ASCII�X
			EncodeMode=3;                                             //�аO��:8bitBinary
			break;

		case 8:  
			(*QRENCODEmode)=4;                                        //�аO��:Kanji Mode �~�r�Ҧ�
			tempans=KanjiMode(DataArray,CharacterCountNum,CharacterCount,BadPT,ModeIndicatorLength);
			EncodeMode=4;  
			break;

		case 3:  
			(*QRENCODEmode)=5;                                        //�аO��:Structured Append
			//EncodeMode=99;                                            
			//EndPT=2;
			EncodeMode=5;                                                
			get_structured_append_info(DataArray,ModeIndicatorLength,BadPT);
			break;

		case 5:  
			(*QRENCODEmode)=6;                                        //�аO��:FNC1 mode(1st position)
			//EncodeMode=99;                                            
			//EndPT=2; 
			EncodeMode=6;  
			break;
		case 7:  
			(*QRENCODEmode)=7;                                        //�аO��:ECI mode
			//EncodeMode=99;                                            
			//EndPT=2;
			EncodeMode=7;                                                
			break;
		case 9:  
			(*QRENCODEmode)=9;                                        //�аO��:FNC1 mode(2nd position)
			EncodeMode=99;                                            
			EndPT=2;  
			break;

		default:
			EncodeMode=99;                                            //�аO��:ErrorMode
			EndPT=2;  
			break;
		}

		if (EncodeMode==99) 
			continue;  //add by Joseph@20131119, if we meet error mode, skip the following process

		//////////////////////////////////////////////////////////////////////////////
		//�ˬd�O�_��Mixing Mode
		//�O:��sBadPT,�N�ѽX���G���r��ۥ[�å�J�Ȧs��,�~��U�B�J
		//�_:�ѽX���G���r��ۥ[�ÿ�X���G,�����j��
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
		//�ˬd�O�_��F���I [QR��]    
		if(Version<=40)
		{
			if(DataArray[BadPT+DatabitsLong+0]==0 && DataArray[BadPT+DatabitsLong+1]==0 && DataArray[BadPT+DatabitsLong+2]==0 && DataArray[BadPT+DatabitsLong+3]==0)
			{//�DMixingMode
				 if(BadPT==0) strcpy(ans,tempans);
				 else         strcat(ans,tempans);
				 EndPT=2;
			}
			else if(8*DataArray[BadPT+DatabitsLong+0]+ 4*DataArray[BadPT+DatabitsLong+1]+ 2*DataArray[BadPT+DatabitsLong+2]+ 1*DataArray[BadPT+DatabitsLong+3] <=8)
			{//����MixingMode
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
		//�ˬd�O�_��F���I [MQR��]
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
	}//�ѽX���� while(EndPT==0) END
	return ans;
}



//////////////////////////////////////////////////////////////////////////////
//�禡: RS2binary
//��J: rx            (unsigned char):��l��ư}�C
//      Datacount               (int):�����쪺�ƶq
//
//��X: DataArray   (unsigned char *):�B�z��������ư}�C
//
//����:�N�쥻�HCodeeord�x�s����ư}�C�A��^�h2�i��}�C���覡�x�s
//     �Ҧp: 4,1,2,0 ... => 00000100 00000001 00000010 00000000 ...
//
//////////////////////////////////////////////////////////////////////////////
unsigned char *RS2binary(unsigned char *rx,int Datacount) 
{  
	unsigned char *DataArray = (unsigned char *)malloc(Datacount*8*sizeof(unsigned char));  //�^�Ǽ���Ū�����G
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
//3.5 DataArea�����s�Ƨ�
//��J: rx(unsigned char):��l��ư}�C
//      MRT_V       (int):���X����(Version)
//      MRT_R       (int):�e������(L,M,Q,H)
//      DataCount   (int):��ư϶��ƶq
//      ErrorCount  (int):�e���϶��ƶq
//
//��X:TRA_t        (int):
//
//����:�N��l��ư}�C�i�歫�s�ƦC�A�öi��RS�X���ˬd�A�T�{��ƪ����T
//
//    10/05: �w��s Ver.20�ƦC�W�h
//////////////////////////////////////////////////////////////////////////////
int TransArray(unsigned char *rx,int MRT_V,int MRT_R,int DataCount ,int ErrorCount ,unsigned char QRStructTable[40][32]) 
{  


	int RQ[3]={26,44,70};      //Version1,2,3 ���X��ư϶��`��
	int pass_cnt1=0,pass_cnt2=0;
	int gba=0;      //�p�ƥ�
	//int Fout[T2];  //�ƧǪ�ΰ}�C
	int type=0;     //�B�z�Ҧ�
	memset(Fout, 0, T2);

	//�ھڪ����P�e�����šA���o���ո�T:a(b,c,d),e(f,g,h) 
	int MRT_menu[8];
	for(int i=0;i<8;i++) 
		MRT_menu[i]=QRStructTable[MRT_V-1][8*MRT_R+i];   

	//�S��: V1 V2l V3L, weight: V1 +2 ,L +1 ,V3,V2 +1 ,�YWeight>=2 �ݯS�O�B�z
	int SpecialWei=0;
	if(MRT_V==1) 
		SpecialWei=SpecialWei+2;
	if(MRT_V==2 || MRT_V==3 ) 
		SpecialWei=SpecialWei++;
	if(MRT_R==0) 
		SpecialWei=SpecialWei++;



	//���p0:�̨α��p�A���ݧ�ʶ���
	if(MRT_menu[0]==1 && MRT_menu[4]==0)
		type=1;


	//���p1:�Ȧ��@�ئ�(Datacodeword,RScodeword)�զX
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


	//���p2: ����եH�W��(Datacodeword,RScodeword)�զX
	if(MRT_menu[0]!=0 && MRT_menu[4]!=0) //MRT_menu[6] ���j�� MRT_menu[2]
	{                 
		for(int i=1;i<=MRT_menu[6];i++)//�����Datacodeword���ת��̤j�� =MRT_menu[6]
		{                      
		     for(int j=0;j<MRT_menu[0]+MRT_menu[4];j++)
			 {       
		   
				 if(j<MRT_menu[0]){                               //��1��Datacodeword����_MRT_menu[0]
				   if(i==MRT_menu[6] && j<MRT_menu[0]) {           //��X�������B�z
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
			   
				 if(j>=MRT_menu[0]){                               //��1��Datacodeword����_MRT_menu[4]
					   Fout[gba]=i+ MRT_menu[0]*MRT_menu[2]+ (j-MRT_menu[0])*MRT_menu[6];
					   gba++;
				 }  
			}
		}

	    for(int i=1;i<=MRT_menu[3]*2;i++) //RS�X
		{       
			for(int j=0;j<MRT_menu[0]+MRT_menu[4];j++)
			{
				 Fout[gba]=i+j*MRT_menu[3]*2;
				 gba++;
			}
		}

	}

	//----���Ǫ�إߧ���,�}�l���s�t�m---
	//Fout[134]:�ƧǪ�  rx[]:��Ʀs��϶�
	//int MemberOut[T2];
	memset(MemberOut, 0, T2);


	if(type!=1)
	{
		//Data�Ϧ�m���s�Ƨ�
		for(int i=0;i<DataCount;i++)  
			MemberOut[Fout[i]-1]=rx[i];

		//RS�Ϧ�m���s�Ƨ�
		for(int i=DataCount;i<DataCount+ErrorCount;i++)  
			MemberOut[DataCount+Fout[i]-1]=rx[i];

		//��Ƹm�Jrx[] ,�^��
		for(int i=0;i<DataCount + ErrorCount;i++)        
			rx[i]=MemberOut[i];
	}


	//////////////////////////////////////////////////////////////////////////////
	//4.1 �T�{��Ƨ���ʻP���T��
	//
	//
	//////////////////////////////////////////////////////////////////////////////
	//int *CheckArray = (int *)malloc(160*sizeof(int));  //�C��RS�X���̤j�e�q
	memset(CheckArray,0,160);  //add by Joseph@20131213, move RS checkArray to global
	int Pt=0;                       //�p�ƾ�
	int TRA=0;                       
	int TRA_t=0;                    //�ˬdRS�O�_�ץ�
	int SavedataFlag=0;             //�x�s��ƥΪ����w



	//��1�˴�:���q
	if(SpecialWei<2)
	for(int i=0;i<MRT_menu[0];i++)
	{

		//�m�JDatacode
		for(int a=0;a<MRT_menu[2];a++)
		{
		  CheckArray[Pt]=rx[a +i*MRT_menu[2]];
		  Pt++;
		}
	    
		//�m�JRscode
		for(int b=0;b<MRT_menu[3]*2;b++)
		{
		  CheckArray[Pt]=rx[b +MRT_menu[0]*MRT_menu[2]+MRT_menu[4]*MRT_menu[6] +i*(2*MRT_menu[3]) ]; //MRT_menu[0]*MRT_menu[2]+MRT_menu[4]*MRT_menu[6] =RScode�_�l�I��m =���˥��`��  
		  Pt++;                                                                                      //2*MRT_menu[3]=�C��RScode����
		}
	    
		//RS�ˬd
		TRA=reed_solomon(CheckArray, MRT_menu[1],MRT_menu[2],QR_CODE);  //N���� K�T��
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

		//�M�Ű}�C
		memset(CheckArray,0,160);
		//for(int d=0;d<160;d++) CheckArray[d]=0;
		Pt=0; 
	}



	//��1�˴�:�S���p
	if(SpecialWei>=2)
	for(int i=0;i<MRT_menu[0];i++)
	{
		//�m�JDatacode
		for(int a=0;a<MRT_menu[2];a++){
		  CheckArray[Pt]=rx[a +i*MRT_menu[2]];
		  Pt++;
		}
	    
		//�m�JRscode
		for(int b=0;b<RQ[MRT_V-1]-MRT_menu[2];b++){
		  CheckArray[Pt]=rx[b +MRT_menu[0]*MRT_menu[2]+MRT_menu[4]*MRT_menu[6] +i*(RQ[MRT_V-1]-MRT_menu[2]) ]; //MRT_menu[0]*MRT_menu[2]+MRT_menu[4]*MRT_menu[6] =RScode�_�l�I��m =���˥��`��  
		  Pt++;                                                                                                //RQ[MRT_V-1]-MRT_menu[2]=�C��RScode����
		}
	    
		//RS�ˬd
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

		//�M�Ű}�C
		memset(CheckArray,0,160);
		//for(int d=0;d<160;d++) CheckArray[d]=0;
		Pt=0;
	}



	//��2�˴�:����|��,�YMRT_menu[4]=0�h����
	for(int i=0;i<MRT_menu[4];i++){
		//�m�JDatacode
		for(int a=0;a<MRT_menu[6];a++){
		  CheckArray[Pt]=rx[a +MRT_menu[0]*MRT_menu[2] +i*MRT_menu[6]];   //�_�l�I:�Ĥ@��Datacode�ƾڤ���
		  Pt++;
		}
	    
		//�m�JRscode
		for(int b=0;b<MRT_menu[7]*2;b++){
		  CheckArray[Pt]=rx[b +MRT_menu[0]*MRT_menu[2]+MRT_menu[4]*MRT_menu[6]+2*MRT_menu[0]*MRT_menu[3] +i*(2*MRT_menu[7]) ];  	  //�_�l�I:�Ĥ@��RScode�ƾڤ���
		  Pt++;                                                                                                                       //��2�qRScode�_�l�I��m =���˥��`�� + ��1�qRScode=MRT_menu[0]*MRT_menu[2] + MRT_menu[4]*MRT_menu[6] + 2*MRT_menu[0]*MRT_menu[3]
		}

		//RS�ˬd
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
	    
		//�M�Ű}�C
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
//�禡: GetFormatInformation    Ver.2
//��J: rx(unsigned char) :��l��ư}�C
//      XORmask[]         :XOR�ϥΪ��B�n
//      ModulesNum        :module size
//��X: ans(unsigned char):�T�w����15,�ӱ��X�榡�϶�����T
//����:
//
//    
//////////////////////////////////////////////////////////////////////////////
unsigned char *GetFormatInformation(unsigned char datamap[177][177],int XORmask[],unsigned char QRFormatTable[32][15],int ModulesNum)
{
	unsigned char *ans = (unsigned char *)malloc(15*sizeof(unsigned char));
    int temp=0;

	for(int i=0;i<=8;i++)   //Ū�����W���u(0~7)
	{
		 if(i<6)         
			 ans[i]=datamap[i][8];
			 else if(i==6)   
				 temp=0;
			 else if(i>=7)  
				 ans[i-1]=datamap[i][8];
	}


	for(int j=0	;j<=8;j++)   //Ū�����W��u(7~14)
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


    //���~�ȥ�
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
			ans[a]=0;                             //�Y���~�Ӧh �h�M�Ű}�C
	  
	if (ErrorLM >3) 
	{
		for(int i=0;i<=7;i++) 
		{  //Ū���k�W��u(0~7)
            ans[i]=datamap[8][ModulesNum-i-1];
		}
		for(int i=0;i<=6;i++)
		{ //Ū�����U��u(8~14)
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
				ans[a]=0;                             //�Y���~�Ӧh �h�M�Ű}�C
	}

    //�N��T�϶��i��XOR�B��
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
//�禡:NumericMode
//��J: DataArray        (��lBitstream)
//      CharacterCountNum(�r���ƶq)
//      CharacterCount   (���Y�ɪ���)
//      NumericTable     (Numeric�r����) 
//     
//��X: char *ans
//
//����: �ѽX_�Ʀr�Ҧ�, �T��digit�s��10bit, �l�U�@��digit: 4bit, �l�U���digit:7bit
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
	//printf("���B����ư϶��`�줸���׭p�⤽��:B=4+C+10(D % 3)+R \n");
	//printf("B:����`�줸����  C:�r�Ʋέp�Ŧ��  D:��J�r�����ƶq     \n");

	if(CharacterCountNum%3==2)        R=7;                                       //�p��R��:�Y�L�k�㰣�A�l���AR=7 
    else if(CharacterCountNum%3==1)   R=4;                                       //�p��R��:�Y�L�k�㰣�A�l�@��
	DatabitsLong= ModeIndicatorLength + CharacterCount + 10*(CharacterCountNum / 3) + R;           //�p���ư϶��`���׬� n ��bit
	if (CharacterCountNum==0) return NULL;
	ans2=(char *)malloc((CharacterCountNum+2)*sizeof(char));                      //��ans���г]�w�O����϶��j�p [�Ƶ�:�~���^�ǥ�] 
    int Blockbits = DatabitsLong-CharacterCount-ModeIndicatorLength;  
    int PT=CharacterCount+ModeIndicatorLength+BadPT;
   
   //�}�l�ѽX
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
		Flag1++;   	//0426�ץ�
   }
   else if(R==7)
   {
	   Blockbits=Blockbits*10;
	   Blocknum = 64*DataArray[PT+Blockbits]  +32*DataArray[PT+Blockbits+1]+16*DataArray[PT+Blockbits+2] + 8*DataArray[PT+Blockbits+3]+ 4*DataArray[PT+Blockbits+4]+ 2*DataArray[PT+Blockbits+5] + 1*DataArray[PT+Blockbits+6];
     
	   //DataString[Flag1]=Blocknum/10;   	 //0426�ץ�
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
//�禡:AlphanumericMode
//��J: DataArray            (��lBitstream)
//      CharacterCountNum    (�r���ƶq)
//      CharacterCount       (���Y�ɪ���)
//      AlphanumericTable    (AlphaNumeric�r����) 
//     
//��X: char *ans
//
//����: �ѽX_�Ʀr�Ÿ��Ҧ�, �ѽX���Ʀr(0~44)����A�d�X�������r��(0~9,A~Z, others)
//    
//////////////////////////////////////////////////////////////////////////////
char *AlphanumericMode(unsigned char DataArray[],int CharacterCountNum,int CharacterCount ,char AlphanumericTable[],int BadPT,int ModeIndicatorLength)
{
    
	int R=0; //��Ƥ���,�O�ƥ�
    int Blocknum;
    //int DataString[800]; //mark by Joseph@20131210, dynamic allocate temp array       
    int* DataString = NULL;
	int n1,n2,Flag1=0;
	int DatabitsLong;
    char *ans2=NULL;   

	//printf("[Alphanumeric Mode]  \n");
	//printf("���B����ư϶��`�줸���׭p�⤽��:B=4+C+11(D / 2)+6(D % 2)  \n");
	//printf("B:����`�줸����  C:�r�Ʋέp�Ŧ��  D:��J�r�����ƶq     \n");

	DatabitsLong=ModeIndicatorLength + CharacterCount + 11*(CharacterCountNum / 2)+6*(CharacterCountNum % 2);	
	//int Blockbits = DatabitsLong-CharacterCount-4;  //��s! �Ҧ����ܲŪ���(ModeIndicatorLength)�ëD�����Ҭ�4,MicroQR�����׬��ʺA
	int Blockbits = DatabitsLong-CharacterCount-ModeIndicatorLength;
	int PT=CharacterCount+ModeIndicatorLength+BadPT;
    if (CharacterCountNum==0) return NULL;
    ans2=(char *)malloc((CharacterCountNum+2)*sizeof(char));    //��ans���г]�w�O����϶��j�p [AlphaNumeric]
    DataString=(int *)malloc((CharacterCountNum+2)*sizeof(int));  //add by Joseph@20131210, dynamic allocate temp array
    R=(Blockbits/11);                                  
   
    for(int a=0;a<R;a++)
	{
		Blocknum= 1024*DataArray[PT+0+(a*11)]+512*DataArray[PT+1+(a*11)]+256*DataArray[PT+2+(a*11)]
             + 128*DataArray[PT+3+(a*11)]+ 64*DataArray[PT+4+(a*11)]+ 32*DataArray[PT+5+(a*11)]
			 +  16*DataArray[PT+6+(a*11)]+  8*DataArray[PT+7+(a*11)]+  4*DataArray[PT+8+(a*11)]
			 +   2*DataArray[PT+9+(a*11)]+  1*DataArray[PT+10+(a*11)];

		if(Blocknum<0 || Blocknum>2024) 
			Blocknum=0;  //�d�򤶩�0 ~ 2^11
		
		n1=Blocknum/45;
		n2=Blocknum%45;
	    

		if(n1<45 && n2<45 )
		{
			DataString[Flag1]  =AlphanumericTable[n1];
			DataString[Flag1+1]=AlphanumericTable[n2];
			Flag1=Flag1+2;	
		}

   }

   if(Blockbits%11 !=0)                          //�Y�L�k�㰣�A�إ�[6]
   {
	   R=R*11;
	   Blocknum=  32*DataArray[PT+R]  +16*DataArray[PT+R+1]+8*DataArray[PT+R+2] + 4*DataArray[PT+R+3]+ 2*DataArray[PT+R+4]+1*DataArray[PT+R+5];  
    
	   if(Blocknum<0 || Blocknum>64) 
		   Blocknum=0;   //�d�򤶩�0 ~ 2^11
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
//�禡: 8bitMode �q�l�o����
//��J: DataArray            (��lBitstream)
//      CharacterCountNum    (�r���ƶq)
//      CharacterCount       (���Y�ɪ���)
//     
//��X: char *ans 
//
//����: �ѽX_8bitMode�Ҧ� ,������ܬ�ASCII�X 
//    
//////////////////////////////////////////////////////////////////////////////
char *BabitModeTicket(unsigned char DataArray[],int CharacterCountNum,int CharacterCount,int BadPT,int ModeIndicatorLength){

    int R=0; //��Ƥ���,�O�ƥ�
    int Blocknum=0,Blocknum2=0;  
	int n1=0,n2=0,Flag1=0;
	int DatabitsLong=0;
    char *ans=NULL;   

	//printf("[8bitsBinary Mode]  \n");
	//printf("���B����ư϶��`�줸���׭p�⤽��:B=4+C+8D  \n");
	//printf("B:����`�줸����  C:�r�Ʋέp�Ŧ��  D:��J�r�����ƶq     \n");

    DatabitsLong=ModeIndicatorLength + CharacterCount + (8 * CharacterCountNum);	//8bitMode��
	int Blockbits = DatabitsLong-CharacterCount-ModeIndicatorLength;
	int PT=ModeIndicatorLength+CharacterCount+BadPT;
    if (CharacterCountNum==0) return NULL;
	ans=(char*)malloc(sizeof(char)*CharacterCountNum+2);      //��ans���г]�w�O����϶��j�p 

	//�ѽX                         
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
//�禡: GetCharacterCountIndicator ���o�r�Ʋέp�Ū���
//��J: DataArray            (��lBitstream)
//      CharacterCountNum    *(�r���ƶq)
//      CharacterCount       *(���Y�ɪ���)
//      Version              (���X��������) 
//      BadPT                (�첾���w)
//      IMnum                *(�s�X�Ҧ�) 
//��X:  *IMnum, *CharacterCount,int ,*CharacterCountNum
//
//����:   [0731��s]�[�JKanjiMode�������
//    
//////////////////////////////////////////////////////////////////////////////
void *GetCharacterCountIndicator(unsigned char DataArray[],int Version,int BadPT,int *IMnum,int *CharacterCount,int *CharacterCountNum)
{

	int IM[4];  //IndicatorsMode ��ƽs�X�N��
	int CharacterCountTable[3][4]={{10, 9, 8,8},
								   {12,11,16,10},
								   {14,13,16,12}};
	for(int a=0;a<4;a++) 
		IM[a]=DataArray[BadPT+a];

	*IMnum=8*IM[0]+4*IM[1]+2*IM[2]+IM[3];
	//�ˬd���Ҫ����P�s�X�Ҧ��A�M�w�r�Ʋέp�Ŧ��
	//step.1 �ھ�Version�P��ƽs�X�Ҧ��A�M�w�r�Ʋέp�Ŧ��
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
	   *IMnum=99;   //����:�Y���D���W�� => Error 
	   return 0;    //add by Joseph@20131119, if we meet unsupported mode, skip following process
	}

	//if(EncodeMode==99)  IMnum=99;  //����:�ư��H�k����,���I

	//���o�r�Ʋέp��(CharacterCount)����
	*CharacterCount = CharacterCountTable[tempA][tempB];   

	//�p��ӱ��X���r���ƶq
	*CharacterCountNum = 3 + *CharacterCount;
	for(int a=0;a<16;a++) 
		CharacterCountTemp[a]=0;  //�ϥΫe�}�C���M��


	for(int a=0;a < *CharacterCount ;a++)
		CharacterCountTemp[a] = DataArray[BadPT+*CharacterCountNum-a];  
	*CharacterCountNum=(1*CharacterCountTemp[0])+(2*CharacterCountTemp[1])+(4*CharacterCountTemp[2])+(8*CharacterCountTemp[3])+(16*CharacterCountTemp[4])+(32*CharacterCountTemp[5])
					  +(64*CharacterCountTemp[6])+(128*CharacterCountTemp[7])+(256*CharacterCountTemp[8])+(512*CharacterCountTemp[9])+(1024*CharacterCountTemp[10])+(2048*CharacterCountTemp[11])+(4096*CharacterCountTemp[12]); //add by Joseph@20131210, fixed bug when chars over 2047

	return 0;
}


//////////////////////////////////////////////////////////////////////////////
//3.5 DataArea�����s�Ƨ�(MicroQR)
//��J: rx(unsigned char):��l��ư}�C
//      MRT_V       (int):���X����(Version)
//      MRT_R       (int):MicroQR�����սs��
//
//��X:TRA_t        (int):
//////////////////////////////////////////////////////////////////////////////
int MicroQRTransArray(unsigned char *rx,int MRT_V,int MRT_R,int DataCount ,int ErrorCount) 
{  

	//MQVersion�d��:1~8 =MRT_V
	int MicroQRStructTable[9][8]={{0,  0,  0, 0,0,0,0,0 },   //�d�հ�
								  {1,  5,  3, 0,0,0,0,0 },   //M1
								  {1, 10,  5, 1,0,0,0,0 },   //M2_L
								  {1, 10,  4, 2,0,0,0,0 },   //M2_M
								  {1, 17, 11, 2,0,0,0,0 },   //M3_L
								  {1, 17,  9, 4,0,0,0,0 },   //M3_M
								  {1, 24, 16, 3,0,0,0,0 },   //M4_L
								  {1, 24, 14, 5,0,0,0,0 },   //M4_M
								  {1, 24, 10, 7,0,0,0,0 }};  //M4_Q
	//M1,M2,M3,M4 ���X��ư϶��`��
	int RQ[9]={0,5,10,10,17,17,24,24,24};      

	int gba=0;      //�p�ƥ�
	//int Fout[T2];  //�ƧǪ�ΰ}�C
	int type=0;     //�B�z�Ҧ�




	//�ھڪ����P�e�����šA���o���ո�T:a(b,c,d),e(f,g,h) 
	int MRT_menu[8];
	for(int i=0;i<8;i++) 
		MRT_menu[i]=MicroQRStructTable[MRT_V][i];   


	//���p0:�̨α��p�A���ݧ�ʶ���
	if(MRT_menu[0]==1 && MRT_menu[4]==0)
		type=1;

	//////////////////////////////////////////////////////////////////////////////
	//4.1 �T�{��Ƨ���ʻP���T��
	//////////////////////////////////////////////////////////////////////////////
	
	memset(CheckArray,0,160*4); //add by Joseph@20131213, move RS checkArray to global
	int Pt=0;                       //�p�ƾ�
	int TRA=0;                       
	int TRA_t=0;                    //�ˬdRS�O�_�ץ�
	int SavedataFlag=0;             //�x�s��ƥΪ����w



	//��1�˴�:�S���p
	//if(SpecialWei>=2)
	for(int i=0;i<MRT_menu[0];i++)
	{
		//�m�JDatacode
		for(int a=0;a<MRT_menu[2];a++)
		{
			CheckArray[Pt]=rx[a +i*MRT_menu[2]];
			Pt++;
		}
	    
		//�m�JRscode

		for(int b=0;b<RQ[MRT_V]-MRT_menu[2];b++)
		{
			CheckArray[Pt]=rx[b +MRT_menu[0]*MRT_menu[2]+MRT_menu[4]*MRT_menu[6] +i*(RQ[MRT_V]-MRT_menu[2]) ]; //MRT_menu[0]*MRT_menu[2]+MRT_menu[4]*MRT_menu[6] =RScode�_�l�I��m =���˥��`��  
			Pt++;                                                                                                //RQ[MRT_V-1]-MRT_menu[2]=�C��RScode����
		}
	    
		//RS�ˬd
		TRA=reed_solomon(CheckArray, MRT_menu[1],MRT_menu[2],QR_CODE);
		//�ױ�micro QR M1��������׫o�|�ѽX������
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

		//�M�Ű}�C
		for(int d=0;d<160;d++) 
			CheckArray[d]=0;
		Pt=0;
	}

	return TRA_t;
}

//////////////////////////////////////////////////////////////////////////////
//�禡: GetFormatInformation [MicroQR��]
//��J: rx(unsigned char) :��l��ư}�C
//      XORmask[]         :XOR�ϥΪ��B�n
//��X: ans(unsigned char):�T�w����15,�ӱ��X�榡�϶�����T
//����:
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
		{                             //�N��T�϶��i��XOR�B��
			if(ans[k]==XORmask[k])  
				ans[k]=0;
			else                   
				ans[k]=1;
		}
		return ans;
	}
*/
	for(int i=1;i<=8;i++)  
		ans[i-1]=datamap[i][8];//Ū�����W���u(1~8)

	for(int j=1;j<=8;j++)  
		ans[15-j]=datamap[8][j]; //Ū�����W��u(8~14)



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
//�禡: GetCharacterCountIndicatorMQR ���o�r�Ʋέp�Ū���[MicroQR��]
//��J: DataArray            (��lBitstream)
//      CharacterCountNum    *(�r���ƶq)
//      CharacterCount       *(���Y�ɪ���)
//      IMnum                *(�s�X�Ҧ�) 
//      BadPT                (�첾���w)
//      ErrorLevel           (MicroQR���X���� ,���I)
//      ModeIndicatorLength  *(�Ҧ����ܲŪ���)
//��X:  *IMnum, *CharacterCount,int ,*CharacterCountNum
//
//����: M1�Ȧ�Numeric , M2�Ȧ�Numeric�PAlphaNumeric�AM3�PM4�h�����㪺4�ؽs�X�Ҧ�
//    
//////////////////////////////////////////////////////////////////////////////
void *GetCharacterCountIndicatorMQR(unsigned char DataArray[],int ErrorLevel,int BadPT,int *IMnum,int *CharacterCount,int *CharacterCountNum,int *ModeIndicatorLength)
{
	int tempN=0;                        //�P�_�ӱ��X����ؽs�X�Ҧ�
	switch(ErrorLevel)
	{
	case 0://M1_X
		 *ModeIndicatorLength=0;
		 *CharacterCount=3;
		 *IMnum=1;                          //M1�Ȧ�Numeric�Ҧ�
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

	//if(EncodeMode==99)  IMnum=99;  //����:�ư��H�k����,���I

	//���o�r�Ʋέp��(CharacterCount)����
	//*CharacterCount = CharacterCountTable[tempA][tempB];   

	//�p��ӱ��X���r���ƶq
	*CharacterCountNum = (*ModeIndicatorLength-1) + *CharacterCount;
	for(int a=0;a<16;a++) 
		CharacterCountTemp[a]=0;  //�ϥΫe�}�C���M��


	for(int a=0;a < *CharacterCount ;a++)
		CharacterCountTemp[a] = DataArray[BadPT+*CharacterCountNum-a];  

	*CharacterCountNum=(1*CharacterCountTemp[0])+(2*CharacterCountTemp[1])+(4*CharacterCountTemp[2])+(8*CharacterCountTemp[3])+(16*CharacterCountTemp[4])+(32*CharacterCountTemp[5])
					  +(64*CharacterCountTemp[6])+(128*CharacterCountTemp[7])+(256*CharacterCountTemp[8])+(512*CharacterCountTemp[9])+(1024*CharacterCountTemp[10]);

	return 0;
}


//////////////////////////////////////////////////////////////////////////////
//�禡: KanjiMode             ���~�r�Ҧ�
//��J: DataArray            (��lBitstream)
//      CharacterCountNum    (�r���ƶq)
//      CharacterCount       (���Y�ɪ���)
//      ModeIndicatorLength  (�Ҧ����ܲŪ���)
//
//��X: char *ans
//
//����: �ѽX_�~�r�Ÿ��Ҧ�
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
	//printf("���B����ư϶��`�줸���׭p�⤽��:B = 4(�Ҧ����ܲ�) + C + 13D  \n");
	//printf("B:����`�줸����  C:�r�Ʋέp�Ŧ��  D:��J�r�����ƶq     \n");
	//DatabitsLong=ModeIndicatorLength + CharacterCount + 13*CharacterCountNum;	
	int PT=CharacterCount+ModeIndicatorLength+BadPT;                                 //�B�z���w
    if (CharacterCountNum==0) return NULL;
   
    /*
	    use Windows-1252 to display, need double size, AACC => AA CC,
		if display correct number one by one, it need 4 times size, AACC=> A A C C
	*/ 
    ans2 =(char *)malloc((2*CharacterCountNum+1)*sizeof(char)); 
	for(int a=0;a<CharacterCountNum;a++)
	{
		//step.1 �@13��ƪ��G�i��X
		Blocknum=   4096* DataArray[PT+0+(a*13)]                                                                                  //13_1
			      +  256*(8*DataArray[PT+1+(a*13)]+4*DataArray[PT+2+(a*13)]+2*DataArray[PT+3+(a*13)]+DataArray[PT+4+(a*13)])      //13_2~5
				  +   16*(8*DataArray[PT+5+(a*13)]+4*DataArray[PT+6+(a*13)]+2*DataArray[PT+7+(a*13)]+DataArray[PT+8+(a*13)])      //13_6~9
				  +    1*(8*DataArray[PT+9+(a*13)]+4*DataArray[PT+10+(a*13)]+2*DataArray[PT+11+(a*13)]+DataArray[PT+12+(a*13)]);  //13_10~13
	
		//step.2 �P���H C0(Hex) = 192 ���oA,B�ȫ�p���`�M�C���B�ϥ�temp[4]�ӼȦs���G
		temp[0]= (Blocknum/192)/16;
		temp[1]= (Blocknum/192)%16;
		temp[2]= (Blocknum%192)/16;
		temp[3]= (Blocknum%192)%16;
		Blocknum2= 4096*temp[0]+256*temp[1]+16*temp[2]+1*temp[3];

		//step.3 �ˬd�Ӧr���O�ݩ�JIS X 0208��檺���Ӱϰ�
		ALC=Blocknum2+49472;
		if(57408<=ALC && ALC<=60351)      
			Blocknum2=ALC;
		else  Blocknum2=Blocknum2+33088;
	}
    ans2[2*CharacterCountNum]='\0';
	return ans2;

}



//////////////////////////////////////////////////////////////////////////////
//�禡: 8bitMode �@��Ҧ� 
//��J: DataArray            (��lBitstream)
//      CharacterCountNum    (�r���ƶq)
//      CharacterCount       (���Y�ɪ���)
//     
//��X: char *ans 
//
//����: �ѽX_8bitMode�Ҧ� ,������ܬ�UTF-8��16�i��X, �ثe���ϥ�
//    
//////////////////////////////////////////////////////////////////////////////
//char *BabitMode(int DataArray[],int CharacterCountNum,int CharacterCount){
//
//    int R=0; //��Ƥ���,�O�ƥ�
//    int Blocknum=0,Blocknum2=0;
//	int n1=0,n2=0,Flag1=0;
//	int DatabitsLong;
//    char *ans2=NULL;   
//
//	//printf("[8bitsBinary Mode]  \n");
//	//printf("���B����ư϶��`�줸���׭p�⤽��:B=4+C+8D  \n");
//	//printf("B:����`�줸����  C:�r�Ʋέp�Ŧ��  D:��J�r�����ƶq     \n");
//
//    DatabitsLong=4 + CharacterCount + (8 * CharacterCountNum);	//8bitMode��
//	int Blockbits = DatabitsLong-CharacterCount-4;
//	int PT=CharacterCount+4;
//
//    ans2=(char *)malloc((CharacterCountNum+2)*sizeof(char));    //��ans���г]�w�O����϶��j�p [AlphaNumeric]
//
//	//�ѽX
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