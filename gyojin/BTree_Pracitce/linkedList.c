#include <stdio.h>
#include <stdlib.h>

struct NODE
{
    int data;
    struct NODE *link[2];
};

void main(void){
    struct NODE *head = (struct NODE *)malloc(sizeof(struct NODE));

    struct NODE *node1 = (struct NODE *)malloc(sizeof(struct NODE));
    head->link[0] = NULL;
    head->link[1] = node1;
    node1->data = 10;
    
    struct NODE *node2 = (struct NODE *)malloc(sizeof(struct NODE));
    node1->link[0] = head;
    node1->link[1] = node2;
    node2->data = 20;

    node2->link[0] = node1;
    node2->link[1] = NULL;

    printf("node1의 data :: %d\n", node1->data);
    printf("node1의 값 :: %d\n", *node1);
    printf("node1의 연결리스트 1 :: %d\n", node1->link[0]);
    printf("node1의 연결리스트 2 :: %d\n", node1->link[1]);
    printf("node2의 주소 :: %d\n", node2);
    printf("node1의 주소 :: %d\n", node1);

    free(node2);
    free(node1);
    free(head);
}