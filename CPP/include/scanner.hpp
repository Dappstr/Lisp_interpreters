#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "token.hpp"

class Scanner final {
private:
    size_t m_start = 0;
    size_t m_current = 0;

    std::string m_src{};
    std::vector<Token> m_tokens{};
    const std::unordered_map<std::string, Token_Type> m_keywords = {
        {"if", IF},
        {"def", DEF},
        {"let", LET},
        {"fn", FN},
        {"true", TRUE},
        {"false", FALSE},
        {"nil", NIL},
        {"do", DO},
        {"quote", QUOTE},
        {"eval", EVAL},
        {"cons", CONS},
        {"first", FIRST},
        {"rest", REST},
        {"list", LIST},
        {"println", PRINTLN},
        {"not", NOT}
    };

public:
    explicit Scanner(const std::string &src);
    explicit Scanner(std::string &&src);

    [[nodiscard]] std::vector<Token> scan_tokens() &;
    void scan_token() &;
    [[nodiscard]] inline bool is_at_end() const& noexcept;
    inline char advance() &;
    void add_null_token(Token_Type type) & noexcept;
    void add_token(Token_Type type, const lit &literal) &;
    bool match(char expected) &;
    char peek() const&;
    [[nodiscard]] char peek_next() const&;
    void str() &;
    void number() &;
    void identifier()&;
};
