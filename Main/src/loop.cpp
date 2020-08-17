#include "parser_comp.h"

ASTNode * Parser::whileLoop()
{
    ASTNode * conditionalResults;
    ASTNode * conditionalResults_P;
    ASTNode * expressionResult;
    ASTNode * stmtResults = ASTUtility::createASTNode(WHILE_LOOP_STATEMENT, ASTUtility::createASTNullNode(), ASTUtility::createASTNullNode());
    
    map<string, bool> placeholder_symbol_table = symbol_table;
    map<string, bool> placeholder_array_table = arr_table;

    ASTUtility::validToken(T_KEYWORD, "while", currToken);
    ASTUtility::validToken(T_SYMBOL, "(", currToken);

    expressionResult = this->expression();

    ASTUtility::validToken(T_SYMBOL, ")", currToken);
    ASTUtility::validToken(T_SYMBOL, "{", currToken);

    this->stmtList(stmtResults->right);

    ASTUtility::validToken(T_SYMBOL, "}", currToken);

    conditionalResults = ASTUtility::createASTNode(WHILE_LOOP_COND, expressionResult, ASTUtility::createASTNullNode());
    conditionalResults_P = ASTUtility::createASTNode(WHILE_LOOP_COND_P, ASTUtility::cloneASTNode(expressionResult), ASTUtility::createASTNullNode());
    
    arr_table.clear();
    symbol_table.clear();
    
    symbol_table = placeholder_symbol_table;
    arr_table = placeholder_array_table;

    return ASTUtility::createASTNode(WHILE_LOOP, conditionalResults, ASTUtility::createASTNode(WHILE_LOOP_EXIT, stmtResults, conditionalResults_P));
}

ASTNode * Parser::forLoop()
{
    ASTNode * conditionalResults;
    ASTNode * conditionalResults_P;
    ASTNode * stmtResults = ASTUtility::createASTNode(FOR_LOOP_STATEMENT, NULL, ASTUtility::createASTNode(UNKNOWN, NULL, NULL));
    
    map<string, bool> placeholder_symbol_table = symbol_table;

    ASTUtility::validToken(T_KEYWORD, "for", currToken);
    ASTUtility::validToken(T_SYMBOL, "(", currToken);
    
    ASTNode * assignmentResult = this->assignment();
    ASTNode * tempConditionResult = this->expression();
    
    ASTUtility::validToken(T_SYMBOL, ";", currToken);
    
    ASTNode * updateResult = this->assignment(false);
    
    ASTUtility::validToken(T_SYMBOL, ")", currToken);
    ASTUtility::validToken(T_SYMBOL, "{", currToken);
    
    this->stmtList(stmtResults->right);
    
    ASTUtility::validToken(T_SYMBOL, "}", currToken);
    
    conditionalResults = ASTUtility::createASTNode(FOR_LOOP_COND, assignmentResult, tempConditionResult);
    conditionalResults_P = ASTUtility::createASTNode(FOR_LOOP_COND_P, updateResult, ASTUtility::cloneASTNode(tempConditionResult));
    
    symbol_table.clear();
    symbol_table = placeholder_symbol_table;

    return ASTUtility::createASTNode(FOR_LOOP, conditionalResults, ASTUtility::createASTNode(FOR_LOOP_EXIT, stmtResults, conditionalResults_P));
}