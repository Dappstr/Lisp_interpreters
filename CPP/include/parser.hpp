#pragma once

#include "token.hpp"
#include "ast.hpp"
#include <cstddef>
#include <memory>
#include <vector>

class Parser {
private:
    std::vector<Token> m_tokens{};
    size_t m_current = 0;

public:
    explicit Parser(const std::vector<Token> &tokens) : m_tokens(tokens) {}
    std::vector<AST_Node> parse();

private:
    AST_Node expression();
    AST_Node atom();
    std::shared_ptr<List_Node> list();
    const Token &advance();
    const Token &peek() const;
    const Token &previous() const;
    bool match(const Token_Type type);
    bool is_at_end() const;
    const Token &consume(const Token_Type type,
                         const std::string &error_message);
    
};
