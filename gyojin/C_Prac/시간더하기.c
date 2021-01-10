#include <stdio.h>

// 전역변수
int hour;
int minute;
int minuteAdd;

int counter()
{
    minute += minuteAdd;
    hour += minute / 60;
    minute %= 60;
    hour %= 24;
}

int main(void)
{
    printf("시를 입력하세요 : ");
    scanf("%d", &hour);
    printf("분을 입력하세요 : ");
    scanf("%d", &minute);
    printf("더할 분을 입력하세요 : ");
    scanf("%d", &minuteAdd);
    counter();
    printf("더해진 시간은 %d시 %d분 입니다.\n", hour, minute);
    return 0;
}