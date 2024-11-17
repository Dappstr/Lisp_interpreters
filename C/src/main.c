#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

static char buffer[2048];

const char *read(const char *input) {
    return input;
}

const char *eval(const char *input) {
    return input;
}

const char *print(const char *input) {
    return input;
}

// read evaluate print
const char *rep(const char *input) {
    const char *ast = read(input);
    const char* result = eval(ast);
    return print(result);
}

char *get_user_input(const char *prompt) {
    printf("%s", prompt);
    fgets(buffer, 2048, stdin);
    char *cpy = (char *)malloc(strlen(buffer) + 1);
    if (!cpy) {
        printf("Failed to allocate memory for dynamic buffer in %s\n",
               __FUNCTION__);
        return NULL;
    }
    strcpy_s(cpy, strlen(cpy), buffer);
    cpy[strlen(cpy) - 1] = '\0';
    return cpy;
}

int main() {
    while (true) {
        char *input = get_user_input("user> ");
        assert(input && "Something went wrong when returning pointer to memory!");
        printf("%s\n", input);
        free(input);
    }
    return 0;
}
