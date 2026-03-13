#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char character;
    int count;
    struct Node *next;
} Node;

int main() {
    FILE *fp = fopen("main.c", "r");
    if (fp == NULL) {
        printf("Error: Cannot open main.c\n");
        return 1;
    }

    Node *head = NULL;
    int ch;

    printf("Scanner Character Stream:\n");

    while ((ch = fgetc(fp)) != EOF) {
        if (ch < 0 || ch > 127) continue;

        if (ch == '\n') printf("'\\n' ");
        else if (ch == '\t') printf("'\\t' ");
        else if (ch == ' ') printf("'_' "); 
        else printf("'%c' ", ch);

        Node *current = head;
        Node *prev = NULL;
        int found = 0;

        while (current != NULL) {
            if (current->character == (char)ch) {
                current->count++;
                found = 1;
                break;
            }
            prev = current;
            current = current->next;
        }

        if (!found) {
            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->character = (char)ch;
            newNode->count = 1;
            newNode->next = NULL;
            if (head == NULL) head = newNode;
            else prev->next = newNode;
        }
    }
    fclose(fp);

    printf("\n\nLinked-list Output:\n");

    Node *temp = head;
    while (temp != NULL) {
        if (temp->character == '\n') printf("\\n, ");
        else if (temp->character == '\t') printf("\\t, ");
        else if (temp->character == ' ') printf("_, ");
        else printf("%c, ", temp->character);
        
        Node *toDelete = temp;
        temp = temp->next;
        free(toDelete);
    }
    printf(".....\n");

    return 0;
}
