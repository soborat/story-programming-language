#pragma once

#include <string>
#include <vector>
#include "Node.h"
#include "../Exceptions.h"

class DivisionNode : public Node {
public:
    std::string receiver;
    std::string value;

    explicit DivisionNode(const std::vector<std::string> &words) {
        if (words.size() != 4 || words[2] != "by" || std::islower(words[1][0])) {
            throw DivisionError();
        }
        nodeType = DIV;
        value = words[3];
        receiver = words[1];
    }
};