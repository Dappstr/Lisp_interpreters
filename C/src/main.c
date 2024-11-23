#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

static char buffer[2048];

typedef struct Node {
    struct Node *next;
    struct Node *children;
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
    node->value = value ? _strdup(value) : NULL; // Duplicate value if provided
    node->children = NULL;
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

Node *read_expr(const char **input) {
    // Skip whitespace
    while (**input == ' ' || **input == '\t' || **input == '\n') {
        (*input)++;
    }
    // Check if we reached the end of the source
    if (**input == '\0') {
        return NULL;
    } else {
        // Begin writing an expression
        if (**input == '(') { // Create a new node to hold the operator and its "atoms" (operands)
            (*input)++; // Skip opening parenth

            Node *list = create_node(NULL);
            Node *tail = NULL;
            Node *operator= read_expr(input); // Get the operator, which is given becaause `input` was incremented previously

            if (!operator || operator->value == NULL) {
                printf("Error: Invalid operator in list.\n");
                exit(1);
            }
            
            list->value = operator->value;
            free(operator);
            
            while (**input != ')' && **input != '\0') {
              Node *child = read_expr(input); // Recursive call. If it
                                              // encounters another
                                              // opening parenth, this block
                                              // will be executed. Otherwise it will try to parse a token like a number or symbol name
              if (!child) {
                printf("Error: Unexpected null child node.\n");
                exit(1);
              }
              if (!list->children) {
                  list->children = child;
              } else {
                  tail->next = child;
              }
                tail = child; //
            }
            if (**input == ')') {
                (*input)++; // Skip closing parenth
            } else {
                printf("Error: Unmatched opening parenthesis.\n");
                exit(1);
            }
            return list; // We've reached the end of an expression
        } else if (**input == ')') {
            printf("Error: Unmatched closing parenthesis.\n");
            exit(1);
        } else {
            char token[512];
            size_t i = 0;
            while (**input != ' ' && **input != '\t' && **input != '\n' &&
                   **input != '(' && **input != ')' && **input != '\0') {
                token[i++] = *(*input)++;
            }
            token[i] = '\0';
            return create_node(token);
        }
    }
}
void print_ast(Node *node, int indent) {
    if (!node) {
        return;
    }

    // Print the current node's value with indentation
    for (size_t i = 0; i < indent; ++i) {
        printf(" ");
    }
    if (node->value) {
        printf("%s\n", node->value);
    } else {
        printf("()\n");
    }

    // Recursively print children with increased indentation
    if (node->children) {
        print_ast(node->children, indent + 2);
    }

    // Recursively print siblings at the same indentation level
    if (node->next) {
        print_ast(node->next, indent);
    }
}

Node *read(const char *input) {
    const char *ptr = input;
    return read_expr(&ptr);
}

Node *eval(Node *ast) {
    return ast;
}

char *print(Node *ast) {
    if (!ast) {
        return _strdup(""); // If the AST is empty
    }
    size_t buff_size = 1024;
    char *output = (char *)malloc(buff_size);
    if (!output) {
        printf("Failed to allocate memory for output in %s\n",
               __FUNCTION__);
        exit(1);
    }
    output[0] = '\0';
    Node *current_node = ast; // Start at the first node pointed to by `ast`
    while (current_node) {
        if (strlen(output) + strlen(current_node->value) + 2 > buff_size) { // Check if there is enough room for a delimiter and null terminator
          buff_size *= 2;
          output = (char *)realloc(output, buff_size);
          if (!output) {
              printf("Failed to reallocate buffer for string in %s\n",
                   __FUNCTION__);
              exit(1);
          }
      }
      strcat_s(output, buff_size, current_node->value);
      if (current_node->next) { // If there is another lexeme after the current one
          strcat_s(output, buff_size, "\n"); // Append a newline to the string stored into `output`
      }
      current_node = current_node->next;
    }
    return output;
}

// read evaluate print
char *rep(const char *input) {
    Node *ast = read(input);
    Node *result = eval(ast);
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
        Node *ast = read(input);
        printf("Parsed AST:\n");
        print_ast(ast, 0);
        Node *result = eval(ast);
        char *output = print(result);
        free_ast(ast);
        free(input);
        free(output);
    }
    return 0;
}
