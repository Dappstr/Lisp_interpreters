#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <variant>
#include "ast.hpp"

// We also need a shared_ptr for list node to represent a variable containing a list
using Value = std::variant<double, std::string, std::shared_ptr<List_Node>>;

// An environment mostly just represents a scope
class Environment {
private:
    std::unordered_map<std::string, Value> m_variables{};
    std::shared_ptr<Environment> m_parent;
public:
    explicit Environment(std::shared_ptr<Environment> parent = nullptr);
    Value get(const std::string &name) const;
    void define(const std::string& name, const Value &val);
};
