#include <stdio.h>

// 재귀함수로 조합 구현
int nCr(int n, int r)
{
    if (r == n || r == 0)
    {
        return 1;
    }
    else
    {
        return nCr(n - 1, r - 1) + nCr(n - 1, r);
    }
    
}

int main(void)
{
    int n, r;
    scanf("%d %d", &n, &r);
    printf("%d", nCr(n, r));
    return 0;
}