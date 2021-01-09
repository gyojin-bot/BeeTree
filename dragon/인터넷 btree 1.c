#include "stdio.h"
#include "stdlib.h"

#define M 3
// M = 3 -> 3차트리?



// NODE 선언
// 구조체의 별칭 선언 (관례적으로 _를 접두어로 쓴다)
typedef struct _node {
        int    n; /* n < M No. of keys in node will always less than order of B tree */
                    // order : 늘어날 수 있는 최대 다리
        int   keys[M - 1]; /*array of keys 어레이 선언 */
        struct _node *p[M]; /* (n+1 pointers will be in use) */
} node;

// node의 root값 null ?
node *root = NULL;

// 키상태 선언 KeyStatus {복사, 검색실패, 성공, 삽입?, 남은키?}
typedef enum KeyStatus {
        Duplicate,
        SearchFailure,
        Success,
        InsertIt,
        LessKeys,
} KeyStatus;

// 삽입 (key값)
void insert(int key);

//  표시 (node *root<-어디서 온 값?, 정수 <- 어떤값?)
void display(node *root, int);

// 노드 삭제 (정수 x 삽입)
void DelNode(int x);

// 검색 (정수 x 삽입, x는 키인가?)
void search(int x);

// ins는 함수인가? (삽입?)
KeyStatus ins(node *r, int x, int* y, node** u);

// 정수 구조체? 
int searchPos(int x, int *key_arr, int n);

// 삭제? r은 무엇인가
KeyStatus del(node *r, int x);

// 선을 먹지마
void eatline(void);

// ptr이 사용됨
void inorder(node *ptr);
int totalKeys(node *ptr);
void printTotal(node *ptr);
int getMin(node *ptr);
int getMax(node *ptr);
void getMinMax(node *ptr);
int max(int first, int second, int third);
int maxLevel(node *ptr);
void printMaxLevel(node *ptr);


int main() {
        int key;
        int choice;
        printf("Creation of B tree for M=%d\n", M);
        while (1) {
                printf("1.Insert\n");
                printf("2.Delete\n");
                printf("3.Search\n");
                printf("4.Display\n");
                printf("5.Quit\n");
                printf("6.Enumerate\n");
                printf("7.Total Keys\n");
                printf("8.Min and Max Keys\n");
                printf("9.Max Level\n");
                printf("Enter your choice : ");
                scanf("%d", &choice); eatline();

                switch (choice) {
                // 1. Insert : 키 삽입
                case 1:
                        printf("Enter the key : ");
                        scanf("%d", &key); eatline();
                        insert(key);
                        break;

                // 2.Delete : 키 삭제
                case 2:
                        printf("Enter the key : ");
                        scanf("%d", &key); eatline();
                        DelNode(key);
                        break;

                // 3.Search : 키 검색?
                case 3:
                        printf("Enter the key : ");
                        scanf("%d", &key); eatline();
                        search(key);
                        break;

                // 4.Display : 현재 트리 표시
                case 4:
                        printf("Btree is :\n");
                        display(root, 0);
                        break;
                
                // 5. 종료
                case 5:
                        exit(1);

                // 6. root 표시?
                case 6:
                        printf("Btree in sorted order is:\n");
                        inorder(root); putchar('\n');
                        break;

                // 7. 총 갯수?
                case 7:
                        printf("The total number of keys in this tree is:\n");
                        printTotal(root);
                        break;

                // 8. Min, Max key 표시?
                case 8:
                        getMinMax(root);
                        break;

                // 9. Max level : 높이? 
                case 9:
                        printf("The maximum level in this tree is:\n");
                        printMaxLevel(root);
                        break;
                default:
                        printf("Wrong choice\n");
                        break;
                }/*End of switch*/
        }/*End of while*/
        return 0;
}/*End of main()*/

