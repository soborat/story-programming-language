#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include "Parser.h"
#include "exceptions.h"


class Interpreter {
    static std::unordered_map<std::string, int> variables;
    static std::unordered_map<std::string, Node*> functions;

    static int getVariableOrConstant(const std::string& str) {
        if (getConstant(str) != -1) {
            return getConstant(str);
        } else if (find(variables, str)) {
            return variables[str];
        } else {
            throw UndefinedVariableError(str);
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
            std::cout << getVariableOrConstant(variableName) << '\n';
        }
        if (node->nodeType == READ) {
            auto *readNode = dynamic_cast<ReadNode*>(node);
            std::string variableName = readNode->variable;
            std::string str;
            std::cin >> str;
            if(getConstant(str) == -1) {
                throw InvalidNumberError(str);
            }
            variables[variableName] = getConstant(str);
        }
        if (node->nodeType == IF) {
            auto *ifNode = dynamic_cast<IfNode *>(node);
            std::string variableName = ifNode->variable;
            int value = getVariableOrConstant(variableName);
            std::string ifOperator = ifNode->ifOperator;
            std::string operand = ifNode->operand;
            bool condition;
            if(ifOperator == "is") {
                if(operand == "odd") {
                    condition = value % 2 == 1;
                }
                else {
                    condition = value % 2 == 0;
                }
            }
            else {
                int operandValue = getVariableOrConstant(operand);
                if(ifOperator == "equals") {
                    condition = value == operandValue;
                }
                if(ifOperator == "not-equals") {
                    condition = value != operandValue;
                }
                if(ifOperator == "greater-than") {
                    condition = value > operandValue;
                }
                if(ifOperator == "greater-equal") {
                    condition = value >= operandValue;
                }
                if(ifOperator == "lesser-than") {
                    condition = value < operandValue;
                }
                if(ifOperator == "lesser-equal") {
                    condition = value <= operandValue;
                }
            }
            if(condition) {
                for(Node *n: node->body) {
                    eval(n);
                }
            }
            else if(!ifNode->elseBody->empty()) {
                for(Node *n: *ifNode->elseBody) {
                    eval(n);
                }
            }

        }
        if (node->nodeType == FOR) {
            auto *forNode = dynamic_cast<ForNode *>(node);
            int start = getVariableOrConstant(forNode->start);
            int stop = getVariableOrConstant(forNode->stop);
            if (start <= stop) {
                for (int i = start; i <= stop; i++) {
                    variables[forNode->variable] = i;
                    for (Node *n: node->body) {
                        eval(n);
                    }
                }
            } else {
                for (int i = start; i >= stop; i--) {
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
                throw FuncAlreadyDefError(functionName);
            }
            functions[funcDeclNode->name] = node;
            node->nodeType = ROOT;
        }
        if(node->nodeType == FUNC_CALL) {
            auto *funcCallNode = dynamic_cast<FunctionCallNode*>(node);
            std::string functionName = funcCallNode->name;
            if(not_find(functions, functionName)) {
                throw FucNotDefError(functionName);
            }
            eval(functions[functionName]);
        }
        if(node->nodeType == ADD) {
            auto *additionNode = dynamic_cast<AdditionNode*>(node);
            std::string value = additionNode->value;
            std::string receiver = additionNode->receiver;
            int extraValue = getVariableOrConstant(value);
            int receiverValue = getVariableOrConstant(receiver);
            if(not_find(variables, receiver)) {
                throw UndefinedVariableError(receiver);
            }

            receiverValue += extraValue;
            if(receiverValue > 99) {
                throw OutOfBoundsValueError(receiverValue);
            }
            variables[receiver] = receiverValue;
        }
        if(node->nodeType == SUB) {
            auto *subtractionNode = dynamic_cast<SubtractionNode*>(node);
            std::string value = subtractionNode->value;
            std::string receiver = subtractionNode->receiver;
            int subtractValue = getVariableOrConstant(value);
            int receiverValue = getVariableOrConstant(receiver);
            if(not_find(variables, receiver)) {
                throw UndefinedVariableError(receiver);
            }
            receiverValue -= subtractValue;
            if(receiverValue < 0) {
                throw OutOfBoundsValueError(receiverValue);
            }
            variables[receiver] = receiverValue;
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
    // todo: ><==!=, break into files
    static void run(Node *node) {
        eval(node);
        releaseMemory(node);
    }
};

std::unordered_map<std::string, int> Interpreter::variables;
std::unordered_map<std::string, Node*> Interpreter::functions;
