#include "../include/environment.hpp"

#include <evaluator.hpp>

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
    // Check for a built-in function in the current environment
    if (const auto builtin_it = m_builtins.find(name); builtin_it != m_builtins.end()) {
        return builtin_it->second;
    }
    if (const auto uf_it = m_user_functions.find(name); uf_it != m_user_functions.end()) {
        const User_Function &func = uf_it->second;
        return [func](const std::vector<Value> &args) -> Value {
            Environment fn_env(func.closure);

            if (args.size() != func.args.size()) {
                throw std::runtime_error("Arity error: function '" + func.name + "' expects "
                                         + std::to_string(func.args.size()) + " args, got "
                                         + std::to_string(args.size()));
            }
            for (size_t i = 0; i < args.size(); ++i) {
                fn_env.define(func.args[i], args[i]);
            }
            return evaluate(func.body, fn_env);
        };
    }
    // If not found, try to locate it in the parent function
    if (m_parent) {
        return m_parent->get_callable(name);
    }
    throw std::runtime_error("Symbol '" + name + "' is not callable.");
}

