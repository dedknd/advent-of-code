#include <stdio.h>

#if 0
#define INPUT "input.txt"
#else
#define INPUT "input2.txt"
#endif

#define SEGMENTS 7

typedef enum 
{
    FALSE,
    TRUE
} boolean;

boolean OUTPUT_PARSE = FALSE;

int main(void)
{
    char c;
    FILE *file = fopen(INPUT, "r");
    int segmentCount = 0;
    int digit[SEGMENTS + 1] = {0};

    while ((c = getc(file)) != EOF)
    {
        if (c == '|')
        {
            OUTPUT_PARSE = TRUE;
        }
        else if (c == '\n')
        {
            OUTPUT_PARSE = FALSE;
        }
        else if (c != ' ' && OUTPUT_PARSE)
        {
            segmentCount++;
        }
        else
        {
            digit[segmentCount]++;
            segmentCount = 0;
        }
    }
    fclose(file);

    // digit: segments -> uniqueDigit, (2->1, 3->7, 4->4, 3->7)
    int uniqueCount = digit[2] + digit[3] + digit[4] + digit[7];
    printf("Digits 1, 4, 7 and 8 appear %d times!\n", uniqueCount);
}
