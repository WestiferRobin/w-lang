#ifndef _TOKEN_TYPES_H_
#define _TOKEN_TYPES_H_
#include <iostream>

using namespace std;

enum TokenType
{
    T_NUMBER,
    T_VARIABLE,
    T_KEYWORD,
    T_SYMBOL,
    T_ENDOFFILE,
    T_CHAR,
    T_STRING,
};

class TokenEntry
{
public:
    TokenType tType;
    string entry;
    TokenEntry(TokenType newType, string newEntry) : tType(newType), entry(newEntry) {}
    ~TokenEntry() {}
};

#endif