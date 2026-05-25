#pragma once
#include<string>
using namespace std;

enum class TokenType {
    NUMBER, IDENTIFIER, KEYWORD, OPERATOR, PUNCTUATION, END
};

struct Token {
    TokenType type;
    string text;
    int intValue;
    Token(TokenType t = TokenType::END, const string& txt = "")
        : type(t), text(txt), intValue(0) {
    }
};