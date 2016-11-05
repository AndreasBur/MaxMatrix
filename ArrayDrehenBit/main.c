#include <stdio.h>
#include <stdlib.h>

/* Bit setzen */
#define setBit(Var, Bit) \
((Var) |= (1 << (Bit)))

/* Bit löschen */
#define clearBit(Var, Bit) \
((Var) &= (unsigned)~(1 << (Bit)))

/* Bit togglen */
#define toggleBit(Var, Bit) \
((Var) ^= (1 << (Bit)))

/* Bit Operationen */
#define readBit(Var, Bit) \
(((Var) & (1 << (Bit))) >> Bit)

#define writeBit(Var, Bit, Value) \
(Value ? setBit(Var, Bit) : clearBit(Var, Bit))

#define isBitSet(Var, Bit) \
((Var) & (1 << (Bit)))

#define isBitCleared(Var, Bit) \
!isBitSet(Var, Bit)

/* Bit Gruppen abfragen */
#define readRegisterBitGroup(Register, BitGroupMask, BitGroupPosition) \
((Register & ((uint8_t)BitGroupMask)) >> BitGroupPosition)

/* Bit Gruppen setzen */
#define writeRegisterBitGroup(Register, BitGroupMask, BitGroupPosition, Value) \
(Register = ((Register & ~((uint8_t)BitGroupMask)) | ((Value << BitGroupPosition) & ((uint8_t)BitGroupMask))))


