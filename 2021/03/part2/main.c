#include <stdio.h>
#include <stdlib.h>


#if 0
#define SIZE 5 
#define LINES 12
#define INPUT "input.txt"
#else
#define SIZE 12
#define LINES 1000
#define INPUT "input2.txt"
#endif


int oxygen[SIZE];
int co2[SIZE];

enum generator_rating {
    OXYGEN,
    CO2
};

void slice(int *diagnostic_slice, int index, int lineCount, enum generator_rating rating);
int main(void)
{
    FILE *file = fopen(INPUT, "r");
    char c;
    int index = 0;
    int lineCount = 0;
    int diagnostic[SIZE * LINES];
    while ((c = getc(file)) != EOF)
    {
        if (c == '\n')
        {
            index = 0;
            lineCount++;
        }
        else
        {
            diagnostic[lineCount * SIZE + index] = c - '0';
            index += 1;
        }
    }

    slice(diagnostic, 0, lineCount, OXYGEN);    
    slice(diagnostic, 0, lineCount, CO2);    

    int co2Dec = 0;
    int oxygenDec = 0;
    for (int i = 0; i < SIZE; i++)
    {
        co2Dec = co2Dec * 2 + co2[i];
        oxygenDec = oxygenDec * 2 + oxygen[i];
    }
    printf("life support rating of submarine: %d\n", co2Dec * oxygenDec);
    fclose(file);
}

void slice(int *diagnostic_slice, int index, int lineCount, enum generator_rating rating)
{
    // array one (zero) temporarily stores pointers to the lines of diagnostic_slice with a 1 (0) at index
    int **one = malloc(lineCount * sizeof(int *));
    int **zero = malloc(lineCount * sizeof(int *));

    int oneCount = 0;
    int zeroCount = 0;
    for (int i = 0; i < lineCount; i++)
    {
        if (diagnostic_slice[i * SIZE + index] == 1)
        {
            one[oneCount++] = &diagnostic_slice[i * SIZE];
        }
        else
        {
            zero[zeroCount++] = &diagnostic_slice[i * SIZE];
        }
    }
    if (oneCount >= zeroCount)
    {
        if (rating == OXYGEN)
        {
            oxygen[index] = 1;

            int *arrayWithOneAtIndex = malloc(oneCount * SIZE * sizeof(int));
            for (int i = 0; i < oneCount; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    arrayWithOneAtIndex[i * SIZE + j] = *(one[i] + j);
                }
            }
            free(one);
            free(zero);
            if (index < SIZE - 1)
            {
                if (oneCount > 1)
                {
                    slice(arrayWithOneAtIndex, index + 1, oneCount, rating);
                }
                else
                {
                    for (int i = 0; i < SIZE; i++)
                    {
                        oxygen[i] = arrayWithOneAtIndex[i];
                    }
                }
            }
            free(arrayWithOneAtIndex);
            return;
        }
        else if (rating == CO2)
        {
            co2[index] = 0;

            int *arrayWithZeroAtIndex = malloc(zeroCount * SIZE * sizeof(int));
            for (int i = 0; i < zeroCount; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    arrayWithZeroAtIndex[i * SIZE + j] = *(zero[i] + j);
                }
            }
            free(one);
            free(zero);
            if (index < SIZE - 1)
            {
                if (zeroCount > 1)
                {
                    slice(arrayWithZeroAtIndex, index + 1, zeroCount, rating);
                }
                else 
                {
                    for (int i = 0; i < SIZE; i++)
                    {
                        co2[i] = arrayWithZeroAtIndex[i];
                    }
                }
            }
            free(arrayWithZeroAtIndex);
            return;
        }
    }
    else 
    {
        if (rating == OXYGEN)
        {
            oxygen[index] = 0;

            int *arrayWithZeroAtIndex = malloc(zeroCount * SIZE * sizeof(int));
            for (int i = 0; i < zeroCount; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    arrayWithZeroAtIndex[i * SIZE + j] = *(zero[i] + j);
                }
            }
            free(one);
            free(zero);
            if (index < SIZE - 1)
            {
                if (zeroCount > 1)
                {
                    slice(arrayWithZeroAtIndex, index + 1, zeroCount, rating);
                }
                else
                {
                    for (int i = 0; i < SIZE; i++)
                    {
                        oxygen[i] = arrayWithZeroAtIndex[i];
                    }
                }
            }
            free(arrayWithZeroAtIndex);
            return;
        }
        else if (rating == CO2) 
        {
            co2[index] = 1;

            int *arrayWithOneAtIndex = malloc(oneCount * SIZE * sizeof(int));
            for (int i = 0; i < oneCount; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    arrayWithOneAtIndex[i * SIZE + j] = *(one[i] + j);
                }
            }
            free(one);
            free(zero);
            if (index < SIZE - 1)
            {
                if (oneCount > 1)
                {
                    slice(arrayWithOneAtIndex, index + 1, oneCount, rating);
                }
                else
                {
                    for (int i = 0; i < SIZE; i++)
                    {
                        co2[i] = arrayWithOneAtIndex[i];
                    }
                }
            }
            free(arrayWithOneAtIndex);
            return;
        }
    }
}
