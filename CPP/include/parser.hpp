#pragma once

#include "token.hpp"
#include "ast.hpp"
#include <cstddef>
#include <memory>
#include <vector>

class Parser final {
private:
    std::vector<Token> m_tokens{};
    size_t m_current = 0;

public:
    explicit Parser(std::vector<Token> &&tokens) : m_tokens(tokens) {}
    [[nodiscard]] std::vector<AST_Node> parse() &;

private:
    [[nodiscard]] AST_Node expression() &;
    [[nodiscard]] AST_Node atom() &;
    [[nodiscard]] std::shared_ptr<List_Node> list() &;
    const Token &advance() &;
    [[nodiscard]] const Token &peek() const&;
    [[nodiscard]] const Token &previous() const&;
    [[nodiscard]] bool match(Token_Type type) &;
    [[nodiscard]] bool is_at_end() const&;
    [[nodiscard]] const Token &consume(Token_Type type,
                                       const std::string &error_message) &;
    
};
