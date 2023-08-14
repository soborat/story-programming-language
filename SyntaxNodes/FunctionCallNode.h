#pragma once

#include <string>
#include "Node.h"
#include "../Exceptions.h"

class FunctionCallNode : public Node {
public:
    std::string name;

    explicit FunctionCallNode(const std::vector<std::string> &words) {
        if (words.size() != 2 || std::islower(words[1][0])) {
            throw FuncCallError();
        }
        nodeType = FUNC_CALL;
        name = words[1];
    }
};