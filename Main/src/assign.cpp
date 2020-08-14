#include "parser_comp.h"

ASTNode * Parser::assignment()
{
    return assignment(true);
}

ASTNode * Parser::assignment(bool isUsingSemi)
{
    ASTNode * exp = NULL;
    ASTNode * varAssign = NULL;
    ASTNode * resultNode = NULL;
    ASTType inst = ASSIGN_NONE;
    bool isConstant = false;

    switch(currToken->tType)
    {
        case T_KEYWORD:
            if (currToken->entry != "let" && currToken->entry != "const") {throw ERROR_INVALID_KEYWORD;}
            isConstant = currToken->entry == "const";
            currToken++;
            if (symbol_table.find(currToken->entry) != symbol_table.end() ||
                global_symbol_table.find(currToken->entry) != global_symbol_table.end() || 
                arr_table.find(currToken->entry) != arr_table.end()
            ) {throw ERROR_VAR_UNKNOWN;}
            else if (currToken->tType != T_VARIABLE) { cout << "line 243" << endl; throw ERROR_INVALID_SYMBOL; } 
            varAssign = ASTUtility::createASTVariableNode(*currToken);
            currToken++;
            if (currToken->entry == "[")
            {
                if (isConstant)
                    throw ERROR_VAR_UNKNOWN;
                ASTUtility::validToken(T_SYMBOL, "[", currToken);
                if (currToken->entry != "]")
                {
                    resultNode = ASTUtility::createASTArrayNode(*currToken);
                    currToken++;
                    arr_table.insert(varAssign->key);
                    ASTUtility::validToken(T_SYMBOL, "]", currToken);
                    if (isUsingSemi) {ASTUtility::validToken(T_SYMBOL, ";", currToken);}
                    return ASTUtility::createASTNode(ARRAY_INIT_SIZE, varAssign, resultNode);
                }
                else
                {
                    ASTUtility::validToken(T_SYMBOL, "]", currToken);
                    ASTUtility::validToken(T_SYMBOL, "=", currToken);
                    if (currToken->entry == "[")
                    {
                        ASTUtility::validToken(T_SYMBOL, "[", currToken);
                        resultNode = ASTUtility::createASTNode(ARRAY_INIT_PRE_ELM, ASTUtility::createASTNullNode(), ASTUtility::createASTNullNode());
                        ASTNode * array_elements = resultNode->right;
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
                        arr_table.insert(varAssign->key);
                        ASTUtility::validToken(T_SYMBOL, "]", currToken);;
                        if (isUsingSemi) {ASTUtility::validToken(T_SYMBOL, ";", currToken);;}
                        return ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
                    }
                    else if (currToken->entry == "\"")
                    {
                        ASTUtility::validToken(T_SYMBOL, "\"", currToken);;
                        resultNode = ASTUtility::createASTNode(ARRAY_INIT_PRE_ELM, ASTUtility::createASTNullNode(), ASTUtility::createASTNullNode());
                        ASTNode * char_elements = resultNode->right;
                        while (currToken->entry != "\"")
                        {
                            char_elements->right = ASTUtility::createASTNumberNode(*currToken);
                            char_elements = char_elements->right;
                            currToken++;
                        }
                        TokenEntry te(T_NUMBER, "0");
                        resultNode->left = ASTUtility::createASTNumberNode(te);
                        arr_table.insert(varAssign->key);
                        ASTUtility::validToken(T_SYMBOL, "\"", currToken);
                        if (isUsingSemi) {ASTUtility::validToken(T_SYMBOL, ";", currToken);}
                        return ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
                    }
                    else if (this->isValidFunction())
                    {
                        resultNode = this->functionCall();
                        arr_table.insert(varAssign->key);
                        return ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
                    }
                    else if (arr_table.find(currToken->entry) != arr_table.end())
                    {
                        resultNode = ASTUtility::createASTWholeArrayNode(*currToken);
                        arr_table.insert(varAssign->key);
                        currToken++;
                        if (isUsingSemi) {ASTUtility::validToken(T_SYMBOL, ";", currToken);}
                        return ASTUtility::createASTNode(ARRAY_EXCH, varAssign, resultNode);
                    }
                    else
                    {
                        cout << "line 294" << endl;
                        throw ERROR_INVALID_SYMBOL;
                    }
                }
                arr_table.insert(varAssign->key);
                ASTUtility::validToken(T_SYMBOL, "]", currToken);
                if (isUsingSemi) {ASTUtility::validToken(T_SYMBOL, ";", currToken);}
                return ASTUtility::createASTNode(inst, varAssign, resultNode);
            }
            else
            {
                symbol_table.insert({varAssign->key, isConstant});
            }
            break;
        case T_VARIABLE:
            if (symbol_table.find(currToken->entry) == symbol_table.end() &&
                global_symbol_table.find(currToken->entry) == global_symbol_table.end() && 
                arr_table.find(currToken->entry) == arr_table.end()
            ) {throw ERROR_VAR_UNKNOWN;}
            if (arr_table.find(currToken->entry) != arr_table.end())
            {
                varAssign = ASTUtility::createASTArrayNode(*currToken);
                currToken++;
                ASTUtility::validToken(T_SYMBOL, "[", currToken);
                if (symbol_table.find(currToken->entry) != symbol_table.end() && 
                    global_symbol_table.find(currToken->entry) != global_symbol_table.end()
                )
                {
                    varAssign->key += "[" + currToken->entry + "]";
                }
                else if (stoi(currToken->entry) >= 0)
                {
                    varAssign->key += "[" + currToken->entry + "]";
                }
                else
                {
                    throw ERROR_VAR_UNKNOWN;
                }
                currToken++;
                ASTUtility::validToken(T_SYMBOL, "]", currToken);
                break;
            }
            if (symbol_table[currToken->entry]) {throw ERROR_VAR_UNKNOWN;}
            else if (global_symbol_table[currToken->entry]) {throw ERROR_VAR_UNKNOWN;}
            varAssign = ASTUtility::createASTVariableNode(*currToken);
            currToken++;
            break;
        default:
            cout << "Line 342" << endl;
            throw ERROR_INVALID_SYMBOL;
    }
    
    switch (currToken->entry[0])
    {
        case ';':
            inst = ASSIGN_NONE;
            resultNode = resultNode == nullptr ? ASTUtility::createASTNullNode() : resultNode;
            currToken++;
            return ASTUtility::createASTNode(inst, varAssign, resultNode);
        case '=':
            inst = ASSIGN;
            break;
        case '+':
            inst = ASSIGN_ADD;
            break;
        case '-':
            inst = ASSIGN_MINUS;
            break;
        case '|':
            inst = ASSIGN_OR;
            break;
        case '&':
            inst = ASSIGN_AND;
            break;
        case '/':
            inst = ASSIGN_DIV;
            break;
        case '*':
            inst = ASSIGN_MULTI;
            break;
        case '^':
            inst = ASSIGN_XOR;
            break;
        case '%':
            inst = ASSIGN_MODULUS;
            break;
        case '<':
            if (currToken->entry != "<<=") {throw ERROR_VAR_UNKNOWN;}
            inst = ASSIGN_SHIFT_LEFT;
            break;
        case '>':
            if (currToken->entry != ">>=") {throw ERROR_VAR_UNKNOWN;}
            inst = ASSIGN_SHIFT_RIGHT;
            break;
        default:
            cout << "Line 389" << endl;
            throw ERROR_INVALID_SYMBOL; 
    }

    currToken++;

    if (this->isValidFunction())
    {
        resultNode = this->functionCall();
        isUsingSemi = false;
    }
    else
    {
        resultNode = this->expression();
    }

    if (isUsingSemi) {ASTUtility::validToken(T_SYMBOL, ";", currToken);}
    
    return ASTUtility::createASTNode(inst, varAssign, resultNode);
}

