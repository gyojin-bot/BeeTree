#include <stdio.h>
#include <stdlib.h>

struct NODE
{
    int data;
    struct NODE *next;
};

void main(void){
    struct NODE *head = malloc(sizeof(struct NODE));

    struct NODE *node1 = malloc(sizeof(struct NODE));
    head->next = node1;
    node1->data = 10;
    
    struct NODE *node2 = malloc(sizeof(struct NODE));
    node1->next = node2;
    node2->data = 20;

    node2->next = NULL;

    printf("node1의 data :: %d\n", node1->data);
    printf("node1의 값 :: %d\n", *node1);
    printf("node1의 연결리스트 :: %d\n", node1->next);
    printf("node2의 주소 :: %d\n", node2);
    printf("node1의 주소 :: %d\n", node1);

    free(node2);
    free(node1);
    free(head);
}