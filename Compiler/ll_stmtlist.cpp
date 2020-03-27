#include "compiler_comp.hpp"

void LLParser::StmtList(ASTNode * root)
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
            stmt = Assignment();
        }
        else if (currToken->entry == "if" && currToken->tType == T_KEYWORD)
        {
            stmt = IfStatemnt();
        }
        else if (currToken->entry == "while" && currToken->tType == T_KEYWORD)
        {
            stmt = WhileLoop();
        }
        else if (currToken->entry == "for" && currToken->tType == T_KEYWORD)
        {
            stmt = ForLoop();
        }
        else if (currToken->entry == "PRINT" && currToken->tType == T_KEYWORD)
        {
            stmt = Print();
        }
        else if (currToken->entry == "switch" && currToken->tType == T_KEYWORD)
        {
            stmt = SwitchStatement();
        }
        else if (IsValidFunction())
        {
            stmt = FunctionCall();
        }
        else if (currToken->entry == "return" && currToken->tType == T_KEYWORD)
        {
            stmt = ReturnCall();
        }
        else if (currToken->entry == "delete" && currToken->tType == T_KEYWORD)
        {
            stmt = DeleteStatement();
        }
        else
        {
            throw ERROR_INVALID_KEYWORD;
        }
        
        stmtList->right = createASTNode(STMTLIST, stmt, NULL);
        stmtList = stmtList->right;
    }
}

ASTNode * LLParser::ReturnCall()
{
    ASTNode * returnNode = createASTNode(UNKNOWN, NULL, NULL);
    
    validToken(T_KEYWORD, "return");
    if (this->IsValidFunction())
    {
        returnNode = FunctionCall();
    }
    else if (currToken->tType == T_VARIABLE || currToken->tType == T_NUMBER)
    {
        returnNode = Expression();
        validToken(T_SYMBOL, ";");
    }
    else
    {
        returnNode = createASTNullNode();
        validToken(T_SYMBOL, ";");
    }
    
    return createASTNode(arr_table.find(returnNode->key) == arr_table.end() ? RETURN : RETURN_ARR, returnNode, NULL);
}

bool LLParser::IsValidFunction()
{
    string tokenInstance = currToken->entry;
    if (tokenInstance.length() <= 1) return false;
    for (int i = 0; i < tokenInstance.length(); i++)
    {
        if ((tokenInstance[i] >= 'a' && tokenInstance[i] <= 'z') || 
            isdigit(tokenInstance[i]) || 
            tokenInstance[i] == '\'' || 
            tokenInstance[i] == '\"')
        {
            return false;
        }
    }
    return true;
}

ASTNode * LLParser::FunctionCall()
{
    ASTNode * functionCall = createASTNode(FUNCTION_CALL, createASTNode(UNKNOWN, NULL, NULL), NULL);
    ASTNode * stmtList = functionCall->left;
    functionCall->key = currToken->entry;
    currToken++;
    validToken(T_SYMBOL, "(");
    int count = 0;
    const string paramArrayTitle = "PARAM_ARRAY_";
    while (currToken->entry != ")")
    {
        
        if (currToken->entry != "," && currToken->entry != "\'" && currToken->entry != "\"")
        {
            ASTType ast_type = arr_table.find(currToken->entry) != arr_table.end() ? PARAM_ARRAY_CALL : PARAM_CALL;
            stmtList->right = createASTNode(ast_type, createASTVariableNode(*currToken), NULL);
            stmtList = stmtList->right;
        }
        else if (currToken->entry == "\"")
        {
            validToken(T_SYMBOL, "\"");
            TokenEntry variableName(T_VARIABLE, paramArrayTitle + to_string(count));
            ASTNode * varAssign = createASTVariableNode(variableName);
            ASTNode * resultNode = createASTNode(ARRAY_INIT_PRE_ELM, createASTNullNode(), createASTNullNode());
            ASTNode * char_elements = resultNode->right;
            while (currToken->entry != "\"")
            {
                char_elements->right = createASTNumberNode(*currToken);
                char_elements = char_elements->right;
                currToken++;
            }
            TokenEntry te(T_NUMBER, "0");
            resultNode->left = createASTNumberNode(te);
            validToken(T_SYMBOL, "\"");
            ASTNode * rawStringNode = createASTNode(ARRAY_INIT, varAssign, resultNode);
            rawStringNode->key = paramArrayTitle + to_string(count++);
            stmtList->right = createASTNode(PARAM_ARRAY_CALL, rawStringNode , createASTNullNode());
            stmtList = stmtList->right;
        }
        currToken++;
    }
    validToken(T_SYMBOL, ")");
    validToken(T_SYMBOL, ";");
    return functionCall;
}

