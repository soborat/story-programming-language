#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include "Parser.h"
#include "exceptions.h"


class Interpreter {
    static std::unordered_map<std::string, int> variables;
    static std::unordered_map<std::string, Node*> functions;

    static int getValue(const std::string& str) {
        if (getNumber(str) != -1) {
            return getNumber(str);
        } else if (find(variables, str)) {
            return variables[str];
        } else {
            throw undefined_variable(str);
        }
    }

    static void eval(Node *node) {
        if (node->nodeType == ROOT) {
            for (Node *n: node->body) {
                eval(n);
            }
        }
        if (node->nodeType == LET) {
            auto *variableNode = dynamic_cast<VariableNode *>(node);
            variables[variableNode->name] = variableNode->value;
        }
        if (node->nodeType == SAY) {
            auto *sayNode = dynamic_cast<SayNode *>(node);
            std::string variableName = sayNode->variable;
            std::cout << getValue(variableName) << '\n';
        }
        if (node->nodeType == READ) {
            auto *readNode = dynamic_cast<ReadNode*>(node);
            std::string variableName = readNode->variable;
            std::string str;
            std::cin >> str;
            if(getNumber(str) == -1) {
                throw invalid_number(str);
            }
            variables[variableName] = getNumber(str);
        }
        if (node->nodeType == IF) {
            auto *ifNode = dynamic_cast<IfNode *>(node);
            std::string variableName = ifNode->variable;
            int value = getValue(variableName);
            bool condition;
            if (ifNode->condition == "odd") {
                condition = value % 2 == 1;
            } else {
                condition = value % 2 == 0;
            }
            if (condition) {
                for (Node *n: node->body) {
                    eval(n);
                }
            }
            else if (!ifNode->elseBody->empty()) {
                for(Node *n: *ifNode->elseBody) {
                    eval(n);
                }
            }
        }
        if (node->nodeType == FOR) {
            auto *forNode = dynamic_cast<ForNode *>(node);
            if (forNode->start <= forNode->stop) {
                for (int i = forNode->start; i <= forNode->stop; i++) {
                    variables[forNode->variable] = i;
                    for (Node *n: node->body) {
                        eval(n);
                    }
                }
            } else {
                for (int i = forNode->start; i >= forNode->stop; i--) {
                    variables[forNode->variable] = i;
                    for (Node *n: node->body) {
                        eval(n);
                    }
                }
            }
        }
        if(node->nodeType == FUNC_DECL) {
            auto *funcDeclNode = dynamic_cast<FunctionDeclarationNode*>(node);
            std::string functionName = funcDeclNode->name;
            if(find(functions, functionName)) {
                throw func_already_def(functionName);
            }
            functions[funcDeclNode->name] = node;
            node->nodeType = ROOT;
        }
        if(node->nodeType == FUNC_CALL) {
            auto *funcCallNode = dynamic_cast<FunctionCallNode*>(node);
            std::string functionName = funcCallNode->name;
            if(not_find(functions, functionName)) {
                throw fuc_not_def(functionName);
            }
            eval(functions[functionName]);
        }
        if(node->nodeType == ADD) {
            auto *additionNode = dynamic_cast<AdditionNode*>(node);
            std::string value = additionNode->value;
            std::string receiver = additionNode->receiver;
            int extraValue = getValue(value);
            if(not_find(variables, receiver)) {
                throw undefined_variable(receiver);
            }
            variables[receiver] += extraValue;
        }
    }

    static void deleteNode(Node *root) {
        for (Node *node: root->body) {
            deleteNode(node);
        }
        delete root;
    }

    static void releaseMemory(Node *node) {
        variables.clear();
        functions.clear();
        deleteNode(node);
    }

public:
    static void debug(Node *root) {
        std::cout << enumToString(root->nodeType);
        std::cout << "(";
        for (Node *node: root->body) {
            debug(node);
        }
        std::cout << ")";
        // ROOT(LET()FOR(IF(SAY())IF(SAY())FOR(FOR(SAY()))))
    }
    // todo: add else, math, strings, ><==!=, break into files
    // todo: multiple param?
    // todo: comment last //
    static void run(Node *node) {
        eval(node);
        releaseMemory(node);
    }
};

std::unordered_map<std::string, int> Interpreter::variables;
std::unordered_map<std::string, Node*> Interpreter::functions;
