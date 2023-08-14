#pragma once

#include <string>
#include "Node.h"
#include "../Exceptions.h"

class MultiplicationNode : public Node {
public:
    std::string receiver;
    std::string value;

    explicit MultiplicationNode(const std::vector<std::string> &words) {
        if (words.size() != 4 || words[2] != "by" || std::islower(words[1][0])) {
            throw MultiplicationError();
        }
        nodeType = MUL;
        value = words[3];
        receiver = words[1];
    }

};