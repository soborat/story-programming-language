//
// Created by ppint on 8/14/2023.
//

#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include "Parser.h"
#include "Exceptions.h"

class Transpiler {

    static std::string code;
    static std::string functions;
    static std::stack<std::string> indents;
    static std::stack<bool> isFunction;
    static std::vector<std::string> variables;

    template <typename... Args>
    static void write(const char* format, Args... args) {
        if(isFunction.top()) {
            functions += indents.top() + fmt(format, args...) + "\n";
        }
        else {
            code += indents.top() + fmt(format, args...) + "\n";
        }
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
            if(not_find(variables, variableNode->name)) {
                variables.push_back(variableNode->name);
            }
            std::string value = variableNode->value;
            if(getConstant(value) != -1) {
                value = std::to_string(getConstant(value));
            }
            write("%s = %s;", variableNode->name, value);
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
            if(not_find(variables, readNode->variable)) {
                variables.emplace_back(readNode->variable);
            }
            write("cin >> %s;", readNode->variable);
        }
        if (node->nodeType == ADD) {
            auto *additionNode = dynamic_cast<AdditionNode *>(node);
            std::string value = additionNode->value;
            if(getConstant(value) != -1) {
                value = std::to_string(getConstant(value));
            }
            write("%s = %s + %s;", additionNode->receiver, additionNode->receiver, value);
        }
        if (node->nodeType == SUB) {
            auto *subtractionNode = dynamic_cast<SubtractionNode *>(node);
            std::string value = subtractionNode->value;
            if(getConstant(value) != -1) {
                value = std::to_string(getConstant(value));
            }
            write("%s = %s - %s;", subtractionNode->receiver, subtractionNode->receiver, value);
        }
        if(node->nodeType == MUL) {
            auto *multiplicationNode = dynamic_cast<MultiplicationNode *>(node);\
            std::string value = multiplicationNode->value;
            if(getConstant(value) != -1) {
                value = std::to_string(getConstant(value));
            }
            write("%s = %s * %s;", multiplicationNode->receiver, multiplicationNode->receiver, value);
        }
        if(node->nodeType == DIV) {
            auto *divisionNode = dynamic_cast<DivisionNode *>(node);
            std::string value = divisionNode->value;
            if(getConstant(value) != -1) {
                value = std::to_string(getConstant(value));
            }
            write("%s = %s / %s;", divisionNode->receiver, divisionNode->receiver, value);
        }
        if (node->nodeType == IF) {
            auto *ifNode = dynamic_cast<IfNode *>(node);
            if(ifNode->ifOperator == "is") {
                std::string condition;
                if (ifNode->operand == "odd") {
                    condition = "% 2 == 1";
                } else {
                    condition = "% 2 == 0";
                }
                write("if (%s %s) {", ifNode->variable, condition);
            }
            else if(ifNode->ifOperator == "divisible-by") {
                write("if (%s %% %s == 0) {", ifNode->variable, ifNode->operand);
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
            if(not_find(variables, variable)) {
                variables.push_back(variable);
            }
            if(start <= stop) {
                write("for (%s = %s; %s <= %s; %s++) {",
                            variable, start, variable, stop, variable);
            }
            else {
                write("for (%s = %s; %s >= %s; %s--) {",
                            variable, start, variable, stop, variable);
            }
            for (Node *n: node->body) {
                translate(n, indentValue + 1);
            }
            write("}");
        }
        if(node->nodeType == FUNC_DECL) {
            auto *funcDeclNode = dynamic_cast<FunctionDeclarationNode*>(node);
            isFunction.emplace(true);
            indents.emplace("");
            indentValue = 0;
            write("void %s() {", funcDeclNode->name);
            for (Node *n: node->body) {
                translate(n, indentValue + 1);
            }
            write("}\n");
            isFunction.pop();
            indents.pop();

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
        variables.clear();
        functions.clear();
        while(!indents.empty()) {
            indents.pop();
        }
        while(!isFunction.empty()) {
            isFunction.pop();
        }
    }

public:
    static void run(Node *node) {
        indents.emplace("");
        isFunction.push(false);
        translate(node);
        std::string codeStart = "#include <iostream>\nusing namespace std;\n\n";
        if(!variables.empty()) {
            codeStart += "int ";
            for(int i = 0; i < variables.size(); i++) {
                codeStart += variables[i];
                if(i != variables.size() - 1) {
                    codeStart += ", ";
                }
            }
            codeStart += ";\n\n";
        }
        if(!functions.empty()) {
            codeStart += functions + "\n";
        }
        codeStart += "int main() ";
        code = codeStart + code;
        releaseMemory(node);
    }
    static std::string getCode() {
        std::string copy(code);
        code.clear();
        return copy;
    }
};

std::string Transpiler::code;
std::string Transpiler::functions;
std::stack<std::string> Transpiler::indents;
std::stack<bool> Transpiler::isFunction;
std::vector<std::string> Transpiler::variables;