#include <stdio.h>
#include <stdlib.h>

#if 0
#define INPUT "input.txt"
#define MAX_CAVES 6
#define MAX_NEIGHBORS 4
#elif 1
#define INPUT "input2.txt"
#define MAX_CAVES 20
#define MAX_NEIGHBORS 20
#endif

#if 0
//#define INPUT "input-larger.txt"
#define INPUT "input-even-larger.txt"
#define MAX_CAVES 100
#define MAX_NEIGHBORS 100
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

int buildMap(cave **map, int *caveCount);
void cleanMap(cave **map, int caveCount);
int checkMap(cave **map, cave *currentCave, int *caveCount);
int findPath(cave *currentCave);
boolean sameCave(char *firstCave, char *secondCave);

int main(void)
{
    cave *map[MAX_CAVES];
    int caveCount = EMPTY;
    int startCave = buildMap(map, &caveCount);
    cave *currentCave = map[startCave];
    int pathCount = findPath(currentCave);
    cleanMap(map, caveCount);
    printf("Visited %d paths through this cave system.\n", pathCount);
}

int findPath(cave *currentCave)
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
        if (sameCave(nextCave->name, "end"))
        {
            pathCount++;
        }
        else if (nextCave->isAccessible)
        {
            pathCount += findPath(nextCave);
        }
        neighborCount--;
    }
    currentCave->isAccessible = TRUE;
    return pathCount;
}

int checkMap(cave **map, cave *currentCave, int *caveCount)
{
            int length = *caveCount;
            for (int i = 0; i <= length; i++)
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

int buildMap(cave **map, int *caveCount)
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
            }

            // build adjacency list
            firstCave->neighbor[firstCave->neighborCount++] = secondCave;
            secondCave->neighbor[secondCave->neighborCount++] = firstCave;
        }
    }
    fclose(file);

    // find map index for start cave
    int startCave;
    int count = *caveCount;
    for (int i = 0; i <= count; i++)
    {
        if (sameCave(map[i]->name, "start"))
        {
            startCave = i;
        }
    }

    return startCave;
}

void cleanMap(cave **map, int caveCount)
{
    for (int i = 0; i < caveCount; i++)
    {
        free(map[i]->name);
        free(map[i]);
    }
}
