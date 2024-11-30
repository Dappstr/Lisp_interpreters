#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "token.hpp"

class Scanner {
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
    Scanner(const std::string &src);
    std::vector<Token> scan_tokens();
    void scan_token();
    inline bool is_at_end();
    inline char advance();
    void add_null_token(const Token_Type type);
    void add_token(const Token_Type type, const lit &literal);
    bool match(const char expected);
    char peek();
    char peek_next();
    void str();
    void number();
    void identifier();
};
