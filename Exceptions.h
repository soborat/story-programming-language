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
    return CustomException("Wrong variable declaration, example: let N be five\n");
}

CustomException SayError() {
    return CustomException("Wrong say statement, example: say N\n");
}

CustomException ReadError() {
    return CustomException("Wrong read statement, example: read N\n");
}

CustomException IfError() {
    return CustomException("Wrong if statement, example: if N greater-equal eight\n");
}

CustomException ElseError() {
    return CustomException("Else without previous if\n");
}

CustomException ForError() {
    return CustomException("Wrong for statement, example: for every N from one to fifteen\n");
}

CustomException AdditionError() {
    return CustomException("Wrong addition, example: add four to Number\n");
}

CustomException SubtractionError() {
    return CustomException("Wrong subtraction, example: subtract four from N");
}

CustomException MultiplicationError() {
    return CustomException("Wrong multiplication, example: multiply N by four");
}

CustomException DivisionError() {
    return CustomException("Wrong division, example: divide N by four");
}

CustomException DivisionByZeroError() {
    return CustomException("Wrong multiplication, example: multiply N by four");
}

CustomException IndentError() {
    return CustomException("Indents must be a multiple of 4\n");
}

CustomException OutOfBoundsValueError(int x) {
    return CustomException("Value " + std::to_string(x) + " is not in the range [0-99]\n");
}