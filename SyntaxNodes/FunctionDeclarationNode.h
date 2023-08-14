#pragma once

#include <string>
#include "Node.h"
#include "../Exceptions.h"


class FunctionDeclarationNode : public Node {
public:
    std::string name;

    explicit FunctionDeclarationNode(const std::vector<std::string> &words) {
        if (words.size() != 2 || std::islower(words[1][0])) {
            throw FuncDeclError();
        }
        nodeType = FUNC_DECL;
        name = words[1];
    }
};