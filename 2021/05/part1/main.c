#include <stdio.h>

#if 0
#define SIZE 10
#define LINES 10
#define INPUT "input.txt"
#else
#define SIZE 1000
#define LINES 500
#define INPUT "input2.txt"
#endif

typedef struct {
    int x;
    int y;
} vector;

typedef struct {
    vector start;
    vector end;
} line;

typedef enum {
    FALSE,
    TRUE,
} boolean;

int parse_file(line *lines);
inline int abs(int number);

int grid[SIZE][SIZE] = {0};

int main(void)
{
    line lines[LINES];
    int lineCount;

    lineCount = parse_file(lines);
    

    // draw line into grid
    for (int i = 0; i < lineCount; i++)
    {
        vector difference;
        vector direction = {0}; // unit vector
        int lambda;
        difference.x = lines[i].end.x - lines[i].start.x;
        difference.y = lines[i].end.y - lines[i].start.y;

        // only vertical, horizontal and diagonal line segments as input
        if (abs(difference.x) > abs(difference.y))
        {
            lambda = abs(difference.x);
            direction.x = difference.x / abs(difference.x); 
        }
        else if (abs(difference.x) < abs(difference.y))
        {
            lambda = abs(difference.y);
            direction.y = difference.y / abs(difference.y); 
        }
        else
        {
            lambda = abs(difference.x);
            direction.y = difference.y / abs(difference.y); 
            direction.x = difference.x / abs(difference.x); 
        }

        // draw line segment into grid 
        if ((abs(direction.x) + abs(direction.y)) == 1)
        {
            for (int j = 0; j <= lambda; j++)
            {
                grid[lines[i].start.y + j * direction.y][lines[i].start.x + j * direction.x] += 1;
            }
        }
    }
    
    int points = 0;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (grid[i][j] >= 2)
            {
                points++;
            }
        }
    }
    printf("At %d points at least two lines overlap!\n", points);
}

int parse_file(line *lines)
{
    char c;
    FILE *file = fopen(INPUT, "r");
    int value = 0;
    int lineCount = 0;

    boolean NEWLINE = TRUE;
    while ((c = getc(file)) != EOF)
    {
        if (c >= '0' && c <= '9')
        {
            value = value * 10 + (c - '0');
        }
        if (c == ',') 
        {
            if (NEWLINE)
            {
                lines[lineCount].start.x = value;
            }
            else
            {
                lines[lineCount].end.x = value;
            }
            value = 0;
        }
        else if (c == ' ')
        {
            if (NEWLINE)
            {
                lines[lineCount].start.y = value;
                NEWLINE = FALSE;
                value = 0;
            }
        }
        if (c == '\n')
        {
            lines[lineCount].end.y = value;
            value = 0;
            NEWLINE = TRUE;
            lineCount++;
        }
    }
    return lineCount;
}

inline int abs(int number)
{
    return (number >= 0) ? number : (-1) * number; 
}
