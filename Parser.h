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
#include "exceptions.h"

std::unordered_map<std::string, int> generateNumbers() {
    std::unordered_map<std::string, int> m;
    std::ifstream file("numbers.txt");
    std::string letters, eq, number;

    while (file >> letters >> eq >> number) {
        m[letters] = stoi(number);
    }
    return m;
}

int getConstant(const std::string &number) {
    static std::unordered_map<std::string, int> allNumbers = generateNumbers();
    if (find(allNumbers, number)) {
        return allNumbers[number];
    }
    return -1;
}


enum NodeType {
    ROOT, LET, FOR, IF, ELSE, SAY, READ, FUNC_DECL, FUNC_CALL, ADD, SUB, MUL, DIV, MOD
};

std::vector<std::string> keywords = {"let", "say", "read", "if", "else", "for", "function", "call", "add", "subtract"};
std::vector<std::string> ifOperators = {"is", "equals", "not-equals", "greater-than", "greater-equal", "lesser-than",  "lesser-equal", "divisible-by"};


std::string enumToString(NodeType value) {
    static std::unordered_map<NodeType, std::string> enumMap = {
            {ROOT, "ROOT"},
            {LET, "LET"},
            {FOR, "FOR"},
            {IF, "IF"},
            {ELSE, "ELSE"},
            {SAY, "SAY"},
            {READ, "READ"},
            {FUNC_DECL, "FUNC_DECL"},
            {FUNC_CALL, "FUNC_CALL"},
            {ADD, "ADD"},
            {SUB, "SUB"}
    };
    if(find(enumMap, value)) {
        return enumMap[value];
    }
    return "UNKNOWN";
}

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

class VariableNode : public Node {
public:
    std::string name;
    std::string value;

    explicit VariableNode(const std::vector<std::string> &words) {
        if(words.size() == 2 && std::isupper(words[1][0])) {
            nodeType = LET;
            name = words[1];
            value = "zero";
        }
        else if (words.size() == 4 && std::isupper(words[1][0]) && words[2] == "be") {
            nodeType = LET;
            name = words[1];
            value = words[3];
        }
        else {
            throw VarDeclError();
        }
    }
};

class SayNode : public Node {
public:
    std::string variable;

    explicit SayNode(const std::vector<std::string> &words) {
        if (words.size() != 2) {
            throw SayError();
        }
        nodeType = SAY;
        variable = words[1];
    }
};

class ReadNode : public Node {
public:
    std::string variable;

    explicit ReadNode(const std::vector<std::string> &words) {
        if(words.size() != 2) {
            throw ReadError();
        }
        nodeType = READ;
        variable = words[1];
    }
};

class IfNode : public Node {
public:
    std::string variable;
    std::string ifOperator;
    std::string operand;
    std::vector<Node*>* elseBody;

    explicit IfNode(const std::vector<std::string> &words) {
        if (words.size() != 4 || not_find(ifOperators, words[2])) {
            throw IfError();
        }
        if(words[2] == "is" && not_find({"even", "odd"}, words[3])) {
            throw IfError();
        }
        nodeType = IF;
        variable = words[1];
        ifOperator = words[2];
        operand = words[3];
        elseBody = nullptr;
    }

    void setElseBody(std::vector<Node*>& nodes) {
        elseBody = &nodes;
    }

};

class ElseNode : public Node {
public:
    explicit ElseNode(Node* node) {
        if (node->nodeType != IF) {
            throw ElseError();
        }
        nodeType = ELSE;
    }
};

class ForNode : public Node {
public:
    std::string variable;
    std::string start;
    std::string stop;

    explicit ForNode(const std::vector<std::string> &words) {
        if (words.size() != 7 || words[1] != "every" || words[3] != "from" || words[5] != "to" ||
            std::islower(words[2][0])) {
            throw ForError();
        }
        nodeType = FOR;
        variable = words[2];
        start = words[4];
        stop = words[6];
    }
};

class FunctionDeclarationNode : public Node {
public:
    std::string name;
    explicit FunctionDeclarationNode(const std::vector<std::string> &words) {
        if(words.size() != 2 || std::islower(words[1][0])) {
            throw FuncDeclError();
        }
        nodeType = FUNC_DECL;
        name = words[1];
    }
};

class FunctionCallNode : public Node {
public:
    std::string name;
    explicit FunctionCallNode(const std::vector<std::string> &words) {
        if(words.size() != 2 || std::islower(words[1][0])) {
            throw FuncCallError();
        }
        nodeType = FUNC_CALL;
        name = words[1];
    }
};

class AdditionNode : public Node {
public:
    std::string receiver;
    std::string value;
    explicit AdditionNode(const std::vector<std::string> &words) {
        if(words.size() != 4 || words[2] != "to" || std::islower(words[3][0])) {
            throw AdditionError();
        }
        nodeType = ADD;
        value = words[1];
        receiver = words[3];
    }
};

class SubtractionNode : public Node {
public:
    std::string receiver;
    std::string value;
    explicit SubtractionNode(const std::vector<std::string> &words) {
        if(words.size() != 4 || words[2] != "from" || std::islower(words[3][0])) {
            throw SubtractionError();
        }
        nodeType = SUB;
        value = words[1];
        receiver = words[3];
    }
};

class Parser {
    static std::vector<std::string> removeEmptyLines(const std::vector<std::string> &lines) {
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
    static std::vector<std::string> removeCommentLines(const std::vector<std::string>& lines) {
        std::vector<std::string> newLines;
        std::vector<std::string> comments;
        for (const std::string &line: lines) {
            std::size_t len = line.size();
            if(len >= 2 && line[len-1] == '/' && line[len-2] == '/') {
                comments.push_back(line);
            }
            else {
                newLines.push_back(line);
            }
        }
        return newLines;
    }

    static std::vector<int> getIndents(const std::vector<std::string> &lines) {
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

    static bool checkIndent(const std::vector<std::string> &lines, const std::vector<int> &indents) {
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

    static Node *parse(const std::string &code) {
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
                levelChange = (indents[i-1] - indents[i]) / 4;
            }
            while (levelChange > 0) {
                level.pop();
                levelChange--;
            }
            if (not_find(keywords, task)) {
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
                auto *ifNode = dynamic_cast<IfNode*>(prev);
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
            if(task == "call") {
                node = new FunctionCallNode(words);
                level.top()->add(node);
            }
            if(task == "add") {
                node = new AdditionNode(words);
                level.top()->add(node);
            }
            if(task == "subtract") {
                node = new SubtractionNode(words);
                level.top()->add(node);
            }
        }
        return root;
    }
};