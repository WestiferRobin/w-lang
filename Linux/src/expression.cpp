#include "parser_comp.hpp"

ASTNode * Parser::andOrCondition()
{
    ASTNode * expLeft;
    ASTNode * expRight;
    ASTType type;
    expLeft = this->condition();
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
    expRight = this->expression();
    return ASTUtility::createASTNode(type, expLeft, expRight);
}

ASTNode * Parser::arithExpression()
{
    ASTNode * term;
    ASTNode * expP;

    switch (currToken->tType)
    {
        case T_NUMBER:
        case T_VARIABLE:
        case T_CHAR:
            term = this->term();
            break;
        case T_SYMBOL:
            if (currToken->entry[0] == '(' || currToken->entry[0] == '!') {term = this->term(); break;}
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
                        expP = this->arithExpressionP(term);
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

ASTNode * Parser::arithExpressionP(ASTNode* root)
{
    ASTType tmp;
    switch (currToken->tType)
    {
        case T_SYMBOL:
            switch (currToken->entry[0])
            {
                case '+':
                    currToken++;
                    return ASTUtility::createASTNode(ADD, root, this->expression()); 
                case '-':
                    currToken++;
                    return ASTUtility::createASTNode(MINUS, root, this->expression());
                default:
                    throw ERROR_INVALID_ARITH_OP;
            }
        default:
            throw ERROR_INVALID_ARITH_OP;
    }
}

ASTNode * Parser::condition()
{
    ASTNode * expLeft;
    ASTNode * expRight;
    ASTType type;
    expLeft = this->arithExpression();
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
    expRight = this->arithExpression();
    return ASTUtility::createASTNode(type, expLeft, expRight);
}

ASTNode * Parser::expression()
{
    return this->andOrCondition();
}

ASTNode * Parser::factor()
{
    ASTNode * tmp;
    switch (currToken->tType)
    {
        case T_NUMBER:
            tmp = ASTUtility::createASTNumberNode(*currToken);
            currToken++;
            return tmp;
        case T_VARIABLE:
            if (symbol_table.find(currToken->entry) == symbol_table.end() &&
                global_symbol_table.find(currToken->entry) == global_symbol_table.end() &&
                arr_table.find(currToken->entry) == arr_table.end()
            ) {throw ERROR_VAR_UNKNOWN;}
            tmp = ASTUtility::createASTVariableNode(*currToken);
            currToken++;
            if (currToken->entry == "[")
            {
                ASTUtility::validToken(T_SYMBOL, "[", currToken);
                if (symbol_table.find(currToken->entry) != symbol_table.end() || 
                global_symbol_table.find(currToken->entry) != global_symbol_table.end()
                )
                {
                    tmp->key += "[" + currToken->entry + "]";
                }
                else if (stoi(currToken->entry) >= 0)
                {
                    tmp->key += "[" + currToken->entry + "]";
                }
                else
                {
                    throw ERROR_VAR_UNKNOWN;
                }
                currToken++;
                ASTUtility::validToken(T_SYMBOL, "]", currToken);
            }
            return tmp;
        case T_SYMBOL:
            switch (currToken->entry[0])
            {
                case '!':
                    currToken++;
                    tmp = this->factor();
                    return ASTUtility::createASTNode(NOT, NULL, tmp);
                case '(':
                    currToken++;
                    tmp = this->expression();
                    if (currToken->entry[0] != ')') {throw ERROR_INVALID_SYMBOL;}
                    currToken++;
                    return tmp;
            }
        case T_CHAR:
            tmp = ASTUtility::createASTCharNode(*currToken);
            currToken++;
            return tmp;
        default:
            throw ERROR_INVALID_SYMBOL;
    }
    return NULL;
}

ASTNode * Parser::term()
{
    ASTNode * factor;
    ASTNode * termP;

    switch (currToken->tType)
    {
        case T_NUMBER:
        case T_VARIABLE:
        case T_CHAR:
            factor = this->factor();
            break;
        case T_SYMBOL:
            if (currToken->entry[0] == '(' || currToken->entry[0] == '!') {factor = this->factor(); break;}
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
                    termP = this->termP(factor);
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

ASTNode * Parser::termP(ASTNode* root)
{
    ASTType tmp;
    switch (currToken->tType)
    {
        case T_SYMBOL:
            switch (currToken->entry[0])
            {
                case '*':
                    currToken++;
                    return ASTUtility::createASTNode(MULTI, root, this->expression());
                case '/':
                    currToken++;
                    return ASTUtility::createASTNode(DIV, root, this->expression());
                case '%':
                    currToken++;
                    return ASTUtility::createASTNode(MODULUS, root, this->expression());
                case '|':
                    currToken++;
                    return ASTUtility::createASTNode(BIT_OR, root, this->expression());
                case '&':
                    currToken++;
                    return ASTUtility::createASTNode(BIT_AND, root, this->expression());
                case '^':
                    currToken++;
                    return ASTUtility::createASTNode(BIT_XOR, root, this->expression());
                case '>':
                    if (currToken->entry == ">>")
                    {
                        currToken++;
                        return ASTUtility::createASTNode(BIT_RIGHT, root, this->expression());
                    }
                    else
                    {
                        throw ERROR_INVALID_SYMBOL;
                    }
                case '<':
                    if (currToken->entry == "<<")
                    {
                        currToken++;
                        return ASTUtility::createASTNode(BIT_LEFT, root, this->expression());
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