ASTNode * LLParser::Print()
{ 
    ASTNode * printContent;
    ASTNode * printMode;
    validToken(T_SYMBOL, "PRINT");
    validToken(T_SYMBOL, "(");
    printContent = Expression();
    validToken(T_SYMBOL, ",");
    switch (currToken->tType)
    {
        case T_VARIABLE:
            printMode = createASTVariableNode(*currToken);
            break;
        case T_NUMBER:
            printMode = createASTNumberNode(*currToken);
            break; 
        default:
            cout << "line 140" << endl;
            throw ERROR_INVALID_SYMBOL;
    }
    currToken++;
    validToken(T_SYMBOL, ")");
    validToken(T_SYMBOL, ";");

    return createASTNode(PRINT, printContent, printMode);
}

ASTNode * LLParser::DeleteStatement()
{
    ASTNode * delVariable;
    validToken(T_SYMBOL, "delete");
    if (symbol_table.find(currToken->entry) != symbol_table.end())
    {
        symbol_table.erase(currToken->entry);
        delVariable = createASTVariableNode(*currToken);
        currToken++;
    }
    else if (arr_table.find(currToken->entry) != arr_table.end())
    {
        arr_table.erase(currToken->entry);
        delVariable = createASTVariableNode(*currToken);
        currToken++;
    }
    else
    {
        throw ERROR_VAR_UNKNOWN;
    }
    validToken(T_SYMBOL, ";");
    return createASTNode(DELETE, delVariable, NULL);
}

ASTNode * LLParser::GlobalAssignment()
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
            varAssign = createASTVariableNode(*currToken);
            currToken++;
            if (currToken->entry == "[")
            {
                validToken(T_SYMBOL, "[");
                resultNode = createASTArrayNode(*currToken);
                arr_table.insert(varAssign->key);
                inst = ARRAY_INIT;
                currToken++;
                validToken(T_SYMBOL, "]");
                validToken(T_SYMBOL, ";");
                return createASTNode(inst, varAssign, resultNode);
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

    validToken(T_SYMBOL, "=");

    if (this->IsValidFunction())
    {
        resultNode = FunctionCall();
    }
    else
    {
        resultNode = Expression();
        validToken(T_SYMBOL, ";");
    }

    return createASTNode(ASSIGN, varAssign, resultNode);
}

ASTNode * LLParser::Assignment()
{
    return Assignment(true);
}

