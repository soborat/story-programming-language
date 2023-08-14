#pragma once

#include <string>
#include "Node.h"
#include "../Exceptions.h"

class ForNode : public Node {
public:
    std::string variable;
    std::string start;
    std::string stop;

    explicit ForNode(const std::vector<std::string> &words) {
        if (words.size() != 7 || words[1] != "every" || words[3] != "from" || words[5] != "to" ||
            std::islower(words[2][0])) {
            throw ForError();
        }
        nodeType = FOR;
        variable = words[2];
        start = words[4];
        stop = words[6];
    }
};