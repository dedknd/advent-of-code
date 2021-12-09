#include <stdio.h>

#if 0
#define INPUT "input.txt"
#else
#define INPUT "input2.txt"
#endif

#define LENGTH 7

typedef enum 
{
    FALSE,
    TRUE
} boolean;

typedef struct
{
    int length;
    int hash;
    char segment[LENGTH];
} digit;

typedef struct
{
    char firstElement;
    char secondElement;
} set;

// used to avoid hash collisions 
primes[LENGTH] = {2, 3, 5, 7, 11, 13, 17};

boolean isSubset(digit *superSet, set *subSet);

int main(void)
{
    char c;
    FILE *file = fopen(INPUT, "r");
    int length = 0;
    int decode[10] = {0};
    digit input[10];
    digit output[4];
    int inputCount = 0;
    int outputCount = 0;
    int hash = 0;
    int segmentCount = 0;
    int outputSum = 0;

    boolean OUTPUT_PARSE = FALSE;
    while ((c = getc(file)) != EOF)
    {
        if (c == '|')
        {
            OUTPUT_PARSE = TRUE;
            c = getc(file); // eat whitespace after |
        }
        else if (c == '\n')
        {
            OUTPUT_PARSE = FALSE;
            output[outputCount].hash = hash;
            output[outputCount].length = length;
            outputCount++;

            // build decoder
            // maps output[] hash of digit segments to the numerical decimal value 
            set firstPair;
            set secondPair;
            for (int i = 0; i < inputCount; i++)
            {
                // decimal value 1 has 2 segments
                // build subset firstPair containing the 2 segments
                if (input[i].length == 2)
                {
                    decode[1] = input[i].hash;
                    firstPair = (set){input[i].segment[0], input[i].segment[1]};
                }
            }

            for (int i = 0; i < inputCount; i++)
            {
                // decimal value 7 has 3 segments 
                if (input[i].length == 3)
                {
                    decode[7] = input[i].hash;
                }
            }

            for (int i = 0; i < inputCount; i++)
            {
                // decimal value 4 has 4 segments
                if (input[i].length == 4)
                {
                    decode[4] = input[i].hash;
                    boolean FIRSTPASS = TRUE;
                    // build subset secondPair containing the segments in decimal value 4, not already contained in subset firstPair 
                    for (int j = 0; j < 4; j++)
                    {
                        if (input[i].segment[j] != firstPair.firstElement && input[i].segment[j] != firstPair.secondElement && FIRSTPASS)
                        {
                            secondPair.firstElement = input[i].segment[j]; 
                            FIRSTPASS = FALSE;
                            continue;
                        }
                        if ((input[i].segment[j] != firstPair.firstElement) && (input[i].segment[j] != firstPair.secondElement) && !FIRSTPASS)
                        {
                            secondPair.secondElement = input[i].segment[j];
                            break;
                        }
                    }
                }
            }

            boolean FOUNDFIRST = FALSE;
            boolean FOUNDSECOND = FALSE;
            for (int i = 0; i < inputCount; i++)
            {
                // decimal values 2, 3 and 5 have 5 segments
                if (input[i].length == 5)
                {
                    // decimal value 3 must contain subset firstPair
                    if (isSubset(&input[i], &firstPair) && !FOUNDFIRST)
                    {
                        decode[3] = input[i].hash;
                        FOUNDFIRST = TRUE;
                        i = -1;
                        continue;
                    }
                    // decimal value 5 must contain subset secondPair
                    if (isSubset(&input[i], &secondPair) && !FOUNDSECOND)
                    {
                        decode[5] = input[i].hash; 
                        FOUNDSECOND = TRUE;
                        i = -1;
                        continue;
                    }
                    // decimal value 2 must be the left over 5 segment hash
                    if ((input[i].hash != decode[5]) && (input[i].hash != decode[3]) && FOUNDFIRST && FOUNDSECOND)
                    {
                        decode[2] = input[i].hash;
                    }
                }
            }

            FOUNDFIRST = FALSE;
            FOUNDSECOND = FALSE;
            for (int i = 0; i < inputCount; i++)
            {
                // decimal values 0, 6 and 9 have 6 segments 
                if (input[i].length == 6)
                {
                    // decimal value 9 must contain both subsets firstPair and secondPair
                    if (isSubset(&input[i], &firstPair))
                    {
                        if (isSubset(&input[i], &secondPair) && !FOUNDFIRST)
                        {
                            decode[9] = input[i].hash;
                            FOUNDFIRST = TRUE;
                            i = -1;
                            continue;
                        }
                    }
                    // decimal value 0 must contain firstPair and not secondPair
                    if (input[i].hash != decode[9] && isSubset(&input[i], &firstPair) && !FOUNDSECOND && FOUNDFIRST)
                    {
                        decode[0] = input[i].hash;
                        FOUNDSECOND = TRUE;
                        i = -1;
                        continue;
                    }
                    // decimal value 6 must the left over 6 segment hash
                    if (input[i].hash != decode[0] && input[i].hash != decode[9] && FOUNDFIRST && FOUNDSECOND)
                    {
                        decode[6] = input[i].hash;
                    }
                }
            }

            // decimal value 8 contains 7 segments
            for (int i = 0; i < inputCount; i++)
            {
                if (input[i].length == 7)
                {
                    decode[8] = input[i].hash;
                }
            }

            // decode[j] maps output[i].hash to decimal value j 
            int value = 0;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    if (decode[j] == output[i].hash)
                    {
                        value = value * 10 + j;
                    }
                }
            }
            outputSum += value;

            inputCount = 0;
            outputCount = 0;
            segmentCount = 0;
            hash = 0;
            length = 0;
        }
        else if (c != ' ')
        {
            // simple ASCII hash to exploit commutative property of addition
            // all permutations of segments for a digit hash to the same value
            if (OUTPUT_PARSE)
            {
                hash = hash + (int)c * primes[c - 'a'];            
            }
            else
            {
                hash = hash + (int)c * primes[c - 'a'];            
            }

            if (OUTPUT_PARSE)
            {
                output[outputCount].segment[segmentCount] = c;
            }
            else 
            {
                input[inputCount].segment[segmentCount] = c; 
            }
            segmentCount++;
            length++;
        }
        else
        {
            if (OUTPUT_PARSE)
            {
                output[outputCount].hash = hash;
                output[outputCount].length = length;
                outputCount++;
            }
            else
            {
                input[inputCount].hash = hash;
                input[inputCount].length = length;
                inputCount++;
            }
            hash = 0;
            length = 0;
            segmentCount = 0;
        }
    }
    fclose(file);

    printf("Sum of all the output values: %d\n", outputSum);
}

boolean isSubset(digit *superSet, set *subSet)
{
    boolean FIRST = FALSE;
    boolean SECOND = FALSE;
    int length = superSet->length;
    
    for (int i = 0; i < length; i++)
    {
        if (superSet->segment[i] == subSet->firstElement) FIRST = TRUE;
        else if (superSet->segment[i] == subSet->secondElement) SECOND = TRUE;
    }

    return FIRST && SECOND;
}
