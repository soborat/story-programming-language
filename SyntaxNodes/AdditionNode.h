#pragma once

#include <string>
#include "Node.h"
#include "../Exceptions.h"

class AdditionNode : public Node {
public:
    std::string receiver;
    std::string value;

    explicit AdditionNode(const std::vector<std::string> &words) {
        if (words.size() != 4 || words[2] != "to" || std::islower(words[3][0])) {
            throw AdditionError();
        }
        nodeType = ADD;
        value = words[1];
        receiver = words[3];
    }
};