#include <stdio.h>
#include <stdlib.h>

#if 0
#define INPUT "input.txt"
#define MAX_CAVES 6
#define MAX_NEIGHBORS 4
#else 
#define INPUT "input2.txt"
#define MAX_CAVES 20
#define MAX_NEIGHBORS 20
#endif

#define EMPTY (-1)
#define NOT_IN_MAP (-1)
#define LENGTH 10

typedef enum
{
    FALSE,
    TRUE
} boolean;

typedef struct cave
{
    struct cave *neighbor[MAX_NEIGHBORS];
    char *name;
    int neighborCount;
    boolean isAccessible;
    boolean isBig;
} cave;

void buildMap(cave **map, int *caveCount);
void cleanMap(cave **map, int caveCount);
int checkMap(cave **map, cave *currentCave, int *caveCount);
int findPath(cave *currentCave, boolean joker, cave *revisitedCave);
boolean sameCave(char *firstCave, char *secondCave);

cave *start;
cave *end;

int main(void)
{

    cave *map[MAX_CAVES];
    int caveCount = EMPTY;
    buildMap(map, &caveCount);

    cave *currentCave = start;
    boolean joker = TRUE; // this token is used to communicate between recursive call stacks
    cave *revisitedCave = NULL; // stores small cave when visiting twice 
    int pathCount = findPath(currentCave, joker, revisitedCave);

    cleanMap(map, caveCount);
    printf("Visited %d paths through this cave system.\n", pathCount);
}

int findPath(cave *currentCave, boolean joker, cave *revisitedCave)
{
    int pathCount = 0;
    int neighborCount = currentCave->neighborCount; 

    if (!currentCave->isBig)
    {
        currentCave->isAccessible = FALSE;
    }

    while (neighborCount > 0)
    {
        cave *nextCave = currentCave->neighbor[neighborCount - 1];
        if (nextCave == end)
        {
            pathCount++;
        }
        else if (nextCave->isAccessible && nextCave != start)
        {
             pathCount += findPath(nextCave, joker, revisitedCave);
        }
        else if (nextCave != start && !nextCave->isAccessible && joker)
        {
            revisitedCave = nextCave;
            joker = FALSE;
            pathCount += findPath(nextCave, joker, revisitedCave);
            joker = TRUE;
        }
        neighborCount--;
    }
    if (currentCave != revisitedCave)
    {
        currentCave->isAccessible = TRUE;
    }
    else
    {
        revisitedCave = NULL;
    }
    return pathCount;
}

int checkMap(cave **map, cave *currentCave, int *caveCount)
{
            int localCount = *caveCount;
            for (int i = 0; i <= localCount; i++)
            {
                if(sameCave(currentCave->name, map[i]->name))
                {
                    return i;
                }
            }
            return NOT_IN_MAP;
}

boolean sameCave(char *firstCave, char *secondCave)
{
    int index = 0;
    while (firstCave[index] == secondCave[index] && firstCave[index] != '\0')
    {
        index++; 
    }
    return (firstCave[index] == secondCave[index]) ? TRUE : FALSE;
}

void buildMap(cave **map, int *caveCount)
{
    char c;
    FILE *file = fopen(INPUT, "r");
    char buffer[LENGTH];
    int length = 0;
    int index;
    cave *firstCave;
    cave *secondCave;
    
    while ((c = getc(file)) != EOF)
    {
        if (c != '-' && c != '\n')
        {
            buffer[length++] = c;
        }
        else if (c == '-')
        {
            buffer[length++] = '\0';         
            cave *currentCave = malloc(sizeof(cave));
            currentCave->name = malloc(sizeof(char) * length);
            currentCave->neighborCount = 0;
            currentCave->isAccessible = TRUE;

            if (buffer[0] >= 'A' && buffer[0] <= 'Z')
            {
                currentCave->isBig = TRUE;
            }
            else
            {
                currentCave->isBig = FALSE;
            }

            // copy buffer to currentCave->name
            int i = 0;
            while (length > 0)
            {
                currentCave->name[i] = buffer[i];
                i++;
                length--;
            }

            // check for duplicates
            index = checkMap(map, currentCave, caveCount); 
            if (index == NOT_IN_MAP)
            {
                map[++(*caveCount)] = currentCave;
                firstCave = currentCave;
            }
            else
            {
                firstCave = map[index];
                free(currentCave->name);
                free(currentCave);
            }
        }
        else if (c == '\n')
        {
            buffer[length++] = '\0';         
            cave *currentCave = malloc(sizeof(cave));
            currentCave->name = malloc(sizeof(char) * length);
            currentCave->neighborCount = 0;
            currentCave->isAccessible = TRUE;

            if (buffer[0] >= 'A' && buffer[0] <= 'Z')
            {
                currentCave->isBig = TRUE;
            }
            else
            {
                currentCave->isBig = FALSE;
            }

            // copy buffer to currentCave->name
            int i = 0;
            while (length > 0)
            {
                currentCave->name[i] = buffer[i];
                i++;
                length--;
            }

            // check for duplicates
            index = checkMap(map, currentCave, caveCount); 
            if (index == NOT_IN_MAP)
            {
                map[++(*caveCount)] = currentCave;
                secondCave = currentCave;
            }
            else
            {
                secondCave = map[index];
                free(currentCave->name);
                free(currentCave);
            }
            
            // build adjacency list
            firstCave->neighbor[firstCave->neighborCount++] = secondCave;
            secondCave->neighbor[secondCave->neighborCount++] = firstCave;
        }
    }
    fclose(file);

    // find map index for start cave
    int startCave;
    int localCount = *caveCount;
    for (int i = 0; i <= localCount; i++)
    {
        if (sameCave(map[i]->name, "start"))
        {
            start = map[i];
        }
        else if (sameCave(map[i]->name, "end"))
        {
            end = map[i]; 
        }
    }

    return;
}

void cleanMap(cave **map, int caveCount)
{
    for (int i = 0; i < caveCount; i++)
    {
        free(map[i]->name);
        free(map[i]);
    }
}
