#pragma once
#include"MyTree.h"
#include"IMap.h"
#include<iostream>

class Visitor {
public:
    virtual ~Visitor() {}
    virtual void visit(NumberNode* n) = 0;
    virtual void visit(VariableNode* n) = 0;
    virtual void visit(BinaryOpNode* n) = 0;
    virtual void visit(AssignNode* n) = 0;
    virtual void visit(SeqNode* n) = 0;
    virtual void visit(WhileNode* n) = 0;
    virtual void visit(IfNode* n) = 0;
};


class PrintVisitor : public Visitor {
    ostream& out;
    int indent;
    void printIndent();
public:
    PrintVisitor(ostream& os);

    void visit(NumberNode* n) override;
    void visit(VariableNode* n) override;
    void visit(BinaryOpNode* n) override;
    void visit(AssignNode* n) override;
    void visit(SeqNode* n) override;
    void visit(WhileNode* n) override;
    void visit(IfNode* n) override;
};




class ExecVisitor : public Visitor {
    IMap<string, int>* vars;

    int eval(ExprNode* e);

public:
    ExecVisitor(IMap<string, int>* map);
    void visit(NumberNode*) override;
    void visit(VariableNode*) override;
    void visit(BinaryOpNode*) override;
    void visit(AssignNode* n) override;
    void visit(SeqNode* n) override;
    void visit(WhileNode* n) override;
    void visit(IfNode* n) override;
};



class StepDebugger : public Visitor {
    ostream& out;
    IMap<string, int>* vars;
    int eval(ExprNode* e);
    void wait();
    void printAssign(VariableNode* var, ExprNode* expr);
    void printExpr(ExprNode* e);

public:
    StepDebugger(ostream& os, IMap<string, int>* map);
    void visit(NumberNode*) override;
    void visit(VariableNode*) override;
    void visit(BinaryOpNode*) override;
    void visit(AssignNode* n) override;
    void visit(SeqNode* n) override;
    void visit(WhileNode* n) override;
    void visit(IfNode* n) override;
};