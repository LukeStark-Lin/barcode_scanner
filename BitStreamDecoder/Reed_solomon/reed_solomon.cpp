#include "./head/reed_solomon.h"

int AtoD_DATA_MATRIX[256] = {1,2,4,8,16,32,64,128,45,90,180,69,138,57,114,228,229,231,227,235,251,219,155,27,54,108,216,157,23,46,92,184,93,186,89,178,73,146,9,18,36,72,144,13,26,52,104,208,141,55,110,220,149,7,14,28,56,112,224,237,247,195,171,123,246,193,175,115,230,225,239,243,203,187,91,182,65,130,41,82,164,101,202,185,95,190,81,162,105,210,137,63,126,252,213,135,35,70,140,53,106,212,133,39,78,156,21,42,84,168,125,250,217,159,19,38,76,152,29,58,116,232,253,215,131,43,86,172,117,234,249,223,147,11,22,44,88,176,77,154,25,50,100,200,189,87,174,113,226,233,255,211,139,59,118,236,245,199,163,107,214,129,47,94,188,85,170,121,242,201,191,83,166,97,194,169,127,254,209,143,51,102,204,181,71,142,49,98,196,165,103,206,177,79,158,17,34,68,136,61,122,244,197,167,99,198,161,111,222,145,15,30,60,120,240,205,183,67,134,33,66,132,37,74,148,5,10,20,40,80,160,109,218,153,31,62,124,248,221,151,3,6,12,24,48,96,192,173,119,238,241,207,179,75,150,1};
int DtoA_DATA_MATRIX[256] = {255,255,1,240,2,225,241,53,3,38,226,133,242,43,54,210,4,195,39,114,227,106,134,28,243,140,44,23,55,118,211,234,5,219,196,96,40,222,115,103,228,78,107,125,135,8,29,162,244,186,141,180,45,99,24,49,56,13,119,153,212,199,235,91,6,76,220,217,197,11,97,184,41,36,223,253,116,138,104,193,229,86,79,171,108,165,126,145,136,34,9,74,30,32,163,84,245,173,187,204,142,81,181,190,46,88,100,159,25,231,50,207,57,147,14,67,120,128,154,248,213,167,200,63,236,110,92,176,7,161,77,124,221,102,218,95,198,90,12,152,98,48,185,179,42,209,37,132,224,52,254,239,117,233,139,22,105,27,194,113,230,206,87,158,80,189,172,203,109,175,166,62,127,247,146,66,137,192,35,252,10,183,75,216,31,83,33,73,164,144,85,170,246,65,174,61,188,202,205,157,143,169,82,72,182,215,191,251,47,178,89,151,101,94,160,123,26,112,232,21,51,238,208,131,58,69,148,18,15,16,68,17,121,149,129,19,155,59,249,70,214,250,168,71,201,156,64,60,237,130,111,20,93,122,177,150};

int AtoD_QR_CODE[256] = {1,2,4,8,16,32,64,128,29,58,116,232,205,135,19,38,76,152,45,90,180,117,234,201,143,3,6,12,24,48,96,192,157,39,78,156,37,74,148,53,106,212,181,119,238,193,159,35,70,140,5,10,20,40,80,160,93,186,105,210,185,111,222,161,95,190,97,194,153,47,94,188,101,202,137,15,30,60,120,240,253,231,211,187,107,214,177,127,254,225,223,163,91,182,113,226,217,175,67,134,17,34,68,136,13,26,52,104,208,189,103,206,129,31,62,124,248,237,199,147,59,118,236,197,151,51,102,204,133,23,46,92,184,109,218,169,79,158,33,66,132,21,42,84,168,77,154,41,82,164,85,170,73,146,57,114,228,213,183,115,230,209,191,99,198,145,63,126,252,229,215,179,123,246,241,255,227,219,171,75,150,49,98,196,149,55,110,220,165,87,174,65,130,25,50,100,200,141,7,14,28,56,112,224,221,167,83,166,81,162,89,178,121,242,249,239,195,155,43,86,172,69,138,9,18,36,72,144,61,122,244,245,247,243,251,235,203,139,11,22,44,88,176,125,250,233,207,131,27,54,108,216,173,71,142,1};
int DtoA_QR_CODE[256] = {255,255,1,25,2,50,26,198,3,223,51,238,27,104,199,75,4,100,224,14,52,141,239,129,28,193,105,248,200,8,76,113,5,138,101,47,225,36,15,33,53,147,142,218,240,18,130,69,29,181,194,125,106,39,249,185,201,154,9,120,77,228,114,166,6,191,139,98,102,221,48,253,226,152,37,179,16,145,34,136,54,208,148,206,143,150,219,189,241,210,19,92,131,56,70,64,30,66,182,163,195,72,126,110,107,58,40,84,250,133,186,61,202,94,155,159,10,21,121,43,78,212,229,172,115,243,167,87,7,112,192,247,140,128,99,13,103,74,222,237,49,197,254,24,227,165,153,119,38,184,180,124,17,68,146,217,35,32,137,46,55,63,209,91,149,188,207,205,144,135,151,178,220,252,190,97,242,86,211,171,20,42,93,158,132,60,57,83,71,109,65,162,31,45,67,216,183,123,164,118,196,23,73,236,127,12,111,246,108,161,59,82,41,157,85,170,251,96,134,177,187,204,62,90,203,89,95,176,156,169,160,81,11,245,22,235,122,117,44,215,79,174,213,233,230,231,173,232,116,214,244,234,168,80,88,175};


