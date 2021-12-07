#include <stdio.h>

#if 0
#define INPUT "input.txt"
#define CRABS 10
#else
#define INPUT "input2.txt"
#define CRABS 1000
#endif

typedef struct {
    int fuelCost;
    int position;
} result;

result cost(int *position, int lowerBound, int upperBound);
inline int abs(int value);

int main(void)
{
    int position[CRABS];
    int crabCount = 0;

    char c;
    int value = 0;
    int upperBound = 0;
    int lowerBound = -1 ^ (1 << 31); // INT_MAX

    FILE *file = fopen(INPUT, "r");
    while ((c = getc(file)) != EOF)
    {
        if (c != ',' && c != '\n')
        {
            value = value * 10 + (c - '0');
        }
        else
        {
            position[crabCount++] = value;

            if (value > upperBound)
            {
                upperBound = value;
            }
            else if (value < lowerBound)
            {
                lowerBound = value;
            }
            value = 0;
        }
    }
    fclose(file);

    result solution = cost(position, lowerBound, upperBound);
    printf("Minimum fuel usage is %d for position %d!\n", solution.fuelCost, solution.position);
}

result cost(int *position, int lowerBound, int upperBound)
{
    int guess = (lowerBound + upperBound) / 2;

    // compute cost for positional guess and its immediate neighbors
    int fuelCost = 0;
    int fuelCostUpOne = 0;
    int fuelCostDownOne = 0;
    for (int i = 0; i < CRABS; i++)
    {
        fuelCost += abs(guess - position[i]);
        fuelCostUpOne += abs((guess + 1) - position[i]);
        fuelCostDownOne += abs((guess - 1) - position[i]);
    }

    // recursively search for lowest cost
    if (fuelCost > fuelCostUpOne)
    {
        return cost(position, guess + 1, upperBound);
    }
    else if (fuelCost > fuelCostDownOne)
    {
        return cost(position, lowerBound, guess - 1);
    }
    else 
    {
        return (result){fuelCost, guess};
    }
}

inline int abs(int value)
{
    return (value >= 0) ? value : (-1) * value;
}
