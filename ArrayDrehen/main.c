#include <stdio.h>
#include <stdlib.h>

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
    int i,a;
    printf("\n original\n");
    for(i = 0; i <= 7; i++)
    {
        for(a = 0; a <= 7; a++)
        {
            printf("%i,",array[i][a]);
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

    printf("\n rotation 90\n");
    for(i = 0; i < 8; i++)
    {
        for(a = 7; a >= 0; a--)
            printf("%d,", array[a][i]);
        printf("\n");
    }

    printf("\n rotation 180\n");
    for(i = 7; i >= 0; i--)
    {
        for(a = 7; a >= 0; a--)
            printf("%d,", array[a][i]);
        printf("\n");
    }

    printf("\n rotation 270\n");
    for(i = 7; i >= 0; i--)
    {
        for(a = 0; a <= 7; a++)
            printf("%d,", array[a][i]);
        printf("\n");
    }

    return 0;
}
