#include"Visitors.h"

void PrintVisitor::printIndent() { for (int i = 0; i < indent; ++i) out << "  "; }
PrintVisitor::PrintVisitor(std::ostream& os) : out(os), indent(0) {}

void PrintVisitor::visit(NumberNode* n)  { out << n->value; }

void PrintVisitor::visit(VariableNode* n)  { out << n->name; }

void PrintVisitor::visit(BinaryOpNode* n){
    out << "(";
    n->left->accept(this);
    out << " " << n->op << " ";
    n->right->accept(this);
    out << ")";
}

void PrintVisitor::visit(AssignNode* n)  {
    n->var->accept(this);
    out << " = ";
    n->expr->accept(this);
    out << ";";
}

void PrintVisitor::visit(SeqNode* n)  {
    n->left->accept(this);
    if (n->right) {
        out << "\n";
        printIndent();
        n->right->accept(this);
    }
}

void PrintVisitor::visit(WhileNode* n){
    out << "while ";
    n->cond->accept(this);
    out << " do\n";
    indent++;
    printIndent();
    n->body->accept(this);
    indent--;
    out << "\n";
    printIndent();
    out << "end;";
}

void PrintVisitor::visit(IfNode* n){
    out << "if ";
    n->cond->accept(this);
    out << " then\n";
    indent++;
    printIndent();
    n->thenPart->accept(this);
    indent--;
    if (n->elsePart) {
        out << "\n";
        printIndent();
        out << "else\n";
        indent++;
        printIndent();
        n->elsePart->accept(this);
        indent--;
    }
    out << "\n";
    printIndent();
    out << "end;";
}












int ExecVisitor::eval(ExprNode* e) {
    if (auto n = dynamic_cast<NumberNode*>(e))
        return n->value;
    if (auto v = dynamic_cast<VariableNode*>(e)) {
        int* p = vars->find(v->name);
        if (!p)
            throw runtime_error("Variable not initialized: " + v->name);
        return *p;
    }
    if (auto b = dynamic_cast<BinaryOpNode*>(e)) {
        int l = eval(b->left);
        int r = eval(b->right);
        if (b->op == "+") return l + r;
        if (b->op == "-") return l - r;
        if (b->op == "*") return l * r;
        if (b->op == "/") { if (r == 0) throw runtime_error("Division by zero"); return l / r; }
        if (b->op == "<")  return l < r;
        if (b->op == "<=") return l <= r;
        if (b->op == ">")  return l > r;
        if (b->op == ">=") return l >= r;
        if (b->op == "=")  return l == r;
        throw runtime_error("Unknown operator: " + b->op);
    }
    throw runtime_error("Invalid expression node");
}


void ExecVisitor::visit(NumberNode*) {}
void ExecVisitor::visit(VariableNode*)  {}
void ExecVisitor::visit(BinaryOpNode*)  {}

ExecVisitor::ExecVisitor(IMap<std::string, int>* map) : vars(map) {}

void ExecVisitor::visit(AssignNode* n) {
    int val = eval(n->expr);
    (*vars)[n->var->name] = val;
}

void ExecVisitor::visit(SeqNode* n)  {
    n->left->accept(this);
    if (n->right) n->right->accept(this);
}

void ExecVisitor::visit(WhileNode* n) {
    while (eval(n->cond))
        n->body->accept(this);
}

void ExecVisitor::visit(IfNode* n)  {
    if (eval(n->cond))
        n->thenPart->accept(this);
    else if (n->elsePart)
        n->elsePart->accept(this);
}











int StepDebugger::eval(ExprNode* e) {
    if (auto n = dynamic_cast<NumberNode*>(e))
        return n->value;
    if (auto v = dynamic_cast<VariableNode*>(e)) {
        int* p = vars->find(v->name);
        if (!p)
            throw runtime_error("Variable not initialized: " + v->name);
        return *p;
    }
    if (auto b = dynamic_cast<BinaryOpNode*>(e)) {
        int l = eval(b->left);
        int r = eval(b->right);
        if (b->op == "+") return l + r;
        if (b->op == "-") return l - r;
        if (b->op == "*") return l * r;
        if (b->op == "/") { if (r == 0) throw runtime_error("Division by zero"); return l / r; }
        if (b->op == "<")  return l < r;
        if (b->op == "<=") return l <= r;
        if (b->op == ">")  return l > r;
        if (b->op == ">=") return l >= r;
        if (b->op == "=")  return l == r;
        throw runtime_error("Unknown operator: " + b->op);
    }
    throw runtime_error("Invalid expression node");
}

void StepDebugger::wait() {
    out << "\n[Enter] -> next step";
    std::cin.get();
    out << "\n";
}

void StepDebugger::printAssign(VariableNode* var, ExprNode* expr) {
    out << var->name << " = ";
    if (auto n = dynamic_cast<NumberNode*>(expr))
        out << n->value;
    else if (auto v = dynamic_cast<VariableNode*>(expr))
        out << v->name;
    else if (auto b = dynamic_cast<BinaryOpNode*>(expr)) {
        out << "(";
        printExpr(b->left);
        out << " " << b->op << " ";
        printExpr(b->right);
        out << ")";
    }
    out << ";";
}

void StepDebugger::printExpr(ExprNode* e) {
    if (auto n = dynamic_cast<NumberNode*>(e))
        out << n->value;
    else if (auto v = dynamic_cast<VariableNode*>(e))
        out << v->name;
    else if (auto b = dynamic_cast<BinaryOpNode*>(e)) {
        out << "(";
        printExpr(b->left);
        out << " " << b->op << " ";
        printExpr(b->right);
        out << ")";
    }
}


void StepDebugger::visit(NumberNode*) {}
void StepDebugger::visit(VariableNode*)  {}
void StepDebugger::visit(BinaryOpNode*)  {}


StepDebugger::StepDebugger(std::ostream& os, IMap<std::string, int>* map) : out(os), vars(map) {}
void StepDebugger::visit(AssignNode* n)  {
    printAssign(n->var, n->expr);
    int val = eval(n->expr);
    (*vars)[n->var->name] = val;
    wait();
}

void StepDebugger::visit(SeqNode* n) {
    n->left->accept(this);
    if (n->right) {
        n->right->accept(this);
    }
}

void StepDebugger::visit(WhileNode* n) {
    while (eval(n->cond)) {
        n->body->accept(this);
    }
}


void StepDebugger::visit(IfNode* n)  {
    if (eval(n->cond)) {
        n->thenPart->accept(this);
    }
    else if (n->elsePart) {
        n->elsePart->accept(this);
    }
}

void NumberNode::accept(Visitor* v) { v->visit(this); }
void VariableNode::accept(Visitor* v) { v->visit(this); }
void BinaryOpNode::accept(Visitor* v) { v->visit(this); }
void AssignNode::accept(Visitor* v) { v->visit(this); }
void SeqNode::accept(Visitor* v) { v->visit(this); }
void WhileNode::accept(Visitor* v) { v->visit(this); }
void IfNode::accept(Visitor* v) { v->visit(this); }