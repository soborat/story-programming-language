#pragma once

#include "Node.h"
#include "../Exceptions.h"

class ElseNode : public Node {
public:
    explicit ElseNode(Node *node) {
        if (node->nodeType != IF) {
            throw ElseError();
        }
        nodeType = ELSE;
    }
};