typedef void (*AlgorithmEuclid)(int *syndrome, int TotalLength, int DataLength, int *sigmax, int *degsigmax,int *omegax, int *degomegax, int *AtoD, int *DtoA);
typedef void (*OperatorPolyDivide)(int *in1, int deg1, int *in2, int deg2, int *Q, int *degQ,int *R, int *degR, int *AtoD, int *DtoA);
typedef int (*OperatorAdd)(int a, int b);
typedef int (*OperatorMultiply)(int a, int b, int *AtoD, int *DtoA);
typedef void (*OperatorFiniteFieldDifferential)( int *in, int degin,  int *out, int *degout);

AlgorithmEuclid Euclid;
OperatorPolyDivide PolyDivide;
OperatorAdd GFadd;
OperatorMultiply GFmultiply;
OperatorFiniteFieldDifferential FiniteFieldDifferential;

int FIELD_NUM = 0;
int *AtoD = NULL, *DtoA = NULL;

/****************************
** Function:reed_solomon
** return status:
**    [0] some error, Can not correcting
**    [1] some error, Can correcting
**	  [2] no error
****************************/
int reed_solomon(int *rx, int TotalLength, int DataLength, int type)
{
    int i = 0;

    int R = TotalLength-DataLength;
    int startPower = 0;
    int *syndrome = NULL;

    int *sigmax = NULL, *omegax = NULL;
    int degsigmax = 0, degomegax = 0;
    int *diffsigmax = NULL;
    int degdiffsigmax = 0;
    int *ErrorMagnitude = NULL;

    int DecodeStatusFlag = 0; 
    int ErrorCount = 0;
    int tmp = 0;

    if(TotalLength <= DataLength) {
        return 0;
    }

    // Set parameters & functions for different barcode type
    switch(type)
    {
        case DATA_MATRIX:
            startPower = 1;
            FIELD_NUM = 256;
            AtoD = AtoD_DATA_MATRIX;
            DtoA = DtoA_DATA_MATRIX;

            Euclid = Euclid_binary;
            PolyDivide = PolyDivide_binary;
            GFadd = GFadd_binary;
            GFmultiply = GFmultiply_DATA_MATRIX;
            FiniteFieldDifferential = FiniteFieldDifferential_binary;
            break;
        case QR_CODE:
            startPower = 0;
            FIELD_NUM = 256;
            AtoD = AtoD_QR_CODE;
            DtoA = DtoA_QR_CODE;

            Euclid = Euclid_binary;
            PolyDivide = PolyDivide_binary;
            GFadd = GFadd_binary;
            GFmultiply = GFmultiply_QR_CODE;
            FiniteFieldDifferential = FiniteFieldDifferential_binary;
            break;
        default:
            return 0; // can not correcting
            break;
    }

    // Compute Syndrome
        syndrome = (int *)malloc(R*sizeof(int));
        ComputeSyndrome(rx, TotalLength, DataLength, startPower, syndrome, AtoD, DtoA);
    // Check Syndrome
        if(CheckIfRxIsCorrect(syndrome, TotalLength, DataLength)) {
            //printf("[RS] There is no error in the codewords.\n");
            free(syndrome);
            return 2; //完全沒有錯誤
        }

    // Solve Key Eqation by Extended Euclid's alogrithm
    // Finding out Sigma(x) & Omega(x)
        sigmax = (int *)malloc((R+1)*sizeof(int));
        omegax = (int *)malloc((R+1)*sizeof(int));
        Euclid(syndrome, TotalLength, DataLength, sigmax, &degsigmax, omegax, &degomegax, AtoD, DtoA);

    // Use Time Approach to correct error
        DecodeStatusFlag = 0; // 代表無法修正
        if( sigmax[0] != 0 && degomegax < degsigmax ) {
            ErrorCount = 0;
            ErrorMagnitude = (int *)malloc(TotalLength*sizeof(int));

            // 計算sigmax的微分，存到diffsigmax裡面
            degdiffsigmax = degsigmax-1;
            diffsigmax = (int *)malloc((degdiffsigmax+1)*sizeof(int));
            FiniteFieldDifferential(sigmax, degsigmax, diffsigmax, &degdiffsigmax);

            // 對TotalLength個根去做(alpha的0次方、alpha的-1次方(FIELD_NUM-1 -1次方
            for(i = 0 ; i < TotalLength ; i++) {
                //    sigmax( alpha的-i+FIELD_NUM-1次方 )是0
                // 且 diffsigmax( alpha的-i+FIELD_NUM-1次方 )不是0
                // 表示i位置有錯誤

                tmp = M_MOD(i, FIELD_NUM-1);

                if( FunctionValue(sigmax, degsigmax, AtoD[-tmp + FIELD_NUM-1], AtoD, DtoA ) == 0 && 
		            FunctionValue(diffsigmax, degdiffsigmax, AtoD[-tmp + FIELD_NUM-1], AtoD, DtoA) != 0)
                {
                    ErrorCount++;
                    switch(type) {
                        case DATA_MATRIX:
                            ErrorMagnitude[TotalLength-1-i] = GFdivide( FunctionValue( omegax, degomegax, AtoD[-tmp + FIELD_NUM-1], AtoD, DtoA ),
		                              FunctionValue( diffsigmax, degdiffsigmax, AtoD[-tmp + FIELD_NUM-1],AtoD, DtoA ),
					                  AtoD, DtoA );
                            break;

                        case QR_CODE:
                            ErrorMagnitude[TotalLength-1-i] = GFdivide( GFmultiply(AtoD[tmp], FunctionValue( omegax, degomegax, AtoD[-tmp + FIELD_NUM-1], AtoD, DtoA ), AtoD, DtoA),
		                              FunctionValue( diffsigmax, degdiffsigmax, AtoD[-tmp + FIELD_NUM-1],AtoD, DtoA ),
					                  AtoD, DtoA );
                            break;

                        default:
                            ErrorMagnitude[TotalLength-1-i] = GFdivide( FunctionValue( omegax, degomegax, AtoD[-tmp + FIELD_NUM-1], AtoD, DtoA ),
		                              FunctionValue( diffsigmax, degdiffsigmax, AtoD[-tmp + FIELD_NUM-1],AtoD, DtoA ),
					                  AtoD, DtoA );
                            break;
                    }
                    //printf("[RS] ErrorMagnitude[%d] = %d\n", TotalLength-1-i, ErrorMagnitude[TotalLength-1-i]);
                }
                else { // i位置沒有錯 
		            ErrorMagnitude[TotalLength-1-i] = 0;
	            }
            }

            // sigmax的次方數就是錯誤數量
            // 所以算出來的ErrorCount跟degsigmax(sigmax的degree)應該要相等!
            if(ErrorCount == degsigmax) {
                //printf("[RS] There are %d errors.\n", ErrorCount);
                //printf("[RS] Error Correction Successed!!\n");

                DecodeStatusFlag = 1; // 代表可以修正
            }
        }

        if(DecodeStatusFlag == 1) {
            for(i = 0; i < TotalLength; ++i) {
	            rx[i] = GFadd( rx[i], ErrorMagnitude[i] );
	        }
        }
    free(syndrome);
    free(sigmax);
    free(omegax);

    if (diffsigmax != NULL) free(diffsigmax);
    if (ErrorMagnitude != NULL) free(ErrorMagnitude);

    return DecodeStatusFlag;
}

