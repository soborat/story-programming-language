cmake_minimum_required(VERSION 3.26)
project(story-language)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-Wno-format-security)

add_executable(story-language main.cpp
        Parser.h
        Interpreter.h
        Util.h
        Exceptions.h
        Transpiler.h
        SyntaxNodes/Node.h
        SyntaxNodes/VariableNode.h
        SyntaxNodes/SayNode.h
        SyntaxNodes/ReadNode.h
        SyntaxNodes/IfNode.h
        SyntaxNodes/ElseNode.h
        SyntaxNodes/ForNode.h
        SyntaxNodes/FunctionDeclarationNode.h
        SyntaxNodes/FunctionCallNode.h
        SyntaxNodes/AdditionNode.h
        SyntaxNodes/SubtractionNode.h
        SyntaxNodes/MultiplicationNode.h
        SyntaxNodes/DivisionNode.h
        Constants.h)
