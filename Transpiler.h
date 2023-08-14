//
// Created by ppint on 8/14/2023.
//

#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include "Parser.h"
#include "exceptions.h"

class Transpiler {

    static std::string code;
    static std::stack<std::string> indents;

    template <typename... Args>
    static void write(const char* format, Args... args) {
        code += indents.top() + fmt(format, args...) + "\n";
    }

    static void translate(Node *node, int indentValue=0) {
        indents.emplace(repeatString(" ", indentValue * 4));
        if (node->nodeType == ROOT) {
            write("{");
            for (Node *n: node->body) {
                translate(n, indentValue + 1);
            }
            write("}");
        }
        if (node->nodeType == LET) {
            auto *variableNode = dynamic_cast<VariableNode *>(node);
            write("int %s = %d;", variableNode->name, variableNode->value);
        }
        if (node->nodeType == SAY) {
            auto *sayNode = dynamic_cast<SayNode *>(node);
            std::string variable = sayNode->variable;
            if(getConstant(variable) != -1) {
                variable = std::to_string(getConstant(variable));
            }
            if(variable == "space") {
                variable = "' '";
            }
            if(variable == "line") {
                variable = "'\\n'";
            }
            write("cout << %s;", variable);
        }
        if (node->nodeType == READ) {
            auto *readNode = dynamic_cast<ReadNode *>(node);
            write("cin >> %s;", readNode->variable);
        }
        if (node->nodeType == ADD) {
            auto *additionNode = dynamic_cast<AdditionNode *>(node);
            write("%s = %s + %s;", additionNode->receiver, additionNode->receiver,
                        additionNode->value);
        }
        if (node->nodeType == SUB) {
            auto *subtractionNode = dynamic_cast<SubtractionNode *>(node);
            write("%s = %s - %s;", subtractionNode->receiver, subtractionNode->receiver,
                        subtractionNode->value);
        }
        if (node->nodeType == IF) {
            auto *ifNode = dynamic_cast<IfNode *>(node);
            std::string condition;
            if(ifNode->ifOperator == "is") {
                if(ifNode->operand == "odd") {
                    condition = "% 2 == 1";
                }
                else {
                    condition = "% 2 == 0";
                }
                write("if (%s %s) {", ifNode->variable, condition);
            }
            else {
                static std::unordered_map<std::string, std::string> operators = {
                        {"equals", "=="},
                        {"not-equals", "!="},
                        {"greater-than", ">"},
                        {"greater-equal", ">="},
                        {"lesser-than", "<"},
                        {"lesser-equal", "<="}
                };
                std::string operand = ifNode->operand;
                if(getConstant(operand) != -1) {
                    operand = std::to_string(getConstant(operand));
                }
                write("if (%s %s %s) {", ifNode->variable, operators[ifNode->ifOperator], operand);
            }
            for(Node *n: node->body) {
                translate(n, indentValue + 1);
            }
            write("}");
        }
        if(node->nodeType == ELSE) {
            write("else {");
            for(Node *n: node->body) {
                translate(n, indentValue + 1);
            }
            write("}");
        }
        if(node->nodeType == FOR) {
            auto *forNode = dynamic_cast<ForNode*> (node);
            std::string variable = forNode->variable;
            std::string start = forNode->start;
            std::string stop = forNode->stop;
            if(getConstant(start) != -1) {
                start = std::to_string(getConstant(start));
            }
            if(getConstant(stop) != -1) {
                stop = std::to_string(getConstant(stop));
            }
            if(start <= stop) {
                write("for (int %s = %s; %s <= %s; %s++) {",
                            variable, start, variable, stop, variable);
            }
            else {
                write("for (int %s = %s; %s >= %s; %s--) {",
                            variable, start, variable, stop, variable);
            }
            for (Node *n: node->body) {
                translate(n, indentValue + 1);
            }
            write("}");
        }
        if(node->nodeType == FUNC_DECL) {
            auto *funcDeclNode = dynamic_cast<FunctionDeclarationNode*>(node);
            write("void %s() {", funcDeclNode->name);
            for (Node *n: node->body) {
                translate(n, indentValue + 1);
            }
            write("}");
        }
        if(node->nodeType == FUNC_CALL) {
            auto *funcCallNode = dynamic_cast<FunctionCallNode*>(node);
            write("%s();", funcCallNode->name);
        }
        indents.pop();
    }

    static void deleteNode(Node *root) {
        for (Node *node: root->body) {
            deleteNode(node);
        }
        delete root;
    }

    static void releaseMemory(Node *node) {
        deleteNode(node);
        while(!indents.empty()) {
            indents.pop();
        }
    }

public:
    static void run(Node *node) {
        indents.emplace("");
        translate(node);
        releaseMemory(node);
    }
    static std::string getCode() {
        return code;
    }
};

std::string Transpiler::code = "#include <iostream>\nusing namespace std;\n\nint main()\n";
std::stack<std::string> Transpiler::indents;