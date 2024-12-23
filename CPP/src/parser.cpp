#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>
#include "../include/parser.hpp"
#include "../include/token.hpp"
#include "../include/ast.hpp"

std::vector<AST_Node> Parser::parse() & {
    std::vector<AST_Node> nodes;
    while (!is_at_end()) {
        nodes.emplace_back(expression());
    }
    return nodes; // Return all the nodes we've collected
}

AST_Node Parser::expression() & {
    if (match(LEFT_PAREN)) {
        return list();
    } else {
        return atom();
    }
}

AST_Node Parser::atom() & {
    if (match(NUMBER)) {
        if (const auto &literal = previous().literal(); literal.has_value()) {
            return Atom_Node(std::get<double>(literal.value()));
        } else {
            throw std::runtime_error("Expected a numeric literal, but none found.");
        }
    } else if (match(STRING)) {
        const auto &lit = previous().literal();
        if (lit.has_value()) {
            return Atom_Node(std::get<std::string>(lit.value()));
        } else {
            throw std::runtime_error("Expected string literal, but none found.");
        }
    } else if (match(IDENTIFIER) || match(PLUS) || match(MINUS) || match(STAR) || match(SLASH)) {  // Handle identifiers
        return Atom_Node{previous().lexeme(), true}; // We get the previous token's lexeme because `match()` advances if the match returns true
    } else {
        throw std::runtime_error("Expected an atom!");
    }
}

std::shared_ptr<List_Node> Parser::list() & {
    std::vector<AST_Node> elements; // Create a vector of possible expressions
    while (!is_at_end() && peek().type() != RIGHT_PAREN) {
        elements.emplace_back(expression());
    }
    if (!match(RIGHT_PAREN)) { // Once we break out of our initial while loop, we check for a right paren to match with our expression
        throw std::runtime_error("Unmatched \'(\'");
    }
    return std::make_shared<List_Node>(std::move(elements));
}

const Token &Parser::advance() & {
    if (!is_at_end()) {
        m_current++;
    }
    return previous();
}

const Token &Parser::peek() const& {
    return m_tokens.at(m_current);
}

const Token &Parser::previous() const& {
    return m_tokens.at(m_current - 1);
}

bool Parser::match(const Token_Type type) & {
    if (!is_at_end() && peek().type() == type) {
        advance();
        return true;
    }
    return false;
}

bool Parser::is_at_end() const& {
    return peek().type() == EndOfFile;
}

const Token &Parser::consume(const Token_Type type, const std::string &error_message) &{ // Will be used later for verifying token validation and advancing
    if (match(type)) {
        return previous();
    }
    throw std::runtime_error(error_message);
}
