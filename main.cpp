#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "Parser.h"
#include "Interpreter.h"
#include "Transpiler.h"


int main() {
    std::ifstream file("code.txt");
    std::string content(std::istreambuf_iterator<char>(file), {});
    Node* root = Parser::parse(content);
//    Interpreter::run(root);
    Transpiler::run(root);
    std::cout << Transpiler::getCode();
//    Interpreter::debug(root);

}
