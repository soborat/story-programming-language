#pragma once

#include <stdexcept>
#include <string>

class custom_exception : public std::runtime_error {
public:
    explicit custom_exception(const std::string &msg)
            : std::runtime_error(msg) {}
};

custom_exception undefined_variable(const std::string &str) {
    return custom_exception("Variable " + str + " is not defined\n");
}

custom_exception invalid_number(const std::string &str) {
    return custom_exception("Invalid number " + str + ", only 0-99 allowed, example: forty-five\n");
}

custom_exception func_already_def(const std::string &str) {
    return custom_exception("Function " + str + " is already declared\n");
}
custom_exception fuc_not_def(const std::string &str) {
    return custom_exception("Function " + str + " is not defined\n");
}

custom_exception unknown_keyword(const std::string &str) {
    return custom_exception("Unknown keyword: " + str + '\n');
}

custom_exception var_decl() {
    return custom_exception("Wrong variable declaration, example: let A be five\n");
}

