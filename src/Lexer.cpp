#include"Lexer.h"
#include <stdexcept>
bool isDigit(char c) { return c >= '0' && c <= '9'; }
bool isLetter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

Token Lexer::readNumber() {
    string num;
    while (pos < input.size() && isDigit(input[pos]))
        num += input[pos++];
    Token t(TokenType::NUMBER, num);
    t.intValue = std::stoi(num);
    return t;
}

Token Lexer::readWord() {
    string word;
    while (pos < input.size() && (isLetter(input[pos]) || isDigit(input[pos])))
        word += input[pos++];
    if (word == "if")    return Token(TokenType::KEYWORD, "if");
    if (word == "then")  return Token(TokenType::KEYWORD, "then");
    if (word == "else")  return Token(TokenType::KEYWORD, "else");
    if (word == "while") return Token(TokenType::KEYWORD, "while");
    if (word == "do")    return Token(TokenType::KEYWORD, "do");
    if (word == "end")   return Token(TokenType::KEYWORD, "end");
    return Token(TokenType::IDENTIFIER, word);
}

Lexer::Lexer(const string& src) : input(src), pos(0) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (pos < input.size()) {
        char ch = input[pos];
        if (isspace(ch)) { pos++; continue; }
        if (isDigit(ch)) { tokens.push_back(readNumber()); continue; }
        if (isLetter(ch)) { tokens.push_back(readWord()); continue; }
        switch (ch) {
        case '+': tokens.push_back(Token(TokenType::OPERATOR, "+")); pos++; break;
        case '-': tokens.push_back(Token(TokenType::OPERATOR, "-")); pos++; break;
        case '*': tokens.push_back(Token(TokenType::OPERATOR, "*")); pos++; break;
        case '/': tokens.push_back(Token(TokenType::OPERATOR, "/")); pos++; break;
        case '<':
            if (pos + 1 < input.size() && input[pos + 1] == '=') {
                tokens.push_back(Token(TokenType::OPERATOR, "<="));
                pos += 2;
            }
            else {
                tokens.push_back(Token(TokenType::OPERATOR, "<"));
                pos++;
            }
            break;
        case '>':
            if (pos + 1 < input.size() && input[pos + 1] == '=') {
                tokens.push_back(Token(TokenType::OPERATOR, ">="));
                pos += 2;
            }
            else {
                tokens.push_back(Token(TokenType::OPERATOR, ">"));
                pos++;
            }
            break;
        case '=':
            if (pos + 1 < input.size() && input[pos + 1] == '=') {
                tokens.push_back(Token(TokenType::OPERATOR, "=="));
                pos += 2;
            }
            else {
                tokens.push_back(Token(TokenType::OPERATOR, "="));
                pos++;
            }
            break;
        case '!':
            if (pos + 1 < input.size() && input[pos + 1] == '=') {
                tokens.push_back(Token(TokenType::OPERATOR, "!="));
                pos += 2;
            }
            else {
                throw runtime_error("Unexpected character '!'");
            }
            break;
        case ';': tokens.push_back(Token(TokenType::PUNCTUATION, ";")); pos++; break;
        case '(': tokens.push_back(Token(TokenType::PUNCTUATION, "(")); pos++; break;
        case ')': tokens.push_back(Token(TokenType::PUNCTUATION, ")")); pos++; break;
        default: throw runtime_error("Unknown character: " + string(1, ch));
        }
    }
    tokens.push_back(Token(TokenType::END, ""));
    return tokens;
}
