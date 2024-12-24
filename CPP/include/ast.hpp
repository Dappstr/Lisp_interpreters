#pragma once

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

struct Atom_Node; // An atom represents the most basic unit in a programming language
struct List_Node;

using AST_Node = std::variant<Atom_Node, std::shared_ptr<List_Node>>; // A node in an AST can be either an atom or a list (like an expression)

using Value = std::variant<double, std::string, std::shared_ptr<List_Node>>;

// Define the atom which will either represent a literal or an identifier which will combine an identifier/symbol to a value
struct Atom_Node {
    enum class Type {
        Literal,
        Identifier,
    };

    Type type;
    //std::variant<std::string, double> value; // The actual value of the atom
    Value value;
    
    std::optional<std::string> identifier; // An optional member for the cases when we declare/define a variable

    explicit Atom_Node(const double d) : type(Type::Literal), value(d) {} // Constructing a basic number literal
    explicit Atom_Node(const std::string &str, const bool is_identifier = false)
        : type(is_identifier ? Type::Identifier : Type::Literal), value(str) { // Essentially, check if the constructor argument is supplied, if it is then it's an identifier
        if (is_identifier) {
            identifier = str;
        }
    }

    [[nodiscard]] bool is_literal() const { return type == Type::Literal; }
    [[nodiscard]] bool is_identifier() const { return type == Type::Identifier; }

    // Utility functions to check that an atom has a certain value or is an identifier
    [[nodiscard]] std::string as_identifier() const {
        if (type != Type::Identifier ||
            !identifier.has_value()) {
            throw std::runtime_error("Atom_Node does not hold an identifier!");
        }
        return *identifier; // In order to get the value of an optional, you need to dereference it
    }
    [[nodiscard]] double as_number() const {
        if (type != Type::Literal ||
            !std::holds_alternative<double>(value)) {
            throw std::runtime_error("Atom_Node does not hold a number!");
        }
        return std::get<double>(value);
    }
    [[nodiscard]] std::string as_string() const {
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

void print_ast(const AST_Node &, int);
void print_indent(int);
void print_atom(const Atom_Node &, int indent);
void print_list(const List_Node&, int indent);
