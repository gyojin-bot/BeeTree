#include <stdio.h>

int main(void)
{
    int N, i, result;
    scanf("%d", &N);
    for (i=1; i<10; i++)
    {
        result = N * i;
        printf("%d * %d = %d\n", N, i, result);
    }
    return 0;
}