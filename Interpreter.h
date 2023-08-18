#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <emscripten.h>
#include "Parser.h"
#include "Exceptions.h"

using namespace emscripten;

EM_JS(const char*, getInputFromUser_JS, (), {
    var input = prompt("Please enter a value:");
    var lengthBytes = lengthBytesUTF8(input) + 1;
    var stringOnWasmHeap = _malloc(lengthBytes);
    stringToUTF8(input, stringOnWasmHeap, lengthBytes);
    return stringOnWasmHeap;
});

std::string getInput() {
    char* inputChar = (char*)getInputFromUser_JS();
    std::string inputStr = std::string(inputChar);
    free(inputChar);
    return strip(inputStr);
}


class Interpreter {
    std::unordered_map<std::string, int> variables;
    std::unordered_map<std::string, Node *> functions;
    std::string result;

    int getVariableOrConstant(const std::string &str) {
        if (getConstant(str) != -1) {
            return getConstant(str);
        } else if (find(variables, str)) {
            return variables[str];
        } else {
            throw UndefinedVariableError(str);
        }
    }

    void eval(Node *node) {
        if (node->nodeType == ROOT) {
            for (Node *n: node->body) {
                eval(n);
            }
        }
        if (node->nodeType == LET) {
            auto *variableNode = dynamic_cast<VariableNode *>(node);
            variables[variableNode->name] = getVariableOrConstant(variableNode->value);
        }
        if (node->nodeType == SAY) {
            auto *sayNode = dynamic_cast<SayNode *>(node);
            std::string variableName = sayNode->variable;
            if(variableName == "space") {
                result += ' ';
            }
            else if(variableName == "line") {
                result += '\n';
            }
            else {
                result += std::to_string(getVariableOrConstant(variableName));
            }
        }
        if (node->nodeType == READ) {
            auto *readNode = dynamic_cast<ReadNode *>(node);
            std::string variableName = readNode->variable;
            std::string str;
            str = getInput();
            if (getConstant(str) == -1) {
                throw InvalidNumberError(str);
            }
            variables[variableName] = getConstant(str);
        }
        if (node->nodeType == IF) {
            auto *ifNode = dynamic_cast<IfNode *>(node);
            std::string variableName = ifNode->variable;
            int value = getVariableOrConstant(variableName);
            IfOperator ifOperator = ifNode->ifOperator;
            std::string operand = ifNode->operand;
            bool condition;
            if (ifOperator == IS) {
                if (operand == "odd") {
                    condition = value % 2 == 1;
                } else {
                    condition = value % 2 == 0;
                }
            } else {
                int operandValue = getVariableOrConstant(operand);
                if (ifOperator == EQUALS) {
                    condition = value == operandValue;
                }
                if (ifOperator == NOT_EQUALS) {
                    condition = value != operandValue;
                }
                if (ifOperator == GREATER_THAN) {
                    condition = value > operandValue;
                }
                if (ifOperator == GREATER_EQUAL) {
                    condition = value >= operandValue;
                }
                if (ifOperator == LESSER_THAN) {
                    condition = value < operandValue;
                }
                if (ifOperator == LESSER_EQUAL) {
                    condition = value <= operandValue;
                }
                if (ifOperator == DIVISIBLE_BY) {
                    condition = value % operandValue == 0;
                }
            }
            if (condition) {
                for (Node *n: node->body) {
                    eval(n);
                }
            } else if (ifNode->elseBody) {
                for (Node *n: *ifNode->elseBody) {
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
        if (node->nodeType == FUNC_DECL) {
            auto *funcDeclNode = dynamic_cast<FunctionDeclarationNode *>(node);
            std::string functionName = funcDeclNode->name;
            functions[funcDeclNode->name] = node;
            node->nodeType = ROOT;
        }
        if (node->nodeType == FUNC_CALL) {
            auto *funcCallNode = dynamic_cast<FunctionCallNode *>(node);
            std::string functionName = funcCallNode->name;
            if (notFind(functions, functionName)) {
                throw FucNotDefError(functionName);
            }
            functions[functionName]->nodeType = ROOT;
            eval(functions[functionName]);
            functions[functionName]->nodeType = FUNC_DECL;
        }
        if (node->nodeType == ADD) {
            auto *additionNode = dynamic_cast<AdditionNode *>(node);
            std::string value = additionNode->value;
            std::string receiver = additionNode->receiver;
            int extraValue = getVariableOrConstant(value);
            int receiverValue = getVariableOrConstant(receiver);
            if (notFind(variables, receiver)) {
                throw UndefinedVariableError(receiver);
            }
            receiverValue += extraValue;
            if (receiverValue > 99) {
                throw OutOfBoundsValueError(receiverValue);
            }
            variables[receiver] = receiverValue;
        }
        if (node->nodeType == SUB) {
            auto *subtractionNode = dynamic_cast<SubtractionNode *>(node);
            std::string value = subtractionNode->value;
            std::string receiver = subtractionNode->receiver;
            int subtractValue = getVariableOrConstant(value);
            int receiverValue = getVariableOrConstant(receiver);
            if (notFind(variables, receiver)) {
                throw UndefinedVariableError(receiver);
            }
            receiverValue -= subtractValue;
            if (receiverValue < 0) {
                throw OutOfBoundsValueError(receiverValue);
            }
            variables[receiver] = receiverValue;
        }
        if (node->nodeType == MUL) {
            auto *multiplicationNode = dynamic_cast<MultiplicationNode *>(node);
            std::string value = multiplicationNode->value;
            std::string receiver = multiplicationNode->receiver;
            int multiplierValue = getVariableOrConstant(value);
            int receiverValue = getVariableOrConstant(receiver);
            if (notFind(variables, receiver)) {
                throw UndefinedVariableError(receiver);
            }
            receiverValue *= multiplierValue;
            if (receiverValue > 99) {
                throw OutOfBoundsValueError(receiverValue);
            }
            variables[receiver] = receiverValue;
        }
        if (node->nodeType == DIV) {
            auto *divisionNode = dynamic_cast<DivisionNode *>(node);
            std::string value = divisionNode->value;
            std::string receiver = divisionNode->receiver;
            int dividerValue = getVariableOrConstant(value);
            int receiverValue = getVariableOrConstant(receiver);
            if (notFind(variables, receiver)) {
                throw UndefinedVariableError(receiver);
            }
            if (dividerValue == 0) {
                throw DivisionByZeroError();
            }
            receiverValue /= dividerValue;
            variables[receiver] = receiverValue;
        }
    }

    void deleteNode(Node *root) {
        for (Node *node: root->body) {
            deleteNode(node);
        }
        delete root;
    }

    void releaseMemory(Node *node) {
        variables.clear();
        functions.clear();
        deleteNode(node);
    }

public:
    void debug(Node *root) {
        std::cout << enumToString(root->nodeType);
        std::cout << "(";
        for (Node *node: root->body) {
            debug(node);
        }
        std::cout << ")";
    }

    void run(Node *node) {
        eval(node);
        releaseMemory(node);
    }
    std::string getResult() {
        std::string copy(result);
        result.clear();
        return copy;
    }
};