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
    if (match(DEF)) { // Variable definition
        const auto name = consume(IDENTIFIER, "Expected variable name after \"def\"");
        auto value = expression();

        // Ensure each element is explicitly constructed
        AST_Node def_node = Atom_Node("def", true); // Mark "def" as an identifier
        AST_Node name_node = Atom_Node(name.lexeme(), true); // Variable name as an identifier

        std::vector elements = {
            std::move(def_node),
            std::move(name_node),
            std::move(value)
        };
        return std::make_shared<List_Node>(std::move(elements));
    } else if (match(FN)) {
        const auto fn_name_token = consume(IDENTIFIER, "Expected function name after 'fn'");

        // 2) Parse the parameter list
        (void) consume(LEFT_PAREN, "Expected '(' before argument list.");
        std::vector<AST_Node> arg_nodes;
        while (!is_at_end() && peek().type() != RIGHT_PAREN) {
            const Token &arg_token = consume(IDENTIFIER, "Expected identifier in function parameter list");
            arg_nodes.emplace_back(Atom_Node(arg_token.lexeme(), true));
        }
        (void)consume(RIGHT_PAREN, "Expected ')' after argument list.");

        // 3) Parse the body
        AST_Node body_expr = expression(); // single expression as the body

        // 4) Build AST
        std::vector<AST_Node> elements;
        elements.emplace_back(Atom_Node("fn", true));
        elements.emplace_back(Atom_Node(fn_name_token.lexeme(), true));
        auto arg_list = std::make_shared<List_Node>(std::move(arg_nodes));
        elements.emplace_back(arg_list);
        elements.emplace_back(std::move(body_expr));

        return std::make_shared<List_Node>(std::move(elements));
    } else if (match(LEFT_PAREN)) {
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
        if (const auto &lit = previous().literal(); lit.has_value()) {
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
    std::vector<AST_Node> elements;

    if (!is_at_end() && peek().type() == DEF) { // Special form: `def`
        advance(); // Consume `def`
        const auto name = consume(IDENTIFIER, "Expected variable name after \"def\"");
        auto value = expression();

        elements.emplace_back(Atom_Node("def", true)); // Special form
        elements.emplace_back(Atom_Node(name.lexeme(), true)); // Variable name
        elements.emplace_back(std::move(value)); // Value expression

        if (!match(RIGHT_PAREN)) {
            throw std::runtime_error("Expected ')' after def expression.");
        }
        return std::make_shared<List_Node>(std::move(elements));
    } else if (!is_at_end() && peek().type() == FN) {
        advance(); // Consume 'fn'
        // parse function name
        const auto fn_name_token = consume(IDENTIFIER, "Expected function name after 'fn'");

        // parse the '(' that starts the argument list
        (void) consume(LEFT_PAREN, "Expected '(' before argument list.");

        // gather all identifiers up to ')'
        std::vector<AST_Node> arg_nodes;
        while (!is_at_end() && peek().type() != RIGHT_PAREN) {
            const Token &arg_token = consume(IDENTIFIER, "Expected identifier in function parameter list");
            arg_nodes.emplace_back(Atom_Node(arg_token.lexeme(), true));
        }
        (void) consume(RIGHT_PAREN, "Expected ')' after argument list.");

        // parse the body expression
        AST_Node body_expr = expression();

        // Build up the elements that represent (fn NAME (args...) body)
        elements.emplace_back(Atom_Node("fn", true));
        elements.emplace_back(Atom_Node(fn_name_token.lexeme(), true));
        auto args_list = std::make_shared<List_Node>(std::move(arg_nodes));
        elements.emplace_back(args_list);
        elements.emplace_back(std::move(body_expr));

        if (!match(RIGHT_PAREN)) {
            throw std::runtime_error("Expected ')' after fn expression.");
        }
        return std::make_shared<List_Node>(std::move(elements));
    }
    // Handle generic lists
    while (!is_at_end() && peek().type() != RIGHT_PAREN) {
        elements.emplace_back(expression());
    }
    if (!match(RIGHT_PAREN)) {
        throw std::runtime_error("Unmatched '('");
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

const Token &Parser::consume(const Token_Type type, const std::string &error_message) & { // Will be used later for verifying token validation and advancing
    if (match(type)) {
        return previous();
    }
    throw std::runtime_error(error_message);
}
