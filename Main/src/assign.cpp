#include "parser_comp.h"

ASTNode * Parser::assignment()
{
    return assignment(true);
}

ASTNode * Parser::assignment(bool isUsingSemi)
{
    ASTNode * varAssign = NULL;
    ASTNode * resultNode = NULL;
    ASTType inst = ASSIGN_NONE;
    bool isConstant = false;

    switch(currToken->tType)
    {
        case T_KEYWORD:
            if (currToken->entry != "let" && currToken->entry != "const")
            {
                throw (int) ErrorInvalidKeyword;
            }
            isConstant = (currToken++)->entry == "const";

            if (symbol_table.find(currToken->entry) != symbol_table.end() ||
                global_symbol_table.find(currToken->entry) != global_symbol_table.end() || 
                arr_table.find(currToken->entry) != arr_table.end() ) 
            {
                throw (int) ErrorVariableUnknown;
            }
            else if (currToken->tType != T_VARIABLE) 
            { 
                throw (int) ErrorInvalidSymbol; 
            } 
            
            varAssign = ASTUtility::createASTVariableNode(*(currToken++));
            
            if (currToken->entry == "[")
            {
                ASTUtility::validToken(T_SYMBOL, "[", currToken);

                if (currToken->entry != "]")
                {
                    resultNode = ASTUtility::createASTArrayNode(*(currToken++));

                    arr_table.insert({ varAssign->key, isConstant });
                    
                    ASTUtility::validToken(T_SYMBOL, "]", currToken);
                    if (isUsingSemi) 
                    {
                        ASTUtility::validToken(T_SYMBOL, ";", currToken);
                    }

                    return ASTUtility::createASTNode(ARRAY_INIT_SIZE, varAssign, resultNode);
                }
                
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
                    
                    arr_table.insert({ varAssign->key, isConstant });
                    ASTUtility::validToken(T_SYMBOL, "]", currToken);;
                    
                    if (isUsingSemi)
                    {
                        ASTUtility::validToken(T_SYMBOL, ";", currToken);
                    }
                    
                    return ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
                }
                else if (currToken->entry == "\"")
                {
                    ASTUtility::validToken(T_SYMBOL, "\"", currToken);;
                    
                    resultNode = ASTUtility::createASTNode(ARRAY_INIT_PRE_ELM, ASTUtility::createASTNullNode(), ASTUtility::createASTNullNode());
                    ASTNode * char_elements = resultNode->right;
                    
                    while (currToken->entry != "\"")
                    {
                        char_elements->right = ASTUtility::createASTNumberNode(*(currToken++));
                        char_elements = char_elements->right;
                    }

                    TokenEntry te(T_NUMBER, "0");
                    resultNode->left = ASTUtility::createASTNumberNode(te);
                    
                    arr_table.insert({ varAssign->key, isConstant });
                    ASTUtility::validToken(T_SYMBOL, "\"", currToken);
                    
                    if (isUsingSemi)
                    {
                        ASTUtility::validToken(T_SYMBOL, ";", currToken);
                    }
                    
                    return ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
                }
                else if (this->isValidFunction())
                {
                    resultNode = this->functionCall();
                    
                    arr_table.insert({ varAssign->key, isConstant });
                    
                    return ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
                }
                else if (arr_table.find(currToken->entry) != arr_table.end())
                {
                    resultNode = ASTUtility::createASTWholeArrayNode(*(currToken++));
                    
                    arr_table.insert({ varAssign->key, isConstant });
                    
                    if (isUsingSemi)
                    {
                        ASTUtility::validToken(T_SYMBOL, ";", currToken);
                    }

                    return ASTUtility::createASTNode(ARRAY_EXCH, varAssign, resultNode);
                }
                else
                {
                    throw (int) ErrorInvalidSymbol;
                }
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
            ) 
            {
                throw (int) ErrorVariableUnknown;
            }
            if (arr_table.find(currToken->entry) != arr_table.end())
            {
                varAssign = ASTUtility::createASTArrayNode(*currToken);
                
                if (arr_table[(currToken++)->entry]) 
                { 
                    throw (int)ErrorInvalidConstant; 
                }
                
                ASTUtility::validToken(T_SYMBOL, "[", currToken);

                if (symbol_table.find(currToken->entry) != symbol_table.end() && 
                    global_symbol_table.find(currToken->entry) != global_symbol_table.end())
                {
                    varAssign->key += "[" + (currToken++)->entry + "]";
                }
                else if (!isdigit(currToken->entry[0]))
                {
                    ASTUtility::validToken(T_SYMBOL, "]", currToken);
                    ASTUtility::validToken(T_SYMBOL, "=", currToken);

                    if (currToken->entry == "[")
                    {
                        ASTUtility::validToken(T_SYMBOL, "[", currToken);
                        
                        resultNode = ASTUtility::createASTNode(ARRAY_INIT_PRE_ELM, ASTUtility::createASTNullNode(), ASTUtility::createASTNullNode());
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
                        
                        arr_table.insert({ varAssign->key, isConstant });
                        
                        ASTUtility::validToken(T_SYMBOL, "]", currToken);;
                        
                        if (isUsingSemi) 
                        { 
                            ASTUtility::validToken(T_SYMBOL, ";", currToken); 
                        }
                        
                        return ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
                    }
                    else if (currToken->entry == "\"")
                    {
                        ASTUtility::validToken(T_SYMBOL, "\"", currToken);;
                        
                        resultNode = ASTUtility::createASTNode(ARRAY_INIT_PRE_ELM, ASTUtility::createASTNullNode(), ASTUtility::createASTNullNode());
                        ASTNode* char_elements = resultNode->right;
                        
                        while (currToken->entry != "\"")
                        {
                            char_elements->right = ASTUtility::createASTNumberNode(*currToken);
                            char_elements = char_elements->right;
                            currToken++;
                        }
                        
                        TokenEntry te(T_NUMBER, "0");
                        resultNode->left = ASTUtility::createASTNumberNode(te);
                        
                        arr_table.insert({ varAssign->key, isConstant });
                        
                        ASTUtility::validToken(T_SYMBOL, "\"", currToken);
                        
                        if (isUsingSemi) 
                        { 
                            ASTUtility::validToken(T_SYMBOL, ";", currToken); 
                        }
                        
                        return ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
                    }
                    else if (this->isValidFunction())
                    {
                        resultNode = this->functionCall();

                        arr_table.insert({ varAssign->key, isConstant });
                        
                        return ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
                    }
                    else if (arr_table.find(currToken->entry) != arr_table.end())
                    {
                        resultNode = ASTUtility::createASTWholeArrayNode(*(currToken++));

                        arr_table.insert({ varAssign->key, isConstant });
                        
                        if (isUsingSemi) 
                        { 
                            ASTUtility::validToken(T_SYMBOL, ";", currToken); 
                        }
                        
                        return ASTUtility::createASTNode(ARRAY_EXCH, varAssign, resultNode);
                    }
                    else
                    {
                        throw (int)ErrorInvalidSymbol;
                    }
                }
                else if (stoi(currToken->entry) >= 0)
                {
                    varAssign->key += "[" + (currToken++)->entry + "]";
                }
                else
                {
                    throw (int) ErrorVariableUnknown;
                }

                ASTUtility::validToken(T_SYMBOL, "]", currToken);
                
                break;
            }
            if (symbol_table[currToken->entry]) 
            {
                throw (int) ErrorInvalidConstant;
            }
            else if (global_symbol_table[currToken->entry]) 
            {
                throw (int)ErrorInvalidConstant;
            }
           
            varAssign = ASTUtility::createASTVariableNode(*(currToken++));

            break;
        default:
            throw (int) ErrorInvalidSymbol;
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
            if (currToken->entry != "<<=") 
            {
                throw (int) ErrorVariableUnknown;
            }
            inst = ASSIGN_SHIFT_LEFT;
            break;
        case '>':
            if (currToken->entry != ">>=") 
            {
                throw (int) ErrorVariableUnknown;
            }
            inst = ASSIGN_SHIFT_RIGHT;
            break;
        default:
            throw (int) ErrorInvalidSymbol; 
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

    if (isUsingSemi) 
    {
        ASTUtility::validToken(T_SYMBOL, ";", currToken);
    }
    
    return ASTUtility::createASTNode(inst, varAssign, resultNode);
}

