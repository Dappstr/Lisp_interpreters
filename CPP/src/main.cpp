#include <cstdlib>
#include <iostream>
#include <string>

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
            // ...
        }
        std::cout << input << '\n';
    }

    return EXIT_SUCCESS;
}
