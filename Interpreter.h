#ifndef INTERPRETER
#define INTERPRETER

#include <iostream>
#include <unordered_map>
#include <string>
#include "Parser.h"


class Interpreter {
    static std::unordered_map<std::string, int> variables;
    static std::unordered_map<std::string, Node*> functions;

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
            if (getNumber(variableName) != -1) {
                std::cout << getNumber(variableName) << '\n';
            } else if (variables.find(variableName) != variables.end()) {
                std::cout << variables[variableName] << '\n';
            } else {
                throw std::runtime_error("Variable " + variableName + " is not defined\n");
            }
        }
        if (node->nodeType == READ) {
            auto *readNode = dynamic_cast<ReadNode*>(node);
            std::string variableName = readNode->variable;
            std::string str;
            std::cin >> str;
            if(getNumber(str) == -1) {
                throw std::runtime_error("Invalid number, only 0-99 allowed, example: forty-five\n");
            }
            variables[variableName] = getNumber(str);
        }
        if (node->nodeType == IF) {
            auto *ifNode = dynamic_cast<IfNode *>(node);
            std::string variableName = ifNode->variable;
            int value;
            if (getNumber(variableName) != -1) {
                value = getNumber(variableName);
            } else if (variables.find(variableName) != variables.end()) {
                value =  variables[variableName];
            } else {
                throw std::runtime_error("Variable " + variableName + " is not defined\n");
            }
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
            if(functions.find(functionName) != functions.end()) {
                throw std::runtime_error("Function " + functionName + " is already declared\n");
            }
            functions[funcDeclNode->name] = node;
            node->nodeType = ROOT;
        }
        if(node->nodeType == FUNC_CALL) {
            auto *funcCallNode = dynamic_cast<FunctionCallNode*>(node);
            std::string functionName = funcCallNode->name;
            if(functions.find(functionName) == functions.end()) {
                throw std::runtime_error("Function " + functionName + " is not defined\n");
            }
            eval(functions[functionName]);
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
#endif
