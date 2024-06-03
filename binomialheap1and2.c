#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct BinomialNode {
    int key, degree;
    struct BinomialNode *parent, *child, *sibling;
} BinomialNode;

typedef struct BinomialHeap {
    BinomialNode* head;
} BinomialHeap;

BinomialNode* createNode(int key) {
    BinomialNode* node = (BinomialNode*)malloc(sizeof(BinomialNode));
    node->key = key; node->degree = 0;
    node->parent = node->child = node->sibling = NULL;
    return node;
}

BinomialHeap* createHeap() {
    BinomialHeap* heap = (BinomialHeap*)malloc(sizeof(BinomialHeap));
    heap->head = NULL;
    return heap;
}

void linkTrees(BinomialNode* y, BinomialNode* z) {
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;
}

BinomialHeap* mergeHeaps(BinomialHeap* h1, BinomialHeap* h2) {
    BinomialHeap* newHeap = createHeap();
    BinomialNode** pos = &newHeap->head;
    BinomialNode* x = h1->head;
    BinomialNode* y = h2->head;

    while (x && y) {
        if (x->degree <= y->degree) {
            *pos = x;
            x = x->sibling;
        } else {
            *pos = y;
            y = y->sibling;
        }
        pos = &(*pos)->sibling;
    }
    *pos = (x) ? x : y;
    return newHeap;
}

BinomialHeap* unionHeaps(BinomialHeap* h1, BinomialHeap* h2) {
    BinomialHeap* newHeap = mergeHeaps(h1, h2);
    if (!newHeap->head) return newHeap;

    BinomialNode *prev = NULL, *curr = newHeap->head, *next = curr->sibling;
    while (next) {
        if ((curr->degree != next->degree) ||
            (next->sibling && next->sibling->degree == curr->degree)) {
            prev = curr;
            curr = next;
        } else {
            if (curr->key <= next->key) {
                curr->sibling = next->sibling;
                linkTrees(next, curr);
            } else {
                if (prev) prev->sibling = next;
                else newHeap->head = next;
                linkTrees(curr, next);
                curr = next;
            }
        }
        next = curr->sibling;
    }
    return newHeap;
}

BinomialHeap* insert(BinomialHeap* heap, int key) {
    BinomialHeap* tempHeap = createHeap();
    tempHeap->head = createNode(key);
    return unionHeaps(heap, tempHeap);
}

BinomialNode* getMin(BinomialHeap* heap) {
    BinomialNode *y = NULL, *x = heap->head;
    int min = INT_MAX;
    while (x) {
        if (x->key < min) {
            min = x->key;
            y = x;
        }
        x = x->sibling;
    }
    return y;
}

BinomialHeap* extractMin(BinomialHeap* heap) {
    if (!heap->head) return NULL;
    BinomialNode *minNode = getMin(heap), *prevMin = NULL;
    BinomialNode *curr = heap->head, *prev = NULL;

    while (curr != minNode) {
        prev = curr;
        curr = curr->sibling;
    }
    if (prev) prev->sibling = curr->sibling;
    else heap->head = curr->sibling;

    BinomialHeap* newHeap = createHeap();
    BinomialNode* child = curr->child;
    while (child) {
        BinomialNode* temp = child;
        child = child->sibling;
        temp->sibling = newHeap->head;
        newHeap->head = temp;
    }
    free(curr);
    return unionHeaps(heap, newHeap);
}

void decreaseKey(BinomialHeap* heap, BinomialNode* node, int newKey) {
    if (newKey > node->key) return;
    node->key = newKey;
    BinomialNode* y = node;
    BinomialNode* z = node->parent;

    while (z && y->key < z->key) {
        int temp = y->key;
        y->key = z->key;
        z->key = temp;
        y = z;
        z = z->parent;
    }
}

void deleteNode(BinomialHeap* heap, BinomialNode* node) {
    decreaseKey(heap, node, INT_MIN);
    heap = extractMin(heap);
}

int main() {
    BinomialHeap* heap = createHeap();
    heap = insert(heap, 10);
    heap = insert(heap, 20);
    heap = insert(heap, 5);

    BinomialNode* minNode = getMin(heap);
    printf("Minimum key: %d\n", minNode->key);

    heap = extractMin(heap);
    minNode = getMin(heap);
    printf("Minimum key after extraction: %d\n", minNode->key);

    return 0;
}