ASTNode * LLParser::Assignment(bool isUsingSemi)
{
    ASTNode * exp;
    ASTNode * varAssign;
    ASTNode * resultNode = NULL;
    ASTType inst = ASSIGN_NONE;
    bool isConstant = false;

    switch(currToken->tType)
    {
        case T_KEYWORD:
            if (currToken->entry != "let" && currToken->entry != "const") {throw ERROR_INVALID_KEYWORD;}
            isConstant = currToken->entry == "const";
            currToken++;
            if ((symbol_table.find(currToken->entry) != symbol_table.end() &&
                global_symbol_table.find(currToken->entry) != global_symbol_table.end()) || 
                arr_table.find(currToken->entry) != arr_table.end()
            ) {throw ERROR_VAR_UNKNOWN;}
            else if (currToken->tType != T_VARIABLE) { cout << "line 243" << endl; throw ERROR_INVALID_SYMBOL; } 
            varAssign = createASTVariableNode(*currToken);
            currToken++;
            if (currToken->entry == "[")
            {
                validToken(T_SYMBOL, "[");
                if (currToken->entry != "]")
                {
                    resultNode = createASTArrayNode(*currToken);
                    currToken++;
                    arr_table.insert(varAssign->key);
                    validToken(T_SYMBOL, "]");
                    if (isUsingSemi) {validToken(T_SYMBOL, ";");}
                    return createASTNode(ARRAY_INIT_SIZE, varAssign, resultNode);
                }
                else
                {
                    validToken(T_SYMBOL, "]");
                    validToken(T_SYMBOL, "=");
                    if (currToken->entry == "[")
                    {
                        validToken(T_SYMBOL, "[");
                        resultNode = createASTNode(ARRAY_INIT_PRE_ELM, createASTNullNode(), createASTNullNode());
                        ASTNode * array_elements = resultNode->right;
                        while (currToken->entry != "]")
                        {
                            if (currToken->entry != ",")
                            {
                                array_elements->right = createASTNumberNode(*currToken);
                                array_elements = array_elements->right;
                            }
                            currToken++;
                        }
                        TokenEntry te(T_NUMBER, "0");
                        resultNode->left = createASTNumberNode(te);
                        arr_table.insert(varAssign->key);
                        validToken(T_SYMBOL, "]");
                        if (isUsingSemi) {validToken(T_SYMBOL, ";");}
                        return createASTNode(ARRAY_INIT, varAssign, resultNode);
                    }
                    else if (currToken->entry == "\"")
                    {
                        validToken(T_SYMBOL, "\"");
                        resultNode = createASTNode(ARRAY_INIT_PRE_ELM, createASTNullNode(), createASTNullNode());
                        ASTNode * char_elements = resultNode->right;
                        while (currToken->entry != "\"")
                        {
                            char_elements->right = createASTNumberNode(*currToken);
                            char_elements = char_elements->right;
                            currToken++;
                        }
                        TokenEntry te(T_NUMBER, "0");
                        resultNode->left = createASTNumberNode(te);
                        arr_table.insert(varAssign->key);
                        validToken(T_SYMBOL, "\"");
                        if (isUsingSemi) {validToken(T_SYMBOL, ";");}
                        return createASTNode(ARRAY_INIT, varAssign, resultNode);
                    }
                    else if (this->IsValidFunction())
                    {
                        resultNode = FunctionCall();
                        arr_table.insert(varAssign->key);
                        return createASTNode(ARRAY_INIT, varAssign, resultNode);
                    }
                    else if (arr_table.find(currToken->entry) != arr_table.end())
                    {
                        resultNode = createASTWholeArrayNode(*currToken);
                        arr_table.insert(varAssign->key);
                        currToken++;
                        if (isUsingSemi) {validToken(T_SYMBOL, ";");}
                        return createASTNode(ARRAY_EXCH, varAssign, resultNode);
                    }
                    else
                    {
                        cout << "line 294" << endl;
                        throw ERROR_INVALID_SYMBOL;
                    }
                }
                arr_table.insert(varAssign->key);
                validToken(T_SYMBOL, "]");
                if (isUsingSemi) {validToken(T_SYMBOL, ";");}
                return createASTNode(inst, varAssign, resultNode);
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
                varAssign = createASTArrayNode(*currToken);
                currToken++;
                validToken(T_SYMBOL, "[");
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
                validToken(T_SYMBOL, "]");
                break;
            }
            if (symbol_table[currToken->entry]) {throw ERROR_VAR_UNKNOWN;}
            else if (global_symbol_table[currToken->entry]) {throw ERROR_VAR_UNKNOWN;}
            varAssign = createASTVariableNode(*currToken);
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
            resultNode = resultNode == nullptr ? createASTNullNode() : resultNode;
            currToken++;
            return createASTNode(inst, varAssign, resultNode);
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

    if (this->IsValidFunction())
    {
        resultNode = FunctionCall();
        isUsingSemi = false;
    }
    else
    {
        resultNode = Expression();
    }

    if (isUsingSemi) {validToken(T_SYMBOL, ";");}
    
    return createASTNode(inst, varAssign, resultNode);
}

ASTNode * LLParser::SwitchStatement()
{
    ASTNode * conditionalResults;
    ASTNode * stmtResults = createASTNode(UNKNOWN, NULL, NULL);

    validToken(T_KEYWORD, "switch");
    validToken(T_SYMBOL, "(");
    if (symbol_table.find(currToken->entry) == symbol_table.end() && 
        global_symbol_table.find(currToken->entry) == global_symbol_table.end()
    ) {throw ERROR_VAR_UNKNOWN;}
    conditionalResults = createASTNode(SWITCH_COND, createASTVariableNode(*currToken), NULL);
    currToken++;
    validToken(T_SYMBOL, ")");
    validToken(T_SYMBOL, "{");
    if (currToken->entry == "case")
    {
        CaseStatemnt(conditionalResults->right, stmtResults);
    }
    validToken(T_SYMBOL, "}");

    return createASTNode(SWITCH_CMPR, conditionalResults, stmtResults);
}

void LLParser::CaseStatemnt(ASTNode *& conditionalResults, ASTNode *& stmtResults)
{
    ASTNode * previousStmt = stmtResults;
    if (currToken->entry == "case")
    {
        validToken(T_KEYWORD, "case");
        if (currToken->tType == T_NUMBER && case_set.find(currToken->entry) == case_set.end())
        {
            case_set.insert(currToken->entry);
            conditionalResults = createASTNode(CASE_COND, createASTNumberNode(*currToken), NULL);
            currToken++;
        }
        else
        {
            cout << "line 447" << endl;
            throw ERROR_INVALID_SYMBOL;
        }
        validToken(T_SYMBOL, ":");
        stmtResults = createASTNode(CASE_STMT, previousStmt, createASTNode(UNKNOWN, NULL, NULL));
        StmtList(stmtResults->right);
        validToken(T_KEYWORD, "break");
        validToken(T_SYMBOL, ";");
        if (currToken->entry == "default" || currToken->entry == "case")
        {
            CaseStatemnt(conditionalResults->right, stmtResults);
        }
        else
        {
            conditionalResults->right = createASTNode(SWITCH_END, NULL, NULL);
            case_set.clear();
        }
    }
    else if (currToken->entry == "default")
    {
        stmtResults = createASTNode(DEFAULT_STMT, previousStmt, createASTNode(UNKNOWN, NULL, NULL));
        validToken(T_KEYWORD, "default");
        conditionalResults = createASTNode(DEFAULT_COND, NULL, NULL);
        validToken(T_SYMBOL, ":");
        StmtList(stmtResults->right);
        validToken(T_KEYWORD, "break");
        validToken(T_SYMBOL, ";");
        conditionalResults->right = createASTNode(SWITCH_END, NULL, NULL);
    }
    else
    {
        if (currToken->tType == T_NUMBER)
        {
            conditionalResults = createASTNode(CASE_COND, createASTNumberNode(*currToken), NULL);
            currToken++;
        }
        else
        {
            cout << "line 485" << endl;
            throw ERROR_INVALID_SYMBOL;
        }
        validToken(T_SYMBOL, ":");
        if (currToken->entry == "case")
        {
            validToken(T_KEYWORD, "case");
            CaseStatemnt(conditionalResults->right, stmtResults);
        }
    }
}

ASTNode * LLParser::IfStatemnt()
{
    ASTNode * conditionalResults;
    ASTNode * stmtResults = createASTNode(IF_STMT, NULL, createASTNode(UNKNOWN, NULL, NULL));
    map<string, bool> placeholder_symbol_table = symbol_table;
    set<string> placeholder_array_table = arr_table;

    validToken(T_KEYWORD, "if");
    validToken(T_SYMBOL, "(");
    conditionalResults = createASTNode(IF_COND, Expression(), NULL);
    validToken(T_SYMBOL, ")");
    validToken(T_SYMBOL, "{");
    StmtList(stmtResults->right);
    validToken(T_SYMBOL, "}");

    if (currToken->entry == "elif" || currToken->entry == "else")
    {
        ElseOrElseIfStatemnt(conditionalResults->right, stmtResults);
    }
    arr_table.clear();
    symbol_table.clear();
    symbol_table = placeholder_symbol_table;
    arr_table = placeholder_array_table;
    return createASTNode(COND_CMPR, conditionalResults, stmtResults);
}

void LLParser::ElseOrElseIfStatemnt(ASTNode *& conditionalResults, ASTNode *& stmtResults)
{
    ASTNode * previousStmt = stmtResults;
    map<string, bool> placeholder_symbol_table = symbol_table;
    set<string> placeholder_arr_table = arr_table;

    if (currToken->entry == "elif")
    {
        stmtResults = createASTNode(ELIF_STMT, previousStmt, createASTNode(UNKNOWN, NULL, NULL));

        validToken(T_KEYWORD, "elif");
        validToken(T_SYMBOL, "(");
        conditionalResults = createASTNode(ELIF_COND, Expression(), NULL);
        validToken(T_SYMBOL, ")");
        validToken(T_SYMBOL, "{");
        StmtList(stmtResults->right);
        validToken(T_SYMBOL, "}");
        symbol_table.clear();
        arr_table.clear();
        symbol_table = placeholder_symbol_table;
        arr_table = placeholder_arr_table;

        ElseOrElseIfStatemnt(conditionalResults->right, stmtResults);
    }
    else if (currToken->entry == "else")
    {
        stmtResults = createASTNode(ELSE_STMT, previousStmt, createASTNode(UNKNOWN, NULL, NULL));
        conditionalResults = createASTNode(ELSE_COND, NULL, NULL);
        validToken(T_KEYWORD, "else");
        validToken(T_SYMBOL, "{");
        StmtList(stmtResults->right);
        validToken(T_SYMBOL, "}");
        symbol_table.clear();
        arr_table.clear();
        symbol_table = placeholder_symbol_table;
        arr_table = placeholder_arr_table;
    }
}

ASTNode * LLParser::WhileLoop()
{
    ASTNode * conditionalResults;
    ASTNode * conditionalResults_P;
    ASTNode * stmtResults = createASTNode(WHILE_LOOP_STATEMENT, NULL, createASTNode(UNKNOWN, NULL, NULL));
    map<string, bool> placeholder_symbol_table = symbol_table;
    set<string> placeholder_array_table = arr_table;

    validToken(T_KEYWORD, "while");
    validToken(T_SYMBOL, "(");
    conditionalResults = createASTNode(WHILE_LOOP_COND, Expression(), NULL);
    validToken(T_SYMBOL, ")");
    validToken(T_SYMBOL, "{");
    StmtList(stmtResults->right);
    validToken(T_SYMBOL, "}");

    conditionalResults_P = createASTNode(WHILE_LOOP_COND_P, conditionalResults->left, NULL);
    arr_table.clear();
    symbol_table.clear();
    symbol_table = placeholder_symbol_table;
    arr_table = placeholder_array_table;

    return createASTNode(WHILE_LOOP, conditionalResults, createASTNode(WHILE_LOOP_EXIT, stmtResults, conditionalResults_P));
}

ASTNode * LLParser::ForLoop()
{
    ASTNode * conditionalResults;
    ASTNode * conditionalResults_P;
    ASTNode * stmtResults = createASTNode(FOR_LOOP_STATEMENT, NULL, createASTNode(UNKNOWN, NULL, NULL));
    map<string, bool> placeholder_symbol_table = symbol_table;

    validToken(T_KEYWORD, "for");
    validToken(T_SYMBOL, "(");
    ASTNode * assignmentResult = Assignment();
    ASTNode * tempConditionResult = Expression();
    validToken(T_SYMBOL, ";");
    ASTNode * updateResult = Assignment(false);
    validToken(T_SYMBOL, ")");
    validToken(T_SYMBOL, "{");
    StmtList(stmtResults->right);
    validToken(T_SYMBOL, "}");
    conditionalResults = createASTNode(FOR_LOOP_COND, assignmentResult, tempConditionResult);

    conditionalResults_P = createASTNode(FOR_LOOP_COND_P, updateResult, tempConditionResult);
    symbol_table.clear();
    symbol_table = placeholder_symbol_table;

    return createASTNode(FOR_LOOP, conditionalResults, createASTNode(FOR_LOOP_EXIT, stmtResults, conditionalResults_P));
}