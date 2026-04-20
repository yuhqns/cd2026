#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_space(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_alnum(char c) {
    return is_alpha(c) || is_digit(c);
}

// 修改 HW0 的 Node，改成儲存 Token 字串與種類
typedef struct Node {
    char lexeme[100];      // 存原始字串，例如 "int", "cd2026", "=="
    char token_type[50];   // 存 Token 類別，例如 "TYPE_TOKEN"
    struct Node *next;
} Node;

Node *head = NULL;
Node *tail = NULL; // 增加 tail 指標，讓新增節點的時間複雜度降為 O(1)

// 將讀取到的 Token 加入 Linked-list 尾端
void addToken(const char* lexeme, const char* token_type) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) return;
    
    strcpy(new_node->lexeme, lexeme);
    strcpy(new_node->token_type, token_type);
    new_node->next = NULL;

    if (head == NULL) {
        head = new_node;
        tail = new_node;
    } else {
        tail->next = new_node;
        tail = new_node;
    }
}

// 印出所有 Token 並釋放記憶體
void printAndFreeList() {
    Node *temp;
    while (head != NULL) {
        printf("%s: %s\n", head->lexeme, head->token_type);
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    // 讀取名為 "test.c" 的測試檔案
    FILE *fp = fopen("test.c", "r");
    if (fp == NULL) {
        printf("Error: Cannot open test.c\n");
        printf("請確保同一個資料夾下有一個 test.c 檔案放著要掃描的程式碼。\n");
        return 1;
    }

    int c;
    while ((c = fgetc(fp)) != EOF) {
        // 1. 忽略空白、換行與 Tab
        if (is_space(c)) {
            continue;
        }

        char lexeme[100] = {0};
        int len = 0;

        // 2. 判斷 Identifier (id) 或是 Keywords
        if (is_alpha(c)) {
            lexeme[len++] = c;
            // 讀取直到不是字母、數字或底線
            while ((c = fgetc(fp)) != EOF && (is_alnum(c) || c == '_')) {
                lexeme[len++] = c;
            }
            if (c != EOF) ungetc(c, fp); // 將多讀的一個字元退回 buffer
            
            // 辨識 Keywords
            if (strcmp(lexeme, "int") == 0) addToken(lexeme, "TYPE_TOKEN");
            else if (strcmp(lexeme, "main") == 0) addToken(lexeme, "MAIN_TOKEN");
            else if (strcmp(lexeme, "if") == 0) addToken(lexeme, "IF_TOKEN");
            else if (strcmp(lexeme, "else") == 0) addToken(lexeme, "ELSE_TOKEN");
            else if (strcmp(lexeme, "while") == 0) addToken(lexeme, "WHILE_TOKEN");
            // 不是 Keywords 就是 ID
            else addToken(lexeme, "ID_TOKEN");
        } 
        // 3. 判斷 Number (僅處理整數 int)
        else if (is_digit(c)) {
            lexeme[len++] = c;
            while ((c = fgetc(fp)) != EOF && is_digit(c)) {
                lexeme[len++] = c;
            }
            if (c != EOF) ungetc(c, fp);
            addToken(lexeme, "LITERAL_TOKEN");
        } 
        // 4. 判斷運算子 (Relop) 與特殊符號
        else {
            lexeme[0] = c;
            if (c == '=') {
                int next_c = fgetc(fp);
                if (next_c == '=') {
                    lexeme[1] = '='; addToken(lexeme, "EQUAL_TOKEN");
                } else {
                    if (next_c != EOF) ungetc(next_c, fp);
                    addToken(lexeme, "ASSIGN_TOKEN");
                }
            } 
            else if (c == '>') {
                int next_c = fgetc(fp);
                if (next_c == '=') {
                    lexeme[1] = '='; addToken(lexeme, "GREATEREQUAL_TOKEN");
                } else {
                    if (next_c != EOF) ungetc(next_c, fp);
                    addToken(lexeme, "GREATER_TOKEN");
                }
            }
            else if (c == '<') {
                int next_c = fgetc(fp);
                if (next_c == '=') {
                    lexeme[1] = '='; addToken(lexeme, "LESSEQUAL_TOKEN");
                } else {
                    if (next_c != EOF) ungetc(next_c, fp);
                    addToken(lexeme, "LESS_TOKEN");
                }
            }
            else if (c == '(') addToken(lexeme, "LEFTPAREN_TOKEN");
            else if (c == ')') addToken(lexeme, "RIGHTPAREN_TOKEN"); 
            else if (c == '{') addToken(lexeme, "LEFTBRACE_TOKEN");
            else if (c == '}') addToken(lexeme, "RIGHTBRACE_TOKEN"); 
            else if (c == ';') addToken(lexeme, "SEMICOLON_TOKEN");
            else if (c == '+') addToken(lexeme, "PLUS_TOKEN");
            else if (c == '-') addToken(lexeme, "MINUS_TOKEN");
            // 若遇到未定義的符號，可以選擇略過或加入錯誤處理
        }
    }

    fclose(fp);
    printAndFreeList();
    return 0;
}