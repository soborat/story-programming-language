#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include "SyntaxNodes/Node.h"
#include "Util.h"

std::vector<std::string> keywords = {"let", "say", "read", "if", "else", "for", "function", "call", "add", "subtract", "multiply", "divide"};
std::vector<std::string> ifOperators = {"is", "equals", "not-equals", "greater-than", "greater-equal", "lesser-than",  "lesser-equal", "divisible-by"};

std::string enumToString(NodeType value) {
    static std::unordered_map<NodeType, std::string> enumMap = {
            {ROOT, "ROOT"},
            {LET, "LET"},
            {FOR, "FOR"},
            {IF, "IF"},
            {ELSE, "ELSE"},
            {SAY, "SAY"},
            {READ, "READ"},
            {FUNC_DECL, "FUNC_DECL"},
            {FUNC_CALL, "FUNC_CALL"},
            {ADD, "ADD"},
            {SUB, "SUB"},
            {MUL, "MUL"},
            {DIV, "DIV"}
    };
    if(find(enumMap, value)) {
        return enumMap[value];
    }
    return "UNKNOWN";
}


std::unordered_map<std::string, int> generateNumbers() {
    std::unordered_map<std::string, int> m;
    std::ifstream file("numbers.txt");
    std::string letters, eq, number;

    while (file >> letters >> eq >> number) {
        m[letters] = stoi(number);
    }
    return m;
}

int getConstant(const std::string &number) {
    static std::unordered_map<std::string, int> allNumbers = generateNumbers();
    if (find(allNumbers, number)) {
        return allNumbers[number];
    }
    return -1;
}