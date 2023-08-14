#pragma once

#include <stdexcept>
#include <string>

class CustomException : public std::runtime_error {
public:
    explicit CustomException(const std::string &msg)
            : std::runtime_error(msg) {}
};

CustomException UndefinedVariableError(const std::string &str) {
    return CustomException("Variable " + str + " is not defined\n");
}

CustomException InvalidNumberError(const std::string &str) {
    return CustomException("Invalid number " + str + ", only 0-99 allowed, example: forty-five\n");
}

CustomException FuncDeclError() {
    return CustomException("Wrong function declaration, example: function Fn\n");
}

CustomException FuncCallError() {
    return CustomException("Wrong function call, example: call Fn\n");
}

CustomException FuncAlreadyDefError(const std::string &str) {
    return CustomException("Function " + str + " is already declared\n");
}
CustomException FucNotDefError(const std::string &str) {
    return CustomException("Function " + str + " is not defined\n");
}

CustomException UnknownKeywordError(const std::string &str) {
    return CustomException("Unknown keyword: " + str + '\n');
}

CustomException VarDeclError() {
    return CustomException("Wrong variable declaration, example: let A be five\n");
}

CustomException SayError() {
    return CustomException("Wrong say statement, example: say Number\n");
}

CustomException ReadError() {
    return CustomException("Wrong read statement, example: read Number\n");
}

CustomException IfError() {
    return CustomException("Wrong if statement, example: if Number greater-equal eight\n");
}

CustomException ElseError() {
    return CustomException("Else without previous if\n");
}

CustomException ForError() {
    return CustomException("Wrong for statement, example: for every Number from one to fifteen\n");
}

CustomException AdditionError() {
    return CustomException("Wrong addition, example: add four to Number\n");
}

CustomException SubtractionError() {
    return CustomException("Wrong subtraction, example: subtract four from Number");
}

CustomException IndentError() {
    return CustomException("Change code to Python-style 4 spaces indent\n");
}

CustomException OutOfBoundsValueError(int x) {
    return CustomException("Value " + std::to_string(x) + " is not in the range [0-99]\n");
}