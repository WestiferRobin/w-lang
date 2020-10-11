#include "parser_comp.hpp"

/*
    TODO: 
        - Apply the RAW_ARRAY/STRING to the following bellow but please remove the redundent functionality.
        - Test them all for christ sake.
        - Finish the append, remove, and random.
*/

ASTNode* Parser::random()
{
    return NULL;
}

ASTNode * Parser::append()
{
    return NULL;
}

ASTNode * Parser::remove()
{
    return NULL;
}

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
        cout << "WILL FIX!" << endl;
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
    else if (currToken->entry == "[")
    {
        ASTUtility::validToken(T_SYMBOL, "[", currToken);
        ASTNode* varAssign = ASTUtility::createASTArrayNode(*(new TokenEntry(TokenType::T_VARIABLE, "RAW_ARRAY_TEST")));
        ASTNode* resultNode = ASTUtility::createASTNode(ARRAY_INIT_PRE_ELM, ASTUtility::createASTNullNode(), ASTUtility::createASTNullNode());
        ASTNode* array_elements = resultNode->right;

        while (currToken->entry != "]")
        {
            if (currToken->entry != ",")
            {
                array_elements->right = ASTUtility::createASTNumberNode(*currToken);
                array_elements = array_elements->right;
            }
            currToken++;
        }

        TokenEntry te(T_NUMBER, "0");
        resultNode->left = ASTUtility::createASTNumberNode(te);

        arr_table.insert({ varAssign->key, false });

        ASTUtility::validToken(T_SYMBOL, "]", currToken);


        lenSource = ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
        lenSource->key = varAssign->key;
    }
    else if (currToken->entry == "\"")
    {
        ASTNode* varAssign = ASTUtility::createASTArrayNode(*(new TokenEntry(TokenType::T_VARIABLE, "RAW_STRING_TEST")));
        ASTUtility::validToken(T_SYMBOL, "\"", currToken);;

        ASTNode* resultNode = ASTUtility::createASTNode(ARRAY_INIT_PRE_ELM, ASTUtility::createASTNullNode(), ASTUtility::createASTNullNode());
        ASTNode* char_elements = resultNode->right;

        while (currToken->entry != "\"")
        {
            char_elements->right = ASTUtility::createASTNumberNode(*(currToken++));
            char_elements = char_elements->right;
        }

        TokenEntry te(T_NUMBER, "0");
        resultNode->left = ASTUtility::createASTNumberNode(te);

        arr_table.insert({ varAssign->key, false });

        ASTUtility::validToken(T_SYMBOL, "\"", currToken);

        lenSource = ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
        lenSource->key = varAssign->key;
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
        cout << "WILL FIX!" << endl;
    }

    ASTUtility::validToken(T_SYMBOL, ",", currToken);

    if (currToken->tType == T_VARIABLE && arr_table.find(currToken->entry) != arr_table.end())
    {
        rightSide = ASTUtility::createASTWholeArrayNode(*currToken++);
    }
    else if (currToken->entry == "\"")
    {
        cout << "WILL FIX!" << endl;
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

    if (currToken->entry == "[")
    {
        ASTUtility::validToken(T_SYMBOL, "[", currToken);
        ASTNode* varAssign = ASTUtility::createASTArrayNode(*(new TokenEntry(TokenType::T_VARIABLE, "RAW_ARRAY_TEST")));
        ASTNode* resultNode = ASTUtility::createASTNode(ARRAY_INIT_PRE_ELM, ASTUtility::createASTNullNode(), ASTUtility::createASTNullNode());
        ASTNode* array_elements = resultNode->right;

        while (currToken->entry != "]")
        {
            if (currToken->entry != ",")
            {
                array_elements->right = ASTUtility::createASTNumberNode(*currToken);
                array_elements = array_elements->right;
            }
            currToken++;
        }

        TokenEntry te(T_NUMBER, "0");
        resultNode->left = ASTUtility::createASTNumberNode(te);

        arr_table.insert({ varAssign->key, false });

        ASTUtility::validToken(T_SYMBOL, "]", currToken);


        printContent = ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
        printContent->key = varAssign->key;
    }
    else if (currToken->entry == "\"")
    {
        ASTNode* varAssign = ASTUtility::createASTArrayNode(*(new TokenEntry(TokenType::T_VARIABLE, "RAW_STRING_TEST")));
        ASTUtility::validToken(T_SYMBOL, "\"", currToken);;

        ASTNode* resultNode = ASTUtility::createASTNode(ARRAY_INIT_PRE_ELM, ASTUtility::createASTNullNode(), ASTUtility::createASTNullNode());
        ASTNode* char_elements = resultNode->right;

        while (currToken->entry != "\"")
        {
            char_elements->right = ASTUtility::createASTNumberNode(*(currToken++));
            char_elements = char_elements->right;
        }

        TokenEntry te(T_NUMBER, "0");
        resultNode->left = ASTUtility::createASTNumberNode(te);

        arr_table.insert({ varAssign->key, false });

        ASTUtility::validToken(T_SYMBOL, "\"", currToken);

        printContent = ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
        printContent->key = varAssign->key;
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