// 삽입(key받음)
void insert(int key) {
        node *newnode;
        int upKey;
        KeyStatus value;
        value = ins(root, key, &upKey, &newnode);
        if (value == Duplicate)
                printf("Key already available\n");
        if (value == InsertIt) {
                node *uproot = root;
                root = (node*)malloc(sizeof(node));
                root->n = 1;
                root->keys[0] = upKey;
                root->p[0] = uproot;
                root->p[1] = newnode;
        }/*End of if */
}/*End of insert()*/

KeyStatus ins(node *ptr, int key, int *upKey, node **newnode) {
        node *newPtr, *lastPtr;
        int pos, i, n, splitPos;
        int newKey, lastKey;
        KeyStatus value;
        if (ptr == NULL) {
                *newnode = NULL;
                *upKey = key;
                return InsertIt;
        }
        n = ptr->n;
        pos = searchPos(key, ptr->keys, n);
        if (pos < n && key == ptr->keys[pos])
                return Duplicate;
        value = ins(ptr->p[pos], key, &newKey, &newPtr);
        if (value != InsertIt)
                return value;
        /*If keys in node is less than M-1 where M is order of B tree*/
        if (n < M - 1) {
                pos = searchPos(newKey, ptr->keys, n);
                /*Shifting the key and pointer right for inserting the new key*/
                for (i = n; i>pos; i--) {
                        ptr->keys[i] = ptr->keys[i - 1];
                        ptr->p[i + 1] = ptr->p[i];
                }
                /*Key is inserted at exact location*/
                ptr->keys[pos] = newKey;
                ptr->p[pos + 1] = newPtr;
                ++ptr->n; /*incrementing the number of keys in node*/
                return Success;
        }/*End of if */
         /*If keys in nodes are maximum and position of node to be inserted is last*/
        if (pos == M - 1) {
                lastKey = newKey;
                lastPtr = newPtr;
        }
        else { /*If keys in node are maximum and position of node to be inserted is not last*/
                lastKey = ptr->keys[M - 2];
                lastPtr = ptr->p[M - 1];
                for (i = M - 2; i>pos; i--) {
                        ptr->keys[i] = ptr->keys[i - 1];
                        ptr->p[i + 1] = ptr->p[i];
                }
                ptr->keys[pos] = newKey;
                ptr->p[pos + 1] = newPtr;
        }
        splitPos = (M - 1) / 2;
        (*upKey) = ptr->keys[splitPos];

        (*newnode) = (node*)malloc(sizeof(node));/*Right node after split*/
        ptr->n = splitPos; /*No. of keys for left splitted node*/
        (*newnode)->n = M - 1 - splitPos;/*No. of keys for right splitted node*/
        for (i = 0; i < (*newnode)->n; i++) {
                (*newnode)->p[i] = ptr->p[i + splitPos + 1];
                if (i < (*newnode)->n - 1)
                        (*newnode)->keys[i] = ptr->keys[i + splitPos + 1];
                else
                        (*newnode)->keys[i] = lastKey;
        }
        (*newnode)->p[(*newnode)->n] = lastPtr;
        return InsertIt;
}/*End of ins()*/

void display(node *ptr, int blanks) {
        if (ptr) {
                int i;
                for (i = 1; i <= blanks; i++)
                        printf(" ");
                for (i = 0; i < ptr->n; i++)
                        printf("%d ", ptr->keys[i]);
                printf("\n");
                for (i = 0; i <= ptr->n; i++)
                        display(ptr->p[i], blanks + 10);
        }/*End of if*/
}/*End of display()*/

void search(int key) {
        int pos, i, n;
        node *ptr = root;
        printf("Search path:\n");
        while (ptr) {
                n = ptr->n;
                for (i = 0; i < ptr->n; i++)
                        printf(" %d", ptr->keys[i]);
                printf("\n");
                pos = searchPos(key, ptr->keys, n);
                if (pos < n && key == ptr->keys[pos]) {
                        printf("Key %d found in position %d of last dispalyed node\n", key, i);
                        return;
                }
                ptr = ptr->p[pos];
        }
        printf("Key %d is not available\n", key);
}/*End of search()*/

