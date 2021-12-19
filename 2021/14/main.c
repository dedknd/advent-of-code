#include <stdio.h>

#if 0
#define INPUT "input.txt"
#define DAYS 40
#else
#define INPUT "input2.txt"
#define DAYS 40
#endif

// constants chosen for whole alphabet and easy indexing
// there is ample opportunity for packing data more tightly and cache friendly (only using 4 letters)
#define COLS_SIZE 26
#define ROWS_SIZE 26
#define MAX_ELEMENTS 26

typedef enum {
    FALSE,
    TRUE
} boolean;

void readInput(unsigned long long (*neighbor)[COLS_SIZE], unsigned long long *elementCount, int (*insertionRule)[COLS_SIZE]);
int main(void)
{

    unsigned long long neighbor[ROWS_SIZE][COLS_SIZE] = {0}; // indexing: first (second) element is row (col) and stores count of the element pair 
    unsigned long long elementCount[MAX_ELEMENTS] = {0}; 
    long long neighborDifference[ROWS_SIZE][COLS_SIZE] = {0}; // stores all changes of neighbor[][] in current step before committing them
    int insertionRule[ROWS_SIZE][COLS_SIZE] = {0}; // indexing: first (second) letter is row (col) and maps to index for elementCount[]

    readInput(neighbor, elementCount, insertionRule);

    for (int day = DAYS; day > 0; day--)
    {
        unsigned long long neighborCount = 0;
        for (int i = 0; i < ROWS_SIZE; i++)
        {
            for (int j = 0; j < COLS_SIZE; j++)
            {
                if ((neighborCount = neighbor[i][j]) > 0)
                {
                    int element = insertionRule[i][j];
                    elementCount[element] += neighborCount;
                    neighborDifference[i][j] -= neighborCount;        
                    neighborDifference[i][element] += neighborCount;
                    neighborDifference[element][j] += neighborCount;
                }
            }
        }
        for (int i = 0; i < ROWS_SIZE; i++)
        {
            for (int j = 0; j < COLS_SIZE; j++)
            {
                neighbor[i][j] += neighborDifference[i][j];
                neighborDifference[i][j] = 0;
            }
        }

    }
    unsigned long long mostCommonelement = 0;
    unsigned long long leastCommonelement = (unsigned long long)((long long)(-1));
    for (int i = 0; i < MAX_ELEMENTS; i++)
    {
        if (elementCount[i] > mostCommonelement) mostCommonelement = elementCount[i];
        if (elementCount[i] < leastCommonelement && elementCount[i] > 0) leastCommonelement = elementCount[i];
    }
    unsigned long long solution = mostCommonelement - leastCommonelement;
    printf("Difference of most common element und least common element: %llu\n", solution);
}

void readInput(unsigned long long (*neighbor)[COLS_SIZE], unsigned long long *elementCount, int (*insertionRule)[COLS_SIZE])
{
    int count = 0;
    char c;
    char last;
    char firstElement;
    char secondElement;
    boolean TEMPLATE_PARSE = TRUE;
    boolean PARSE_PAIR = TRUE;

    FILE *file = fopen(INPUT, "r");
    while ((c = getc(file)) != EOF)
    {
        if (TEMPLATE_PARSE)
        {
            if (c != '\n')
            {
                elementCount[c - 'A']++;
                count++;

                if (count > 1)
                {
                    neighbor[last - 'A'][c - 'A'] += 1;
                    
                }
                last = c;
            }
            else
            {
                TEMPLATE_PARSE = FALSE;
                c = getc(file);
            }
        }
        else
        {
            if (c != ' ' && PARSE_PAIR)
            {
                firstElement = c;
                secondElement = getc(file);
                PARSE_PAIR = FALSE;
            }
            else
            {
                if (c == '\n')
                {
                    insertionRule[firstElement - 'A'][secondElement - 'A'] = last - 'A';
                    PARSE_PAIR = TRUE;
                }
                last = c;
            }
        }
    }
    fclose(file);
}
