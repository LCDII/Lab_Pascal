#pragma once
#include<vector>
#include"Token.h"
#include"MyTree.h"



class Parser {
    vector<Token> tokens;
    size_t pos;
    Token cur() const;
    bool check(TokenType type, const string& text = "");
    void expect(TokenType type, const string& text = "");
    bool accept(TokenType type, const string& text = "");

    ExprNode* parsePrimary();
    int precedence(const string& op);
    ExprNode* parseBinaryOp(int minPrec, ExprNode* left);
    ExprNode* parseExpr();
    StmtNode* parseStatementList(const string& stopToken = "");
    StmtNode* parseStatement();
public:
    Parser(const vector<Token>& tok);

    StmtNode* parse();
};
