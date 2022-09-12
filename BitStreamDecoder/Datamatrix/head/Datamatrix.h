#ifndef H_DATAMATRIX_
#define H_DATAMATRIX_
#include "../../stdType.h"


#define BITSTREAM 0

#define LINE_COUNT 2
#define ANG_MAX 180
#define ANG_MIN 0
#define ANG_DIF (ANG_MAX - ANG_MIN)
#define RHO_MAX 800
#define RHO_MIN 0
#define RHO_DIF (RHO_MAX - RHO_MIN + 1)
#define PI 3.14159265
#define THRESHOLD 0
#define MIN_THRESHOLD 0
#define MAX_THRESHOLD 200

#define MAX_CHARACTER_LENGTH 3116
#define MAX__cw 2178
#define MAX_frame 30
#define DMLEFT 0
#define DMTOP 1
#define DMRIGHT 2
#define DMBOTTOM 3
#define MAX_X 144               // Maximum x-axis symbol size
#define MAX_Y 144               // Maximum y-axis symbol size
#define MAX_SYMBOL MAX_X *MAX_Y // Maximum symbol size
#define MAX_MATRIX 132 * 132    // Maximum matrix size
#define MAX_WH 1280             // 對應到前處理的最大影像長寬 */
#define MAX_RS_BLOCK_N 10       // Maximum_RS_block_number = 8 + 2 (144x144)
#define MAX_RS_BLOCK_L 243      // Maximum RS_block_length = 175 + 68 (120x120)
#define Row_bar_number 450



















#endif