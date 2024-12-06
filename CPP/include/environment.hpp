#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <string>
#include <variant>
#include <vector>
#include "ast.hpp"

// We also need a shared_ptr for list node to represent a variable containing a list
//using Value = std::variant<double, std::string, std::shared_ptr<List_Node>>;

using Callable = std::function<Value(const std::vector<Value> &)>;

// An environment mostly just represents a scope
class Environment {
private:
    std::unordered_map<std::string, Value> m_variables{};
    std::unordered_map<std::string, Callable> m_builtins;
    std::shared_ptr<Environment> m_parent;
public:
    explicit Environment(std::shared_ptr<Environment> parent = nullptr);
    Value get(const std::string &name) const;
    void define(const std::string& name, const Value &val);

    // For defining built in functions like "+" or "-"
    void define_builtin(const std::string &name,
                        std::function<Value(const std::vector<Value> &)> func);

    // Will retrieve the implementation of a built-in function by the name
    Callable get_callable(const std::string &name) const;
    
};

