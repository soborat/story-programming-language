#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "Parser.h"
#include "Interpreter.h"


int main() {
    std::ifstream file("code.txt");
    std::string content(std::istreambuf_iterator<char>(file), {});
    Node* root = Parser::parse(content);
    Interpreter::run(root);
//    Interpreter::debug(root);
    return 0;
}
