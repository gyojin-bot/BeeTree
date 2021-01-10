#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
/*
    1) search == True >> swap 하고 delete
    2) search == False >> 반복[has_Keyvalue(sub 트리를 결정해야 한다.) > search] 하고 swap 대상 leaf에서 찾고 그 키와 swap한다.
*/
#define DEBUG 1
#define M 3
#define T 2
typedef struct BtreeNode
{
    int KeyCount;                // key의 갯수
    bool leaf;                   // leaf인지 판정
    int keys[2 * T - 1];         // node가 가지고 있는 key 값들
    struct BtreeNode *childs[M]; // 현재 노드와 연결되어있는 child의 배열이다.
} BTREENODE;

// tree 구조체 선언
typedef struct Btree
{
    struct BtreeNode *root;
} BTREE;
// Btree라는 구조체는 node포인터를 가진다. (이름이 root)
// 비트리 루트 노드의 포인터를 가짐으로서 비트리 전체에 관여할 수 있도록한다.

BTREENODE *Allocate();
void Tree_Create(BTREE *);
void Insert_nonfull(BTREENODE *, int);
void Insert(BTREE *, int);
void Insert_Of_N(BTREE *, int);
void Delete(BTREENODE *, int);
bool Search(BTREENODE *, int);
void Split_Child(BTREENODE *, int);
int Get_Rand_Int();
void Traverse(BTREENODE *, int);
int Has_keyValue(BTREENODE *, int);
bool Swap_Keys(BTREENODE *, int);
void Swap_Keys_Left(BTREENODE *, int);
void Swap_Keys_Right(BTREENODE *, int);
void Merge_Nodes(BTREENODE *, int, int);

int main()
{
    srand((unsigned int)time(NULL));
    BTREE tree;
    Tree_Create(&tree);
    // Insert_Of_N(&tree, 10);
    Insert(&tree, 10);
    Insert(&tree, 20);
    Insert(&tree, 30);
    Insert(&tree, 40);
    Insert(&tree, 50);
    Insert(&tree, 60);
    Insert(&tree, 70);
    Insert(&tree, 80);
    Insert(&tree, 90);
    Insert(&tree, 100);
    Traverse(tree.root, 0);
    Delete(tree.root, 40);
    Traverse(tree.root, 0);
    Delete(tree.root, 50);
    Traverse(tree.root, 0);
    Delete(tree.root, 100);
    Traverse(tree.root, 0);

    return 0;
}

// 노드를 생성하기 전에 메모리를 할당한다.
BTREENODE *Allocate()
{

    // BTREENODE의 크기만큼 할당한다. (malloc은 메모리를 할당하고, 할당한 주소값을 반환한다.)
    BTREENODE *new_node = (BTREENODE *)malloc(sizeof(BTREENODE));
    // BTREENODE*형태의 new_node 선언 = BTREENODE*의 형태의 반환된 malloc의 주소값

    // new_node(BTREENODE*)를 반환
    return new_node;
}

// 트리를 만든다.
void Tree_Create(BTREE *tree)
{                                     // BTREE는 root* 의 주소를 가진 구조체이다.
    BTREENODE *new_node = Allocate(); // 새 노드를 만들기 위해 포인터에 주소를 할당한다.
    new_node->KeyCount = 0;           // 새로만들면 key가 안들어있으므로 keycount를 0으로 한다.
    new_node->leaf = true;            // 마찬가지로 leaf속성을 on한다.
    tree->root = new_node;            // new_node가 루트노드가 되므로 tree의 root는 new_node로 한다. (둘다 포인터이다.)
}

