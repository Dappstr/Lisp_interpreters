#include <iostream>
#include <string>
#include <vector>
#include "../include/token.hpp"
#include "../include/scanner.hpp"

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
            Scanner scanner = Scanner(input);
            std::vector<Token> tokens = scanner.scan_tokens();

            for (const auto &t : tokens) {
                std::cout << t << '\n';
            }
        }
    }
    return EXIT_SUCCESS;
}
