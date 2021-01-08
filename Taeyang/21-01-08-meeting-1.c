#include <stdio.h>
int main(void) {
    int arr1[5] = {11,42,33,24,15};
    int arr2[5] = {63,47,58,69,10};

    for(int i =0; i<5; ++i) {
        printf("&arr1 + %d :: %p\n", i,&arr1+i);
        printf("&arr2 + %d :: %p\n", i,&arr2+i);
    }
    for(int i =0; i<5; ++i) {
        printf("size of &arr1 :: %d\n", sizeof(arr1+i));
        printf("size of &arr2 :: %d\n", sizeof(arr2+i));
    }
    for(int i =0; i<5; ++i) {
        printf("arr1 + %d :: %p\n", i,arr1+i);
        printf("arr2 + %d :: %p\n", i,arr2+i);
    }
    for(int i =0; i<5; ++i) {
        printf("*arr1 + %d :: %d\n", i,*(arr1+i));
        printf("*arr2 + %d :: %d\n", i,*(arr2+i));
    }
}
