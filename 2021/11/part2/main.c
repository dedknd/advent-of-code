#include <stdio.h>

#if 0
#define INPUT "input.txt"
#define COLS 10
#define ROWS 10
#define SIZE (COLS * ROWS)
#else
#define COLS 10
#define ROWS 10 
#define SIZE (COLS * ROWS)
#define INPUT "input2.txt"
#endif

#define STEPS 10000
#define MAX_ENERGY 9

typedef enum {
    FALSE,
    TRUE
} boolean;

typedef struct {
    int energy;
    boolean highlighted;
} dumbo;

typedef struct {
    int row;
    int col;
} coordinates;

void readInput(dumbo (*octopus)[COLS]);
void cascadeEnergy(dumbo (*octopus)[COLS], coordinates currentOctopus, int *flashCount);

int main(void)
{
    dumbo octopus[ROWS][COLS];
    readInput(octopus);

    int flashCount = 0;
    int synchronicityCount = 0;
    int step;
    for (step = 0; step <= STEPS; step++)
    {
        // increment energy
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (octopus[i][j].energy == 0)
                {
                    synchronicityCount++;
                }
                octopus[i][j].energy++;
                octopus[i][j].highlighted = FALSE;
            }
        }

        if (synchronicityCount == SIZE)
        {
            break;
        }
        else
        {
            synchronicityCount = 0;
        }

        // check energy levels and cascade to neighbors
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (octopus[i][j].energy > MAX_ENERGY)
                {
                    coordinates currentOctopus = (coordinates){i, j};
                    cascadeEnergy(octopus, currentOctopus, &flashCount);
                }
            }
        }
    }
    printf("On step %d all octopuses flash!", step);
}

void cascadeEnergy(dumbo (*octopus)[COLS], coordinates currentOctopus, int *flashCount)
{
    int i = currentOctopus.row;
    int j = currentOctopus.col;

    if (!octopus[i][j].highlighted && octopus[i][j].energy > MAX_ENERGY)
    {
        octopus[i][j].energy = 0;
        octopus[i][j].highlighted = TRUE;
        (*flashCount)++;

        // edge cases
        int top = (i == 0) ? 1 : 0;
        int right = (j == COLS - 1) ? 1 : 0;
        int bottom = (i == ROWS -1) ? 1 : 0;
        int left = (j == 0) ? 1 : 0;

        // above
        if (!top && !octopus[i - 1][j].highlighted && octopus[i - 1][j].energy < 10)
        {
            coordinates currentOctopus = (coordinates){i - 1, j};
            cascadeEnergy(octopus, currentOctopus, flashCount); 
        }
        if (!top && !left && !octopus[i - 1][j - 1].highlighted && octopus[i - 1][j - 1].energy < 10)
        {
            coordinates currentOctopus = (coordinates){i - 1, j - 1};
            cascadeEnergy(octopus, currentOctopus, flashCount); 
        }
        if (!top && !right && !octopus[i - 1][j + 1].highlighted && octopus[i - 1][j + 1].energy < 10)
        {
            coordinates currentOctopus = (coordinates){i - 1, j + 1};
            cascadeEnergy(octopus, currentOctopus, flashCount); 
        }

        // lateral
        if (!left && !octopus[i][j - 1].highlighted && octopus[i][j - 1].energy < 10)
        {
            coordinates currentOctopus = (coordinates){i, j - 1};
            cascadeEnergy(octopus, currentOctopus, flashCount); 
        }
        if (!right && !octopus[i][j + 1].highlighted && octopus[i][j + 1].energy < 10)
        {
            coordinates currentOctopus = (coordinates){i, j + 1};
            cascadeEnergy(octopus, currentOctopus, flashCount); 
        }

        // below
        if (!bottom && !octopus[i + 1][j].highlighted && octopus[i + 1][j].energy < 10)
        {
            coordinates currentOctopus = (coordinates){i + 1, j};
            cascadeEnergy(octopus, currentOctopus, flashCount); 
        }
        if (!bottom && !left && !octopus[i + 1][j - 1].highlighted && octopus[i + 1][j - 1].energy < 10)
        {
            coordinates currentOctopus = (coordinates){i + 1, j - 1};
            cascadeEnergy(octopus, currentOctopus, flashCount); 
        }
        if (!bottom && !right && !octopus[i + 1][j + 1].highlighted && octopus[i + 1][j + 1].energy < 10)
        {
            coordinates currentOctopus = (coordinates){i + 1, j + 1};
            cascadeEnergy(octopus, currentOctopus, flashCount); 
        }
    }
    else
    {
        octopus[i][j].energy++;
        if (octopus[i][j].energy > MAX_ENERGY)
        {
            coordinates currentOctopus = (coordinates){i, j};
            cascadeEnergy(octopus, currentOctopus, flashCount);
        }
    }

    return;
}

void readInput(dumbo (*octopus)[COLS])
{
    char c;
    FILE *file = fopen(INPUT, "r");
    int row = 0;
    int col = 0;
    while ((c = getc(file)) != EOF)
    {
        if (c != '\n')
        {
            octopus[row][col].energy = c - '0';
            octopus[row][col].highlighted = FALSE;
            col++;
        }
        else
        {
            col = 0;
            row++;
        }
    }
    fclose(file);
}
