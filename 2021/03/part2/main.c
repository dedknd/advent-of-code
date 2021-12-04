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

void oxygen_slice(int *diagnostic_slice, int index, int lineCount, int *oxygen);
void co2_slice(int *diagnostic_slice, int index, int lineCount, int *co2);

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

    int oxygen[SIZE];
    int co2[SIZE];

    oxygen_slice(diagnostic, 0, lineCount, oxygen);    
    co2_slice(diagnostic, 0, lineCount, co2);    

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

void oxygen_slice(int *diagnostic_slice, int index, int lineCount, int *oxygen)
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
                oxygen_slice(arrayWithOneAtIndex, index + 1, oneCount, oxygen);
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
    else 
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
                oxygen_slice(arrayWithZeroAtIndex, index + 1, zeroCount, oxygen);
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
}

void co2_slice(int *diagnostic_slice, int index, int lineCount, int *co2)
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
                co2_slice(arrayWithZeroAtIndex, index + 1, zeroCount, co2);
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
    else 
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
                co2_slice(arrayWithOneAtIndex, index + 1, oneCount, co2);
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
