#include "parser_comp.h"

void Parser::caseStatemnt(ASTNode *& conditionalResults, ASTNode *& stmtResults)
{
    ASTNode * previousStmt = stmtResults;
    if (currToken->entry == "case")
    {
        ASTUtility::validToken(T_KEYWORD, "case", currToken);
        if (currToken->tType == T_NUMBER && case_set.find(currToken->entry) == case_set.end())
        {
            case_set.insert(currToken->entry);
            conditionalResults = ASTUtility::createASTNode(CASE_COND, ASTUtility::createASTNumberNode(*currToken), NULL);
            currToken++;
        }
        else
        {
            cout << "line 447" << endl;
            throw ERROR_INVALID_SYMBOL;
        }
        ASTUtility::validToken(T_SYMBOL, ":", currToken);
        stmtResults = ASTUtility::createASTNode(CASE_STMT, previousStmt, ASTUtility::createASTNode(UNKNOWN, NULL, NULL));
        this->stmtList(stmtResults->right);
        ASTUtility::validToken(T_KEYWORD, "break", currToken);
        ASTUtility::validToken(T_SYMBOL, ";", currToken);
        if (currToken->entry == "default" || currToken->entry == "case")
        {
            this->caseStatemnt(conditionalResults->right, stmtResults);
        }
        else
        {
            conditionalResults->right = ASTUtility::createASTNode(SWITCH_END, NULL, NULL);
            case_set.clear();
        }
    }
    else if (currToken->entry == "default")
    {
        stmtResults = ASTUtility::createASTNode(DEFAULT_STMT, previousStmt, ASTUtility::createASTNode(UNKNOWN, NULL, NULL));
        ASTUtility::validToken(T_KEYWORD, "default", currToken);
        conditionalResults = ASTUtility::createASTNode(DEFAULT_COND, NULL, NULL);
        ASTUtility::validToken(T_SYMBOL, ":", currToken);
        this->stmtList(stmtResults->right);
        ASTUtility::validToken(T_KEYWORD, "break", currToken);
        ASTUtility::validToken(T_SYMBOL, ";", currToken);
        conditionalResults->right = ASTUtility::createASTNode(SWITCH_END, NULL, NULL);
    }
    else
    {
        if (currToken->tType == T_NUMBER)
        {
            conditionalResults = ASTUtility::createASTNode(CASE_COND, ASTUtility::createASTNumberNode(*currToken), NULL);
            currToken++;
        }
        else
        {
            cout << "line 485" << endl;
            throw ERROR_INVALID_SYMBOL;
        }
        ASTUtility::validToken(T_SYMBOL, ":", currToken);
        if (currToken->entry == "case")
        {
            ASTUtility::validToken(T_KEYWORD, "case", currToken);
            this->caseStatemnt(conditionalResults->right, stmtResults);
        }
    }
}

void Parser::elseOrElseIfStatemnt(ASTNode *& conditionalResults, ASTNode *& stmtResults)
{
    ASTNode * previousStmt = stmtResults;
    map<string, bool> placeholder_symbol_table = symbol_table;
    set<string> placeholder_arr_table = arr_table;

    if (currToken->entry == "elif")
    {
        stmtResults = ASTUtility::createASTNode(ELIF_STMT, previousStmt, ASTUtility::createASTNode(UNKNOWN, NULL, NULL));

        ASTUtility::validToken(T_KEYWORD, "elif", currToken);
        ASTUtility::validToken(T_SYMBOL, "(", currToken);
        conditionalResults = ASTUtility::createASTNode(ELIF_COND, this->expression(), NULL);
        ASTUtility::validToken(T_SYMBOL, ")", currToken);
        ASTUtility::validToken(T_SYMBOL, "{", currToken);
        this->stmtList(stmtResults->right);
        ASTUtility::validToken(T_SYMBOL, "}", currToken);
        symbol_table.clear();
        arr_table.clear();
        symbol_table = placeholder_symbol_table;
        arr_table = placeholder_arr_table;

        this->elseOrElseIfStatemnt(conditionalResults->right, stmtResults);
    }
    else if (currToken->entry == "else")
    {
        stmtResults = ASTUtility::createASTNode(ELSE_STMT, previousStmt, ASTUtility::createASTNode(UNKNOWN, NULL, NULL));
        conditionalResults = ASTUtility::createASTNode(ELSE_COND, NULL, NULL);
        ASTUtility::validToken(T_KEYWORD, "else", currToken);
        ASTUtility::validToken(T_SYMBOL, "{", currToken);
        this->stmtList(stmtResults->right);
        ASTUtility::validToken(T_SYMBOL, "}", currToken);
        symbol_table.clear();
        arr_table.clear();
        symbol_table = placeholder_symbol_table;
        arr_table = placeholder_arr_table;
    }
}

ASTNode * Parser::ifStatemnt()
{
    ASTNode * conditionalResults;
    ASTNode * stmtResults = ASTUtility::createASTNode(IF_STMT, NULL, ASTUtility::createASTNode(UNKNOWN, NULL, NULL));
    map<string, bool> placeholder_symbol_table = symbol_table;
    set<string> placeholder_array_table = arr_table;

    ASTUtility::validToken(T_KEYWORD, "if", currToken);
    ASTUtility::validToken(T_SYMBOL, "(", currToken);
    conditionalResults = ASTUtility::createASTNode(IF_COND, this->expression(), NULL);
    ASTUtility::validToken(T_SYMBOL, ")", currToken);
    ASTUtility::validToken(T_SYMBOL, "{", currToken);
    this->stmtList(stmtResults->right);
    ASTUtility::validToken(T_SYMBOL, "}", currToken);

    if (currToken->entry == "elif" || currToken->entry == "else")
    {
        this->elseOrElseIfStatemnt(conditionalResults->right, stmtResults);
    }
    arr_table.clear();
    symbol_table.clear();
    symbol_table = placeholder_symbol_table;
    arr_table = placeholder_array_table;
    return ASTUtility::createASTNode(COND_CMPR, conditionalResults, stmtResults);
}

ASTNode * Parser::switchStatement()
{
    ASTNode * conditionalResults;
    ASTNode * stmtResults = ASTUtility::createASTNode(UNKNOWN, NULL, NULL);

    ASTUtility::validToken(T_KEYWORD, "switch", currToken);
    ASTUtility::validToken(T_SYMBOL, "(", currToken);
    if (symbol_table.find(currToken->entry) == symbol_table.end() && 
        global_symbol_table.find(currToken->entry) == global_symbol_table.end()
    ) {throw ERROR_VAR_UNKNOWN;}
    conditionalResults = ASTUtility::createASTNode(SWITCH_COND, ASTUtility::createASTVariableNode(*currToken), NULL);
    currToken++;
    ASTUtility::validToken(T_SYMBOL, ")", currToken);
    ASTUtility::validToken(T_SYMBOL, "{", currToken);
    if (currToken->entry == "case")
    {
        this->caseStatemnt(conditionalResults->right, stmtResults);
    }
    ASTUtility::validToken(T_SYMBOL, "}", currToken);

    return ASTUtility::createASTNode(SWITCH_CMPR, conditionalResults, stmtResults);
}

