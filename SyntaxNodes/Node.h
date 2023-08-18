#pragma once

#include <vector>
#include "../Exceptions.h"

enum NodeType {
    ROOT, LET, FOR, IF, ELSE, SAY, READ, FUNC_DECL, FUNC_CALL, ADD, SUB, MUL, DIV
};

enum IfOperator {
    IS,
    EQUALS,
    NOT_EQUALS,
    GREATER_THAN,
    GREATER_EQUAL,
    LESSER_THAN,
    LESSER_EQUAL,
    DIVISIBLE_BY,
};

enum IfOperand {
    EVEN,
    ODD
};

class Node {
public:
    NodeType nodeType;
    std::vector<Node *> body;

    Node() : nodeType(ROOT) {}

    explicit Node(NodeType nodeType) : nodeType(nodeType) {}

    virtual ~Node() = default;

    void add(Node *node) {
        body.push_back(node);
    }
};