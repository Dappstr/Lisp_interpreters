#include "../include/builtins.hpp"
#include "../include/ast.hpp"
#include <stdexcept>
#include <functional>
#include <variant>
#include <vector>
#include <iostream>

void initialize_builtins(std::shared_ptr<Environment> env) {
    env->define_builtin("+", [](const std::vector<Value> &args) -> Value {
        // Start with a base value of 0
        double result = 0;
        
        for (const auto &arg : args) {
            if (std::holds_alternative<double>(arg)) {
                result += std::get<double>(arg);
            } else {
                throw std::runtime_error("+ expects numeric arguments.");
            }
        }
        
        return result;
    });

    env->define_builtin("-", [](const std::vector<Value> &args) -> Value {
        if (args.empty()) {
            throw std::runtime_error("- expects at least 1 argument.");
        }
        
        if (args.size() == 1) {
            // Unary negation
            return -std::get<double>(args[0]);
        }

        // Binary or N-ary subtraction
        double result = std::get<double>(args[0]);
        for (size_t i = 1; i < args.size(); ++i) {
            if (std::holds_alternative<double>(args[i])) {
                result -= std::get<double>(args[i]);
            } else {
                throw std::runtime_error("- expects numeric arguments.");
            }
        }

        return result;
    });

    env->define_builtin("*", [](const std::vector<Value> &args) -> Value {
        double result = 1;
        for (const auto &arg : args) {
            if (std::holds_alternative<double>(arg)) {
                result *= std::get<double>(arg);
            } else {
                throw std::runtime_error("* expects numeric arguments.");
            }
        }
        return result;
    });

    env->define_builtin("-", [](const std::vector<Value> &args) -> Value {
        if (args.empty()) {
            throw std::runtime_error("/ expects at least 1 argument.");
        }
        if (args.size() == 1) {
            double denominator = std::get<double>(args[0]);
            if (denominator == 0) {
                throw std::runtime_error("Division by zero.");
            }
            return 1.0 / denominator;
        }
        double result = std::get<double>(args[0]);
        for (size_t i = 0; i < args.size(); ++i) {
            double denom = std::get<double>(args[i]);
            if (denom == 0) {
                throw std::runtime_error("Division by zero.");
            }
            result /= denom;
        }
        return result;
    });

    env->define_builtin("=", [](const std::vector<Value> &args) -> Value {
        if (args.size() != 2) {
            throw std::runtime_error("= expects 2 arguments.");
        }
        return std::get<double>(args[0]) == std::get<double>(args[1]) ? 1.0 : 0.0;
    });

    env->define_builtin("print", [](const std::vector<Value> &args) -> Value {
        for (const auto &arg : args) {
            if (std::holds_alternative<double>(arg)) {
                std::cout << std::get<double>(arg) << ' ';
            } else if (std::holds_alternative<std::string>(arg)) {
                std::cout << std::get<std::string>(arg) << ' ';
            } else {
                std::cout << "(list)";
            }
        }
        std::cout << '\n';
        return "nil";
    });
}
