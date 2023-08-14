#include <iostream>
#include <fstream>
#include "Parser.h"
#include "Transpiler.h"
#include "Interpreter.h"


int main() {
    std::ifstream file("code.txt");
    std::string content(std::istreambuf_iterator<char>(file), {});
    Node* root = Parser::parse(content);
//    Interpreter::run(root);
    Transpiler::run(root);
    std::cout << Transpiler::getCode();
//    Interpreter::debug(root);

}
