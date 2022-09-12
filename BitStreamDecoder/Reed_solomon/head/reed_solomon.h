#ifndef RSERRORDECODER_H_
#define RSERRORDECODER_H_

#include <stdio.h>
#include <stdlib.h>
#include "../BarcodeType.h"

#define M_MOD(a,b) (a - (b)*(int)((a)/(b)))

int reed_solomon(int *rx, int TotalLength, int DataLength, int type);
void ComputeSyndrome(int *rx, int TotalLength, int DataLength, int startPower, int *syndrome, int *AtoD, int *DtoA);
char CheckIfRxIsCorrect(int *syndrome, int TotalLength, int DataLength);

void Euclid_digital(int *syndrome, int TotalLength, int DataLength, int *sigmax, int *degsigmax, int *omegax, int *degomegax, int *AtoD, int *DtoA);
void PolyDivide_digital(int *in1, int deg1, int *in2, int deg2, int *Q, int *degQ, int *R, int *degR, int *AtoD, int *DtoA);
void FiniteFieldDifferential_digital( int *in, int degin,  int *out, int *degout);

int GFmultiply_DATA_MATRIX(int a, int b, int *AtoD, int *DtoA);
int GFmultiply_QR_CODE(int a, int b, int *AtoD, int *DtoA);

void Euclid_binary(int *syndrome, int TotalLength, int DataLength, int *sigmax, int *degsigmax, int *omegax, int *degomegax, int *AtoD, int *DtoA);
void PolyDivide_binary(int *in1, int deg1, int *in2, int deg2, int *Q, int *degQ, int *R, int *degR, int *AtoD, int *DtoA);
int GFadd_binary(int a, int b);
void FiniteFieldDifferential_binary( int *in, int degin,  int *out, int *degout);

int GFinverse(int a, int *AtoD, int *DtoA);
int GFdivide(int a, int b, int *AtoD, int *DtoA);
int GFpower(int x, int k, int* AtoD, int*DtoA);
void PolyAdd(int *in1, int deg1, int *in2, int deg2, int *out, int *deg);
void PolySub(int *in1, int deg1, int *in2, int deg2, int *out, int *deg);
void PolyMultiply(int *in1, int deg1, int *in2, int deg2, int *out, int *deg);
int FunctionValue(int *coefficient, int deg, int x, int *AtoD, int *DtoA);

#endif
