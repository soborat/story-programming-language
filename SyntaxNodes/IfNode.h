#pragma once

#include <string>
#include "Node.h"
#include "../Exceptions.h"
#include "../Constants.h"

class IfNode : public Node {
public:
    std::string variable;
    std::string ifOperator;
    std::string operand;
    std::vector<Node *> *elseBody;

    explicit IfNode(const std::vector<std::string> &words) {
        if (words.size() != 4 || not_find(ifOperators, words[2])) {
            throw IfError();
        }
        if (words[2] == "is" && not_find({"even", "odd"}, words[3])) {
            throw IfError();
        }
        nodeType = IF;
        variable = words[1];
        ifOperator = words[2];
        operand = words[3];
        elseBody = nullptr;
    }

    void setElseBody(std::vector<Node *> &nodes) {
        elseBody = &nodes;
    }

};