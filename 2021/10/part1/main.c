#include <stdio.h>

#if 0
#define INPUT "input.txt"
#define SIZE 24
#else
#define INPUT "input2.txt"
#define SIZE 200
#endif

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
    int scoreSum = 0;
    int points = 0;

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
            points += scoreSum;
            scoreSum = 0;
            stack.top = EMPTY;
        }
    }
    printf("Total syntax error score: %d", points);
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