ASTNode * Parser::globalAssignment()
{
    ASTNode * varAssign;
    bool isConstant = false;
    ASTNode * resultNode = NULL;
    string tmpEntry;
    ASTType inst = ASSIGN_NONE;

    switch (currToken->tType)
    {
        case T_KEYWORD:
            if (currToken->entry != "let" && currToken->entry != "const") {throw ERROR_INVALID_KEYWORD;}
            isConstant = currToken->entry == "const";
            currToken++;
            if (global_symbol_table.find(currToken->entry) != global_symbol_table.end() || 
                arr_table.find(currToken->entry) != arr_table.end()) 
            {throw ERROR_VAR_UNKNOWN;}
            if (currToken->tType != T_VARIABLE) { cout << "line 176" << endl; throw ERROR_INVALID_SYMBOL;}
            varAssign = ASTUtility::createASTVariableNode(*currToken);
            currToken++;
            if (currToken->entry == "[")
            {
                if (isConstant)
                    throw ERROR_VAR_UNKNOWN;
                ASTUtility::validToken(T_SYMBOL, "[", currToken);
                if (currToken->entry != "]")
                {
                    resultNode = ASTUtility::createASTArrayNode(*currToken);
                    currToken++;
                    arr_table.insert(varAssign->key);
                    ASTUtility::validToken(T_SYMBOL, "]", currToken);
                    ASTUtility::validToken(T_SYMBOL, ";", currToken);
                    return ASTUtility::createASTNode(ARRAY_INIT_SIZE, varAssign, resultNode);
                }
                else
                {
                    ASTUtility::validToken(T_SYMBOL, "]", currToken);
                    ASTUtility::validToken(T_SYMBOL, "=", currToken);
                    if (currToken->entry == "[")
                    {
                        ASTUtility::validToken(T_SYMBOL, "[", currToken);
                        resultNode = ASTUtility::createASTNode(ARRAY_INIT_PRE_ELM, ASTUtility::createASTNullNode(), ASTUtility::createASTNullNode());
                        ASTNode * array_elements = resultNode->right;
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
                        arr_table.insert(varAssign->key);
                        ASTUtility::validToken(T_SYMBOL, "]", currToken);
                        ASTUtility::validToken(T_SYMBOL, ";", currToken);
                        return ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
                    }
                    else if (currToken->entry == "\"")
                    {
                        ASTUtility::validToken(T_SYMBOL, "\"", currToken);
                        resultNode = ASTUtility::createASTNode(ARRAY_INIT_PRE_ELM, ASTUtility::createASTNullNode(), ASTUtility::createASTNullNode());
                        ASTNode * char_elements = resultNode->right;
                        while (currToken->entry != "\"")
                        {
                            char_elements->right = ASTUtility::createASTNumberNode(*currToken);
                            char_elements = char_elements->right;
                            currToken++;
                        }
                        TokenEntry te(T_NUMBER, "0");
                        resultNode->left = ASTUtility::createASTNumberNode(te);
                        arr_table.insert(varAssign->key);
                        ASTUtility::validToken(T_SYMBOL, "\"", currToken);
                        ASTUtility::validToken(T_SYMBOL, ";", currToken);
                        return ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
                    }
                    else if (this->isValidFunction())
                    {
                        resultNode = this->functionCall();
                        arr_table.insert(varAssign->key);
                        return ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
                    }
                    else if (arr_table.find(currToken->entry) != arr_table.end())
                    {
                        resultNode = ASTUtility::createASTWholeArrayNode(*currToken);
                        arr_table.insert(varAssign->key);
                        currToken++;
                        ASTUtility::validToken(T_SYMBOL, ";", currToken);
                        return ASTUtility::createASTNode(ARRAY_EXCH, varAssign, resultNode);
                    }
                    else
                    {
                        cout << "line 294" << endl;
                        throw ERROR_INVALID_SYMBOL;
                    }
                }
                arr_table.insert(varAssign->key);
                ASTUtility::validToken(T_SYMBOL, "]", currToken);
                ASTUtility::validToken(T_SYMBOL, ";", currToken);
                return ASTUtility::createASTNode(inst, varAssign, resultNode);
            }
            else
            {
                global_symbol_table.insert({(currToken - 1)->entry, isConstant});
            }
            break;
        default:
            cout << "line 196" << endl;
            throw ERROR_INVALID_SYMBOL;
    }

    ASTUtility::validToken(T_SYMBOL, "=", currToken);

    if (this->isValidFunction())
    {
        resultNode = this->functionCall();
    }
    else
    {
        resultNode = this->expression();
        ASTUtility::validToken(T_SYMBOL, ";", currToken);
    }

    return ASTUtility::createASTNode(ASSIGN, varAssign, resultNode);
}

