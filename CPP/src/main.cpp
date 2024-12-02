#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../include/token.hpp"
#include "../include/scanner.hpp"
#include "../include/parser.hpp"
#include "../include/ast.hpp"

std::string get_user_input(const char *prompt) noexcept {
    std::string input{};
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;   
}

int main() {
    while (true) {
        std::string input = get_user_input("user> ");
        if (input == "quit") {
            std::cout << "Exiting.\n";
            break;
        } else {
            try {
                Scanner scanner = Scanner(input);
                std::vector<Token> tokens = scanner.scan_tokens();

                for (const auto &t : tokens) {
                    std::cout << t;
                }
                Parser parser = Parser(std::move(tokens));
                std::vector<AST_Node> ast = parser.parse();

                std::cout << "Parsed AST:\n";
                for (const auto &node : ast) {
                    print_ast(node, 0);
                }
            } catch (const std::runtime_error &e) {
                std::cerr << "Error: " << e.what() << '\n';
            }
        }
    }
    return EXIT_SUCCESS;
}
