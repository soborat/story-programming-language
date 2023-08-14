#pragma once

#include <string>
#include "Node.h"
#include "../Exceptions.h"

class ReadNode : public Node {
public:
    std::string variable;

    explicit ReadNode(const std::vector<std::string> &words) {
        if (words.size() != 2) {
            throw ReadError();
        }
        nodeType = READ;
        variable = words[1];
    }
};