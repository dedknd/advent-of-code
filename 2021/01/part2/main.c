#include <stdio.h>

int main(void)
{
    char c;
    int value = 0;
    int array[4] = {0};
    int increaseCounter = 0; 

    FILE *file = fopen("input2.txt", "r");
    while ((c = getc(file)) != EOF)
    {
        if (c != '\n')
        {
            value = value * 10 + (c - '0');        
        }
        else
        {
            // "sliding window"
            array[0] = array[1];
            array[1] = array[2];
            array[2] = array[3];
            array[3] = value;

            value = 0;

            // array[0] > 0 ensures a "window" filled with numbers
            if (array[0] > 0 && array[3] - array[0] > 0)
            {
                increaseCounter++; 
            }
        }
    }
    fclose(file);
    printf("%d measurements were larger than the previous measurement.\n", increaseCounter);
    return 0;
}