int searchPos(int key, int *key_arr, int n) {
        int pos = 0;
        while (pos < n && key > key_arr[pos])
                pos++;
        return pos;
}/*End of searchPos()*/

void DelNode(int key) {
        node *uproot;
        KeyStatus value;
        value = del(root, key);
        switch (value) {
        case SearchFailure:
                printf("Key %d is not available\n", key);
                break;
        case LessKeys:
                uproot = root;
                root = root->p[0];
                free(uproot);
                break;
        default:
                return;
        }/*End of switch*/
}/*End of delnode()*/

KeyStatus del(node *ptr, int key) {
        int pos, i, pivot, n, min;
        int *key_arr;
        KeyStatus value;
        node **p, *lptr, *rptr;

        if (ptr == NULL)
                return SearchFailure;
        /*Assigns values of node*/
        n = ptr->n;
        key_arr = ptr->keys;
        p = ptr->p;
        min = (M - 1) / 2;/*Minimum number of keys*/

                                          //Search for key to delete
        pos = searchPos(key, key_arr, n);
        // p is a leaf
        if (p[0] == NULL) {
                if (pos == n || key < key_arr[pos])
                        return SearchFailure;
                /*Shift keys and pointers left*/
                for (i = pos + 1; i < n; i++)
                {
                        key_arr[i - 1] = key_arr[i];
                        p[i] = p[i + 1];
                }
                return --ptr->n >= (ptr == root ? 1 : min) ? Success : LessKeys;
        }/*End of if */

         //if found key but p is not a leaf
        if (pos < n && key == key_arr[pos]) {
                node *qp = p[pos], *qp1;
                int nkey;
                while (1) {
                        nkey = qp->n;
                        qp1 = qp->p[nkey];
                        if (qp1 == NULL)
                                break;
                        qp = qp1;
                }/*End of while*/
                key_arr[pos] = qp->keys[nkey - 1];
                qp->keys[nkey - 1] = key;
        }/*End of if */
        value = del(p[pos], key);
        if (value != LessKeys)
                return value;

        if (pos > 0 && p[pos - 1]->n > min) {
                pivot = pos - 1; /*pivot for left and right node*/
                lptr = p[pivot];
                rptr = p[pos];
                /*Assigns values for right node*/
                rptr->p[rptr->n + 1] = rptr->p[rptr->n];
                for (i = rptr->n; i>0; i--) {
                        rptr->keys[i] = rptr->keys[i - 1];
                        rptr->p[i] = rptr->p[i - 1];
                }
                rptr->n++;
                rptr->keys[0] = key_arr[pivot];
                rptr->p[0] = lptr->p[lptr->n];
                key_arr[pivot] = lptr->keys[--lptr->n];
                return Success;
        }/*End of if */
         //if (posn > min)
        if (pos < n && p[pos + 1]->n > min) {
                pivot = pos; /*pivot for left and right node*/
                lptr = p[pivot];
                rptr = p[pivot + 1];
                /*Assigns values for left node*/
                lptr->keys[lptr->n] = key_arr[pivot];
                lptr->p[lptr->n + 1] = rptr->p[0];
                key_arr[pivot] = rptr->keys[0];
                lptr->n++;
                rptr->n--;
                for (i = 0; i < rptr->n; i++) {
                        rptr->keys[i] = rptr->keys[i + 1];
                        rptr->p[i] = rptr->p[i + 1];
                }/*End of for*/
                rptr->p[rptr->n] = rptr->p[rptr->n + 1];
                return Success;
        }/*End of if */

        if (pos == n)
                pivot = pos - 1;
        else
                pivot = pos;

        lptr = p[pivot];
        rptr = p[pivot + 1];
        /*merge right node with left node*/
        lptr->keys[lptr->n] = key_arr[pivot];
        lptr->p[lptr->n + 1] = rptr->p[0];
        for (i = 0; i < rptr->n; i++) {
                lptr->keys[lptr->n + 1 + i] = rptr->keys[i];
                lptr->p[lptr->n + 2 + i] = rptr->p[i + 1];
        }
        lptr->n = lptr->n + rptr->n + 1;
        free(rptr); /*Remove right node*/
        for (i = pos + 1; i < n; i++) {
                key_arr[i - 1] = key_arr[i];
                p[i] = p[i + 1];
        }
        return --ptr->n >= (ptr == root ? 1 : min) ? Success : LessKeys;
}/*End of del()*/