void ComputeSyndrome(int *rx, int TotalLength, int DataLength, int startPower, int *syndrome, int *AtoD, int *DtoA)
{
    int i = 0, j = 0;
    int R = TotalLength-DataLength; // 原始codeword有DataLength個，加上修正碼後有TotalLength個，所以修正碼共有R=TotalLength-DataLength個
    int tmp = 0;
    int *TempRx = (int *)malloc(TotalLength*sizeof(int));

    for(i = 0; i < TotalLength; ++i) {
        TempRx[i] = rx[i];
    }

    for(j = 0; R>0; ++j, R--) {
        syndrome[j] = 0;
        if(j==0&&startPower==0) {
            for(i = 0; i < TotalLength; ++i) {
                syndrome[j] = GFadd(syndrome[j], TempRx[i]);
            }
            continue;
        }
        for(i = 0; i < TotalLength; ++i) {
            // 要計算α^(N-1-i)
            // 為預防 N-1-i 的值大於 FIELD_NUM-1
            // 會超過我們建立的Alpha 轉 Digit表格的大小
            // 所以先做mod
            tmp = M_MOD(TotalLength-1-i, FIELD_NUM-1);

            TempRx[i] = GFmultiply(AtoD[tmp], TempRx[i], AtoD, DtoA);
            syndrome[j] = GFadd(syndrome[j], TempRx[i]);
        }
    }

    free(TempRx);
}