void Parser::stmtList(ASTNode * root)
{
    ASTNode * stmtList = root;
    ASTNode * stmt;
    while (currToken->tType != T_SYMBOL && currToken->entry != "break")
    {
        if ((currToken->entry == "let" && currToken->tType == T_KEYWORD) || 
            (currToken->entry == "const" && currToken->tType == T_KEYWORD) || 
            symbol_table.find(currToken->entry) != symbol_table.end() ||
            arr_table.find(currToken->entry) != arr_table.end() || 
            global_symbol_table.find(currToken->entry) !=  global_symbol_table.end()
        )
        {
            stmt = this->assignment();
        }
        else if (currToken->entry == "if" && currToken->tType == T_KEYWORD)
        {
            stmt = this->ifStatemnt();
        }
        else if (currToken->entry == "while" && currToken->tType == T_KEYWORD)
        {
            stmt = this->whileLoop();
        }
        else if (currToken->entry == "for" && currToken->tType == T_KEYWORD)
        {
            stmt = this->forLoop();
        }
        else if (currToken->entry == "PRINT" && currToken->tType == T_KEYWORD)
        {
            stmt = this->print();
        }
        else if (currToken->entry == "SCAN" && currToken->tType == T_KEYWORD)
        {
            stmt = this->scan();
        }
        else if (currToken->entry == "COPY" && currToken->tType == T_KEYWORD)
        {
            stmt = this->copy();
        }
        else if (currToken->entry == "LENGTH" && currToken->tType == T_KEYWORD)
        {
            stmt = this->length();
        }
        else if (currToken->entry == "EQUAL" && currToken->tType == T_KEYWORD)
        {
            stmt = this->equal();
        }
        else if (currToken->entry == "switch" && currToken->tType == T_KEYWORD)
        {
            stmt = this->switchStatement();
        }
        else if (this->isValidFunction())
        {
            stmt = this->functionCall();
        }
        else if (currToken->entry == "return" && currToken->tType == T_KEYWORD)
        {
            stmt = this->returnCall();
        }
        else if (currToken->entry == "delete" && currToken->tType == T_KEYWORD)
        {
            stmt = this->deleteStatement();
        }
        else
        {
            throw ERROR_INVALID_KEYWORD;
        }
        
        stmtList->right = ASTUtility::createASTNode(STMTLIST, stmt, NULL);
        stmtList = stmtList->right;
    }
}