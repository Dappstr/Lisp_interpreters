#include <cctype>
#include <iostream>
#include <string>
#include <vector>

struct Node {
    std::string value;
    Node *next;
};

class Lexer {
private:
    const std::string m_buff;
public:
    Lexer(std::string &&source) : m_buff(std::move(source)) {}
    [[nodiscard]]const std::vector<std::string> lex() noexcept {
        std::vector<std::string> lexemes{};
        std::string lexeme {};
        for (size_t i = 0; i < m_buff.size(); ++i) {
          while (m_buff.at(i) == ' ' || m_buff.at(i) == '\t' ||
                 m_buff.at(i) == '\n') {
              ++i;
          }
          if (i >= m_buff.size()) {
              break;
          }
          if (isalpha(m_buff.at(i))) {
              size_t x = i + 1;
              while (x < m_buff.size() && isalpha(m_buff.at(x))) {
                  ++x;
              }
              lexeme = m_buff.substr(i, x - i);
              i = x - 1;
          } else if (isdigit(m_buff.at(i))) {
              size_t x = i + 1;
              while (x < m_buff.size() && isdigit(m_buff.at(x))) {
                  ++x;
              }
              lexeme = m_buff.substr(i, x - i);
              i = x - 1;
          } else {
              // Will be a single character
              lexeme.push_back(m_buff.at(i));
          }
          lexemes.emplace_back(lexeme);
          lexeme.clear();
        }
        return lexemes;
    }
};

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
            Lexer lexer = Lexer(std::move(input));
            std::vector<std::string> lexemes = lexer.lex();
            for (const auto &s : lexemes) {
                std::cout << s << '\n';
            }
        }
    }
    return EXIT_SUCCESS;
}
