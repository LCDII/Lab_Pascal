#pragma once
#include<string>
#include<vector>
#include"Token.h"
using namespace std;

class Lexer {
    string input;
    size_t pos;

    Token readNumber();
    Token readWord();
public:
    Lexer(const string& src);
    vector<Token> tokenize();
};