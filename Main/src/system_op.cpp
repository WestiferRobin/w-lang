#include "parser_comp.hpp"

ASTNode * Parser::copy()
{
    ASTNode * copyTarget = NULL;
    ASTNode * copySource = NULL;
    ASTUtility::validToken(T_KEYWORD, "COPY", currToken);
    ASTUtility::validToken(T_SYMBOL, "(", currToken);

    if (currToken->tType == T_VARIABLE && arr_table.find(currToken->entry) != arr_table.end())
    {
        copyTarget = ASTUtility::createASTWholeArrayNode(*currToken++);
    }

    ASTUtility::validToken(T_SYMBOL, ",", currToken);

    if (currToken->tType == T_VARIABLE && arr_table.find(currToken->entry) != arr_table.end())
    {
        copySource = ASTUtility::createASTWholeArrayNode(*currToken++);
    }
    else if (currToken->entry == "\"")
    {
        throw (int) ErrorNoRawString;
    }

    ASTUtility::validToken(T_SYMBOL, ")", currToken);
    ASTUtility::validToken(T_SYMBOL, ";", currToken);

    return ASTUtility::createASTNode(COPY, copyTarget, copySource);
}

ASTNode * Parser::deleteStatement()
{
    ASTNode * delVariable;

    ASTUtility::validToken(T_KEYWORD, "delete", currToken);

    if (symbol_table.find(currToken->entry) != symbol_table.end())
    {
        if (symbol_table[currToken->entry])
        {
            throw (int) ErrorInvalidConstant;
        }

        symbol_table.erase(currToken->entry);

        delVariable = ASTUtility::createASTVariableNode(*currToken);
        
        currToken++;
    }
    else if (arr_table.find(currToken->entry) != arr_table.end())
    {
        if (arr_table[currToken->entry])
        {
            throw (int) ErrorInvalidConstant;
        }

        arr_table.erase(currToken->entry);

        delVariable = ASTUtility::createASTVariableNode(*currToken);

        currToken++;
    }
    else
    {
        throw (int) ErrorVariableUnknown;
    }

    ASTUtility::validToken(T_SYMBOL, ";", currToken);

    return ASTUtility::createASTNode(DELETE, delVariable, NULL);
}

ASTNode * Parser::length()
{
    ASTNode * lenTarg = NULL;
    ASTNode * lenSource = NULL;

    ASTUtility::validToken(T_KEYWORD, "LENGTH", currToken);
    ASTUtility::validToken(T_SYMBOL, "(", currToken);

    if (currToken->tType == T_VARIABLE && symbol_table.find(currToken->entry) != symbol_table.end())
    {
        lenTarg = ASTUtility::createASTVariableNode(*currToken++);
    }

    ASTUtility::validToken(T_SYMBOL, ",", currToken);
    
    if (currToken->tType == T_VARIABLE && arr_table.find(currToken->entry) != arr_table.end())
    {
        lenSource = ASTUtility::createASTWholeArrayNode(*currToken++);
    }
    else if (currToken->entry == "\"")
    {
        throw (int) ErrorNoRawString;
    }

    ASTUtility::validToken(T_SYMBOL, ")", currToken);
    ASTUtility::validToken(T_SYMBOL, ";", currToken);

    return ASTUtility::createASTNode(LENGTH, lenTarg, lenSource);
}

ASTNode * Parser::equal()
{
    ASTNode * result = NULL;
    ASTNode * leftSide = NULL;
    ASTNode * rightSide = NULL;

    ASTUtility::validToken(T_KEYWORD, "EQUAL", currToken);
    ASTUtility::validToken(T_SYMBOL, "(", currToken);

    if (currToken->tType == T_VARIABLE && symbol_table.find(currToken->entry) != symbol_table.end())
    {
        result = ASTUtility::createASTVariableNode(*currToken++);
    }

    ASTUtility::validToken(T_SYMBOL, ",", currToken);

    if (currToken->tType == T_VARIABLE && arr_table.find(currToken->entry) != arr_table.end())
    {
        leftSide = ASTUtility::createASTWholeArrayNode(*currToken++);
    }
    else if (currToken->entry == "\"")
    {
        throw (int) ErrorNoRawString;
    }

    ASTUtility::validToken(T_SYMBOL, ",", currToken);

    if (currToken->tType == T_VARIABLE && arr_table.find(currToken->entry) != arr_table.end())
    {
        rightSide = ASTUtility::createASTWholeArrayNode(*currToken++);
    }
    else if (currToken->entry == "\"")
    {
        throw (int) ErrorNoRawString;
    }

    ASTUtility::validToken(T_SYMBOL, ")", currToken);
    ASTUtility::validToken(T_SYMBOL, ";", currToken);

    return ASTUtility::createASTNode(EQUAL_FUNC, result, ASTUtility::createASTNode(UNKNOWN, leftSide, rightSide));
}

ASTNode * Parser::print()
{ 
    ASTNode * printContent;
    ASTNode * printMode;

    ASTUtility::validToken(T_KEYWORD, "PRINT", currToken);
    ASTUtility::validToken(T_SYMBOL, "(", currToken);

    if (currToken->entry == "\"")
    {
        throw (int) ErrorNoRawString;
    }
    else
    {
        printContent = this->expression();
    }

    ASTUtility::validToken(T_SYMBOL, ",", currToken);

    switch (currToken->tType)
    {
        case T_VARIABLE:
            printMode = ASTUtility::createASTVariableNode(*currToken);
            break;
        case T_NUMBER:
            printMode = ASTUtility::createASTNumberNode(*currToken);
            break; 
        default:
            throw (int) ErrorInvalidSymbol;
    }

    currToken++;

    ASTUtility::validToken(T_SYMBOL, ")", currToken);
    ASTUtility::validToken(T_SYMBOL, ";", currToken);

    return ASTUtility::createASTNode(PRINT, printContent, printMode);
}

ASTNode * Parser::scan()
{
    ASTNode * inputVariable;

    ASTUtility::validToken(T_KEYWORD, "SCAN", currToken);
    ASTUtility::validToken(T_SYMBOL, "(", currToken);

    if (currToken->tType == T_VARIABLE && symbol_table.find(currToken->entry) != symbol_table.end())
    {
        inputVariable = ASTUtility::createASTVariableNode(*currToken++);
    }
    else
    {
        throw (int) ErrorInvalidSymbol;
    }
    
    ASTUtility::validToken(T_SYMBOL, ")", currToken);
    ASTUtility::validToken(T_SYMBOL, ";", currToken);

    return ASTUtility::createASTNode(SCAN, inputVariable, NULL);
}