#include <stdio.h>

int main(void)
{
    int FORWARD = 'f' + 'o' + 'r' + 'w' + 'a' + 'r' + 'd';
    int UP = 'u' + 'p';
    int DOWN = 'd' + 'o' + 'w' + 'n';

    FILE *file = fopen("input2.txt", "r");
    char c;
    int hash = 0;
    int value = 0;

    struct {
        int forward;
        int depth;
    } position;

    position.forward = 0;
    position.depth = 0;


    while ((c = getc(file)) != EOF)
    {
        if (c != '\n')
        {
            if (c >= 'a' && c <= 'z')
            {
                hash = hash + (int)c;
            }
            else if (c >= '0' && c <= '9')
            {
                value = value * 10 + c - '0';
            }
        }
        else
        {
            if (hash == FORWARD) position.forward += value; 
            else if (hash == UP) position.depth -= value;
            else if (hash == DOWN) position.depth += value;

            hash = 0;
            value = 0;
        }
    }
    printf("horizontal: %d\ndepth: %d\nproduct: %d\n", position.forward, position.depth, position.forward * position.depth);
    fclose(file);
}
