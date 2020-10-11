#include "parser_comp.hpp"

void Parser::elseOrElseIfStatemnt(ASTNode *& conditionalResults, ASTNode *& stmtResults)
{
    ASTNode * previousStmt = stmtResults;
    map<string, bool> placeholder_symbol_table = symbol_table;
    map<string, bool> placeholder_arr_table = arr_table;

    if (currToken->entry == "elif")
    {
        stmtResults = ASTUtility::createASTNode(ELIF_STMT, previousStmt, ASTUtility::createASTNode(UNKNOWN, NULL, NULL));

        ASTUtility::validToken(T_KEYWORD, "elif", currToken);
        ASTUtility::validToken(T_SYMBOL, "(", currToken);

        conditionalResults = ASTUtility::createASTNode(ELIF_COND, expression(), NULL);

        ASTUtility::validToken(T_SYMBOL, ")", currToken);
        ASTUtility::validToken(T_SYMBOL, "{", currToken);

        stmtList(stmtResults->right);
        ASTUtility::validToken(T_SYMBOL, "}", currToken);

        symbol_table.clear();
        arr_table.clear();

        symbol_table = placeholder_symbol_table;
        arr_table = placeholder_arr_table;

        elseOrElseIfStatemnt(conditionalResults->right, stmtResults);
    }
    else if (currToken->entry == "else")
    {
        stmtResults = ASTUtility::createASTNode(ELSE_STMT, previousStmt, ASTUtility::createASTNode(UNKNOWN, NULL, NULL));
        conditionalResults = ASTUtility::createASTNode(ELSE_COND, NULL, NULL);

        ASTUtility::validToken(T_KEYWORD, "else", currToken);
        ASTUtility::validToken(T_SYMBOL, "{", currToken);
        
        stmtList(stmtResults->right);

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
    map<string, bool> placeholder_array_table = arr_table;

    ASTUtility::validToken(T_KEYWORD, "if", currToken);
    ASTUtility::validToken(T_SYMBOL, "(", currToken);

    conditionalResults = ASTUtility::createASTNode(IF_COND, expression(), NULL);

    ASTUtility::validToken(T_SYMBOL, ")", currToken);
    ASTUtility::validToken(T_SYMBOL, "{", currToken);

    stmtList(stmtResults->right);

    ASTUtility::validToken(T_SYMBOL, "}", currToken);

    if (currToken->entry == "elif" || currToken->entry == "else")
    {
        elseOrElseIfStatemnt(conditionalResults->right, stmtResults);
    }

    arr_table.clear();
    symbol_table.clear();

    symbol_table = placeholder_symbol_table;
    arr_table = placeholder_array_table;

    return ASTUtility::createASTNode(COND_CMPR, conditionalResults, stmtResults);
}