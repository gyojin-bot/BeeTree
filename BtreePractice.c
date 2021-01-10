#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define M 3
#define T 2

typedef struct BtreeNode  {
    int KeyCount; // key의 갯수
    bool leaf; // leaf인지 판정
    int keys[2*T-1]; // node가 가지고 있는 key 값들
    struct BtreeNode* childs[M]; // 현재 노드와 연결되어있는 child의 배열이다.
} BTREENODE;


// tree 구조체 선언
typedef struct Btree {
    struct BtreeNode* root; 
} BTREE;
// Btree라는 구조체는 node포인터를 가진다. (이름이 root)
// 비트리 루트 노드의 포인터를 가짐으로서 비트리 전체에 관여할 수 있도록한다.


BTREENODE* Allocate();
void TreeCreate(BTREE*);
void Insert_nonfull(BTREENODE*, int);
void Insert(BTREE*, int);
void Delete(BTREENODE*, int);
bool Search(BTREENODE*, int);
void Split_Child(BTREENODE*, int);
int Get_Ran_Int();

int main() {
    BTREE tree;
    TreeCreate(&tree);
    Insert(&tree, 10);
    Insert(&tree, 20);
    Insert(&tree, 30);
    Insert(&tree, 40);
    Insert(&tree, 25);
    Insert(&tree, 15);
    Insert(&tree, 50);
    // Split_Child(tree.root, 0);
    // for ( int i = 0; i < tree.root->KeyCount; ++i) {
    //     printf("keys: %d\n", tree.root->keys[i]);
    //     printf("keysaddress: %d\n", &(tree.root->keys[i]));
    // }
    // printf("root keycount: %d\n", tree.root->KeyCount);
    // printf("root key 0: %d\n", tree.root->keys[0]);
    // printf("root key 1: %d\n", tree.root->keys[1]);
    // printf("root key 2: %d\n", tree.root->keys[2]);
    // printf("child key 0-0: %d\n", tree.root->childs[0]->keys[0]);
    // printf("child key 1-0: %d\n", tree.root->childs[1]->keys[0]);
    // printf("child key 0-1: %d\n", tree.root->childs[0]->keys[1]);
    // printf("child key 1-1: %d\n", tree.root->childs[1]->keys[1]);
    if (Search(tree.root, 15)){
        printf("\nYES");
    } else {
        printf("\nNONE");
    }
    
    return 0;
}

// 노드를 생성하기 전에 메모리를 할당한다.
BTREENODE* Allocate() {

    // BTREENODE의 크기만큼 할당한다. (malloc은 메모리를 할당하고, 할당한 주소값을 반환한다.)
    BTREENODE* new_node = (BTREENODE*)malloc(sizeof(BTREENODE));
    // BTREENODE*형태의 new_node 선언 = BTREENODE*의 형태의 반환된 malloc의 주소값 
    
    // new_node(BTREENODE*)를 반환
    return new_node;
}


// 트리를 만든다.
void TreeCreate(BTREE* tree) { // BTREE는 root* 의 주소를 가진 구조체이다.
    BTREENODE* new_node = Allocate(); // 새 노드를 만들기 위해 포인터에 주소를 할당한다.
    new_node->KeyCount = 0; // 새로만들면 key가 안들어있으므로 keycount를 0으로 한다.
    new_node->leaf = true; // 마찬가지로 leaf속성을 on한다.
    tree->root = new_node; // new_node가 루트노드가 되므로 tree의 root는 new_node로 한다. (둘다 포인터이다.)
}