char CheckIfRxIsCorrect(int *syndrome, int TotalLength, int DataLength)
{
    int i = 0;
    int R = TotalLength-DataLength;

    for(i = 0; i < R; ++i) {
        if(syndrome[i]!=0) return 0;
    }

    return 1;
}

/****************************
** Inputs:
**       s: Syndrome
**       TotalLength: codewords總長度(message長度+RS碼長度)
**       DataLength: message長度
**    AtoD: α的次方數 轉 10進位     的表格
**    DtoA: 10進位     轉 α的次方數 的表格
**
** Outputs:
**     sigmax: σ(x)
**  degsigmax: σ(x)的degree
**     omegax: ω(x)
**  degomegax: ω(x)的degree
**
** 說明:
**    利用Extended Euclidean Algorithm，輸入s(x)、x^2t
**    計算出σ(x)和ω(x)
****************************/
void Euclid_digital(int *syndrome, int TotalLength, int DataLength, int *sigmax, int *degsigmax,
            int *omegax, int *degomegax, int *AtoD, int *DtoA)
{
    int r = TotalLength-DataLength;
    int i = 0;

    int degs0 = 0;
    int degs1 = 0;  
    int degt1 = 0;
    int degAx = 0;
    int degBx = 0;
    int degQ = 0;
    int degR = 0;
    int degs1Q = 0;
    int degt1Q = 0;
    int degtempS = 0;
    int degtempT = 0;

    int *s0    = (int *) calloc( r+1, sizeof(int) );
    int *s1    = (int *) calloc( r+1, sizeof(int) );
    int *t0    = (int *) calloc( r+1, sizeof(int) );
    int *t1    = (int *) calloc( r+1, sizeof(int) );
    int *Ax    = (int *) calloc( r+1, sizeof(int) );
    int *Bx    = (int *) calloc( r+1, sizeof(int) );
    int *Q     = (int *) calloc( r+1, sizeof(int) );
    int *R     = (int *) calloc( r+1, sizeof(int) );
    int *s1Q   = (int *) calloc( r+1, sizeof(int) );
    int *t1Q   = (int *) calloc( r+1, sizeof(int) );
    int *tempS = (int *) calloc( r+1, sizeof(int) );
    int *tempT = (int *) calloc( r+1, sizeof(int) );

    int v = 0;
    // 設定Ax為x的r次方
        Ax[r] = 1;
        degAx = r;
    
    // 設定Bx為s
        for(i = 0; i < r; ++i) {
            Bx[i] = syndrome[i];
            if(syndrome[i] != 0)
                degBx = i;
        }
    
    // 設定s0為1
        s0[0] = 1;
    
    // 設定t1為1
        t1[0] = 1;
    v = r/2-1; /*  ceil[ (n-k)/2 ]-1 */

    for(;;) {
        PolyDivide( Ax, degAx, Bx, degBx, Q, &degQ, R, &degR, AtoD, DtoA); // Ax = Bx*Q + R
        PolyMultiply( s1, degs1, Q, degQ, s1Q, &degs1Q);       // s1Q = s1*Q
        PolyMultiply( t1, degt1, Q, degQ, t1Q, &degt1Q);       // t1Q = t1*Q
        PolySub(s0, degs0, s1Q, degs1Q, tempS, &degtempS);                 // tempS = s0-s1Q
        PolySub(t0, degs0, t1Q, degt1Q, tempT, &degtempT);                 // tempT = t0-t1Q

        // 更新s0、s1、t0、t1
            for(i = 0; i <= r; ++i) {
                s0[i] = s1[i];
                s1[i] = tempS[i];

                t0[i] = t1[i];
                t1[i] = tempT[i];
            }

            degs0 = degs1;
            degs1 = degtempS;
            //degt0 = degt1;
            degt1 = degtempT;

            if(degR <= v) {
                break;
            }

        // 更新Ax、Bx
            for(i = 0; i <= r; ++i) {
                Ax[i] = Bx[i];
                Bx[i] = R[i];
            }

            degAx=degBx;
            degBx=degR;
    }

    // 設定sigmax、omegax
        for(i = 0; i <= r; ++i) {
            sigmax[i] = t1[i];
            omegax[i] = R[i];
        }

        *degsigmax = degt1;
        *degomegax = degR; 

    free(s0);
    free(s1);
    free(t0);
    free(t1);
    free(Ax);
    free(Bx);
    free(Q);
    free(R);
    free(s1Q);
    free(t1Q);
    free(tempS);
    free(tempT);
}