// key 갯수가 2T-1보다 작은 node에 값을 삽입한다.
void Insert_nonfull(BTREENODE *node, int KeyValue)
{
    // KeyIndex는 node의 key 갯수를 할당한다.
    // 즉, key[]의 가장 마지막 key의 idx를 할당한다.
    int KeyIndex = node->KeyCount;

    // 만약 node가 leaf node라면
    if (node->leaf)
    {

        // node의 key 갯수가 1 이상이고 (빈 노드가 아니고),
        // 넣을 key값이 key[idx]보다 작아질때까지 idx를 줄인다.
        // idx는 새로운 key값을 넣을 자리가 된다.
        while (KeyIndex >= 1 && KeyValue < node->keys[KeyIndex - 1])
        {
            // 새로운 key값을 넣을 자리를 마련해준다. (한칸씩 오른쪽으로 이동한다.)
            node->keys[KeyIndex] = node->keys[KeyIndex - 1];
            KeyIndex--;
        }

        // key값을 넣을 자리를 찾았고, 위에서 해당 자리도 비워놨기 때문에
        // 위에서 찾은 key[idx]에 새로운 key값을 할당한다.
        node->keys[KeyIndex] = KeyValue;
        // 그리고 노드의 keycount를 1 늘려준다.
        node->KeyCount += 1;
    }
    else
    {
        while (KeyIndex >= 1 && KeyValue < node->keys[KeyIndex - 1])
        {
            KeyIndex--;
        }
        //! KeyIndex++;
        if (node->childs[KeyIndex]->KeyCount == 2 * T - 1)
        {
            Split_Child(node, KeyIndex);
            if (KeyValue > node->keys[KeyIndex])
            {
                KeyIndex++;
            }
        }
        Insert_nonfull(node->childs[KeyIndex], KeyValue);
    }
}

// node의 key 갯수가 2T-1인 경우, node를 분리한다.
void Split_Child(BTREENODE *parentNode, int ChildIndex)
{
    // right_node는 분리하면서 만들어질 새로운 node이기 때문에 메모리를 새로 할당한다.
    BTREENODE *right_node = Allocate();

    //
    BTREENODE *left_node = parentNode->childs[ChildIndex];

    // right_node의 leaf 속성은 left_node의 leaf를 받아온다.
    right_node->leaf = left_node->leaf;

    // 2T-1를 분리하면서 가운데 값은 부모로 올리고
    // 나머지  2T-2개의 key들을 left와 right가 나누어가진다.
    right_node->KeyCount = T - 1;
    left_node->KeyCount = T - 1;

    // 그래서 left_node[i]
    for (int i = 0; i < T - 1; ++i)
    {
        right_node->keys[i] = left_node->keys[T + i];
    }
    for (int i = 0; i < T; ++i)
    {
        right_node->childs[i] = left_node->childs[T + i];
    }
    for (int i = parentNode->KeyCount; i > ChildIndex; --i)
    {
        parentNode->childs[i + 1] = parentNode->childs[i];
        parentNode->keys[i] = parentNode->keys[i - 1];
    }
    parentNode->childs[ChildIndex + 1] = right_node;
    parentNode->keys[ChildIndex] = left_node->keys[T - 1];
    parentNode->KeyCount++;
}

// key값을 tree에 삽입한다.
void Insert(BTREE *tree, int keyValue)
{
    BTREENODE *rootNode = tree->root;
    if (rootNode->KeyCount == 2 * T - 1)
    {
        BTREENODE *newRootNode = Allocate();
        tree->root = newRootNode;
        newRootNode->leaf = false;
        newRootNode->KeyCount = 0;
        newRootNode->childs[0] = rootNode;
        Split_Child(newRootNode, 0);
        Insert_nonfull(newRootNode, keyValue);
    }
    else
    {
        Insert_nonfull(rootNode, keyValue);
    }
}

void Insert_Of_N(BTREE *tree, int n)
{
    printf("Inserted randNum :: ");
    for (int index = 0; index < n; ++index)
    {
        int item = Get_Rand_Int() % 101;
        Insert(tree, item);
        if (DEBUG)
            printf("%d ", item);
    }
}

bool Search(BTREENODE *node, int keyValue)
{
    int index = 0;
    while (index < node->KeyCount && keyValue > node->keys[index])
    {
        index++;
    }
    if (index <= node->KeyCount && keyValue == node->keys[index])
    {
        return true;
    }
    else if (node->leaf)
    {
        return false;
    }
    else
    {
        return Search(node->childs[index], keyValue);
    }
}

int Get_Rand_Int()
{

    return rand();
}

void Traverse(BTREENODE *p, int level)
{
    printf("\n");
    int i;
    for (i = 0; i < p->KeyCount; i++)
    {
        if (p->leaf == false)
        {
            Traverse(p->childs[i], level + 1);
        }
        printf(" %d", p->keys[i]);
    }
    if (p->leaf == false)
    {
        Traverse(p->childs[i], level + 1);
    }
    printf("\n");
}

int Find_Value(BTREENODE* node, int keyValue){
    for(int idx = 0; idx< node->KeyCount; ++idx) {
        if(node->keys[idx] == keyValue) {
            return idx;
        }
    }
    return -1;
}

