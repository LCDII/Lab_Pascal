#pragma once
#include"MyTree.h"
#include<iostream>

class Programm {
    StmtNode* ast;
public:
    Programm(const string& source);
    ~Programm();
    void print(ostream& out = cout);
    void execute();
    void stepDebug(ostream& out = cout);
};