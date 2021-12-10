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
    int value;
    boolean isLowPoint;
} point;

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

    // calculate risk
    int sum = 0;
    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COL_SIZE; j++)
        {
            if (height[i][j].isLowPoint)
            {
                sum += height[i][j].value + 1;
            }
        }
    }
    printf("Sum of the risk levels of all low points on the heightmap: %d\n", sum);
}