void Delete(BTREENODE *node, int keyValue)
{
    int childIndex = Find_Value(node, keyValue);
    if(childIndex != -1) {
        if(node->leaf) {
            // 삭제 후 종료
            // key를 찾았으니, 그 이후 키들을 앞으로 한칸씩 민다.
            for (int i = childIndex; i < node->KeyCount - 1; ++i)
            {
                node->keys[i] = node->keys[i + 1];
            }
            node->KeyCount--;
            printf("DELEITION is COMPLETE..!;");
            return;
        }
        else { // key가 노드에 존재하나 그 노드가 내부노드인 상태, (정렬이 되어있지 않을 가능성 있음)
            // 좌측이랑 무언가 할 수 있다는 의미야.
            if(childIndex >= 1 && node->childs[childIndex - 1]->KeyCount < T) {
                Merge_Nodes(node, keyValue, childIndex);
                Delete(node->childs[childIndex], keyValue);
            }   
            // 우측이랑 무언가를 할 수 있다.
            else if(node->childs[childIndex+1]->KeyCount < T){
                Merge_Nodes(node, keyValue, childIndex+1);
                Delete(node->childs[childIndex], keyValue);
            }
            else {
                //! 스왑을 하기 이전에 서브트리를 탐색하여 leaf의 키와 
                if(!Swap_Keys(node, childIndex)) {
                    Delete(node->childs[childIndex], keyValue);
                }
                else {
                    Delete(node->childs[childIndex+1], keyValue);
                }
            }
        }
    }
    else { // 정렬이 되어 있는 상태. key가 해당 노드에 없을 때,
        childIndex = Has_keyValue(node, keyValue);
        if (node->childs[childIndex]->KeyCount < T)
        {
            if (node->childs[childIndex - 1]->KeyCount < T && node->childs[childIndex + 1]->KeyCount < T)
            {
                Merge_Nodes(node, keyValue, childIndex);
                Delete(node, keyValue);
            }
            else
            {
                // LEFT
                if (!Swap_Keys(node, childIndex))
                {

                    int tmp_keyCount = node->childs[childIndex - 1]->KeyCount;

                    // 선행 자식 노드의 key들을 오른쪽으로 한칸씩 이동
                    for (int i = T - 1; i > 0; --i)
                    {
                        node->childs[childIndex]->keys[i] = node->childs[childIndex]->keys[i - 1];
                    }

                    // 선행 자식 노드의 마지막 key를 목표 자식 노드의 첫번째로 이동
                    node->childs[childIndex]->keys[0] = node->childs[childIndex - 1]->keys[tmp_keyCount - 1];

                    node->childs[childIndex]->KeyCount++;
                    node->childs[childIndex - 1]->KeyCount--;

                    // 선행 자식 노드의 포인터들을 오른쪽으로 한칸씩 이동
                    for (int i = T; i > 0; --i)
                    {
                        node->childs[childIndex]->childs[i] = node->childs[childIndex]->childs[i - 1];
                    }

                    // 선행 자식 노드의 마지막 포인터를 목표 자식 노드의 첫번째로 이동
                    node->childs[childIndex]->childs[0] = node->childs[childIndex - 1]->childs[tmp_keyCount];
                }
                // RIGHT
                else
                {

                    int tmp_keyCount = node->childs[childIndex + 1]->KeyCount;

                    // 후행 자식 노드의 첫번째 key를 목표 자식 노드의 마지막으로 이동
                    node->childs[childIndex]->keys[T - 1] = node->childs[childIndex + 1]->keys[0];

                    // 후행 자식 노드의 key들을 왼쪽으로 한칸씩 이동
                    for (int i = 0; i < tmp_keyCount; --i)
                    {
                        node->childs[childIndex]->keys[i] = node->childs[childIndex]->keys[i + 1];
                    }

                    node->childs[childIndex]->KeyCount++;
                    node->childs[childIndex + 1]->KeyCount--;

                    // 후행 자식 노드의 첫번째 포인터를 목표 자식 노드의 마지막으로 이동
                    node->childs[childIndex]->childs[T] = node->childs[childIndex + 1]->childs[0];

                    // 후행 자식 노드의 포인터들을 왼쪽으로 한칸씩 이동
                    for (int i = 0; i < tmp_keyCount + 1; --i)
                    {
                        node->childs[childIndex + 1]->childs[i] = node->childs[childIndex + 1]->childs[i + 1];
                    }
                }
                Delete(node->childs[childIndex], keyValue);
            }
        }
        else
        {
            Delete(node->childs[childIndex], keyValue);
        }
    }
}