void Euclid_binary(int *syndrome, int TotalLength, int DataLength, int *sigmax, int *degsigmax, int *omegax, int *degomegax, int *AtoD, int *DtoA)
{
    int r = TotalLength-DataLength;
    int i = 0;

    int degs0 = 0;
    int degs1 = 0;  
    //int degt0 = 0;
    int degt1 = 0;
    int degAx = 0;
    int degBx = 0;
    int degQ = 0;
    int degR = 0;
    int degs1Q = 0;
    int degt1Q = 0;
    int degtempS = 0;
    int degtempT = 0;

    int *s0    = (int *) calloc( r+1, sizeof(int) );
    int *s1    = (int *) calloc( r+1, sizeof(int) );
    int *t0    = (int *) calloc( r+1, sizeof(int) );
    int *t1    = (int *) calloc( r+1, sizeof(int) );
    int *Ax    = (int *) calloc( r+1, sizeof(int) );
    int *Bx    = (int *) calloc( r+1, sizeof(int) );
    int *Q     = (int *) calloc( r+1, sizeof(int) );
    int *R     = (int *) calloc( r+1, sizeof(int) );
    int *s1Q   = (int *) calloc( r+1, sizeof(int) );
    int *t1Q   = (int *) calloc( r+1, sizeof(int) );
    int *tempS = (int *) calloc( r+1, sizeof(int) );
    int *tempT = (int *) calloc( r+1, sizeof(int) );

    // 設定Ax為x的r次方
        Ax[r] = 1;
        degAx = r;
    
    // 設定Bx為s
        for(i = 0; i < r; ++i) {
            Bx[i] = syndrome[i];
            if(syndrome[i] != 0)
                degBx = i;
        }
    
    // 設定s0為1
        s0[0] = 1;
    
    // 設定t1為1
        t1[0] = 1;

    //int u = r/2;   /* floor[ (n-k)/2 ]   */
    int v = r/2-1; /*  ceil[ (n-k)/2 ]-1 */

    for(;;) {
        PolyDivide( Ax, degAx, Bx, degBx, Q, &degQ, R, &degR, AtoD, DtoA); // Ax = Bx*Q + R
        PolyMultiply( s1, degs1, Q, degQ, s1Q, &degs1Q);       // s1Q = s1*Q
        PolyMultiply( t1, degt1, Q, degQ, t1Q, &degt1Q);       // t1Q = t1*Q
        PolyAdd(s0, degs0, s1Q, degs1Q, tempS, &degtempS);                 // tempS = s0-s1Q
        PolyAdd(t0, degs0, t1Q, degt1Q, tempT, &degtempT);                 // tempT = t0-t1Q
        // 更新s0、s1、t0、t1
            for(i = 0; i <= r; ++i) {
                s0[i] = s1[i];
                s1[i] = tempS[i];

                t0[i] = t1[i];
                t1[i] = tempT[i];
            }

            degs0 = degs1;
            degs1 = degtempS;
            //degt0 = degt1;
            degt1 = degtempT;

            if(degR <= v) {
                break;
            }

        // 更新Ax、Bx
            for(i = 0; i <= degBx; ++i) {
                Ax[i] = Bx[i];
            }
            for(i = 0; i <= degR; ++i) {
                Bx[i] = R[i];
            }

            degAx=degBx;
            degBx=degR;
    }

    // 設定sigmax、omegax
        for(i = 0; i <= r; ++i) {
            sigmax[i] = t1[i];
            omegax[i] = R[i];
        }

        *degsigmax = degt1;
        *degomegax = degR; 

    free(s0);
    free(s1);
    free(t0);
    free(t1);
    free(Ax);
    free(Bx);
    free(Q);
    free(R);
    free(s1Q);
    free(t1Q);
    free(tempS);
    free(tempT);
}

