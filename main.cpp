#include <emscripten/bind.h>
#include <iostream>
#include <fstream>
#include "Parser.h"
#include "Transpiler.h"
#include "Interpreter.h"

using namespace emscripten;

struct CodeResult {
    std::string output;
    bool ok;

    CodeResult() : output(""), ok(false) {}

    CodeResult(const std::string &o, bool k) : output(o), ok(k) {}
};

CodeResult transpileCode(const std::string &code) {
    try {
        Parser parser;
        Node *root = parser.parse(code);
        Transpiler transpiler;
        transpiler.run(root);
        return {transpiler.getCode(), true};
    }
    catch (std::exception &e) {
        return {e.what(), false};
    }
}

CodeResult runCode(const std::string &code) {
    try {
        Parser parser;
        Node *root = parser.parse(code);
        Interpreter interpreter;
        interpreter.run(root);
        return {interpreter.getResult(), true};
    } catch (std::exception &e) {
        return {e.what(), false};
    }
}

EMSCRIPTEN_BINDINGS(module) {
        value_object<CodeResult>("RunCodeResult")
                .field("output", &CodeResult::output)
                .field("ok", &CodeResult::ok);
        function("transpileCode", &transpileCode);
        function("runCode", &runCode);
}

int main() {}

