#include <stdio.h>

int main(void)
{
    char c;
    int value = 0;
    int count = 0;
    int last = -1;
    while ((c = getchar()) != EOF)
    {
        if (c == '\n')
        {
            // last >= 0 excludes first number
            if (last >= 0 && last < value)
            {
                count++;
            }
            last = value;
            value = 0;
        }
        else
        {
            value = value * 10 + (c - '0');
        }
    }
    printf("%d measurements were larger than the previous measurement.\n", count);
    return 0;
}
