#include <string.h>
#include <stdlib.h>
#include "linkedlist.h"

// inserts in sorted decreasing order according to the data
void insert(struct node **headPtr, char key[64], int data) {
    struct node *link = create(key, data);
    struct node* head = *headPtr;
    if(head->data < link->data){
        link->next = head;
        *headPtr = link;
    } else {
        struct node* parent = head;
        while (parent->next) {
            if(parent->next->data < link->data){
                link->next = parent->next;
                parent->next = link;
                return;
            }
            parent = parent->next;
        }
        parent->next = link;
    }
}


struct node *create(char key[64], int data) {
    struct node *link = malloc(sizeof(struct node));
    strcpy(link->key, key);
    link->data = data;
    return link;
}

void deleteFirst(struct node **head) {
    struct node *tempLink = *head;
    *head = (*head)->next;
    free(tempLink);
}

void deleteAll(struct node **head) {
    while (*head != NULL) {
        deleteFirst(head);
    }
}

int isEmpty(struct node *head) {
    return head == NULL;
}

int length(struct node *head) {
    int length = 0;
    struct node *current;
    for (current = head; current != NULL; current = current->next) {
        length++;
    }
    return length;
}

struct node *find(struct node *head, char key[64]) {
    struct node *current = head;
    if (head == NULL) {
        return NULL;
    }
    int comparison = strcmp(current->key, key);
    while (comparison) {
        if (current->next == NULL) {
            return NULL;
        } else {
            current = current->next;
        }
        comparison = strcmp(current->key, key);
    }
    return current;
}
