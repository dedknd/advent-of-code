#include <stdio.h>

#if 0
#define INPUT "input.txt"
#define SIZE 24
#else
#define INPUT "input2.txt"
#define SIZE 200
#endif

#define SCORES 45

typedef struct {
    char array[SIZE];
    int top;
} filo;

enum {
    EMPTY = -1
};

void push(filo *stack, char c);
char pop(filo *stack);

int main(void)
{
    char c;
    FILE *file = fopen(INPUT, "r");

    // ), ], }, >
    int errorScore[4] = {3, 57, 1197, 25137};

    filo stack; 
    stack.top = EMPTY;
    int scoreCount = 0;
    unsigned long long scoreArray[SCORES]; 
    unsigned long long completionScore = 0;
    int scoreSum = 0;

    while ((c = getc(file)) != EOF)
    {
        if (c != '\n')
        {
            switch(c)
            {
                case '(': {
                    push(&stack, ')');
                } break;

                case '[': {
                    push(&stack, ']');
                } break;

                case '{': {
                    push(&stack, '}');
                } break;

                case '<': {
                    push(&stack, '>');
                } break;

                case '>': {
                    if (pop(&stack) != c)
                    {
                        scoreSum += errorScore[3];
                    }; 
                } break;

                case '}': {
                    if (pop(&stack) != c)
                    {
                        scoreSum += errorScore[2];
                    }; 
                } break;

                case ']': {
                    if (pop(&stack) != c)
                    {
                        scoreSum += errorScore[1];
                    }; 
                } break;

                case ')': {
                    if (pop(&stack) != c)
                    {
                        scoreSum += errorScore[0];
                    }; 
                } break;
            }
        }
        else
        {
            // discard corrupted lines 
            if (scoreSum > 0)
            {
                scoreSum = 0;
            }
            else
            {
                while (stack.top != EMPTY)
                {
                    c = pop(&stack); 
                    switch (c) 
                    {
                        case '>': {
                            {
                                completionScore = completionScore * 5 + 4;
                            }; 
                        } break;

                        case '}': {
                            {
                                completionScore = completionScore * 5 + 3;
                            }; 
                        } break;

                        case ']': {
                            {
                                completionScore = completionScore * 5 + 2;
                            }; 
                        } break;

                        case ')': {
                            {
                                completionScore = completionScore * 5 + 1;
                            }; 
                        } break;
                    }
                }

                // sort on insertion
                scoreCount++;
                for (int i = scoreCount - 1; i > 0; i--)
                {
                    if (scoreArray[i - 1] < completionScore)
                    {
                        scoreArray[i] = scoreArray[i - 1];
                    }
                    else
                    {
                        scoreArray[i] = completionScore;
                        break;
                    }
                }

                // insert current score if largest
                if (scoreArray[0] < completionScore)
                {
                    scoreArray[0] = completionScore;
                }
                completionScore = 0;
            }
            stack.top = EMPTY;
        }
    }
    printf("Total completion score: %llu", scoreArray[scoreCount / 2]);
}    

void push(filo *stack, char c)
{
    // no bounds checking!
    stack->top += 1;     
    stack->array[stack->top] = c;
}

char pop(filo *stack)
{
    return stack->array[stack->top--];
}