#define B(x) ( \
  (0##x >>  0 & 0001) | \
  (0##x >>  2 & 0002) | \
  (0##x >>  4 & 0004) | \
  (0##x >>  6 & 0010) | \
  (0##x >>  8 & 0020) | \
  (0##x >> 10 & 0040) | \
  (0##x >> 12 & 0100) | \
  (0##x >> 14 & 0200) )


#define MAXMATRIX_COLUMN_NUMBER_OF_MODULE                           8
#define MAXMATRIX_ROW_NUMBER_OF_MODULE                              8
#define MAXMATRIX_NUMBER_OF_MODULES                                 2

char RowValue = B(11111111);

char Buffer[16] = {B(00000000),
                   B(00000011),
                   B(00001100),
                   B(00110000),
                   B(00110000),
                   B(00011000),
                   B(00001100),
                   B(01000011),

                   B(00000000),
                   B(00000000),
                   B(00000000),
                   B(00000000),
                   B(00000000),
                   B(00000000),
                   B(00000000),
                   B(00000000)};

char BufferRotated[8];


char array[8][8] = {{11,12,13,14,15,16,17,18},
                    {21,22,23,24,25,26,27,28},
                    {31,32,33,34,35,36,37,38},
                    {41,42,43,44,45,46,47,48},
                    {51,52,53,54,55,56,57,58},
                    {61,62,63,64,65,66,67,68},
                    {71,72,73,74,75,76,77,78},
                    {81,82,83,84,85,86,87,88}};

char array_trans[8][8];

int main()
{
    int Column=0, Row=0, Module, BitPos=0;
    int ColumnRotated, RowRotated;
    char Bit;

//    printf("\n original\n");
//    for(i = 0; i <= 7; i++)
//    {
//        for(a = 0; a <= 7; a++)
//        {
//            printf("%i,",array[i][a]);
//        }
//        printf("\n");
//    }

    printf("\n Bit original\n");
    for(Column = 0; Column <= MAXMATRIX_COLUMN_NUMBER_OF_MODULE*MAXMATRIX_NUMBER_OF_MODULES-1; Column++)
    {
        for(Row = 7; Row >= 0; Row--)
        {
            Bit = readBit(Buffer[Column], Row);
            printf("%d,", Bit);
        }
        printf("\n");
    }

    Row=0;
	//for(byte Module = 1; Module <= MAXMATRIX_NUMBER_OF_MODULES; Module++) {
		for(Column = 0; Column < MAXMATRIX_COLUMN_NUMBER_OF_MODULE; Column++) {
            writeBit(BufferRotated[Column], MAXMATRIX_ROW_NUMBER_OF_MODULE-Row-1, readBit(RowValue, BitPos));
            BitPos++;
		}
	//}

//    printf("\n Bit rotation 90\n");
//    ColumnRotated = 0;
//    for(Module = 1; Module <= MAXMATRIX_NUMBER_OF_MODULES; Module++)
//    {
//        for(Row = MAXMATRIX_ROW_NUMBER_OF_MODULE-1; Row >= 0; Row--)
//        {
//            RowRotated = MAXMATRIX_ROW_NUMBER_OF_MODULE-1;
//            for(Column = MAXMATRIX_COLUMN_NUMBER_OF_MODULE*Module-1; Column >= MAXMATRIX_COLUMN_NUMBER_OF_MODULE*(Module-1); Column--)
//            {
//                Bit = readBit(Buffer[Column], Row);
//                writeBit(BufferRotated[ColumnRotated], RowRotated, readBit(Buffer[Column], Row));
//                RowRotated--;
//                printf("%d,", Bit);
//            }
//            ColumnRotated++;
//            printf("\n");
//        }
//    }

//    printf("\n Bit rotation 270\n");
//    ColumnRotated = 0;
//    for(Module = 1; Module <= MAXMATRIX_NUMBER_OF_MODULES; Module++)
//    {
//        for(Row = 0; Row <= MAXMATRIX_ROW_NUMBER_OF_MODULE-1; Row++)
//        {
//            RowRotated = MAXMATRIX_ROW_NUMBER_OF_MODULE-1;
//            for(Column = MAXMATRIX_COLUMN_NUMBER_OF_MODULE*(Module-1); Column <= MAXMATRIX_COLUMN_NUMBER_OF_MODULE*Module-1; Column++)
//            {
//                Bit = readBit(Buffer[Column], Row);
//                writeBit(BufferRotated[ColumnRotated], RowRotated, readBit(Buffer[Column], Row));
//                RowRotated--;
//                printf("%d,", Bit);
//            }
//            ColumnRotated++;
//            printf("\n");
//        }
//    }


//    printf("\n Bit rotation 180\n");
//    ColumnRotated = 0;
//    for(Module = 1; Module <= MAXMATRIX_NUMBER_OF_MODULES; Module++)
//    {
//        for(Column = MAXMATRIX_COLUMN_NUMBER_OF_MODULE*Module-1; Column >= MAXMATRIX_COLUMN_NUMBER_OF_MODULE*(Module-1); Column--)
//        {
//            RowRotated = MAXMATRIX_ROW_NUMBER_OF_MODULE-1;
//            for(Row = 0; Row <= MAXMATRIX_ROW_NUMBER_OF_MODULE-1; Row++)
//            {
//                Bit = readBit(Buffer[Column], Row);
//                writeBit(BufferRotated[ColumnRotated], RowRotated, readBit(Buffer[Column], Row));
//                RowRotated--;
//                printf("%d,", Bit);
//            }
//            ColumnRotated++;
//            printf("\n");
//        }
//    }

//    printf("\n Bit rotation 270\n");
//    for(i = 0; i < 8; i++)
//    {
//        for(a = 0; a <= 7; a++)
//        {
//            Bit = readBit(Buffer[a], i);
//            printf("%d,", Bit);
//        }
//        printf("\n");
//    }
//
//    printf("\n rotation 90\n");
//    for(i = 0; i < 8; i++)
//    {
//        for(a = 7; a >= 0; a--)
//            printf("%d,", array[a][i]);
//        printf("\n");
//    }
//
//    printf("\n rotation 180\n");
//    for(i = 7; i >= 0; i--)
//    {
//        for(a = 7; a >= 0; a--)
//            printf("%d,", array[a][i]);
//        printf("\n");
//    }
//
//    printf("\n rotation 270\n");
//    for(i = 7; i >= 0; i--)
//    {
//        for(a = 0; a <= 7; a++)
//            printf("%d,", array[a][i]);
//        printf("\n");
//    }

    printf("\n Bit Rotated\n");
    for(Column = 0; Column <= MAXMATRIX_COLUMN_NUMBER_OF_MODULE*MAXMATRIX_NUMBER_OF_MODULES-1; Column++)
    {
        for(Row = 7; Row >= 0; Row--)
        {
            Bit = readBit(BufferRotated[Column], Row);
            printf("%d,", Bit);
        }
        printf("\n");
    }

//    printf("\n mirrored\n");
//    for(i = 0; i <= 7; i++)
//    {
//        for(a = 0; a <= 7; a++)
//        {
//            printf("%i,",array[a][i]);
//        }
//        printf("\n");
//    }

    return 0;
}