int GFadd_binary(int a, int b)
{
    return (unsigned int)a^(unsigned int)b;
}

int GFmultiply_DATA_MATRIX(int a, int b, int *AtoD, int *DtoA)
{
  if(a==0 || b==0)
      return 0;
  else
      return( AtoD [ (DtoA[a]+DtoA[b]) % 255 ] ) ;
}

int GFmultiply_QR_CODE(int a, int b, int *AtoD, int *DtoA)
{
  if(a==0 || b==0)
      return 0;
  else
      return( AtoD [ (DtoA[a]+DtoA[b]) % 255 ] ) ;
}

/****************************
** Inputs:
**       a: 要找乘法反元素的數
**    AtoD: α的次方數 轉 10進位     的表格
**    DtoA: 10進位     轉 α的次方數 的表格
**
** Outputs:
**    回傳: a的乘法反元素
**
** 說明:
**    Galois Field 乘法反元素運算
****************************/
int GFinverse(int a, int *AtoD, int *DtoA)
{
//dk    int i;
    a = M_MOD(a, FIELD_NUM);  // 為避免a超過FIELD_NUM，先做一次MOD
    if(a < 0) a += FIELD_NUM;

    // 先將digital轉成alpha的次方數
    // 接著用FIELD_NUM-1去減，得到乘法反元素的alpha的次方數
    // 再將新的alpha的次方數轉換成digital並回傳
    return AtoD[FIELD_NUM-1 - DtoA[a]];
}

/****************************
** Inputs:
**       a: 輸入的第一個數
**       b: 輸入的第二個數
**    AtoD: α的次方數 轉 10進位     的表格
**    DtoA: 10進位     轉 α的次方數 的表格
**
** Outputs:
**    回傳: a/b
**
** 說明:
**    Galois Field 除法
****************************/
int GFdivide(int a, int b, int *AtoD, int *DtoA)
{
    if(a == 0) return 0;
    else       return( GFmultiply(a, GFinverse(b, AtoD, DtoA), AtoD, DtoA) ) ;
}

/****************************
** Inputs:
**       x: x^k的x
**       k: x^k的k
**    AtoD: α的次方數 轉 10進位     的表格
**    DtoA: 10進位     轉 α的次方數 的表格
**
** Outputs:
**    回傳: x^k
**
** 說明:
**    Galois Field 指數運算
****************************/
int GFpower(int x, int k, int* AtoD, int*DtoA) {
    // 要計算x^k

    int deg = 0;

    if(x==0) return 0;

    x = M_MOD(x, FIELD_NUM);  
    if(x < 0) x += FIELD_NUM;
    // 先找到d, α^d = x
    // 則x^k = α^(d*k)
    // 令deg = d*k
    deg = DtoA[x]*k;
    deg = M_MOD(deg, FIELD_NUM-1);

    // 接著找出α^(deg*k)的值並回傳
    if(deg < 0) return AtoD[deg + FIELD_NUM-1];
    else        return AtoD[deg];
}

/****************************
** Inputs:
**     in1: 輸入的第一個多項式
**    deg1: 第一個多項式的degree
**     in2: 輸入的第二個多項式
**    deg2: 第二個多項式的degree
**
** Outputs:
**     out: 回傳兩多項式相加的結果
**     deg: out的degree
**
** 說明:
**    in1 + in2，多項式加法
****************************/
void PolyAdd(int *in1, int deg1, int *in2, int deg2, int *out, int *deg) {
    int i = 0;
    int TempDeg = 0, LargerDeg = 0;

    // 設定LargerDeg 為 多項式in1跟in2的最大degree
    LargerDeg = (deg1 > deg2) ? deg1 : deg2;

    // 從次方數為0到次方數為1，將兩個多項式相加
    for(i = 0; i <= LargerDeg; ++i) {
        out[i] = GFadd(in1[i], in2[i]);
        if(out[i] != 0) {
            TempDeg = i; // 設定最高的次方數
        }
    }

    *deg=TempDeg ;
}

