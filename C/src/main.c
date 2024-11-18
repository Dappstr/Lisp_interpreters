#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

static char buffer[2048];

typedef struct Node {
    struct Node *next;
    char *value;
} Node;

// Function to create a new node
Node *create_node(const char *value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        printf("Failed to alloocate memory for node in %s\n",
               __FUNCTION__);
        exit(1);
    }
    node->value = _strdup(value); // Must be passed to `free` to avoid memory leak
    node->next = NULL;
    return node;
}

// Go through the AST and free it
void free_ast(Node *head) {
    while (head) {
        Node *temp = head;
        head = head->next;
        free(temp->value);
        free(temp);
    }
}

Node *read(const char *input) {
    Node *head = NULL, *tail = NULL;
    //const char *delims = " \t\n";
    /*
      char *cpy = _strdup(input);
    if (!cpy) {
      printf("Failed to create duplicate of string in %s\n",
             __FUNCTION__);
      exit(1);
    }
    */
    /*
    char *context = NULL;
    char *token = strtok_s(cpy, delims, &context);
    while (token) {
        Node *node = create_node(token);
        if (!head) {
          head = node;
          tail = node;
        } else {
          tail->next = node;
          tail = node;
        }
        token = strtok_s(NULL, delims, &context);
    }
    free(cpy);
    */
    const char *ptr = input;
    while (*ptr != '\0') {
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') { // Move while the current character is a space, tab, or newline
            ptr++;
        }
        if (*ptr == '\0') {
            break;
        }
        char token[512];
        size_t i = 0;
        if (*ptr == '(' || *ptr == ')') {
            token[i++] = *ptr++; // We use postfix so that it increments after assigning to the current index
        } else {
          while (*ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '(' && *ptr != ')' && *ptr != '\0') {
              token[i++] = *ptr++; // Continue to loop through the next token until one of these is encountered
          }
        }
        token[i] = '\0';
        Node *node = create_node(token);
        if (!head) { // Create the first node
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

Node *eval(Node *ast) {
    return ast;
}

char *print(Node *ast) {
    if (!ast) {
        return _strdup("");
    }

    size_t buff_size = 1024;
    char *output = (char *)malloc(buff_size);
    if (!output) {
        printf("Failed to allocate memory for output in %s\n",
               __FUNCTION__);
        exit(1);
    }
    output[0] = '\0';
    Node *current_node = ast;
    while (current_node) {
      if (strlen(output) + strlen(current_node->value) + 2 > buff_size) {
          buff_size *= 2;
          output = (char *)realloc(output, buff_size);
          if (!output) {
              printf("Failed to reallocate buffer for string in %s\n",
                   __FUNCTION__);
              exit(1);
          }
      }
      strcat_s(output, buff_size, current_node->value);
      if (current_node->next) {
          strcat_s(output, buff_size, "\n");
      }
      current_node = current_node->next;
    }
    return output;
}

// read evaluate print
char *rep(const char *input) {
    Node *ast = read(input);
    Node* result = eval(ast);
    char *output = print(result);
    free_ast(ast);
    return output;
}

char *get_user_input(const char *prompt) {
    printf("%s", prompt);
    if (fgets(buffer, 2048, stdin) == NULL) {
        return NULL;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    char *cpy = (char *)malloc(len + 1);
    if (!cpy) {
        printf("Failed to allocate memory for dynamic buffer in %s\n",
               __FUNCTION__);
        return NULL;
    }
    strcpy_s(cpy, len + 1, buffer);
    return cpy;
}

int main() {
    while (true) {
        char *input = get_user_input("user> ");
        if (!input) {
            printf("Exiting.\n");
            break;
        }
        assert(input && "Something went wrong when returning pointer to memory!\n");
        char *output = rep(input);
        printf("%s\n", output);
        free(input);
        free(output);
    }
    return 0;
}
