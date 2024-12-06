#include "../include/builtins.hpp"
#include <stdexcept>
#include <functional>

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
}