/****************************
** Inputs:
**     in1: 輸入的第一個多項式
**    deg1: 第一個多項式的degree
**     in2: 輸入的第二個多項式
**    deg2: 第二個多項式的degree
**
** Outputs:
**     out: 回傳兩多項式相減的結果
**     deg: out的degree
**
** 說明:
**    in1 - in2，多項式減法
****************************/
void PolySub(int *in1, int deg1, int *in2, int deg2, int *out, int *deg) {
    int i = 0;
    int TempDeg = 0, LargerDeg = 0;

    // 設定LargerDeg 為 多項式in1跟in2的最大degree
    LargerDeg = (deg1 > deg2) ? deg1 : deg2;

    // 從次方數為0到次方數為1，將兩個多項式相減
    for(i = 0; i <= LargerDeg; ++i) {
        out[i] = GFadd(in1[i], -in2[i]);
        if(out[i] != 0) {
            TempDeg = i; // 設定最高的次方數
        }
    }

    *deg = TempDeg ;
}

/****************************
** Inputs:
**     in1: 輸入的第一個多項式
**    deg1: 第一個多項式的degree
**     in2: 輸入的第二個多項式
**    deg2: 第二個多項式的degree
**
** Outputs:
**     out: 回傳兩多項式相乘的結果
**     deg: out的degree
**
** 說明:
**    in1 * in2，多項式乘法
****************************/
void PolyMultiply(int *in1, int deg1, int *in2, int deg2, int *out, int *deg) {
    int i = 0, j = 0;
    int temp = 0;

    // 將多項式out中從第0 ~ deg1+deg2的係數都設為0
    // 因為兩多項式相乘，最多會有deg1+deg2項
    for(i = 0; i <= deg1 + deg2; ++i) {
        out[i] = 0;
    }

    // 對多項式in1中第i個係數，乘上多項式in2中第j個係數，加到out的第i+j個係數中
    for(i = 0; i <= deg1; ++i) {
        for(j = 0; j <= deg2; ++j) {
            temp = GFmultiply( in1[i], in2[j], AtoD, DtoA );
            out[i+j] = GFadd( out[i+j], temp );
        }
    }

    // 設定out最高可能次方項
    (*deg) = deg1 + deg2;
}

/****************************
** Inputs:
**     in1: 輸入的第一個多項式
**    deg1: 第一個多項式的degree
**     in2: 輸入的第二個多項式
**    deg2: 第二個多項式的degree
**    AtoD: α的次方數 轉 10進位     的表格
**    DtoA: 10進位     轉 α的次方數 的表格
**
** Outputs:
**      Q: in1除以in2的商式
**   degQ: Q的degree
**      R: in1除以in2的商式
**   degR: R的degree
**
** 說明:
**    in1 / in2 = Q...R，多項式除法
****************************/
void PolyDivide_digital(int *in1, int deg1, int *in2, int deg2, int *Q, int *degQ,
                int *R, int *degR, int *AtoD, int *DtoA)
{
	int i = 0, j = 0;	//dk
	int TempDegQ = 0;

    // 計算商式的degree = 被除式的degree 減 除式的degree
	*degQ = deg1 - deg2;

    // degQ是否小於0?
    // 如果degQ小於0，表示被除式的degree比除式的degree還小，不能被除
	if(*degQ < 0) {
        *degQ = 0;
        degQ[0] = 0;
        // 設定商為0
        /*for(i = 0; i < MaxPolynomialDegree; i++) {
            Q[i] = 0;
        }*/

        // 設定餘式為被除式
        *degR=deg1 ;
        for(i = 0; i <= *degR; i++) {
            R[i] = in1[i] ;
        } 

        return;
    }

    // 複製in1 (被除式) 到R (餘式) 裡面
	for(i = 0; i <= deg1; i++) {
		R[i] = in1[i];
	}

    // 除法運算
    for(i = deg1, TempDegQ = *degQ; i>=0 && TempDegQ>=0; --i, --TempDegQ) {
        // 計算位置TempDegQ的商式係數
        Q[TempDegQ] = GFdivide(R[i], in2[deg2], AtoD, DtoA);
        // 計算餘式
        for(j = 0; j <= deg2; ++j) {
            R[j+TempDegQ] = GFadd( R[j+TempDegQ] , -GFmultiply( Q[TempDegQ], in2[j], AtoD, DtoA ) ) ;
        }
	}

    // 設定餘式degree
	for(*degR = deg2; *degR >= 0; --(*degR)) {
        if(R[*degR] != 0 ) {
            break;
        }
	}

    // 設定商式中沒有用到的值為0
    /*for(i = *degQ+1; i < MaxPolynomialDegree; i++) {
        Q[i] = 0;
    }*/
}


