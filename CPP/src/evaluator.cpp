#include "../include/evaluator.hpp"
#include "../include/ast.hpp"
#include "../include/environment.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

Value eval_atom(const Atom_Node &atom, const Environment &env) {
    if (atom.is_literal()) {
        return atom.value;
    } else if (atom.is_identifier()) {
        return env.get(atom.as_identifier());
    } else {
        throw std::runtime_error("Unknown atom type!\n");
    }
}

Value eval_list(const List_Node &list, Environment &env) {
    if (list.elements.empty()) {
        throw std::runtime_error("Cannot evaluate an empty list.");
    }

    // First element: function/operator
    const AST_Node &head = list.elements[0];
    Callable callable_func;

    if (std::holds_alternative<Atom_Node>(head)) {
        if (const auto &atom = std::get<Atom_Node>(head); atom.is_identifier()) {
            const std::string &func_name = atom.as_identifier();
            callable_func = env.get_callable(func_name);
        } else {
            throw std::runtime_error("Invalid function/operator in list.");
        }
    } else {
        throw std::runtime_error("The head of the list must be an identifier.");
    }

    // Rest of the elements: arguments
    std::vector<Value> args;
    for (size_t i = 1; i < list.elements.size(); ++i) {
        args.push_back(evaluate(list.elements[i], env));
    }

    // Call the function with the evaluated arguments
    return callable_func(args);
}

Value evaluate(const AST_Node &node, Environment &env) {
    if (std::holds_alternative<Atom_Node>(node)) {
        return eval_atom(std::get<Atom_Node>(node), env);
    } else if (std::holds_alternative<std::shared_ptr<List_Node>>(node)) {
        return eval_list(*std::get<std::shared_ptr<List_Node>>(node), env);
    } else {
      throw std::runtime_error("Unknown AST node type.");
    } 
}
