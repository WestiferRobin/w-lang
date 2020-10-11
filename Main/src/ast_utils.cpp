#include "general_util.hpp"

ASTNode * ASTUtility::createASTNode(ASTType type, ASTNode* left, ASTNode* right)
{
    ASTNode * ans = new ASTNode();
    ans->type = type;
    ans->left = left;
    ans->right = right;

    switch(type)
    {
        case BOOL_AND:
            ans->key = "BIT_AND";
            break;
        case BOOL_OR:
            ans->key = "BIT_OR";
            break;
        case BIT_AND:
            ans->key = "BIT_AND";
            break;
        case BIT_OR:
            ans->key = "BIT_OR";
            break;
        case ADD:
            ans->key = "ADD";
            break;
        case MINUS:
            ans->key = "MINUS";
            break;
        case DIV:
            ans->key = "DIV";
            break;
        case MULTI:
            ans->key = "MULTI";
            break;
        case VAR:
            ans->key = "VAR";
            break;
        case NUM:
            ans->key = "NUM";
            break;
        default:
            ans->key = "OTHER";
            break;
    }

    return ans;
}

ASTNode * ASTUtility::createASTNumberNode(TokenEntry entry)
{
    ASTNode * ans = new ASTNode();
    ans->value = stoi(entry.entry);
    ans->type = NUM;
    ans->key = entry.entry;

    return ans;
}

ASTNode * ASTUtility::createASTVariableNode(TokenEntry entry)
{
    ASTNode * ans = new ASTNode();
    ans->key = entry.entry;
    ans->type = VAR;

    return ans;
}

ASTNode * ASTUtility::createASTArrayNode(TokenEntry entry)
{
    ASTNode * ans = new ASTNode();
    ans->value = -1;
    ans->type = ARRAY_ACCESS;
    ans->key = entry.entry;

    return ans;
}

ASTNode * ASTUtility::createASTNullNode()
{
    ASTNode * ans = new ASTNode();
    ans->type = UNKNOWN;
    ans->left = NULL;
    ans->right = NULL;
    ans->value = INT_MIN;
    ans->key = "0";

    return ans;
}

ASTNode * ASTUtility::createASTCharNode(TokenEntry entry)
{
    ASTNode * ans = new ASTNode();
    ans->value = (int) (entry.entry == "\' \'" ? ' ' : stoi(entry.entry));
    ans->type = CHAR;
    ans->key = to_string(ans->value);

    return ans;
}

ASTNode * ASTUtility::createASTWholeArrayNode(TokenEntry entry)
{
    ASTNode * ans = new ASTNode();
    ans->value = 0;
    ans->type = ARRAY_WHOLE;
    ans->key = entry.entry;

    return ans;
}

ASTNode * ASTUtility::cloneASTNode(ASTNode * clone)
{
    if (clone == NULL) return createASTNullNode();
    ASTNode * ans = new ASTNode();
    ans->type = clone->type;
    ans->value = clone->value;
    ans->key = clone->key;
    ans->left = cloneASTNode(clone->left);
    ans->right = cloneASTNode(clone->right);

    return ans;
}

void ASTUtility::validToken(TokenType param1, string param2, TokenEntry *& currToken)
{
    if (currToken->tType != param1 || currToken->entry != param2) 
    { 
        throw (int) ErrorInvalidSymbol; 
    }
    
    currToken++;
}

void ASTUtility::cleanParameter(map<string, string> paramAdapter, ASTNode*& functionContent)
{
    if (functionContent == NULL) return;

    if (paramAdapter.find(functionContent->key) != paramAdapter.end())
    {
        functionContent->key = paramAdapter[functionContent->key];
    }

    cleanParameter(paramAdapter, functionContent->left);
    cleanParameter(paramAdapter, functionContent->right);
}