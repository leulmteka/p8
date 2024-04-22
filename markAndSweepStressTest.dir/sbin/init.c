#include "libc.h" 

#define NULL ((void *)0)

typedef struct Node {
    int value;
    struct Node* next;
} Node;

Node* createNode(int val) {
    Node* n = (Node*)malloc(sizeof(Node));
    if (n == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    n->value = val;
    n->next = NULL;
    return n;
}

Node* createList(int size) {
    Node* head = NULL;
    Node* current = NULL;
    for (int i = 0; i < size; i++) {
        Node* n = createNode(i);
        if (head == NULL) {
            head = n;
            current = head;
        } else {
            current->next = n;
            current = n;
        }
    }
    return head;
}

void removeList(Node* head) {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    const int iterations = 1000;
    const int listSize = 1000;

    printf("Starting the stress test for Mark-and-Sweep GC\n");

    for (int i = 0; i < iterations; i++) {
        Node* list = createList(listSize);
        removeList(list);
        if (i % 100 == 0) {
            printf("Iteration %d: List created and destroyed\n", i);
        }
    }

    printf("Stress test completed\n");
    shutdown();
    return 0;
}
