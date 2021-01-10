#include <stdio.h>
#include <limits.h>
#define NUMBER 5

int main(void)
{
    int i, max, min, maxIndex, minIndex;
    int array[NUMBER];
    max = 0;
    min = INT_MAX;
    maxIndex = 0;
    minIndex = 0;
    // array[0] ~ array[4] : 5 크기의 배열 선언
    for (i = 0; i < NUMBER; i++)
    {
        scanf("%d", &array[i]);
        if (max < array[i])
        {
            max = array[i];
            maxIndex = i;
        }

        if (min > array[i])
        {
            min = array[i];
            minIndex = i;
        }
    }
    printf("가장 큰 값은 %d 이고 %d번째에 있습니다.\n", max, maxIndex + 1);
    printf("가장 작은 값은 %d 이고 %d번째에 있습니다.\n", min, minIndex + 1);

    return 0;
}