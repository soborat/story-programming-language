#include <emscripten/bind.h>
#include <iostream>
#include <fstream>
#include "Parser.h"
#include "Transpiler.h"
#include "Interpreter.h"

using namespace emscripten;

std::string transpileCode(const std::string &code) {
    Node* root = Parser::parse(code);
    Transpiler::run(root);
    return Transpiler::getCode();
}

std::string runCode(const std::string &code) {
    Node* root = Parser::parse(code);
    Interpreter::run(root);
    //todo: make interpreter return instead of cout, show errors with red, strip "custom exception",
    //todo: change btn colors, change onhover color,
    //todo: make .right editable when "read"
}

int main() {
//    std::ifstream file("code.txt");
//    std::string content(std::istreambuf_iterator<char>(file), {});
//    Node* root = Parser::parse(content);
//    Interpreter::run(root);
//    Transpiler::run(root);
//    std::cout << Transpiler::getCode();
//    Interpreter::debug(root);

}

EMSCRIPTEN_BINDINGS(my_module) {
    function("transpileCode", &transpileCode);
}