ASTNode * Parser::globalAssignment()
{
    ASTNode * varAssign;
    bool isConstant = false;
    ASTNode * resultNode = NULL;
    string tmpEntry;

    switch (currToken->tType)
    {
        case T_KEYWORD:
            if (currToken->entry != "let" && currToken->entry != "const") 
            {
                throw (int) ErrorInvalidKeyword;
            }
            
            isConstant = (currToken++)->entry == "const";
            
            if (global_symbol_table.find(currToken->entry) != global_symbol_table.end() || 
                arr_table.find(currToken->entry) != arr_table.end()) 
            {
                throw (int) ErrorVariableUnknown;
            }
            else if (currToken->tType != T_VARIABLE) 
            { 
                throw (int) ErrorInvalidSymbol;
            }
            
            varAssign = ASTUtility::createASTVariableNode(*(currToken++));
            
            if (currToken->entry == "[")
            {
                if (isConstant)
                    throw (int) ErrorVariableUnknown;
                
                ASTUtility::validToken(T_SYMBOL, "[", currToken);
                
                if (currToken->entry != "]")
                {
                    resultNode = ASTUtility::createASTArrayNode(*(currToken++));
                    
                    arr_table.insert({ varAssign->key, isConstant });
                    
                    ASTUtility::validToken(T_SYMBOL, "]", currToken);
                    ASTUtility::validToken(T_SYMBOL, ";", currToken);
                    
                    return ASTUtility::createASTNode(ARRAY_INIT_SIZE, varAssign, resultNode);
                }
                
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
                    
                    arr_table.insert({ varAssign->key, isConstant });
                    
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
                        char_elements->right = ASTUtility::createASTNumberNode(*(currToken++));
                        char_elements = char_elements->right;
                    }
                    
                    TokenEntry te(T_NUMBER, "0");
                    resultNode->left = ASTUtility::createASTNumberNode(te);
                    
                    arr_table.insert({ varAssign->key, isConstant });
                    
                    ASTUtility::validToken(T_SYMBOL, "\"", currToken);
                    ASTUtility::validToken(T_SYMBOL, ";", currToken);
                    
                    return ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
                }
                else if (this->isValidFunction())
                {
                    resultNode = this->functionCall();
                    
                    arr_table.insert({ varAssign->key, isConstant });
                    
                    return ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
                }
                else if (arr_table.find(currToken->entry) != arr_table.end())
                {
                    resultNode = ASTUtility::createASTWholeArrayNode(*(currToken++));
                    
                    arr_table.insert({ varAssign->key, isConstant });

                    ASTUtility::validToken(T_SYMBOL, ";", currToken);
                    return ASTUtility::createASTNode(ARRAY_EXCH, varAssign, resultNode);
                }
                else
                {
                    throw (int) ErrorInvalidSymbol;
                }
            }
            else
            {
                global_symbol_table.insert({(currToken - 1)->entry, isConstant});
            }
            break;
        default:
            throw (int) ErrorInvalidSymbol;
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
            throw (int) ErrorInvalidKeyword;
        }
        
        stmtList->right = ASTUtility::createASTNode(STMTLIST, stmt, NULL);
        stmtList = stmtList->right;
    }
}
