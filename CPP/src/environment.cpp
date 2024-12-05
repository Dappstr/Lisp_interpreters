#include "../include/environment.hpp"
#include <memory>
#include <stdexcept>

Environment::Environment(std::shared_ptr<Environment> parent)
    : m_parent(std::move(parent)) {}

Value Environment::get(const std::string &name) const {
    auto it = m_variables.find(name);
    if (it != m_variables.end()) {
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

