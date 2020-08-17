#include "parser_comp.h"

ASTNode * Parser::andOrCondition()
{
    ASTNode * expLeft;
    ASTNode * expRight;
    ASTType type;

    expLeft = condition();

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
        throw (int) ErrorInvalidBoolOp;   
    }

    currToken++;

    expRight = expression();

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
            if (currToken->entry[0] == '(' || currToken->entry[0] == '!') 
            {
                term = this->term(); 
                break;
            }
            else 
                throw (int) ErrorInvalidSymbol; 
        default:
            throw (int) ErrorInvalidSymbol;
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
                        expP = arithExpressionP(term);
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
                    throw (int) ErrorInvalidSymbol;
            }
        default:
            throw (int) ErrorInvalidSymbol;
    }

    return NULL;
}

ASTNode * Parser::arithExpressionP(ASTNode* root)
{
    switch (currToken->tType)
    {
        case T_SYMBOL:
            switch (currToken->entry[0])
            {
                case '+':
                    currToken++;
                    return ASTUtility::createASTNode(ADD, root, expression()); 
                case '-':
                    currToken++;
                    return ASTUtility::createASTNode(MINUS, root, expression());
                default:
                    throw (int) ErrorInvalidArithOp;
            }
        default:
            throw (int) ErrorInvalidArithOp;
    }
}

ASTNode * Parser::condition()
{
    ASTNode * expLeft;
    ASTNode * expRight;
    ASTType type;

    expLeft = arithExpression();

    if (currToken->entry == "," || currToken->entry == ";" || 
        currToken->entry == ")" || currToken->entry == "&&" || 
        currToken->entry == "||") 
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
        throw (int) ErrorInvalidBoolOp;     
    }

    currToken++;
    expRight = arithExpression();

    return ASTUtility::createASTNode(type, expLeft, expRight);
}

ASTNode * Parser::expression()
{
    return andOrCondition();
}

ASTNode * Parser::factor()
{
    ASTNode * tmp = NULL;
    switch (currToken->tType)
    {
        case T_NUMBER:
            tmp = ASTUtility::createASTNumberNode(*(currToken++));
            return tmp;
        case T_VARIABLE:
            if (symbol_table.find(currToken->entry) == symbol_table.end() &&
                global_symbol_table.find(currToken->entry) == global_symbol_table.end() &&
                arr_table.find(currToken->entry) == arr_table.end()) 
            { 
                throw (int) ErrorVariableUnknown; 
            }

            tmp = ASTUtility::createASTVariableNode(*(currToken++));

            if (currToken->entry == "[")
            {
                ASTUtility::validToken(T_SYMBOL, "[", currToken);

                if (symbol_table.find(currToken->entry) != symbol_table.end() || 
                    global_symbol_table.find(currToken->entry) != global_symbol_table.end())
                {
                    tmp->key += "[" + currToken->entry + "]";
                }
                else if (stoi(currToken->entry) >= 0)
                {
                    tmp->key += "[" + currToken->entry + "]";
                }
                else
                {
                    throw (int) ErrorVariableUnknown;
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
                    tmp = factor();
                    return ASTUtility::createASTNode(NOT, NULL, tmp);
                case '(':
                    currToken++;
                    
                    tmp = expression();

                    if (currToken->entry[0] != ')') 
                    {
                        throw (int) ErrorInvalidSymbol;
                    }
                    
                    currToken++;
                    
                    return tmp;
            }
        case T_CHAR:
            tmp = ASTUtility::createASTCharNode(*currToken);
            
            currToken++;
            
            return tmp;
        default:
            throw (int) ErrorInvalidSymbol;
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
            if (currToken->entry[0] == '(' || currToken->entry[0] == '!') 
            {   
                factor = this->factor(); 
                break;
            }
            else 
            { 
                throw (int) ErrorInvalidSymbol; 
            }
        default:
            throw (int) ErrorInvalidSymbol;
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
                    throw (int) ErrorInvalidSymbol;
            }
        default:
            throw (int) ErrorInvalidSymbol;
    }

    return NULL;
}

ASTNode * Parser::termP(ASTNode* root)
{
    switch (currToken->tType)
    {
        case T_SYMBOL:
            switch (currToken->entry[0])
            {
                case '*':
                    currToken++;
                    return ASTUtility::createASTNode(MULTI, root, expression());
                case '/':
                    currToken++;
                    return ASTUtility::createASTNode(DIV, root, expression());
                case '%':
                    currToken++;
                    return ASTUtility::createASTNode(MODULUS, root, expression());
                case '|':
                    currToken++;
                    return ASTUtility::createASTNode(BIT_OR, root, expression());
                case '&':
                    currToken++;
                    return ASTUtility::createASTNode(BIT_AND, root, expression());
                case '^':
                    currToken++;
                    return ASTUtility::createASTNode(BIT_XOR, root, expression());
                case '>':
                    if (currToken->entry == ">>")
                    {
                        currToken++;
                        return ASTUtility::createASTNode(BIT_RIGHT, root, expression());
                    }
                    else
                    {
                        throw (int) ErrorInvalidArithOp;
                    }
                case '<':
                    if (currToken->entry == "<<")
                    {
                        currToken++;
                        return ASTUtility::createASTNode(BIT_LEFT, root, expression());
                    }
                    else
                    {
                        throw (int) ErrorInvalidArithOp;
                    }
                default:
                    throw (int) ErrorInvalidArithOp;
            }
        default:
            throw (int) ErrorInvalidArithOp;
    }
    return NULL;
}