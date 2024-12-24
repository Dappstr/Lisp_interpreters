#include "../include/environment.hpp"
#include "../include/ast.hpp"
#include <memory>
#include <stdexcept>
#include <variant>
#include <vector>

Environment::Environment(std::shared_ptr<Environment> parent)
    : m_parent(std::move(parent)) {}

Value Environment::get(const std::string &name) const {
    if (const auto it = m_variables.find(name); it != m_variables.end()) {
        return it->second;
    } else if (m_parent) {
        return m_parent->get(name);
    } else {
        throw std::runtime_error("Error, undefined symbol: " + name);
    }
}

void Environment::define(const std::string &name, const Value &value) {
    m_variables[name] = value;
}

void Environment::define_builtin(
    const std::string &name,
    std::function<Value(const std::vector<Value> &)> func) {
    m_builtins[name] = std::move(func);
}

void Environment::define_function(const std::string &name, const User_Function &func) {
    m_user_functions.emplace(name, func);
}

User_Function Environment::get_function(const std::string &name) const {
    if (const auto it = m_user_functions.find(name); it != m_user_functions.end()) {
        return it->second;
    } else if (m_parent) {
        return m_parent->get_function(name);
    } else {
        throw std::runtime_error("Undefined function: " + name);
    }
}

Callable Environment::get_callable(const std::string &name) const {
    // Check for a built-in function
    if (const auto builtin_it = m_builtins.find(name); builtin_it != m_builtins.end()) {
        return builtin_it->second;
    }

    // Check for a user-defined function or callable in the environment
    Value value = get(name);

    // If the value isn't callable, throw an error
    throw std::runtime_error("Symbol '" + name + "' is not callable.");
}
