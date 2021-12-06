#include <stdio.h>

#if 0
#define INPUT "input.txt"
#define DAYS 256
#else
#define INPUT "input2.txt"
#define DAYS 256
#endif

#define STATE_SIZE 9 
#define RESET 6

int main(void)
{
    char c;
    unsigned long long fishCount = 0;
    unsigned long long state[STATE_SIZE] = {0};

    FILE *file = fopen(INPUT, "r");
    while ((c = getc(file)) != EOF)
    {
        if (c != ',' && c != '\n')
        {
            state[c - '0']++;
            fishCount++;
        }
    }
    fclose(file);

    unsigned long long birthCount;
    for (int i = DAYS; i > 0; i--)
    {
        birthCount = state[0];
        for (int j = 0; j < STATE_SIZE - 1; j++)
        {
            state[j] = state[j + 1];
        }
        state[STATE_SIZE - 1] = birthCount;
        state[RESET] += birthCount;
        fishCount += birthCount;
        birthCount = 0;
    }
    printf("There are %llu lanternfish after %d days.\n", fishCount, DAYS);
}