void PolyDivide_binary(int *in1, int deg1, int *in2, int deg2, int *Q, int *degQ,
                int *R, int *degR, int *AtoD, int *DtoA)
{
	int i = 0, j = 0;	//dk
	int TempDegQ = 0;

	*degQ = deg1 - deg2 ;
	if(*degQ < 0) {
        // 商是0
        *degQ = 0;
        degQ[0] = 0;
        /*for(i = 0; i < MaxPolynomialDegree; i++) {
            Q[i] = 0;
        }*/

        // 餘數是in1
        *degR=deg1 ;
        for(i = 0; i <= *degR; i++) {
            R[i] = in1[i] ;
        } 

        return;
    }

    // 複製in1 (除數) 到R (餘數) 裡面
    // 那是不是應該只要跑0~deg1-1就好?
	//for(i = 0; i < MaxPolynomialDegree; i++) {
	for(i = 0; i <= deg1; i++) {
		R[i] = in1[i];
	}


    for(i = deg1, TempDegQ = *degQ; i>=0 && TempDegQ>=0; --i, --TempDegQ) {
        Q[TempDegQ] = GFdivide(R[i], in2[deg2], AtoD, DtoA);
        for(j = 0; j <= deg2; ++j) {
            R[j+TempDegQ] = GFadd( R[j+TempDegQ] , GFmultiply(Q[TempDegQ], in2[j], AtoD, DtoA) ) ;
        }
	}

	for(*degR = deg2; *degR >= 0; --(*degR)) {
        if(R[*degR] != 0 ) {
            break;
        }
	}
}

/****************************
** Inputs:
**     coefficient: 多項式係數
**             deg: 多項式的degree
**               x: 要帶入多項式的x值
**            AtoD: α的次方數 轉 10進位     的表格
**            DtoA: 10進位     轉 α的次方數 的表格
**
** Outputs:
**          return: 計算出來的值
**
** 說明:
**       多項式如果是f(x)，x帶2進去，就是計算f(2)
****************************/
int FunctionValue(int *coefficient, int deg, int x, int *AtoD, int *DtoA) {
    int i = 0;
    int temp = 0;

    // x是不是0?
    if(x==0) {
        // 如果x是0，答案就為0，回傳
        return coefficient[0];
    }
    else {
        // 把x帶入多項式中計算
        temp=0;
        for(i=0 ; i<=deg ; i++) {
            temp = GFadd( temp, GFmultiply(coefficient[i], GFpower(x, i, AtoD, DtoA), AtoD, DtoA) ) ;
        }

        return temp;
    }
}

/****************************
** Inputs:
**     in: 要微分的多項式
**  degin: in的degree
**
** Outputs:
**    out: 要微分的多項式
**  degin: out的degree
**
** 說明:
**       多項式的一次微分
****************************/
void FiniteFieldDifferential_digital( int *in, int degin,  int *out, int *degout)
{
    int i = 0;
    *degout = 0;
    // 計算多項式in的一次微分
    for(i = 1; i <= degin; ++i) {
        out[i-1] = GFmultiply(i, in[i], AtoD, DtoA);
        if(out[i-1] != 0) {
            (*degout) = i-1;
        }
    }
}


void FiniteFieldDifferential_binary( int *in, int degin,  int *out, int *degout)
{
    int i = 0;
    *degout = 0;

    for(i = 0; i < degin; ++i) {
        if(i%2 == 1) {
            out[i] = 0;
        }
        else {
            out[i] = in[i+1] ;

            if(out[i]!=0) {
                (*degout) = i;
            }
        }  
    }
}