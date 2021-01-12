#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 최대 자식노드 개수
#define T 2

//! Node 구조체 선언
typedef struct BNode
{
    int KeyCount;                // key의 갯수
    bool leaf;                   // leaf인지 판정
    bool root;                   // root인지 판정
    int keys[2 * T - 1];         // node가 가지고 있는 key 값들
    struct BNode *childs[2 * T]; // 현재 노드와 연결되어있는 child의 배열이다.

    struct BNode *parents;  // 부모노드 포인터
    struct BNode *prevNode; // leaf 연결노드 포인터
    struct BNode *nextNode; // leaf 연결노드 포인터
} BNODE;

//! tree 구조체 선언
typedef struct BPlusTree
{
    struct BNode *root;
} BPLUSTREE;

//! ------------------ 함수 선언부 --------------------//
BNODE *Allocate();
void Tree_Create(BPLUSTREE *);
void Split_Child(BNODE *, int);
void Insert_nonfull(BNODE *, int);
void Insert(BPLUSTREE *, int);
BNODE *Find_Leaf(BNODE *, int);
void Print_Tree(BNODE *, int);
//! --------------------------------------------------//

//! ------------------- MAIN 함수 --------------------//
int main()
{
    // srand((unsigned int)time(NULL));
    BPLUSTREE tree;
    Tree_Create(&tree);
    Insert(&tree, 10);
    Insert(&tree, 20);
    Insert(&tree, 30);
    Insert(&tree, 100);
    Insert(&tree, 50);
    Insert(&tree, 80);
    Insert(&tree, 70);
    Insert(&tree, 60);
    Insert(&tree, 90);
    Insert(&tree, 40);

    Print_Tree(tree.root, 0);
    // Deletion(&tree, 20);
    // Print_Tree(tree.root, 0);
    // Deletion(&tree, 10);
    // Print_Tree(tree.root, 0);
    // Deletion(&tree, 30);
    // Print_Tree(tree.root, 0);
    // Deletion(&tree, 40);
    // Print_Tree(tree.root, 0);
    // Deletion(&tree, 50);
    // Print_Tree(tree.root, 0);
    // Deletion(&tree, 60);
    // Print_Tree(tree.root, 0);

    printf("END");
    return 0;
}

//! 새로운 노드 생성 함수
BNODE *Allocate()
{

    // BNODE 크기만큼 할당한다. (malloc은 메모리를 할당하고, 할당한 주소값을 반환한다.)
    BNODE *new_node = (BNODE *)malloc(sizeof(BNODE));
    // BNODE 형태의 new_node 선언 = BNODE 의 형태의 반환된 malloc의 주소값

    // new_node(BNODE)를 반환
    return new_node;
}

//! 트리 생성 함수
void Tree_Create(BPLUSTREE *tree)
{                                 // BTREE는 root* 의 주소를 가진 구조체이다.
    BNODE *new_node = Allocate(); // 새 노드를 만들기 위해 포인터에 주소를 할당한다.
    new_node->KeyCount = 0;       // 새로만들면 key가 안들어있으므로 keycount를 0으로 한다.
    new_node->leaf = true;        // 마찬가지로 leaf속성을 on한다.
    tree->root = new_node;        // new_node가 루트노드가 되므로 tree의 root는 new_node로 한다. (둘다 포인터이다.)
    tree->root->root = true;
}

//! key 삽입 함수
void Insert(BPLUSTREE *tree, int keyValue)
{
    BNODE *rootNode = tree->root;
    if (rootNode->KeyCount == 2 * T - 1)
    {
        BNODE *newRootNode = Allocate();
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

//! 분리 함수
void Split_Child(BNODE *parentNode, int ChildIndex)
{
    // right_node는 분리하면서 만들어질 새로운 node이기 때문에 메모리를 새로 할당한다.
    BNODE *right_node = Allocate();
    BNODE *left_node = parentNode->childs[ChildIndex];

    // right_node의 leaf 속성은 left_node의 leaf를 받아온다.
    right_node->leaf = left_node->leaf;

    //!---- leaf Node를 분리하는 경우 ----//
    if (parentNode->childs[ChildIndex]->leaf)
    {
        // 오른쪽 자식에는 T개 만큼 옮긴다. (부모의 키를 포함해야 하므로)
        right_node->KeyCount = T;
        left_node->KeyCount = T - 1;

        // left_node의 키들을 right_node로 이동
        for (int i = 0; i < T; ++i)
        {
            right_node->keys[i] = left_node->keys[T + i - 1];
        }
        for (int i = parentNode->KeyCount; i > ChildIndex; --i)
        {
            parentNode->childs[i + 1] = parentNode->childs[i];
            parentNode->keys[i] = parentNode->keys[i - 1];
        }
        parentNode->childs[ChildIndex + 1] = right_node;
        //! B+트리는 부모 key = 오른쪽 자식의 첫번째 key 이다.
        parentNode->keys[ChildIndex] = right_node->keys[0];
        parentNode->KeyCount++;
    }

    else
    {
        // 2T-1를 분리하면서 가운데 값은 부모로 올리고
        // 나머지  2T-2개의 key들을 left와 right가 나누어가진다.
        right_node->KeyCount = T - 1;
        left_node->KeyCount = T - 1;

        // left의 키와 자식을 right로 옮긴다.
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

    // 부모, 자식 재구성
    left_node->parents = parentNode;
    right_node->parents = parentNode;
    left_node->nextNode = right_node;
    right_node->prevNode = left_node;
}

// key 갯수가 2T-1보다 작은 node에 값을 삽입한다.
void Insert_nonfull(BNODE *node, int KeyValue)
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

//! Tree 출력 함수
void Print_Tree(BNODE *node, int level)
{
    if (node->KeyCount == 0)
    {
        printf("\n[EMPTY]\n");
    }
    // leaf가 node가 아니면 DFS 실행함
    if (!node->leaf)
    {
        for (int i = 0; i <= node->KeyCount; i++)
        {
            Print_Tree(node->childs[i], level + 1);
            if (i < node->KeyCount)
            {
                for (int j = 0; j < level; j++)
                {
                    printf("--------------------|");
                }

                printf("[%d]", node->keys[i]);
            }
            printf("\n");
            // Print_Tree(node->childs[i + 1], level + 1);
        }
    }
    else
    {
        for (int i = 0; i < level; i++)
        {
            printf("--------------------|");
        }
        for (int i = 0; i < node->KeyCount; i++)
        {
            printf("[%d]", node->keys[i]);
        }
        printf("\n");
    }

    return;
}
