#include "./head/ECC200.h"

// CECC200 ΘㄧΑ
/****************************************************************************************/
//
//耞计Data计
//input : int iGridRow : X禸计
//input : int iGridColumn : Y禸计
/****************************************************************************************/

void ECC200_MappingMatrix(int iGridRow, int iGridColumn, BYTE *_byteBinaryData, BYTE *_byteFormatTempData, BYTE *_byteFormatData)
{
	int row, col, chr;

	for (row = 0; row < iGridRow; row++)
	{
		for (col = 0; col < iGridColumn; col++)
		{
			*(_byteFormatTempData + (row * iGridColumn) + col) = 0;
		}
	}

	//ECC200 Inital location
	chr = 1; //Inital Bluck
	row = 4;
	col = 0;

	do
	{
		// repeatedly first check for one of the special corner cases, then...
		if ((row == iGridRow) && (col == 0))
			corner1(chr++, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);

		if ((row == iGridRow - 2) && (col == 0) && (iGridColumn % 4))
			corner2(chr++, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);

		if ((row == iGridRow - 2) && (col == 0) && (iGridColumn % 8 == 4))
			corner3(chr++, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);

		if ((row == iGridRow + 4) && (col == 2) && (!(iGridColumn % 8)))
			corner4(chr++, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);

		// sweep upward diagonally, inserting successive characters,...
		do
		{
			if ((row < iGridRow) && (col >= 0))
			{
				if (!*(_byteFormatTempData + (row * iGridColumn) + col))
				{
					utah(row, col, chr++, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
				}
			}

			row -= 2;
			col += 2;
		} while ((row >= 0) && (col < iGridColumn));

		row += 1;
		col += 3;

		// & then sweep downward diagonally, inserting successive characters,...
		do
		{
			if ((row >= 0) && (col < iGridColumn))
			{
				if (!*(_byteFormatTempData + (row * iGridColumn) + col))
				{
					utah(row, col, chr++, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
				}
			}

			row += 2;
			col -= 2;
		} while ((row < iGridRow) && (col >= 0));

		row += 3;
		col += 1;

		// ... until the entire array is scanned
	} while ((row < iGridRow) || (col < iGridColumn));

	// Lastly, if the lower righthand corner is untouched, fill in fixed pattern
	if (!*(_byteFormatTempData + (iGridRow * iGridColumn - 1)))
		*(_byteFormatTempData + (iGridRow * iGridColumn - 1)) = *(_byteFormatTempData + (iGridRow * iGridColumn - iGridColumn - 2)) = 1;
}

void module(int row, int col, int chr, int bit, int iGridRow, int iGridColumn, BYTE *_byteBinaryData, BYTE *_byteFormatTempData, BYTE *_byteFormatData)
{
	if (row < 0)
	{
		row += iGridRow;
		col += 4 - ((iGridRow + 4) % 8);
	}

	if (col < 0)
	{
		col += iGridColumn;
		row += 4 - ((iGridColumn + 4) % 8);
	}

	*(_byteFormatTempData + (row * iGridColumn) + col) = 1;
	//printf("\n%c\t%c\n",_byteFormatData[(chr-1) * 8 + (bit - 1)],_byteBinaryData[row * iGridColumn + col]);
	_byteFormatData[(chr - 1) * 8 + (bit - 1)] = _byteBinaryData[row * iGridColumn + col];
}

void utah(int row, int col, int chr, int iGridRow, int iGridColumn, BYTE *_byteBinaryData, BYTE *_byteFormatTempData, BYTE *_byteFormatData)
{
	module(row - 2, col - 2, chr, 1, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(row - 2, col - 1, chr, 2, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(row - 1, col - 2, chr, 3, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(row - 1, col - 1, chr, 4, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(row - 1, col, chr, 5, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(row, col - 2, chr, 6, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(row, col - 1, chr, 7, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(row, col, chr, 8, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
}

void corner1(int chr, int iGridRow, int iGridColumn, BYTE *_byteBinaryData, BYTE *_byteFormatTempData, BYTE *_byteFormatData)
{
	module(iGridRow - 1, 0, chr, 1, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(iGridRow - 1, 1, chr, 2, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(iGridRow - 1, 2, chr, 3, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(0, iGridColumn - 2, chr, 4, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(0, iGridColumn - 1, chr, 5, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(1, iGridColumn - 1, chr, 6, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(2, iGridColumn - 1, chr, 7, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(3, iGridColumn - 1, chr, 8, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
}

void corner2(int chr, int iGridRow, int iGridColumn, BYTE *_byteBinaryData, BYTE *_byteFormatTempData, BYTE *_byteFormatData)
{
	module(iGridRow - 3, 0, chr, 1, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(iGridRow - 2, 0, chr, 2, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(iGridRow - 1, 0, chr, 3, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(0, iGridColumn - 4, chr, 4, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(0, iGridColumn - 3, chr, 5, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(0, iGridColumn - 2, chr, 6, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(0, iGridColumn - 1, chr, 7, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(1, iGridColumn - 1, chr, 8, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
}

void corner3(int chr, int iGridRow, int iGridColumn, BYTE *_byteBinaryData, BYTE *_byteFormatTempData, BYTE *_byteFormatData)
{
	module(iGridRow - 3, 0, chr, 1, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(iGridRow - 2, 0, chr, 2, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(iGridRow - 1, 0, chr, 3, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(0, iGridColumn - 2, chr, 4, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(0, iGridColumn - 1, chr, 5, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(1, iGridColumn - 1, chr, 6, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(2, iGridColumn - 1, chr, 7, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(3, iGridColumn - 1, chr, 8, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
}

void corner4(int chr, int iGridRow, int iGridColumn, BYTE *_byteBinaryData, BYTE *_byteFormatTempData, BYTE *_byteFormatData)
{
	module(iGridRow - 1, 0, chr, 1, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(iGridRow - 1, iGridColumn - 1, chr, 2, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(0, iGridColumn - 3, chr, 3, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(0, iGridColumn - 2, chr, 4, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(0, iGridColumn - 1, chr, 5, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(1, iGridColumn - 3, chr, 6, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(1, iGridColumn - 2, chr, 7, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
	module(1, iGridColumn - 1, chr, 8, iGridRow, iGridColumn, _byteBinaryData, _byteFormatTempData, _byteFormatData);
}

int GetECC200SymbolAttributes(int iSymbolSize_Row, int iSymbolSize_Col, ECC200 *_ecc200, BYTE *_byteFormatData)
{
	if (iSymbolSize_Row == iSymbolSize_Col)
	{
		_ecc200->iSymbolSize_Row = _ecc200->iSymbolSize_Col = iSymbolSize_Row;

		switch (iSymbolSize_Row)
		{
		case 10:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 8;
			_ecc200->iDataRegionNumber = 1;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 8;
			_ecc200->iCodeWordsData = 3;
			_ecc200->iCodeWordsError = 5;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 1;
			break;
		}

		case 12:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 10;
			_ecc200->iDataRegionNumber = 1;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 10;
			_ecc200->iCodeWordsData = 5;
			_ecc200->iCodeWordsError = 7;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 1;

			break;
		}

		case 14:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 12;
			_ecc200->iDataRegionNumber = 1;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 12;
			_ecc200->iCodeWordsData = 8;
			_ecc200->iCodeWordsError = 10;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 1;

			break;
		}

		case 16:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 14;
			_ecc200->iDataRegionNumber = 1;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 14;
			_ecc200->iCodeWordsData = 12;
			_ecc200->iCodeWordsError = 12;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 1;

			break;
		}

		case 18:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 16;
			_ecc200->iDataRegionNumber = 1;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 16;
			_ecc200->iCodeWordsData = 18;
			_ecc200->iCodeWordsError = 14;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 1;

			break;
		}

		case 20:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 18;
			_ecc200->iDataRegionNumber = 1;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 18;
			_ecc200->iCodeWordsData = 22;
			_ecc200->iCodeWordsError = 18;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 1;

			break;
		}

		case 22:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 20;
			_ecc200->iDataRegionNumber = 1;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 20;
			_ecc200->iCodeWordsData = 30;
			_ecc200->iCodeWordsError = 20;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 1;

			break;
		}

		case 24:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 22;
			_ecc200->iDataRegionNumber = 1;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 22;
			_ecc200->iCodeWordsData = 36;
			_ecc200->iCodeWordsError = 24;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 1;

			break;
		}

		case 26:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 24;
			_ecc200->iDataRegionNumber = 1;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 24;
			_ecc200->iCodeWordsData = 44;
			_ecc200->iCodeWordsError = 28;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 1;

			break;
		}

		case 32:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 14;
			_ecc200->iDataRegionNumber = 4;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 28;
			_ecc200->iCodeWordsData = 62;
			_ecc200->iCodeWordsError = 36;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 1;

			break;
		}

		case 36:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 16;
			_ecc200->iDataRegionNumber = 4;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 32;
			_ecc200->iCodeWordsData = 86;
			_ecc200->iCodeWordsError = 42;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 1;

			break;
		}

		case 40:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 18;
			_ecc200->iDataRegionNumber = 4;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 36;
			_ecc200->iCodeWordsData = 114;
			_ecc200->iCodeWordsError = 48;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 1;

			break;
		}

		case 44:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 20;
			_ecc200->iDataRegionNumber = 4;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 40;
			_ecc200->iCodeWordsData = 144;
			_ecc200->iCodeWordsError = 56;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 1;

			break;
		}

		case 48:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 22;
			_ecc200->iDataRegionNumber = 4;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 44;
			_ecc200->iCodeWordsData = 174;
			_ecc200->iCodeWordsError = 68;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 1;

			break;
		}

		case 52:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 24;
			_ecc200->iDataRegionNumber = 4;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 48;
			_ecc200->iCodeWordsData = 204;
			_ecc200->iCodeWordsError = 84;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 2;

			break;
		}

		case 64:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 14;
			_ecc200->iDataRegionNumber = 16;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 56;
			_ecc200->iCodeWordsData = 280;
			_ecc200->iCodeWordsError = 112;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 2;

			break;
		}

		case 72:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 16;
			_ecc200->iDataRegionNumber = 16;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 64;
			_ecc200->iCodeWordsData = 368;
			_ecc200->iCodeWordsError = 144;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 4;

			break;
		}

		case 80:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 18;
			_ecc200->iDataRegionNumber = 16;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 72;
			_ecc200->iCodeWordsData = 456;
			_ecc200->iCodeWordsError = 192;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 4;

			break;
		}

		case 88:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 20;
			_ecc200->iDataRegionNumber = 16;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 80;
			_ecc200->iCodeWordsData = 576;
			_ecc200->iCodeWordsError = 224;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 4;

			break;
		}

		case 96:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 22;
			_ecc200->iDataRegionNumber = 16;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 88;
			_ecc200->iCodeWordsData = 696;
			_ecc200->iCodeWordsError = 272;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 4;

			break;
		}

		case 104:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 24;
			_ecc200->iDataRegionNumber = 16;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 96;
			_ecc200->iCodeWordsData = 816;
			_ecc200->iCodeWordsError = 336;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 6;

			break;
		}

		case 120:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 18;
			_ecc200->iDataRegionNumber = 36;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 108;
			_ecc200->iCodeWordsData = 1050;
			_ecc200->iCodeWordsError = 408;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 6;

			break;
		}

		case 132:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 20;
			_ecc200->iDataRegionNumber = 36;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 120;
			_ecc200->iCodeWordsData = 1304;
			_ecc200->iCodeWordsError = 496;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 8;

			break;
		}

		case 144:
		{
			_ecc200->iDataRegionSize_Row = _ecc200->iDataRegionSize_Col = 22;
			_ecc200->iDataRegionNumber = 36;
			_ecc200->iMappingMatrixSize_Row = _ecc200->iMappingMatrixSize_Col = 132;
			_ecc200->iCodeWordsData = 1558;
			_ecc200->iCodeWordsError = 620;
			//2012.12.10
			_ecc200->i_RS_Block_Number = 10;

			break;
		}

		default:
		{
			return 0;
			//printf("Error: No such format!\n");
			break;
		}
		}
	}

	else
	{
		_ecc200->iSymbolSize_Row = iSymbolSize_Row;
		_ecc200->iSymbolSize_Col = iSymbolSize_Col;

		switch (iSymbolSize_Row)
		{
		case 8:
		{
			switch (iSymbolSize_Col)
			{
			case 18:
			{
				_ecc200->iDataRegionSize_Row = 6;
				_ecc200->iDataRegionSize_Col = 16;
				_ecc200->iDataRegionNumber = 1;
				_ecc200->iMappingMatrixSize_Row = 6;
				_ecc200->iMappingMatrixSize_Col = 16;
				_ecc200->iCodeWordsData = 5;
				_ecc200->iCodeWordsError = 7;
				//2012.12.10
				_ecc200->i_RS_Block_Number = 1;
				break;
			}

			case 32:
			{
				_ecc200->iDataRegionSize_Row = 6;
				_ecc200->iDataRegionSize_Col = 14;
				_ecc200->iDataRegionNumber = 2;
				_ecc200->iMappingMatrixSize_Row = 6;
				_ecc200->iMappingMatrixSize_Col = 28;
				_ecc200->iCodeWordsData = 10;
				_ecc200->iCodeWordsError = 11;
				//2012.12.10
				_ecc200->i_RS_Block_Number = 1;
				break;
			}

			default:
			{
				return 0;
				//printf("Error: No such format!\n");
				break;
			}
			}

			break;
		}

		case 12:
		{
			switch (iSymbolSize_Col)
			{
			case 26:
			{
				_ecc200->iDataRegionSize_Row = 10;
				_ecc200->iDataRegionSize_Col = 24;
				_ecc200->iDataRegionNumber = 1;
				_ecc200->iMappingMatrixSize_Row = 10;
				_ecc200->iMappingMatrixSize_Col = 24;
				_ecc200->iCodeWordsData = 16;
				_ecc200->iCodeWordsError = 14;
				//2012.12.10
				_ecc200->i_RS_Block_Number = 1;
				break;
			}

			case 36:
			{
				_ecc200->iDataRegionSize_Row = 10;
				_ecc200->iDataRegionSize_Col = 16;
				_ecc200->iDataRegionNumber = 2;
				_ecc200->iMappingMatrixSize_Row = 10;
				_ecc200->iMappingMatrixSize_Col = 32;
				_ecc200->iCodeWordsData = 22;
				_ecc200->iCodeWordsError = 18;
				//2012.12.10
				_ecc200->i_RS_Block_Number = 1;
				break;
			}

			default:
			{
				return 0;
				//printf("Error: No such format!\n");
				break;
			}
			}

			break;
		}

		case 16:
		{
			switch (iSymbolSize_Col)
			{
			case 36:
			{
				_ecc200->iDataRegionSize_Row = 14;
				_ecc200->iDataRegionSize_Col = 16;
				_ecc200->iDataRegionNumber = 2;
				_ecc200->iMappingMatrixSize_Row = 14;
				_ecc200->iMappingMatrixSize_Col = 32;
				_ecc200->iCodeWordsData = 32;
				_ecc200->iCodeWordsError = 24;
				//2012.12.10
				_ecc200->i_RS_Block_Number = 1;
				break;
			}

			case 48:
			{
				_ecc200->iDataRegionSize_Row = 14;
				_ecc200->iDataRegionSize_Col = 22;
				_ecc200->iDataRegionNumber = 2;
				_ecc200->iMappingMatrixSize_Row = 14;
				_ecc200->iMappingMatrixSize_Col = 44;
				_ecc200->iCodeWordsData = 49;
				_ecc200->iCodeWordsError = 28;
				//2012.12.10
				_ecc200->i_RS_Block_Number = 1;
				break;
			}

			default:
			{
				return 0;
				//printf("Error: No such format!\n");
				break;
			}
			}

			break;
		}

		default:
		{
			//printf("Error: No such format!\n");
			return 0;
			break;
		}
		}
	}

	return 1;
}
