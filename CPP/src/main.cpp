#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>
#include "../include/token.hpp"
#include "../include/scanner.hpp"
#include "../include/parser.hpp"
#include "../include/ast.hpp"
#include "../include/evaluator.hpp"
#include "../include/environment.hpp"
#include "../include/builtins.hpp"

std::string get_user_input(const char *prompt) noexcept {
    std::string input{};
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;   
}

int main() {

    auto global_scope = std::make_shared<Environment>();
    initialize_builtins(global_scope);
    
    
    while (true) {
        std::string input = get_user_input("user> ");
        if (input == "quit") {
            std::cout << "Exiting.\n";
            break;
        } else {
            try {
                Scanner scanner = Scanner(input);
                std::vector<Token> tokens = scanner.scan_tokens();

                /*
                for (const auto &t : tokens) {
                    std::cout << t;
                }
                */
                
                Parser parser = Parser(std::move(tokens));
                std::vector<AST_Node> ast = parser.parse();

                /*
                std::cout << "Parsed AST:\n";
                for (const auto &node : ast) {
                    print_ast(node, 0);
                }
                */

                for (const auto &node : ast) {
                    Value result = evaluate(node, *global_scope);

                    if (std::holds_alternative<std::string>(result) &&
                        std::get<std::string>(result) == "nil") {
                        continue;
                    }
                    
                    if (std::holds_alternative<double>(result)) {
                        std::cout << "Result: " << std::get<double>(result) << '\n';
                    } else if (std::holds_alternative<std::string>(result)) {
                        std::cout << "Result: " << std::get<std::string>(result) << '\n';
                    } else if (std::holds_alternative <
                               std::shared_ptr<List_Node>>(result)) {
                        std::cout << "Result: (list)\n";
                    }
                }
            } catch (const std::runtime_error &e) {
                std::cerr << "Error: " << e.what() << '\n';
            }
        }
    }
    return EXIT_SUCCESS;
}
