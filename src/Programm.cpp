#include"Programm.h"
#include"Lexer.h"
#include"Parser.h"
#include"Visitors.h"
#include"AVLMap.h"
#include<vector>

Programm::Programm(const std::string& source) : ast(nullptr) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    ast = parser.parse();
}
Programm::~Programm() { delete ast; }

void Programm::print(std::ostream& out) {
    if (!ast) return;
    PrintVisitor printer(out);
    ast->accept(&printer);
    out << std::endl;
}

void Programm::execute() {
    if (!ast) return;
    AVLMap<std::string, int> map;
    ExecVisitor exec(&map);
    ast->accept(&exec);
}

void Programm::stepDebug(std::ostream& out) {
    if (!ast) return;
    AVLMap<std::string, int> map;
    StepDebugger debugger(out, &map);
    ast->accept(&debugger);
    out << std::endl;
}
