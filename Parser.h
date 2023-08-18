#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <unordered_map>
#include "Util.h"
#include "Exceptions.h"
#include "SyntaxNodes/Node.h"
#include "SyntaxNodes/VariableNode.h"
#include "SyntaxNodes/SayNode.h"
#include "SyntaxNodes/ReadNode.h"
#include "SyntaxNodes/IfNode.h"
#include "SyntaxNodes/ElseNode.h"
#include "SyntaxNodes/ForNode.h"
#include "SyntaxNodes/FunctionDeclarationNode.h"
#include "SyntaxNodes/FunctionCallNode.h"
#include "SyntaxNodes/AdditionNode.h"
#include "SyntaxNodes/SubtractionNode.h"
#include "SyntaxNodes/MultiplicationNode.h"
#include "SyntaxNodes/DivisionNode.h"
#include "Constants.h"


class Parser {
    std::vector<std::string> removeEmptyLines(const std::vector<std::string> &lines) {
        std::vector<std::string> newLines;
        for (const std::string &line: lines) {
            int spaces = 0;
            for (const char &c: line) {
                if (std::isspace(c)) {
                    spaces++;
                }
            }
            if (spaces != line.size()) {
                newLines.push_back(line);
            }
        }
        return newLines;
    }

    std::vector<std::string> removeCommentLines(const std::vector<std::string> &lines) {
        std::vector<std::string> newLines;
        std::vector<std::string> comments;
        for (const std::string &line: lines) {
            std::size_t len = line.size();
            if (len >= 2 && line[len - 1] == '/' && line[len - 2] == '/') {
                comments.push_back(line);
            } else {
                newLines.push_back(line);
            }
        }
        return newLines;
    }

    std::vector<int> getIndents(const std::vector<std::string> &lines) {
        std::vector<int> indents;
        for (const std::string &line: lines) {
            int spaces = 0;
            for (int i = 0; i < line.size() && std::isspace(line[i]); i++) {
                if (line[i] == ' ') {
                    spaces++;
                }
                if (line[i] == '\t') {
                    spaces += 4;
                }
            }
            indents.push_back(spaces);
        }
        return indents;
    }

    bool checkIndent(const std::vector<std::string> &lines, const std::vector<int> &indents) {
        for (int i = 1; i < indents.size(); i++) {
            if (indents[i] % 4 != 0 || indents[i] > indents[i - 1] + 4) {
                return false;
            }
            std::string word = split(strip(lines[i - 1]), ' ')[0];
            bool increaseIndent = word == "for" || word == "if" || word == "else" || word == "function";

            if (increaseIndent && indents[i] != indents[i - 1] + 4) {
                return false;
            }
            if (!increaseIndent && indents[i] > indents[i - 1]) {
                return false;
            }
        }
        if (indents[0] != 0) {
            return false;
        }
        return true;
    }

public:
    Node *parse(const std::string &code) {
        std::vector<std::string> lines = split(code, '\n');
        lines = removeEmptyLines(lines);
        lines = removeCommentLines(lines);
        std::vector<int> indents = getIndents(lines);
        if (!checkIndent(lines, indents)) {
            throw IndentError();
        }

        Node *root = new Node{ROOT};
        std::stack<Node *> level;
        level.push(root);
        for (int i = 0; i < lines.size(); i++) {
            std::vector<std::string> words = split(strip(lines[i]), ' ');
            std::string task = words[0];
            int levelChange = 0;
            if (i > 0) {
                levelChange = (indents[i - 1] - indents[i]) / 4;
            }
            while (levelChange > 0) {
                level.pop();
                levelChange--;
            }
            if (notFind(keywords, task)) {
                throw UnknownKeywordError(task);
            }
            Node *node;
            if (task == "let") {
                node = new VariableNode(words);
                level.top()->add(node);
            }
            if (task == "say") {
                node = new SayNode(words);
                level.top()->add(node);
            }
            if (task == "read") {
                node = new ReadNode(words);
                level.top()->add(node);
            }
            if (task == "if") {
                node = new IfNode(words);
                level.top()->add(node);
                level.push(node);
            }
            if (task == "else") {
                Node *prev = level.top()->body.back();
                node = new ElseNode(prev);
                auto *ifNode = dynamic_cast<IfNode *>(prev);
                ifNode->setElseBody(node->body);
                level.top()->add(node);
                level.push(node);
            }
            if (task == "for") {
                node = new ForNode(words);
                level.top()->add(node);
                level.push(node);
            }
            if (task == "function") {
                node = new FunctionDeclarationNode(words);
                level.top()->add(node);
                level.push(node);
            }
            if (task == "call") {
                node = new FunctionCallNode(words);
                level.top()->add(node);
            }
            if (task == "add") {
                node = new AdditionNode(words);
                level.top()->add(node);
            }
            if (task == "subtract") {
                node = new SubtractionNode(words);
                level.top()->add(node);
            }
            if (task == "multiply") {
                node = new MultiplicationNode(words);
                level.top()->add(node);
            }
            if (task == "divide") {
                node = new DivisionNode(words);
                level.top()->add(node);
            }
        }
        return root;
    }
};