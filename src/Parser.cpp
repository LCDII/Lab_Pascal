#include"Parser.h"
#include<stdexcept>



Token Parser::cur() const { return tokens[pos]; }

bool Parser::check(TokenType type, const string& text) {
    return cur().type == type && (text.empty() || cur().text == text);
}

void Parser::expect(TokenType type, const string& text) {
    if (!check(type, text))
        throw runtime_error("Expected " + (text.empty() ? "token" : text));
    pos++;
}

bool Parser::accept(TokenType type, const string& text) {
    if (check(type, text)) { pos++; return true; }
    return false;
}


ExprNode* Parser::parsePrimary() {
    if (check(TokenType::NUMBER)) {
        int v = cur().intValue;
        pos++;
        return new NumberNode(v);
    }
    if (check(TokenType::IDENTIFIER)) {
        string name = cur().text;
        pos++;
        return new VariableNode(name);
    }
    if (accept(TokenType::PUNCTUATION, "(")) {
        ExprNode* e = parseExpr();
        expect(TokenType::PUNCTUATION, ")");
        return e;
    }
    throw runtime_error("Expected number, identifier or '('");
}

int Parser::precedence(const string& op) {
    if (op == "=") return 1;
    if (op == "<" || op == "<=" || op == ">" || op == ">=") return 2;
    if (op == "+" || op == "-") return 3;
    if (op == "*" || op == "/") return 4;
    return 0;
}

ExprNode* Parser::parseBinaryOp(int minPrec, ExprNode* left) {
    while (true) {
        if (!check(TokenType::OPERATOR)) break;
        string op = cur().text;
        int prec = precedence(op);
        if (prec < minPrec) break;
        pos++;
        ExprNode* right = parsePrimary();
        while (true) {
            if (!check(TokenType::OPERATOR)) break;
            string nxtOp = cur().text;
            int nxtPrec = precedence(nxtOp);
            if (nxtPrec <= prec) break;
            right = parseBinaryOp(prec + 1, right);
        }
        left = new BinaryOpNode(op, left, right);
    }
    return left;
}

ExprNode* Parser::parseExpr() {
    ExprNode* left = parsePrimary();
    return parseBinaryOp(0, left);
}


StmtNode* Parser::parseStatementList(const string& stopToken) {
    if (check(TokenType::END) || (!stopToken.empty() && check(TokenType::KEYWORD, stopToken)))
        return nullptr;
    StmtNode* first = parseStatement();
    if (accept(TokenType::PUNCTUATION, ";")) {
        if (!stopToken.empty() && check(TokenType::KEYWORD, stopToken))
            return new SeqNode(first, nullptr);
        StmtNode* rest = parseStatementList(stopToken);
        return new SeqNode(first, rest);
    }
    else {
        return new SeqNode(first, nullptr);
    }
}

StmtNode* Parser::parseStatement() {
    if (check(TokenType::IDENTIFIER)) {
        string var = cur().text;
        pos++;
        expect(TokenType::OPERATOR, "=");
        ExprNode* e = parseExpr();
        return new AssignNode(new VariableNode(var), e);
    }
    if (check(TokenType::KEYWORD, "while")) {
        pos++;
        expect(TokenType::PUNCTUATION, "(");
        ExprNode* cond = parseExpr();
        expect(TokenType::PUNCTUATION, ")");
        expect(TokenType::KEYWORD, "do");
        StmtNode* body = parseStatementList("end");
        expect(TokenType::KEYWORD, "end");
        return new WhileNode(cond, body);
    }
    if (check(TokenType::KEYWORD, "if")) {
        pos++;
        expect(TokenType::PUNCTUATION, "(");
        ExprNode* cond = parseExpr();
        expect(TokenType::PUNCTUATION, ")");
        expect(TokenType::KEYWORD, "then");
        StmtNode* thenPart = parseStatementList("else");
        StmtNode* elsePart = nullptr;
        if (accept(TokenType::KEYWORD, "else")) {
            elsePart = parseStatementList("end");
        }
        expect(TokenType::KEYWORD, "end");
        return new IfNode(cond, thenPart, elsePart);
    }
    throw runtime_error("Unexpected statement");
}

Parser::Parser(const vector<Token>& tok) : tokens(tok), pos(0) {}

StmtNode* Parser::parse() {
    if (tokens.empty() || tokens[0].type == TokenType::END)
        return nullptr;
    StmtNode* program = parseStatementList();
    if (cur().type != TokenType::END)
        throw runtime_error("Extra tokens after program");
    return program;
}