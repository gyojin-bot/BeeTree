#include <stdio.h>
#include <string.h>
typedef struct strt1{
    long long num; //<<< a      swap a and b       >> use 8 byte (1 block)
    char str[17];  //<<< b                         >> use 24 byte (2,3,4 block)
    int a; //                                      >> use - byte (4 block)
    int b; //                                      >> use 8 byte (5 block)
    char ch; //                                    >> use - byte (5 block)
}STRT1;

typedef struct strt2 {
    char str[17]; //17byte    
    long long num; //8byte    
    int a; // 4byte
    int b; // 4byte
    char ch; //1byte
}STRT2;

int main() {
    STRT1 s1;
    STRT2 s2;
    printf("size of strt1 :: %d\n", sizeof(s1)); // -> 40 byte  
    printf("size of strt2 :: %d\n", sizeof(s2)); // -> 48 byte
    return 0;
}