#pragma once

#include <string>
#include "Node.h"
#include "../Exceptions.h"

class SubtractionNode : public Node {
public:
    std::string receiver;
    std::string value;

    explicit SubtractionNode(const std::vector<std::string> &words) {
        if (words.size() != 4 || words[2] != "from" || std::islower(words[3][0])) {
            throw SubtractionError();
        }
        nodeType = SUB;
        value = words[1];
        receiver = words[3];
    }
};