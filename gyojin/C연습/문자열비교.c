#include <stdio.h>
#include <string.h>

int main(void)
{
    // char inputOne[5] = "B";
    // char inputTwo[5] = "A";
    // printf("문자열 비교 : %d\n", strcmp(inputOne, inputTwo));
    char input[10] = "I Love You";
    char result[5] = "Love";
    strcpy(result, input);
    printf("문자열 복사 : %s\n", result);
    return 0;
}