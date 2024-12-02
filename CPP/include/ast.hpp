#pragma once

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

struct Atom_Node; // An atom represents the most basic unit in a programming language
struct List_Node;

using AST_Node = std::variant<Atom_Node, std::shared_ptr<List_Node>>; // A node in an AST can be either a n atom or a list (like an expression)

// Define the atom which will either represent a literal or an identtifier which will combine an identifier/symbol to a value
struct Atom_Node {
    enum class Type {
        Literal,
        Identifier,
    };

    Type type;
    std::variant<std::string, double> value; // The actual value of the atom
    std::optional<std::string> identifier; // An optional member for the cases when we declare/define a variable
    
    Atom_Node(const double d) : type(Type::Literal), value(d) {} // Constructing a basic number literal
    Atom_Node(const std::string &str, bool is_identifier = false)
        : type(is_identifier ? Type::Identifier : Type::Literal), value(str) {
        if (is_identifier) {
            identifier = str;
        }
    }

    bool is_literal() const { return type == Type::Literal; }
    bool is_identifier() const { return type == Type::Identifier; }

    // Utility functions to check that an atom has a certain value or is an identifier
    std::string as_identifier() const {
        if (type != Type::Identifier ||
            !identifier.has_value()) {
            throw std::runtime_error("Atom_Node does not hold an identifier!");
        }
        return *identifier;
    }
    double as_number() const {
        if (type != Type::Literal ||
            !std::holds_alternative<double>(value)) {
            throw std::runtime_error("Atom_Node does not hold a number!");
        }
        return std::get<double>(value);
    }
    std::string as_string() const {
        if (type != Type::Literal ||
            !std::holds_alternative<std::string>(value)) {
            throw std::runtime_error("Atom_Node does not hold a string!");
        }
        return std::get<std::string>(value);
    }
};

// A list node can represent an expression which is a collection atoms or other subexpressions
struct List_Node {
    std::vector<AST_Node> elements;

    explicit List_Node(std::vector<AST_Node> &&elems)
        : elements(std::move(elems)) {}
    explicit List_Node(const std::vector<AST_Node> &elems)
        : elements(elems) {}
};

void print_ast(const AST_Node &, const int);
void print_indent(const int);
void print_atom(const Atom_Node &, int indent);
void print_list(const List_Node&, int indent);

