#include <stdio.h>

// 두 변수의 값을 교환
// 메인함수 내에서 영향을 끼치기 위해서는 포인터를 사용해야 함
void swap(int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

int main(void)
{
    int x = 1;
    int y = 2;
    swap(&x, &y);
    printf("x = %d\ny = %d\n", x, y);
    return 0;
}