// key 갯수가 2T-1보다 작은 node에 값을 삽입한다.
void Insert_nonfull(BTREENODE* node, int KeyValue) {
    // KeyIndex는 node의 key 갯수를 할당한다.
    // 즉, key[]의 가장 마지막 key의 idx를 할당한다.
    int KeyIndex = node->KeyCount;

    // 만약 node가 leaf node라면
    if ( node->leaf) {

        // node의 key 갯수가 1 이상이고 (빈 노드가 아니고),
        // 넣을 key값이 key[idx]보다 작아질때까지 idx를 줄인다.
        // idx는 새로운 key값을 넣을 자리가 된다.
        while ( KeyIndex >= 1 && KeyValue < node->keys[KeyIndex-1]) {
            // 새로운 key값을 넣을 자리를 마련해준다. (한칸씩 오른쪽으로 이동한다.)
            node->keys[KeyIndex] = node->keys[KeyIndex-1];
            KeyIndex--;
        }

        // key값을 넣을 자리를 찾았고, 위에서 해당 자리도 비워놨기 때문에
        // 위에서 찾은 key[idx]에 새로운 key값을 할당한다.
        node->keys[KeyIndex] = KeyValue;
        // 그리고 노드의 keycount를 1 늘려준다.
        node->KeyCount += 1;
    }
    else {
        while (KeyIndex >= 1 && KeyValue < node->keys[KeyIndex-1]) {
            KeyIndex--;
        }
        //! KeyIndex++;
        if (node->childs[KeyIndex]->KeyCount == 2*T - 1){
            Split_Child(node, KeyIndex);
            if (KeyValue > node->keys[KeyIndex]){
                KeyIndex++;
            }
        }
        Insert_nonfull(node->childs[KeyIndex], KeyValue);
    }
    
}


// node의 key 갯수가 2T-1인 경우, node를 분리한다. 
void Split_Child(BTREENODE* parentNode, int ChildIndex) {
    // right_node는 분리하면서 만들어질 새로운 node이기 때문에 메모리를 새로 할당한다.
    BTREENODE* right_node = Allocate();

    // 
    BTREENODE* left_node = parentNode->childs[ChildIndex];

    // right_node의 leaf 속성은 left_node의 leaf를 받아온다.
    right_node->leaf = left_node->leaf;

    // 2T-1를 분리하면서 가운데 값은 부모로 올리고
    // 나머지  2T-2개의 key들을 left와 right가 나누어가진다. 
    right_node->KeyCount = T-1;
    left_node->KeyCount = T-1;

    // 그래서 left_node[i]
    for ( int i = 0; i < T-1; ++i ) {
        right_node->keys[i] = left_node->keys[T+i];
    }
    for ( int i = 0; i < T ; ++i ) {
        right_node->childs[i] = left_node->childs[T+i];
    }
    for ( int i = parentNode->KeyCount; i > ChildIndex ; --i ) {
        parentNode->childs[i+1] = parentNode->childs[i];
        parentNode->keys[i] = parentNode->keys[i-1];
    }
    parentNode->childs[ChildIndex+1] = right_node;
    parentNode->keys[ChildIndex] = left_node->keys[T-1];
    parentNode->KeyCount++;
}


// key값을 tree에 삽입한다.
void Insert(BTREE* tree, int keyValue){
    BTREENODE* rootNode = tree->root;
    printf("rootNode keycount : %d\n", rootNode->KeyCount);
    if (rootNode->KeyCount == 2*T - 1){
        printf("0");
        BTREENODE* newRootNode = Allocate();
        tree->root = newRootNode;
        newRootNode->leaf = false;
        newRootNode->KeyCount = 0;
        newRootNode->childs[0] = rootNode;
        Split_Child(newRootNode, 0);
        Insert_nonfull(newRootNode, keyValue);
    }
    else {
        Insert_nonfull(rootNode, keyValue);
    }

}


bool Search(BTREENODE* node, int keyValue){
    int index = 0;
    while ( index < node->KeyCount && keyValue > node->keys[index]){
        index++;
    }
    if ( index <= node->KeyCount && keyValue == node->keys[index] ){
        return true;
    }
    else if ( node->leaf ){
        return false;
    }
    else {
        return Search(node->childs[index], keyValue);
    }
}