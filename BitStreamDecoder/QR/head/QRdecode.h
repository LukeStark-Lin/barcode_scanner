#include <stdio.h>
#include <math.h>
#include "../../stdType.h"


#define T2 3706                //Version 40 最大支援碼字數量:532個(Ver.13)  1085(Ver.20)  2185(Ver.30) 3706(Ver.40)
#define T4 29648               //Version 40 最大modules數  8683(Ver.20)  17483(Ver.30) 39648(Ver.40)

// QRdecode
char *QR_bit_stream_decoder(BYTE *rx,int ModulesNum,int *QRENCODEmode);  //輸入:讀取結果