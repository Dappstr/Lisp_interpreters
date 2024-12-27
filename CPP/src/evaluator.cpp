#include "../include/evaluator.hpp"
#include "../include/ast.hpp"
#include "../include/environment.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>
#include <iostream>

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
    if (const AST_Node &head = list.elements[0]; std::holds_alternative<Atom_Node>(head)) {
        const auto &atom = std::get<Atom_Node>(head);
        if (atom.is_identifier()) {
            const std::string &func_name = atom.as_identifier();
            if (func_name == "def") {
                if (list.elements.size() != 3) {
                    throw std::runtime_error("'def' expects exactly 2 arguments: name and value.");
                }
                // Get the variable name
                const AST_Node &name_node = list.elements[1];
                if (!std::holds_alternative<Atom_Node>(name_node) ||
                    !std::get<Atom_Node>(name_node).is_identifier()) {
                    throw std::runtime_error("The first argument to 'def' must be an identifier.");
                    }
                const std::string &var_name = std::get<Atom_Node>(name_node).as_identifier();
                const AST_Node &value_node = list.elements[2];
                const Value value = evaluate(value_node, env);
                env.define(var_name, value);
                return "nil";
            } else if (func_name == "fn") {
                if (list.elements.size() != 4) { // 4 because of "fn"
                    throw std::runtime_error("'fn' expects exactly 3 arguments: name and value.");
                }
                const AST_Node &name_node = list.elements[1];
                if (!std::holds_alternative<Atom_Node>(list.elements[1]) || !std::get<Atom_Node>(list.elements[1]).is_identifier()) {
                    throw std::runtime_error("The second element of 'fn' must be an identifier.");
                }
                std::string function_name = std::get<Atom_Node>(name_node).as_identifier();
                const auto &args_node = list.elements[2];
                if (!std::holds_alternative<std::shared_ptr<List_Node>>(args_node)) {
                    throw std::runtime_error("The third element of 'fn' must be a list of arguments.");
                }
                auto arg_list = std::get<std::shared_ptr<List_Node>>(args_node);
                std::vector<std::string> args;
                for (auto &arg_ast : arg_list->elements) {
                    if (!std::holds_alternative<Atom_Node>(arg_ast)) {
                        throw std::runtime_error("Function arguments must be identifiers.");
                    }
                    const auto &arg_atom = std::get<Atom_Node>(arg_ast);
                    if (!arg_atom.is_identifier()) {
                        throw std::runtime_error("Function arguments must be identifiers.");
                    }
                    args.push_back(arg_atom.as_identifier());
                }
                const auto &body_node = list.elements[3];

                const User_Function fn {.name = function_name, .args = args, .body = body_node, .closure = std::make_shared<Environment>(env)};
                env.define_function(function_name, fn);
                return "nil";
            } else {
                const Callable callable = env.get_callable(func_name);
                std::vector<Value> args;
                for (size_t i = 1; i < list.elements.size(); ++i) {
                    args.push_back(evaluate(list.elements[i], env));
                }
                return callable(args);
            }
        }
    }
    throw std::runtime_error("The head of the list must be an identifier.");
}

Value evaluate(const AST_Node &node, Environment &env) {
    if (std::holds_alternative<Atom_Node>(node)) {
        return eval_atom(std::get<Atom_Node>(node), env);
    } else if (std::holds_alternative<std::shared_ptr<List_Node>>(node)) {

        // Debug the first element of the list
        if (const auto &list = *std::get<std::shared_ptr<List_Node>>(node); !list.elements.empty()) {
            if (const auto &head = list.elements[0]; std::holds_alternative<Atom_Node>(head)) {
                const auto &atom = std::get<Atom_Node>(head);
            }
        }
        return eval_list(*std::get<std::shared_ptr<List_Node>>(node), env);
    } else {
        throw std::runtime_error("Unknown AST node type.");
    }
}
