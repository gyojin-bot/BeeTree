#include <stdio.h>
#include <math.h>

int main(void)
{
    int x = 10;
    int y = 20;
    printf("x + y = %d입니다.\n", x + y);
    printf("x - y = %d입니다.\n", x - y);
    printf("x * y = %d입니다.\n", x * y);
    printf("x / y = %d입니다.\n", x / y);
    printf("x의 제곱은 %.1f입니다.\n", pow(x, 2));
}