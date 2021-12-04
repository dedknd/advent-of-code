#include <stdio.h>

#define SIZE 12

int main(void)
{
    FILE *file = fopen("input2.txt", "r");
    char c;
    int array[SIZE] = {0};
    int gamma[SIZE] = {0};
    int epsilon[SIZE] = {0};
    int index = 0;
    int lineCount = 0;
    while ((c = getc(file)) != EOF)
    {
        if (c == '\n')
        {
            index = 0;
            lineCount++;
        }
        else
        {
            array[index] += c - '0';
            index += 1;
        }
    }
    for (int i = 0; i < SIZE; i++)
    {
        if (array[i] >= lineCount / 2)
        {
            gamma[i] = 1;
            epsilon[i] = 0;
        }
        else
        {
            epsilon[i] = 1;
            gamma[i] = 0;
        }
    }
    int gammaDec = 0;
    int epsilonDec = 0;
    for (int i = 0; i < SIZE; i++)
    {
        gammaDec = gammaDec * 2 + gamma[i];
        epsilonDec = epsilonDec * 2 + epsilon[i];
    }
    printf("power-consumption of submarine: %d\n", gammaDec * epsilonDec);
    fclose(file);
}
