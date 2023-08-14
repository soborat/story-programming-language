#pragma once

#include <string>
#include "Node.h"
#include "../Exceptions.h"

class SayNode : public Node {
public:
    std::string variable;

    explicit SayNode(const std::vector<std::string> &words) {
        if (words.size() != 2) {
            throw SayError();
        }
        nodeType = SAY;
        variable = words[1];
    }
};