int Has_keyValue(BTREENODE *node, int keyValue)
{

    int index = 0;

    for (index = 0; index < node->KeyCount; ++index)
    {
        if (keyValue <= node->keys[index])
        {
            return index;
        }
    }
    return index;
}

bool Swap_Keys(BTREENODE *node, int childIndex)
{

    if (childIndex >= 1 && node->childs[childIndex - 1]->KeyCount >= T)
    {
        Swap_Keys_Left(node, childIndex - 1);
        return false;
    }
    else if (node->childs[childIndex + 1]->KeyCount >= T)
    {
        Swap_Keys_Right(node, childIndex + 1);
        return true;
    }
    else
    {
    }
}

void Swap_Keys_Left(BTREENODE *node, int childIndex)
{
    int tmp = node->keys[childIndex + 1];
    int tmp_key = node->childs[childIndex]->KeyCount;
    node->keys[childIndex + 1] = node->childs[childIndex]->keys[tmp_key];
    node->childs[childIndex]->keys[tmp_key] = tmp;
}

void Swap_Keys_Right(BTREENODE *node, int childIndex)
{
    int tmp = node->keys[childIndex - 1];
    node->keys[childIndex - 1] = node->childs[childIndex]->keys[0];
    node->childs[childIndex]->keys[0] = tmp;
}

void Merge_Nodes(BTREENODE *node, int keyValue, int childIndex)
{
    // 선행 자식 노드가 무조건 있는 경우
    if (childIndex >= 1)
    {
        // 부모 노드의 childIndex-1 key를 선행 자식 노드의 끝으로 이동
        node->childs[childIndex - 1]->keys[T - 1] = node->keys[childIndex - 1];

        // 부모 노드의 후행 자식 노드의 key들을 선행 자식 노드로 이동
        for (int i = 0; i < T - 1; ++i)
        {
            node->childs[childIndex - 1]->keys[i + T] = node->childs[childIndex]->keys[i];
        }

        // 부모 노드의 후행 자식 노드의 child들을 선행 자식 노드로 이동
        // childs leaf 가 아닐 때만 수행,
        for (int i = 0; i < T; ++i)
        {
            node->childs[childIndex - 1]->childs[i + T] = node->childs[childIndex]->childs[i];
        }

        // 부모 노드의 선행 자식 노드의 keyCount 갱신
        node->childs[childIndex - 1]->KeyCount = 2 * T - 1;

        // 부모 노드의 key 갱신 및 child 갱신
        //! 부모 노드의 key count 갱신
        for (int i = -1; i < node->KeyCount; ++i)
        {
            node->keys[childIndex + i] = node->keys[childIndex + i + 1];
        }
        for (int i = 0; i < node->KeyCount + 1; ++i)
        {
            node->childs[childIndex + i] = node->childs[childIndex + i + 1];
        }
        node->KeyCount --;
    }

    // 선행 자식 노드가 없는 경우
    else
    {
        // 부모 노드의 childIndex key를 후행 자식 노드의 처음으로 이동
        node->childs[childIndex]->keys[T - 1] = node->keys[0];

        // 부모 노드의 후행 자식 노드의 key들을 선행 자식 노드로 이동
        for (int i = 0; i < T - 1; ++i)
        {
            node->childs[childIndex]->keys[i + T] = node->childs[childIndex + 1]->keys[i];
        }

        // 부모 노드의 후행 자식 노드의 child들을 선행 자식 노드로 이동
        for (int i = 0; i < T; ++i)
        {
            node->childs[childIndex]->childs[i + T] = node->childs[childIndex + 1]->childs[i];
        }

        // 부모 노드의 선행 자식 노드의 keyCount 갱신
        node->childs[childIndex]->KeyCount = 2 * T - 1;

        // 부모 노드의 key 갱신 및 child 갱신
        //! 부모노드의 key count 갱신해야함.
        //! 노드의 key count --;
        for (int i = 0; i < node->KeyCount - 1; ++i)
        {
            node->keys[childIndex + i] = node->keys[childIndex + i + 1];
        }
        for (int i = 1; i < node->KeyCount; ++i)
        {
            node->childs[childIndex + i] = node->childs[childIndex + i + 1];
        }
        node->KeyCount--;
    }
}