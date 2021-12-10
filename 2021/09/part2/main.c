#include <stdio.h>

#if 0
#define INPUT "input.txt"
#define ROW_SIZE 5
#define COL_SIZE 10
#else
#define INPUT "input2.txt"
#define ROW_SIZE 100
#define COL_SIZE 100 
#endif

typedef enum {
    FALSE,
    TRUE
} boolean;

typedef struct {
    int row;
    int col;
} coordinates;

typedef struct {
    int value;
    int basinSize;
    boolean explored;
    boolean isLowPoint;
} point;

int exploreBasin(point (*height)[COL_SIZE], coordinates currentPoint);
int main(void)
{
    char c;
    FILE *file = fopen(INPUT, "r");
    point height[ROW_SIZE][COL_SIZE];
    int row = 0;
    int col = 0;

    while ((c = getc(file)) != EOF)
    {
        if (c != '\n')
        {
            height[row][col].value = c - '0';
            height[row][col].isLowPoint = TRUE;
            height[row][col].explored = FALSE;
            height[row][col].basinSize = 0;
            col++;
        }
        else
        {
            col = 0;
            row++;
        }
    }
    

    int kernel[3][3] = {{0, 1, 0}, {1, 0, 1}, {0, 1, 0}};

    int top, right, bottom, left;
    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COL_SIZE; j++)
        {
            // edge cases
            top = (i == 0) ? 1 : 0;
            right = (j == COL_SIZE - 1) ? 1 : 0;
            bottom = (i == ROW_SIZE - 1) ? 1 : 0;
            left = (j == 0) ? 1 : 0;

            for (int k = 0 + top; k < 3 - bottom; k++)
            {
                for (int l = 0 + left; l < 3 - right; l++)
                {
                    if (kernel[k][l] != 0 && (height[i - 1 + k][j - 1 + l].value * kernel[k][l] <= height[i][j].value))
                    {
                        height[i][j].isLowPoint = FALSE;
                    }
                }
            }
        }
    }

    // exploring the basins
    int largestBasins[3] = {0};
    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COL_SIZE; j++)
        {
            if (height[i][j].isLowPoint)
            {
                coordinates currentPoint = (coordinates){i, j};
                height[i][j].basinSize += exploreBasin(height, currentPoint);

                if (height[i][j].basinSize >= largestBasins[0])
                {
                    largestBasins[2] = largestBasins[1];
                    largestBasins[1] = largestBasins[0];
                    largestBasins[0] = height[i][j].basinSize;
                }
                else if (height[i][j].basinSize >= largestBasins[1])
                {
                    largestBasins[2] = largestBasins[1];
                    largestBasins[1] = height[i][j].basinSize;
                }
                else if (height[i][j].basinSize >= largestBasins[0])
                {
                    largestBasins[2] = height[i][j].basinSize;
                }
            }
        }
    }
    
    int size = 1;
    for (int i = 0; i < 3; i++)
    {
        size *= largestBasins[i]; 
    }
    printf("Product of the three largest basins sizes: %d\n", size);
}

int exploreBasin(point (*height)[COL_SIZE], coordinates currentPoint)
{
    int i = currentPoint.row;
    int j = currentPoint.col;
    height[i][j].explored = TRUE;
    int basinSize = 1;

    // edge cases
    int top = (i == 0) ? 1 : 0;
    int right = (j == COL_SIZE - 1) ? 1 : 0;
    int bottom = (i == ROW_SIZE - 1) ? 1 : 0;
    int left = (j == 0) ? 1 : 0;

    // recursively explore the basin
    if (!top && height[i - 1][j].value >= height[i][j].value && !height[i - 1][j].explored)
    {
        if (height[i - 1][j].value != 9)
        {
            coordinates currentPoint = (coordinates){i - 1, j};
            basinSize += exploreBasin(height, currentPoint);
        }
    }
    if (!bottom && height[i + 1][j].value >= height[i][j].value && !height[i + 1][j].explored)
    {
        if (height[i + 1][j].value != 9)
        {
            coordinates currentPoint = (coordinates){i + 1, j};
            basinSize += exploreBasin(height, currentPoint);
        }
    }
    if (!left && height[i][j - 1].value >= height[i][j].value && !height[i][j - 1].explored)
    {
        if (height[i][j - 1].value != 9)
        {
            coordinates currentPoint = (coordinates){i, j - 1};
            basinSize += exploreBasin(height, currentPoint);
        }
    }
    if (!right && height[i][j + 1].value >= height[i][j].value && !height[i][j + 1].explored)
    {
        if (height[i][j + 1].value != 9)
        {
            coordinates currentPoint = (coordinates){i, j + 1};
            basinSize += exploreBasin(height, currentPoint);
        }
    }
    return basinSize; 
}
