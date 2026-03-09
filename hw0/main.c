#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node {
    char data;
    int count;
    struct Node *next;
} Node;

Node *head = NULL;

void updateNode(char c) {
    Node *temp = head;

    while (temp != NULL) {
        if (temp->data == c) {
            temp->count++;
            return;
        }
        temp = temp->next;
    }

    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        return;
    }
    new_node->data = c;
    new_node->count = 1;
    new_node->next = NULL;

    if (head == NULL) {
        head = new_node;
    } else {
        temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

void freeList() {
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    FILE *fp = fopen(__FILE__, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        updateNode(ch);
    }
    fclose(fp);

    Node *temp = head;
    while (temp != NULL) {
        if (temp->data == ' ') {
            printf("' ':%d\n", temp->count);
        } else if (temp->data == '\n') {
            printf("'\\n':%d\n", temp->count);
        } else if (temp->data == '\t') {
            printf("'\\t':%d\n", temp->count);
        } else if (temp->data == '\r') {
            printf("'\\r':%d\n", temp->count);
        } else {
            printf("%c:%d\n", temp->data, temp->count);
        }
        
        temp = temp->next;
    }

    freeList();
    getchar();
    return 0;
}
