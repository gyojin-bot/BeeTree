#include <stdio.h>
#include <stdlib.h>

// 최소차수 t = 2
// 각 노드가 가질 수 있는 key값이 [t-1,2t-1] 범위 내에 있음
#define MAX 3
#define MIN 2

// MAX + 1은 각 노드가 가질 수 있는 자식 노드의 최대갯수 (2t)
struct BTreeNode
{

    // 데이터를 저장할 멤버
    int val[MAX + 1], count;

    // 자식 노드들의 주소를 저장할 포인터 = link배열
    struct BTreeNode *link[MAX + 1];
};

// 루트노드
// 데이터를 저장하지 않고 root 포인터만 선언
struct BTreeNode *root;

// =============== 노드 만들기 =============== //

// 노드의 주소값에 노드를 만들기 = 부모 노드를 가리키게 만들기
struct BTreeNode *createNode(int val, struct BTreeNode *child)
{
    // 구조체의 포인터 newNode 선언
    struct BTreeNode *newNode;

    // malloc = memory allocation
    // newNode에 구조체의 크기만큼 메모리 할당
    // newNode가 구조체의 메모리 주소를 기억하게 한다.
    newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));

    // 구조체의 변수에 값 할당
    newNode->val[1] = val;
    newNode->count = 1;

    // 포인트 배열인 link에 루트와 자식 노드의 주소 저장
    newNode->link[0] = root;
    newNode->link[1] = child;
    return newNode;
}

// =============== 노드 삽입 =============== //

void insertNode(int val, int pos, struct BTreeNode *node, struct BTreeNode *child)
{
    int j = node->count;
    while (j > pos)
    {
        node->val[j + 1] = node->val[j];
        node->link[j + 1] = node->link[j];
        j--;
    }
    node->val[j + 1] = val;
    node->link[j + 1] = child;
    node->count++;
}

// =============== 노드 분리 =============== //

void splitNode(int val, int *pval, int pos, struct BTreeNode *node,
               struct BTreeNode *child, struct BTreeNode **newNode)
{
    int median, j;

    // 노드를 분리하기 위한 중앙값 결정
    if (pos > MIN)
    {
        median = MIN + 1;
    }
    else
    {
        median = MIN;
    }

    // 새로운 노드를 "주소"로 선언하고 메모리 할당
    *newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    j = median + 1;

    // 현재 노드의 우측 key들을 *newNode에 저장
    while (j <= MAX)
    {
        (*newNode)->val[j - median] = node->val[j];
        (*newNode)->link[j - median] = node->link[j];
        j++;
    }

    // key 개수 업데이트
    node->count = median;
    (*newNode)->count = MAX - median;

    // 현재 key 위치가 좌측이면 node에 저장
    if (pos <= MIN)
    {
        insertNode(val, pos, node, child);
        // 현재 key 위치가 우측이면 *newNode에 저장
    }
    else
    {
        insertNode(val, pos - median, *newNode, child);
    }

    // *pval은 node에 저장된 마지막 값의 주소(부모노드로 올릴듯?)
    *pval = node->val[node->count];
    // *newNode의 root 주소에 node의 마지막 주소 저장
    (*newNode)->link[0] = node->link[node->count];
    node->count--;
}

// =============== Key 넣기가 가능할지 판단 =============== //

int setValue(int val, int *pval, struct BTreeNode *node, struct BTreeNode **child)
{
    int pos;
    // 다음 주소가 없다면? = leaf 라면?
    if (!node)
    {
        *pval = val;
        *child = NULL;
        return 1;
    }

    // key가 노드에서 가장 작은 key보다 작을 때
    if (val < node->val[1])
    {
        pos = 0;
    }
    else
    {
        // 우측 key부터 탐색하면서 같은 값이 나올 때
        for (pos = node->count; (val < node->val[pos] && pos > 1); pos--)
        {
            if (val == node->val[pos])
            {
                printf("Duplicates are not permitted\n");
                return 0;
            }
        }
    }

    // key를 넣을 수 있다면
    if (setValue(val, pval, node->link[pos], child))
    {
        if (node->count < MAX)
        {
            insertNode(*pval, pos, node, *child);
        }
        // node가 FULL 이면 노드 분리
        else
        {
            splitNode(*pval, pval, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}

// =============== Key 삽입 =============== //

void insert(int val)
{
    int flag, i;
    struct BTreeNode *child;

    flag = setValue(val, &i, root, &child);
    if (flag)
    {
        root = createNode(i, child);
    }
}

// =============== Key 탐색 =============== //

void search(int val, int *pos, struct BTreeNode *myNode)
{
    if (!myNode)
    {
        return;
    }

    if (val < myNode->val[1])
    {
        *pos = 0;
    }
    else
    {
        for (*pos = myNode->count; (val < myNode->val[*pos] && *pos > 1); (*pos)--)
        {
            if (val == myNode->val[*pos])
            {
                printf("%d is found", val);
                return;
            }
        }
    }
    search(val, pos, myNode->link[*pos]);

    return;
}

// =============== Key 순회 =============== //

void traversal(struct BTreeNode *myNode)
{
    int i;
    if (myNode)
    {
        for (i = 0; i < myNode->count; i++)
        {
            traversal(myNode->link[i]);
            printf("%d 순회\n", myNode->val[i + 1]);
        }
        traversal(myNode->link[i]);
    }
}

int main()
{
    int val, ch;

    insert(8);
    insert(9);
    insert(10);
    insert(11);
    insert(15);
    insert(16);
    insert(17);
    insert(18);
    insert(20);
    insert(23);

    traversal(root);

    printf("\n");
    search(23, &ch, root);
}