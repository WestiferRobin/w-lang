#include "compiler_comp.hpp"

ASTNode * LLParser::Expression()
{
    return AndOrCondition();
}

ASTNode * LLParser::AndOrCondition()
{
    ASTNode * expLeft;
    ASTNode * expRight;
    ASTType type;
    expLeft = Condition();
    if (currToken->entry == "," || currToken->entry == ";" || currToken->entry == ")") 
    { 
        return expLeft;
    }
    else if (currToken->entry == "&&")
    {
        type = BOOL_AND;
    }
    else if (currToken->entry == "||")
    {
        type = BOOL_OR;
    }
    else 
    { 
        throw ERROR_INVALID_BOOL_OP;   
    }
    currToken++;
    expRight = Expression();
    return createASTNode(type, expLeft, expRight);
}

ASTNode * LLParser::Condition()
{
    ASTNode * expLeft;
    ASTNode * expRight;
    ASTType type;
    expLeft = ArithExpression();
    if (currToken->entry == "," || currToken->entry == ";" || currToken->entry == ")" || currToken->entry == "&&" || currToken->entry == "||") 
    { 
        return expLeft;
    }
    else if (currToken->entry == "==")
    {
        type = EQUALS;
    }
    else if (currToken->entry == "!=")
    {
        type = NOT_EQUALS;
    }
    else if (currToken->entry == "<")
    {
        type = LESS_THAN;
    }
    else if (currToken->entry == ">")
    {
        type = GREATER_THAN;
    }
    else if (currToken->entry == "<=")
    {
        type = LESS_THAN_EQ;
    }
    else if (currToken->entry == ">=")
    {
        type = GREATER_THAN_EQ;
    }
    else 
    { 
        throw ERROR_INVALID_BOOL_OP;     
    }
    currToken++;
    expRight = ArithExpression();
    return createASTNode(type, expLeft, expRight);
}

ASTNode * LLParser::ArithExpression()
{
    ASTNode * term;
    ASTNode * expP;

    switch (currToken->tType)
    {
        case T_NUMBER:
        case T_VARIABLE:
        case T_CHAR:
            term = Term();
            break;
        case T_SYMBOL:
            if (currToken->entry[0] == '(' || currToken->entry[0] == '!') {term = Term(); break;}
            else { throw ERROR_INVALID_SYMBOL; } 
        default:
            throw ERROR_INVALID_SYMBOL;
    }

    switch (currToken->tType)
    {
        case T_ENDOFFILE:
            return term;
        case T_SYMBOL:
            switch (currToken->entry[0])
            {
                case '+':
                case '-':
                case '!':
                    if (currToken->entry != "!=")
                    {
                        expP = ArithExpressionP(term);
                        return expP;
                    }
                case ')':
                case ';':
                case ',':
                case '=':
                case '>':
                case '<':
                case '|':
                case '&':
                    return term;
                default:
                    throw ERROR_INVALID_SYMBOL;
            }
        default:
            throw ERROR_INVALID_SYMBOL;
    }

    return NULL;

}

ASTNode * LLParser::ArithExpressionP(ASTNode* root)
{
    ASTType tmp;
    switch (currToken->tType)
    {
        case T_SYMBOL:
            switch (currToken->entry[0])
            {
                case '+':
                    currToken++;
                    return createASTNode(ADD, root, Expression()); 
                case '-':
                    currToken++;
                    return createASTNode(MINUS, root, Expression());
                default:
                    throw ERROR_INVALID_ARITH_OP;
            }
        default:
            throw ERROR_INVALID_ARITH_OP;
    }
}

ASTNode * LLParser::Term()
{
    ASTNode * factor;
    ASTNode * termP;

    switch (currToken->tType)
    {
        case T_NUMBER:
        case T_VARIABLE:
        case T_CHAR:
            factor = Factor();
            break;
        case T_SYMBOL:
            if (currToken->entry[0] == '(' || currToken->entry[0] == '!') {factor = Factor(); break;}
            else { throw ERROR_INVALID_SYMBOL; }
        default:
            throw ERROR_INVALID_SYMBOL;
    }

    switch (currToken->tType)
    {
        case T_ENDOFFILE:
            return factor;
        case T_SYMBOL:
            switch (currToken->entry[0])
            {
                case '>':
                case '<':
                case '&':
                case '|':
                case '!':
                    if (currToken->entry == ">" || 
                        currToken->entry == ">=" ||
                        currToken->entry == "<" ||
                        currToken->entry == "<=" ||
                        currToken->entry == "&&" ||
                        currToken->entry == "||" ||
                        currToken->entry == "!="
                    )
                    {
                        return factor;
                    }
                case '^':
                case '*':
                case '/':
                case '%':
                    termP = TermP(factor);
                    return termP;
                case ')':
                case ';':
                case ',':
                case '+':
                case '-':
                case '=':
                    return factor;
                default:
                    throw ERROR_INVALID_SYMBOL;
            }
        default:
            throw ERROR_INVALID_SYMBOL;
    }

    return NULL;
}

ASTNode * LLParser::TermP(ASTNode* root)
{
    ASTType tmp;
    switch (currToken->tType)
    {
        case T_SYMBOL:
            switch (currToken->entry[0])
            {
                case '*':
                    currToken++;
                    return createASTNode(MULTI, root, Expression());
                case '/':
                    currToken++;
                    return createASTNode(DIV, root, Expression());
                case '%':
                    currToken++;
                    return createASTNode(MODULUS, root, Expression());
                case '|':
                    currToken++;
                    return createASTNode(BIT_OR, root, Expression());
                case '&':
                    currToken++;
                    return createASTNode(BIT_AND, root, Expression());
                case '^':
                    currToken++;
                    return createASTNode(BIT_XOR, root, Expression());
                case '>':
                    if (currToken->entry == ">>")
                    {
                        currToken++;
                        return createASTNode(BIT_RIGHT, root, Expression());
                    }
                    else
                    {
                        throw ERROR_INVALID_SYMBOL;
                    }
                case '<':
                    if (currToken->entry == "<<")
                    {
                        currToken++;
                        return createASTNode(BIT_LEFT, root, Expression());
                    }
                    else
                    {
                        throw ERROR_INVALID_SYMBOL;
                    }
                default:
                    throw ERROR_INVALID_ARITH_OP;
            }
        default:
            throw ERROR_INVALID_ARITH_OP;
    }
    return NULL;
}

ASTNode * LLParser::Factor()
{
    ASTNode * tmp;
    switch (currToken->tType)
    {
        case T_NUMBER:
            tmp = createASTNumberNode(*currToken);
            currToken++;
            return tmp;
        case T_VARIABLE:
            if (symbol_table.find(currToken->entry) == symbol_table.end() &&
                global_symbol_table.find(currToken->entry) == global_symbol_table.end()
            ) {throw ERROR_VAR_UNKNOWN;}
            tmp = createASTVariableNode(*currToken);
            currToken++;
            return tmp;
        case T_SYMBOL:
            switch (currToken->entry[0])
            {
                case '!':
                    currToken++;
                    tmp = Factor();
                    return createASTNode(NOT, NULL, tmp);
                case '(':
                    currToken++;
                    tmp = Expression();
                    if (currToken->entry[0] != ')') {throw ERROR_INVALID_SYMBOL;}
                    currToken++;
                    return tmp;
            }
        case T_CHAR:
            tmp = createASTCharNode(*currToken);
            currToken++;
            return tmp;
        default:
            throw ERROR_INVALID_SYMBOL;
    }
    return NULL;
}