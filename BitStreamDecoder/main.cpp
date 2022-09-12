#include <stdio.h>
#include <stdlib.h>
#include "stdType.h"
#define TEST_DM
//#define TEST_QR

extern char *QR_bit_stream_decoder(BYTE *rx,int ModulesNum,int *QRENCODEmode);
extern char *Datamatrix_bit_stream_decoder(unsigned char *input,int iGridX,int iGridY);


#ifdef TEST_DM
/* Test 1 */
BYTE input[] = {1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1,1,0,0,0,0,1,1,1,0,1,1,0,1,0,1,0,0,1,1,1,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,1,1,0,1,0,0,1,1,0,1,1,0,0,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,1,1,0,0,0,0,0,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,0,1,1,1,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,0,0,1,0,0,0,0,1,0,0,1,1,1,1,0,0,1,1,0,0,0,1,0,1,1,0,1,1,0,1,1,1,1,0,1,0,1,0,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
#define GRID_X 14
#define GRID_Y 14

/*
BYTE input[] = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,0,1,0,1,0,0,0,0,0,1,1,0,1,1,0,1,0,1,1,1,0,0,1,1,0,1,1,0,0,1,0,0,1,0,0,1,0,0,1,1,0,1,1,0,0,1,1,1,0,1,1,0,0,0,0,1,1,1,1,1,0,1,0,1,1,0,0,1,0,0,0,1,1,0,1,0,1,0,0,1,1,1,0,0,0,0,1,1,1,0,1,0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,1,1,0,1,1,0,1,0,0,0,1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,1,0,1,1,1,0,1,1,0,1,0,0,0,0,0,0,0,1,1,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,0,1,1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,0,0,0,1,0,0,1,1,1,0,0,1,0,1,0,0,1,0,1,1,1,0,1,1,1,0,0,0,0,0,1,0,1,1,0,1,0,0,0,1,1,0,0,1,0,1,1,0,1,1,1,0,0,1,0,1,1,0,1,0,1,1,1,0,0,1,1,0,1,1,1,0,1,0,1,0,0,1,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,1,0,0,0,1,0,1,0,1,0,1,0,0,0,0,1,1,0,0,1,0,0,1,1,1,0,1,1,0,1,0,1,1,0,0,1,1,1,1,1,0,0,1,1,1,0,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,0,1,1,0,1,0,0,1,0,0,0,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,0,0,1,1,1,0,1,0,0,1,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,0,0,1,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,0,1,0,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,1,1,1,0,1,1,1,0,0,0,1,0,1,0,1,1,0,0,0,0,1,0,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,1,1,1,0,0,1,0,1,0,1,1,0,1,1,1,0,0,0,1,1,1,0,0,1,0,1,1,1,0,1,1,1,0,1,1,0,0,1,1,1,0,1,1,0,1,0,1,0,1,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,1,1,0,1,0,0,0,1,1,1,0,1,1,0,1,1,0,0,0,1,1,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,0,0,1,1,1,1,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,1,1,0,1,0,0,1,1,1,1,0,1,1,1,1,1,0,1,0,0,1,1,0,0,1,0,1,0,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,0,0,1,0,1,0,0,1,1,1,1,0,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,1,1,1,0,1,0,1,0,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,1,0,1,1,1,0,0,1,1,0,1,0,1,0,1,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,1,0,1,0,1,1,0,0,1,0,0,1,0,0,1,1,0,1,1,1,1,1,0,0,0,0,0,1,1,0,1,1,1,1,0,1,0,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,0,0,0,1,1,1,0,1,0,1,0,1,1,0,1,0,1,0,1,0,1,0,0,1,0,1,1,0,0,0,0,1,1,1,0,1,0,0,1,1,0,1,0,0,0,1,0,0,0,1,0,0,1,1,1,1,0,1,1,0,0,1,1,0,1,1,1,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,0,1,1,1,0,0,1,0,1,0,0,1,0,1,0,1,1,1,1,1,1,1,0,0,0,1,0,0,1,1,0,1,1,1,0,0,0,0,1,1,0,0,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,1,1,0,0,1,1,1,1,1,1,1,0,0,1,0,0,0,0,1,0,1,1,1,1,1,1,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,1,0,1,1,1,0,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,1,0,0,1,1,1,0,1,1,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,1,1,0,1,1,0,1,1,1,1,0,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,1,1,1,0,1,1,0,1,1,0,0,1,1,1,1,1,1,1,0,0,1,0,1,1,1,0,0,1,1,0,1,1,1,1,0,0,1,1,0,1,1,1,1,0,1,1,1,0,0,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,0,0,1,0,1,0,0,0,1,1,1,0,1,0,1,1,1,0,1,1,0,0,1,0,0,0,1,1,1,0,0,0,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,0,0,1,1,1,1,1,1,0,1,0,0,0,0,0,1,1,0,0,1,0,1,1,1,1,1,1,1,1,0,0,0,1,0,1,0,1,0,0,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,0,1,0,0,0,1,1,1,0,1,1,0,1,0,1,0,0,0,1,0,0,1,0,1,0,1,0,1,0,0,1,1,0,1,0,0,0,1,0,1,0,0,1,0,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,1,0,1,1,1,1,1,0,0,0,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,0,0,1,1,0,1,1,1,1,1,1,0,0,1,0,1,0,0,0,0,0,0,1,1,1,0,1,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,0,0,0,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,1,1,1,1,0,1,0,1,1,0,1,0,1,1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,1,0,1,0,0,1,0,1,0,1,0,1,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,1,1,0,1,0,0,1,1,0,1,0,1,1,1,0,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,1,0,0,1,0,0,0,0,1,1,0,0,1,0,1,0,1,0,0,1,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,1,1,0,0,1,1,0,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1,1,1,1,0,0,1,0,0,1,1,1,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,0,1,0,0,1,1,0,1,1,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,0,1,1,0,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,0,0,0,1,0,0,1,1,1,1,1,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,0,0,0,1,1,1,1,0,1,1,0,1,0,1,0,0,1,1,1,0,0,1,0,1,1,0,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,1,0,1,1,1,1,0,1,0,0,1,0,1,0,0,1,1,1,1,1,1,1,1,0,1,1,0,0,1,1,1,1,0,1,0,1,1,0,1,0,1,0,1,0,1,1,1,0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,1,0,0,0,0,0,1,1,1,0,0,0,1,1,0,0,1,0,0,1,0,0,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,0,0,1,1,0,1,1,0,0,0,0,0,0,0,1,0,0,1,0,1,1,0,0,1,1,1,0,0,1,1,0,1,1,1,0,1,1,1,1,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,0,0,1,0,0,0,1,0,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,0,0,0,1,0,1,1,1,1,0,0,1,1,1,0,1,1,1,0,1,1,0,0,0,1,0,0,1,1,0,0,1,0,1,0,1,0,0,1,0,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,0,1,0,0,1,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,0,0,1,0,0,1,1,1,1,0,1,1,1,0,0,1,0,1,1,0,1,0,0,1,1,1,0,0,0,0,1,0,0,1,1,0,0,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,1,1,0,1,0,1,0,1,1,0,1,1,0,0,0,1,1,1,0,0,0,0,1,1,1,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,0,0,1,1,1,0,1,0,1,0,0,1,1,1,0,0,1,0,1,0,1,1,1,0,1,0,1,0,0,0,1,1,1,1,0,1,1,1,1,1,1,0,1,0,0,0,1,0,0,0,0,0,0,1,1,1,0,1,1,1,1,0,0,1,0,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,0,0,0,0,0,0,1,1,0,1,0,1,1,0,1,0,1,0,0,1,1,1,0,0,1,1,0,0,1,0,0,0,0,0,1,0,1,0,0,0,1,1,0,1,0,0,1,0,0,1,1,1,0,0,1,0,0,0,1,1,1,0,0,1,1,0,1,0,0,1,0,1,1,1,1,1,1,0,1,1,0,1,0,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,1,0,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0,1,0,1,0,1,1,0,1,1,0,0,1,1,0,0,1,0,0,1,1,0,0,1,0,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,1,1,1,0,1,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,0,1,1,1,0,1,0,0,0,1,0,1,1,1,0,1,0,1,0,0,1,0,0,0,0,1,0,1,0,0,1,0,0,0,0,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,0,0,1,0,1,1,0,1,1,1,0,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,0,1,1,0,1,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,1,0,0,0,1,0,1,1,0,0,0,0,1,1,0,0,0,1,0,0,1,1,1,0,0,0,1,1,0,1,0,0,0,0,1,0,1,0,1,1,0,0,1,0,1,0,0,1,0,0,1,0,1,1,0,0,1,0,1,0,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,1,0,0,1,0,1,1,0,1,1,1,0,0,1,0,1,1,0,1,0,1,1,1,1,1,1,0,1,1,1,0,1,1,0,0,1,1,1,0,0,1,1,0,1,0,1,1,0,1,0,1,1,1,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,1,0,0,1,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,1,0,1,1,0,1,1,0,0,0,0,1,1,1,1,0,0,1,1,1,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,1,0,0,1,1,1,1,0,1,1,1,1,0,0,1,0,1,0,1,1,1,0,1,0,1,1,0,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,0,0,0,1,1,1,0,1,1,0,0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,1,0,1,0,1,1,0,1,0,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,0,0,0,1,1,0,1,1,1,1,0,0,1,1,0,1,0,1,0,0,0,1,1,0,1,0,0,1,0,1,0,1,0,0,1,0,1,0,0,1,0,1,1,1,0,1,0,1,1,0,1,0,0,1,1,0,0,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,0,0,0,0,0,1,1,1,0,1,1,1,0,1,0,0,0,0,1,0,0,0,1,1,1,1,0,1,1,1,1,1,0,0,1,0,1,0,1,1,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,1,0,1,1,1,1,0,1,1,1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,0,1,0,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,0,0,1,0,1,0,0,1,0,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,1,1,0,1,1,1,0,1,1,1,1,1,0,0,0,1,1,1,1,0,1,0,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,1,1,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
#define GRID_X 64
#define GRID_Y 64
*/
#else
/* Test 1 */
BYTE input[] = {1,1,1,1,1,1,1,0,0,1,0,1,0,1,1,0,1,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,1,0,0,1,0,0,0,0,0,1,1,0,1,1,1,0,1,0,1,1,1,1,0,0,1,0,1,1,0,1,0,1,0,1,1,0,1,0,1,0,0,0,1,1,0,1,0,0,1,0,1,1,1,0,1,1,0,1,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,0,0,0,0,0,0,1,0,0,1,1,1,0,1,1,0,1,0,1,1,1,0,1,1,0,1,1,1,0,1,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,0,1,0,0,1,0,0,0,0,1,1,1,0,1,0,1,1,1,0,1,1,0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0,1,0,1,1,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,0,0,0,0,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,1,1,0,1,0,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,0,1,0,1,1,0,0,0,1,0,0,1,1,0,1,0,1,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,1,0,1,1,1,0,1,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,0,0,0,1,1,1,1,1,1,1,0,1,1,0,0,0,1,1,0,0,1,1,1,1,0,1,0,0,0,0,1,1,0,0,0,1,0,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,1,1,1,1,0,1,1,1,1,1,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,1,0,1,1,0,1,1,1,1,0,1,1,1,0,0,0,0,1,0,1,0,0,1,0,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,1,0,0,0,1,1,0,1,0,0,1,0,0,0,1,1,0,1,0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,1,0,1,0,1,0,0,0,1,0,0,0,0,1,1,1,0,0,1,0,1,1,0,0,1,0,1,1,1,0,0,1,1,0,0,1,1,0,1,1,1,0,0,1,1,0,1,1,0,1,1,0,0,1,1,1,1,1,0,0,1,0,1,1,1,1,0,0,0,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,0,0,1,0,1,1,0,0,0,0,1,0,0,1,0,0,1,1,0,0,0,0,0,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1,0,1,1,1,1,1,1,0,1,0,1,0,0,0,0,0,1,1,1,0,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,0,1,1,0,1,0,0,0,1,0,1,0,0,0,0,1,0,1,0,0,1,1,0,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1,0,0,1,0,0,0,1,1,0,0,0,1,1,1,0,0,1,0,1,0,0,0,1,1,1,1,1,1,1,0,1,1,0,1,0,1,0,1,1,1,1,0,1,0,0,1,1,1,0,1,0,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,0,1,1,0,0,0,0,0,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,0,1,1,1,0,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,1,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,1,0,0,0,0,1,1,0,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,1,1,1,0,0,1,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,1,0,0,1,0,1,0,1,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,1,1,0,0,1,1,0,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,1,0,1,1,0,0,1,0,1,0,1,0,1,0,0,1,0,1,1,0,0,0,1,1,1,0,1,0,1,0,1,0,1,1,0,0,1,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,0,0,0,1,1,1,1,0,1,0,1,1,0,0,1,0,0,0,0,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,0,1,0,1,0,1,1,1,1,0,0,0,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0,0,1,1,1,0,1,0,0,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,1,0,1,0,0,0,1,1,1,0,1,1,0,0,0,0,0,0,1,0,1,1,0,1,1,0,0,0,1,1,1,1,0,0,0,0,1,1,0,1,0,0,1,1,0,0,1,1,0,1,0,0,1,0,0,0,0,1,1,0,1,1,1,1,0,0,0,0,1,1,0,1,0,1,1,0,1,1,0,1,0,0,1,0,1,0,0,1,1,1,0,1,1,0,0,0,0,1,1,1,1,0,1,0,1,0,0,1,1,0,1,1,1,1,0,1,0,1,1,0,0,0,1,0,1,1,1,1,1,1,1,0,1,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,1,1,0,1,1,1,0,1,0,1,1,1,1,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,1,0,1,0,1,1,1,0,1,1,0,0,0,0,0,1,0,1,1,1,0,0,1,1,1,1,0,0,0,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,1,0,1,1,1,1,1,1,0,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,0,0,1,0,1,0,1,0,0,1,0,0,1,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,1,0,0,1,1,1,0,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,0,0,1,1,0,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,0,1,0,0,1,1,1,0,0,0,0,0,1,0,1,0,0,1,0,0,1,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,0,1,1,1,1,1,1,1,0,0,0,1,0,1,0,1,1,1,1,1,1,1,0,1,1,0,1,0,1,1,1,0,0,1,1,1,0,0,1,0,0,0,0,1,1,1,1,0,1,0,0,1,1,0,1,1,0,0,1,1};
#define TEST_W 45

/*
BYTE input[] = {1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,0,0,0,0,1,1,1,1,0,0,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,1,1,1,0,1,0,0,1,0,1,1,1,0,1,1,0,0,0,0,1,1,0,1,0,0,0,0,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1,1,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,0,1,1,0,1,1,0,1,1,1,0,1,0,0,1,1,0,0,1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,0,0,1,0,1,1,1,0,1,1,0,1,1,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,1,0,1,1,0,0,1,0,0,0,0,1,0,1,1,1,0,1,1,0,0,0,0,0,1,0,1,0,0,1,1,1,0,0,0,0,1,0,1,1,1,0,1,1,1,1,1,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,0,0,0,1,0,1,1,0,1,0,0,1,1,0,1,0,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,1,0,0,0,1,0,0,0,0,1,1,1,0,0,0,1,0,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,0,0,0,0,1,0,1,0,1,0,0,1,0,1,1,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,0,1,1,1,1,1,1,1,1,0,1,0,0,0,0,1,1,0,1,0,0,1,0,1,0,1,0,1,1,1,1,0,1,1,0,0,1,0,0,0,0,1,1,1,1,0,1,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1,1,0,1,0,1,1,0,0,1,0,0,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,1,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,0,1,0,0,0,0,0,0,1,1,0,1,0,1,1,0,0,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1,1,1,1,0,1,0,1,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,1,0,1,1,0,1,1,0,0,0,1,0,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1,1,1,0,1,1,1,0,1,1,0,1,0,1,0,0,1,0,1,1,1,1,0,1,0,0,0,0,1,1,0,0,0,1,0,1,0,1,0,1,1,0,0,0,0,1,1,1,1,1,0,0,1,1,0,1,0,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,0,0,1,1,1,1,1,1,1,0,1,0,0,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,1,1,1,0,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,0,1,1,0,1,0,1,0,1,0,1,0,0,1,1,1,1,0,0,1,0,1,0,1,1,0,1,0,0,1,1,1,1,0,1,0,0,1,1,1,1,0,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,1,1,0,1,1,1,1,1,0,1,0,0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,1,1,0,0,1,0,1,0,1,1,1,1,0,0,0,1,1,0,0,1,0,1,0,0,0,1,1,0,1,0,1,1,0,1,0,0,0,1,0,1,1,0,0,1,0,0,1,0,1,0,1,1,0,1,1,1,1,1,0,0,0,0,1,0,0,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,0,1,0,0,1,1,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,1,1,1,1,0,0,1,1,0,1,1,0,0,0,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,1,1,0,1,1,1,0,0,0,0,0,1,0,0,1,0,0,0,1,1,0,0,0,1,1,1,0,0,1,1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,1,1,1,0,1,0,1,0,0,1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,0,1,1,0,1,0,1,1,1,0,1,0,1,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,1,1,0,0,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,0,1,0,1,1,0,0,0,0,0,1,0,0,0,0,1,0,1,1,1,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,1,1,0,0,1,0,1,1,1,1,1};
#define TEST_W 37

BYTE input[] = {1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,1,1,0,1,0,0,0,1,1,0,1,0,1,1,0,1,0,1,0,0,0,0,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1,1,1,0,1,0,0,1,0,0,1,0,1,1,0,1,1,1,1,1,0,1,1,0,1,0,1,1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,1,0,1,1,1,0,1,1,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,0,0,1,1,0,1,1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,0,0,0,0,1,0,1,1,1,0,1,1,1,1,0,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,0,1,1,0,1,0,1,1,0,0,1,0,1,0,1,0,0,1,1,0,1,1,1,0,1,0,0,0,0,1,0,0,0,0,0,1,0,1,0,0,1,1,1,1,0,1,0,1,1,1,1,1,0,0,1,0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,1,1,1,1,0,0,1,0,1,1,1,0,1,1,0,0,0,0,1,0,0,1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,1,1,0,1,1,1,1,0,1,1,0,1,0,0,1,0,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,1,1,0,0,1,1,1,1,0,1,1,0,0,0,1,1,1,1,0,0,1,0,0,1,1,1,0,0,0,0,1,0,1,1,1,1,0,1,0,1,1,0,1,1,0,1,0,1,0,1,1,0,1,0,1,1,0,1,1,1,1,0,0,0,0,0,1,0,0,1,1,1,0,1,1,1,1,1,1,1,1,0,0,1,0,1,0,1,0,0,0,1,1,1,1,1,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,1,0,0,0,0,1,0,0,0,1,1,0,1,1,1,0,0,1,1,1,1,0,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,1,1,0,1,0,0,0,1,1,0,0,0,0,1,1,0,1,1,1,0,1,0,0,0,1,0,0,1,1,0,0,1,0,0,0,1,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,1,0,0,1,1,1,0,0,1,0,1,1,1,1,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,1,1,1,0,0,1,0,1,0,0,1,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,1,0,0,1,1,1,0,1,1,0,1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,1,0,0,0,1,0,0,0,1,0,1,1,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,1,0,1,0,1,1,0,0,0,1,0,0,0,0,0,1,0,0,1,1,1,0,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,1,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,0,1,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,0,1,1,0,0,1,1,0,1,1,0,0,0,1,0,1,0,1,0,1,1,0,1,1,1,0,1,0,0,0,0,0,1,0,0,0,1,1,1,1,0,0,1,0,1,0,1,1,1,0,0,0,0,1,0,0,0,0,0,1,0,1,1,0,1,0,0,0,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,1,1,1,0,1,0,0,1,1,1,0,1};
#define TEST_W 33
*/
#endif


int main()
{
	int QRENCODEmode;
	char *result = NULL;

#ifdef TEST_DM
	for(int i = 0; i < GRID_Y; i++)
	{
		for(int j = 0; j < GRID_X;j++)
		{
			if(input[i*GRID_X+j])
				printf("��",input[i*GRID_X+j]);
			else
				printf("��",input[i*GRID_X+j]);
		}
		printf("\n");
	}

	result = Datamatrix_bit_stream_decoder(input,GRID_X,GRID_Y);
#else	
	for(int i = 0; i < TEST_W; i++)
	{
		for(int j = 0; j < TEST_W;j++)
		{
			if(input[i*TEST_W+j])
				printf("��",input[i*TEST_W+j]);
			else
				printf("��",input[i*TEST_W+j]);
		}
		printf("\n");
	}
	result = QR_bit_stream_decoder(input,TEST_W,&QRENCODEmode);
#endif

	if(result != NULL)
	{
		printf("ans : %s\n",result);
		free(result);

	}else
		printf("can not decode\n");



	system("PAUSE");
	return 0;
}