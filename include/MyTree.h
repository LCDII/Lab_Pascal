#pragma once
#include<string>
using namespace std;

class Visitor; 

class Node {
public:
    virtual ~Node() {}
    virtual void accept(Visitor* v) = 0;
};

class ExprNode : public Node {};

class NumberNode : public ExprNode {
public:
    int value;
    NumberNode(int val) : value(val) {}
    void accept(Visitor* v) override;
};

class VariableNode : public ExprNode {
public:
    string name;
    VariableNode(const string& n) : name(n) {}
    void accept(Visitor* v) override;
};

class BinaryOpNode : public ExprNode {
public:
    string op;
    ExprNode* left;
    ExprNode* right;
    BinaryOpNode(const string& o, ExprNode* l, ExprNode* r)
        : op(o), left(l), right(r) {
    }
    ~BinaryOpNode() { delete left; delete right; }
    void accept(Visitor* v) override;
};


class StmtNode : public Node {};

class AssignNode : public StmtNode {
public:
    VariableNode* var;
    ExprNode* expr;
    AssignNode(VariableNode* v, ExprNode* e) : var(v), expr(e) {}
    ~AssignNode() { delete var; delete expr; }
    void accept(Visitor* v) override;
};

class SeqNode : public StmtNode {
public:
    StmtNode* left;
    StmtNode* right;
    SeqNode(StmtNode* l, StmtNode* r = nullptr) : left(l), right(r) {}
    ~SeqNode() { delete left; delete right; }
    void accept(Visitor* v) override;
};

class WhileNode : public StmtNode {
public:
    ExprNode* cond;
    StmtNode* body;
    WhileNode(ExprNode* c, StmtNode* b) : cond(c), body(b) {}
    ~WhileNode() { delete cond; delete body; }
    void accept(Visitor* v) override;
};

class IfNode : public StmtNode {
public:
    ExprNode* cond;
    StmtNode* thenPart;
    StmtNode* elsePart;
    IfNode(ExprNode* c, StmtNode* t, StmtNode* e = nullptr)
        : cond(c), thenPart(t), elsePart(e) {
    }
    ~IfNode() { delete cond; delete thenPart; delete elsePart; }
    void accept(Visitor* v) override;
};
