#pragma once

#include <string>
#include "Node.h"
#include "../Exceptions.h"

class VariableNode : public Node {
public:
    std::string name;
    std::string value;

    explicit VariableNode(const std::vector<std::string> &words) {
        if (words.size() == 2 && std::isupper(words[1][0])) {
            nodeType = LET;
            name = words[1];
            value = "zero";
        } else if (words.size() == 4 && std::isupper(words[1][0]) && words[2] == "be") {
            nodeType = LET;
            name = words[1];
            value = words[3];
        } else {
            throw VarDeclError();
        }
    }
};