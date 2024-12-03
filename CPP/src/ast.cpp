#include "../include/ast.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <variant>

void print_ast(const AST_Node &node, const int indent) {
    if (std::holds_alternative<Atom_Node>(node)) {
        print_atom(std::get<Atom_Node>(node), indent);
    } else if (std::holds_alternative<std::shared_ptr<List_Node>>(node)) {
        print_list(*std::get<std::shared_ptr<List_Node>>(node), indent); // We dereference because print_list expects a reference, not a (shared) pointer.
    }
}

void print_indent(const int indent) {
    for (int i = 0; i < indent; ++i) {
        std::cout << ' ';  
    }
}

void print_atom(const Atom_Node &atom, int indent) {
    print_indent(indent);
    if (std::holds_alternative<double>(atom.value)) {
        std::cout << std::get<double>(atom.value) << '\n';
    } else if (std::holds_alternative<std::string>(atom.value)) {
        std::cout << std::get<std::string>(atom.value) << '\n';
    }
}

void print_list(const List_Node &list, int indent) {
    print_indent(indent);
    std::cout << "(\n";

    for (const auto &element : list.elements) {
        print_ast(element, indent + 2);
    }

    print_indent(indent);
    std::cout << ")\n";
}
