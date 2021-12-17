#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

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

//typedef enum
//{
//    FALSE,
//    TRUE
//} boolean;

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
int checkMap(cave **map, char *currentCave, int *caveCount);
int findPath(cave *currentCave, cave *revisitedCave);
boolean sameCave(char *firstCave, char *secondCave);

// low effort globals to avoid sameCave() string comparisons
cave *start;
cave *end;

int main(void)
{
    LARGE_INTEGER perfCount;
    LARGE_INTEGER perfFreq;
    QueryPerformanceFrequency(&perfFreq);
    QueryPerformanceCounter(&perfCount);
    unsigned long long beginCount = perfCount.QuadPart;  

    cave *map[MAX_CAVES];
    int caveCount = 0;
    buildMap(map, &caveCount);

    cave *currentCave = start;
    cave *revisitedCave = NULL; // stores small cave when visiting twice 
    int pathCount = findPath(currentCave, revisitedCave);

    QueryPerformanceCounter(&perfCount);
    unsigned long long endCount = perfCount.QuadPart;
    double time = (double)((endCount - beginCount) * 1000.0) / perfFreq.QuadPart;

    cleanMap(map, caveCount);
    printf("Visited %d paths through this cave system in %lf ms.\n", pathCount, time);
}

int findPath(cave *currentCave, cave *revisitedCave)
{
    int pathCount = 0;
    int neighborCount = currentCave->neighborCount; 
    cave *nextCave;

    if (!currentCave->isBig)
    {
        currentCave->isAccessible = FALSE;
    }

    while (neighborCount > 0)
    {
        nextCave = currentCave->neighbor[neighborCount - 1];
        if (nextCave == end)
        {
            pathCount++;
        }
        else if (nextCave->isAccessible)
        {
             pathCount += findPath(nextCave, revisitedCave);
        }
        else if (!revisitedCave && nextCave != start)
        {
            revisitedCave = nextCave;
            pathCount += findPath(nextCave, revisitedCave);
            revisitedCave = NULL;
        }
        neighborCount--;
    }
    if (currentCave != revisitedCave)
    {
        currentCave->isAccessible = TRUE;
    }
    return pathCount;
}

int checkMap(cave **map, char *currentCave, int *caveCount)
{
            int localCount = *caveCount;
            for (int i = 0; i < localCount; i++)
            {
                if(sameCave(currentCave, map[i]->name))
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

            // check for duplicates
            index = checkMap(map, buffer, caveCount); 
            if (index == NOT_IN_MAP)
            {
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
                for (int i = 0; i < length; i++)
                {
                    currentCave->name[i] = buffer[i];
                }

                map[(*caveCount)++] = currentCave;
                firstCave = currentCave;
            }
            else
            {
                firstCave = map[index];
            }
            length = 0;
        }
        else if (c == '\n')
        {
            buffer[length++] = '\0';         

            // check for duplicates
            index = checkMap(map, buffer, caveCount); 
            if (index == NOT_IN_MAP)
            {
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
                for (int i = 0; i < length; i++)
                {
                    currentCave->name[i] = buffer[i];
                }

                map[(*caveCount)++] = currentCave;
                secondCave = currentCave;
            }
            else
            {
                secondCave = map[index];
            }
            length = 0;
            
            // build adjacency list
            firstCave->neighbor[firstCave->neighborCount++] = secondCave;
            secondCave->neighbor[secondCave->neighborCount++] = firstCave;
        }
    }
    fclose(file);

    // find start and end cave
    int startCave;
    int localCount = *caveCount;
    for (int i = 0; i < localCount; i++)
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
