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
    if (currToken->tType == T_VARIABLE)
    {
        returnNode = createASTVariableNode(*currToken);
        currToken++;
    }   
    validToken(T_SYMBOL, ";");

    return createASTNode(RETURN, returnNode, NULL);
}

bool LLParser::IsValidFunction()
{
    string tokenInstance = currToken->entry;
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
    while (currToken->entry != ")")
    {
        if (currToken->entry != ",")
        {
            stmtList->right = createASTNode(PARAM_CALL, createASTVariableNode(*currToken), NULL);
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
    printMode = createASTNumberNode(*currToken);
    currToken++;
    validToken(T_SYMBOL, ")");
    validToken(T_SYMBOL, ";");

    return createASTNode(PRINT, printContent, printMode);
}

ASTNode * LLParser::GlobalAssignment()
{
    ASTNode * exp;
    ASTNode * varAssign;
    bool isConstant = false;
    string tmpEntry;

    switch (currToken->tType)
    {
        case T_KEYWORD:
            if (currToken->entry != "let" && currToken->entry != "const") {throw ERROR_INVALID_KEYWORD;}
            isConstant = currToken->entry == "const";
            currToken++;
            if (global_symbol_table.find(currToken->entry) != global_symbol_table.end()) {throw ERROR_VAR_UNKNOWN;}
            if (currToken->tType != T_VARIABLE) {throw ERROR_INVALID_SYMBOL;} else {varAssign = createASTVariableNode(*currToken);}
            global_symbol_table.insert({currToken->entry, isConstant});
            break;
        default:
            throw ERROR_INVALID_SYMBOL;
    }
    currToken++;
    validToken(T_SYMBOL, "=");
    exp = createASTNode(ASSIGN, varAssign, Expression());
    validToken(T_SYMBOL, ";");
    return exp;
}

ASTNode * LLParser::Assignment()
{
    return Assignment(true);
}

ASTNode * LLParser::Assignment(bool isUsingSemi)
{
    ASTNode * exp;
    ASTNode * varAssign;
    bool isConstant = false;
    string tmpEntry;

    switch (currToken->tType)
    {
        case T_KEYWORD:
            if (currToken->entry != "let" && currToken->entry != "const") {throw ERROR_INVALID_KEYWORD;}
            isConstant = currToken->entry == "const";
            currToken++;
            if (symbol_table.find(currToken->entry) != symbol_table.end() &&
                global_symbol_table.find(currToken->entry) != global_symbol_table.end()
            ) {throw ERROR_VAR_UNKNOWN;}
            if (currToken->tType != T_VARIABLE) {throw ERROR_INVALID_SYMBOL;} else {varAssign = createASTVariableNode(*currToken);}
            symbol_table.insert({currToken->entry, isConstant});
            break;
        case T_VARIABLE:
            if (symbol_table.find(currToken->entry) == symbol_table.end() &&
                global_symbol_table.find(currToken->entry) == global_symbol_table.end()
            ) {throw ERROR_VAR_UNKNOWN;}
            else if (symbol_table[currToken->entry]) {throw ERROR_VAR_UNKNOWN;}
            else if (global_symbol_table[currToken->entry]) {throw ERROR_VAR_UNKNOWN;}
            tmpEntry = currToken->entry;
            varAssign = createASTVariableNode(*currToken);
            break;
        default:
            throw ERROR_INVALID_SYMBOL;
    }
    ASTType inst;
    currToken++;
    if (symbol_table.find(tmpEntry) == symbol_table.end() &&
        global_symbol_table.find(tmpEntry) == global_symbol_table.end() && currToken->entry != "="
    ) {throw ERROR_VAR_UNKNOWN;}
    switch (currToken->entry[0])
    {
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
            throw ERROR_INVALID_SYMBOL; 
    }
    ASTNode * resultNode;
    currToken++;
    if (this->IsValidFunction())
    {
        resultNode = FunctionCall();
    }
    else
    {
        resultNode = Expression();
        if (isUsingSemi) {validToken(T_SYMBOL, ";");}
    }
    exp = createASTNode(inst, varAssign, resultNode);
    return exp;
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
    symbol_table.clear();
    symbol_table = placeholder_symbol_table;
    return createASTNode(COND_CMPR, conditionalResults, stmtResults);
}

void LLParser::ElseOrElseIfStatemnt(ASTNode *& conditionalResults, ASTNode *& stmtResults)
{
    ASTNode * previousStmt = stmtResults;
    map<string, bool> placeholder_symbol_table = symbol_table;

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
        symbol_table = placeholder_symbol_table;

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
        symbol_table = placeholder_symbol_table;
    }
}

ASTNode * LLParser::WhileLoop()
{
    ASTNode * conditionalResults;
    ASTNode * conditionalResults_P;
    ASTNode * stmtResults = createASTNode(WHILE_LOOP_STATEMENT, NULL, createASTNode(UNKNOWN, NULL, NULL));
    map<string, bool> placeholder_symbol_table = symbol_table;

    validToken(T_KEYWORD, "while");
    validToken(T_SYMBOL, "(");
    conditionalResults = createASTNode(WHILE_LOOP_COND, Expression(), NULL);
    validToken(T_SYMBOL, ")");
    validToken(T_SYMBOL, "{");
    StmtList(stmtResults->right);
    validToken(T_SYMBOL, "}");

    conditionalResults_P = createASTNode(WHILE_LOOP_COND_P, conditionalResults->left, NULL);
    symbol_table.clear();
    symbol_table = placeholder_symbol_table;

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