// 엔터가 아니라면, 값 무시?
void eatline(void) {
        char c;
        while ((c = getchar()) != '\n');
}

/* Function to display each key in the tree in sorted order (in-order traversal)
@param struct node *ptr, the pointer to the node you are currently working with
*/
void inorder(node *ptr) {
        if (ptr) {
                if (ptr->n >= 1) {
                        inorder(ptr->p[0]);
                        printf("%d ", ptr->keys[0]);
                        inorder(ptr->p[1]);
                        if (ptr->n == 2) {
                                printf("%d ", ptr->keys[1]);
                                inorder(ptr->p[2]);
                        }
                }
        }
}

/* Function that returns the total number of keys in the tree.
@param struct node *ptr, the pointer to the node you are currently working with
*/
int totalKeys(node *ptr) {
        if (ptr) {
                int count = 1;
                if (ptr->n >= 1) {
                        count += totalKeys(ptr->p[0]);
                        count += totalKeys(ptr->p[1]);
                        if (ptr->n == 2) count += totalKeys(ptr->p[2]) + 1;
                }
                return count;
        }
        return 0;
}

/* Function that prints the total number of keys in the tree.
@param struct node *ptr, the pointer to the node you are currently working with
*/
void printTotal(node *ptr) {
        printf("%d\n", totalKeys(ptr));
}

/* Function that returns the smallest key found in the tree.
@param struct node *ptr, the pointer to the node you are currently working with
*/
int getMin(node *ptr) {
        if (ptr) {
                int min;
                if (ptr->p[0] != NULL) min = getMin(ptr->p[0]);
                else min = ptr->keys[0];
                return min;
        }
        return 0;
}

/* Function that returns the largest key found in the tree.
@param struct node *ptr, the pointer to the node you are currently working with
*/
int getMax(node *ptr) {
        if (ptr) {
                int max;
                if (ptr->n == 1) {
                        if (ptr->p[1] != NULL) max = getMax(ptr->p[1]);
                        else max = ptr->keys[0];
                }
                if (ptr->n == 2) {
                        if (ptr->p[2] != NULL) max = getMax(ptr->p[2]);
                        else max = ptr->keys[1];
                }
                return max;
        }
        return 0;
}

/* Function that prints the smallest and largest keys found in the tree.
@param struct node *ptr, the pointer to the node you are currently working with
*/
void getMinMax(node *ptr) {
        printf("%d %d\n", getMin(ptr), getMax(ptr));
}

/* Function that determines the largest number.
@param int, integer to compare.
@param int, integer to compare.
@param int, integer to compare.
*/
int max(int first, int second, int third) {
        int max = first;
        if (second > max) max = second;
        if (third > max) max = third;
        return max;
}

/*Function that finds the maximum level in the node and returns it as an integer.
@param struct node *ptr, the node to find the maximum level for.
*/
int maxLevel(node *ptr) {
        if (ptr) {
                int l = 0, mr = 0, r = 0, max_depth;
                if (ptr->p[0] != NULL) l = maxLevel(ptr->p[0]);
                if (ptr->p[1] != NULL) mr = maxLevel(ptr->p[1]);
                if (ptr->n == 2) {
                        if (ptr->p[2] != NULL) r = maxLevel(ptr->p[2]);
                }
                max_depth = max(l, mr, r) + 1;
                return max_depth;
        }
        return 0;
}

/*Function that prints the maximum level in the tree.
@param struct node *ptr, the tree to find the maximum level for.
*/
void printMaxLevel(node *ptr) {
        int max = maxLevel(ptr) - 1;
        if (max == -1) printf("tree is empty\n");
        else printf("%d\n", max);
}