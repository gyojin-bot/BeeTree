#include <stdio.h>
#include <string.h>
typedef struct test{
    char str[17]; //17byte
    char ch; //1byte
    int num; //4byte
    PTEST *nextNode;
}TEST, *PTEST;

typedef struct BTREE {
    PTEST ** head;
};

int main()
{
    TEST s_test;
    printf("struct size: %d\n", sizeof(s_test));//결과는 30byte??
}