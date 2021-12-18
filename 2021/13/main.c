#include <stdio.h>

#if 0
#define INPUT "input.txt"
#define MAX_ROWS (14 + 1)
#define MAX_COLS (10 + 1)
#define MAX_POINTS 18
#define MAX_INSTRUCTIONS 2
#else
#define INPUT "input2.txt"
#define MAX_ROWS (894 + 1) 
#define MAX_COLS (1310 + 1) 
#define MAX_POINTS 867
#define MAX_INSTRUCTIONS 12
#endif

typedef enum
{
    FALSE,
    TRUE
} boolean;

typedef struct {
    int x;
    int y;
} coordinates;

boolean PARSE_COORDINATES = TRUE;
static int grid[MAX_ROWS][MAX_COLS];

// helper function for finding MAX_ROWS and MAX_COLS for compile time grid[][]
coordinates findMax(coordinates *point, int pointCount);

int main(void)
{
    char c;
    char last;
    int value = 0;
    int pointCount = 0;
    int instructionCount = 0;
    coordinates point[MAX_POINTS];     
    coordinates instruction[MAX_INSTRUCTIONS];

    FILE *file = fopen(INPUT, "r");
    while ((c = getc(file)) != EOF)
    {
        if (PARSE_COORDINATES)
        {
            if (c != ',' && c != '\n')
            {
                value = value * 10 + (c - '0'); 
            }
            else if (c == ',')
            {
                point[pointCount].x = value;
                value = 0;
            }
            else if (c == '\n' && last != '\n')
            {
                point[pointCount].y = value;
                pointCount++;
                value = 0;
            }
            // never reached on first iteration when last contains garbage value
            if (last == '\n' && c == '\n')
            {
                PARSE_COORDINATES = FALSE;
            }
            last = c;
        }
        else
        {
            if (c == '=')
            {
                if (last == 'y')
                {
                    value = 0;
                    while ((c = getc(file)) != '\n')
                    {
                        value = value * 10 + (c - '0');
                    }
                    instruction[instructionCount].x = 0;
                    instruction[instructionCount].y = value;
                    instructionCount++;
                }
                else
                {
                    value = 0;
                    while ((c = getc(file)) != '\n')
                    {
                        value = value * 10 + (c - '0');
                    }
                    instruction[instructionCount].x = value;
                    instruction[instructionCount].y = 0;
                    instructionCount++;
                }
            }
            last = c;
        }
    }
    fclose(file);

    coordinates maxCoordinates = findMax(point, pointCount);
    
    // fill grid with points
    for (int i = 0; i < pointCount; i++)
    {
        grid[point[i].y][point[i].x] = 1;
    }

    // "folding" the "transparent paper" 
    int lastFoldX = MAX_COLS;
    int lastFoldY = MAX_ROWS;
    for (int i = 0; i < instructionCount; i++)
    {
        if (instruction[i].x != 0)
        {
            int foldLine = instruction[i].x;
            for (int j = 0; j < lastFoldY; j++)
            {
                for (int k = 0; k < lastFoldX - foldLine; k++)
                {
                    grid[j][0 + foldLine - k] |= grid[j][lastFoldX - 1 - foldLine + k];
                }
            }
            lastFoldX = foldLine;
        }
        else
        {
            int foldLine = instruction[i].y;
            for (int j = 0; j < lastFoldY - foldLine; j++)
            {
                for (int k = 0; k < lastFoldX; k++)
                {
                    grid[0 + foldLine - j][k] |= grid[lastFoldY - 1 - foldLine + j][k];
                }
            }
            lastFoldY = foldLine;
        }
    }

    // counting up all visible dots after folding and printing result to command line
    int visibleDot = 0;
    for (int i = 0; i < lastFoldY; i++)
    {
        printf("\n");
        for (int j = 0; j < lastFoldX; j++)
        {
            if (grid[i][j] == 1)
            {
                visibleDot++;
                printf("#");
            }
            else
            {
                printf(".");
            }
        }
    }

}

coordinates findMax(coordinates *point, int pointCount)
{
    int greatestX = 0;
    int greatestY = 0;
    for (int i = 0; i < pointCount; i++)
    {
        if (point[i].x > greatestX) greatestX = point[i].x;
        if (point[i].y > greatestY) greatestY = point[i].y;
    }
    return (coordinates){greatestX, greatestY};
}
