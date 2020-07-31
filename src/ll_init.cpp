#include "compiler_comp.hpp"

void LLParser::validToken(TokenType param1, string param2)
{
    if (currToken->tType != param1 && currToken->entry != param2) { throw ERROR_INVALID_SYMBOL;}
    currToken++;
}

void LLParser::initGrammar(TokenEntry * token_instance, ASTNode *& the_ast) 
{
    currToken = token_instance;
    Start(the_ast);
}

ASTNode * LLParser::cloneASTNode(ASTNode * clone)
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

ASTNode * LLParser::createASTNullNode()
{
    ASTNode * ans = new ASTNode();
    ans->type = UNKNOWN;
    ans->left = NULL;
    ans->right = NULL;
    ans->value = INT_MIN;
    ans->key = "0";
    return ans;
}

ASTNode * LLParser::createASTNode(ASTType type, ASTNode* left, ASTNode* right)
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
            ans->key = "UNKNOWN";
            break;
    }
    return ans;
}

ASTNode * LLParser::createASTCharNode(TokenEntry entry)
{
    ASTNode * ans = new ASTNode();
    ans->value = (int) (entry.entry == "\' \'" ? ' ' : stoi(entry.entry));
    ans->type = CHAR;
    ans->key = to_string(ans->value);
    return ans;
}

ASTNode * LLParser::createASTWholeArrayNode(TokenEntry entry)
{
    ASTNode * ans = new ASTNode();
    ans->value = 0;
    ans->type = ARRAY_WHOLE;
    ans->key = entry.entry;
    return ans;
}

ASTNode * LLParser::createASTArrayNode(TokenEntry entry)
{
    ASTNode * ans = new ASTNode();
    ans->value = -1;
    ans->type = ARRAY_ACCESS;
    ans->key = entry.entry;
    return ans;
}

ASTNode * LLParser::createASTNumberNode(TokenEntry entry)
{
    bool isDigit = true;
    for (int i = 0; i < entry.entry.length(); i++)
    {
        if (!isdigit(entry.entry[i]))
        {
            isDigit = false;
        }
    }
    if (isDigit == false)
    {
        throw ERROR_INVALID_SYMBOL;
    }
    ASTNode * ans = new ASTNode();
    ans->value = stoi(entry.entry);
    ans->type = NUM;
    ans->key = entry.entry;
    return ans;
}

ASTNode * LLParser::createASTVariableNode(TokenEntry entry)
{
    ASTNode * ans = new ASTNode();
    ans->key = entry.entry;
    ans->type = VAR;
    return ans;
}

ASTNode * LLParser::Main()
{
    ASTNode* mainNode = createASTNode(MAIN, NULL, NULL);
    map<string, bool> placeholder_symbol_table = symbol_table;

    validToken(T_KEYWORD, "function");
    validToken(T_VARIABLE, "MAIN");
    validToken(T_SYMBOL, "(");
    validToken(T_SYMBOL, ")");
    validToken(T_SYMBOL, "{");
    StmtList(mainNode);
    symbol_table.clear();
    validToken(T_SYMBOL, "}");
    return mainNode;
}

// THIS IS THE START OF THE GRAMMAR
void LLParser::Start(ASTNode *& root)
{
    try
    {
        root = createASTNode(UNKNOWN, createASTNullNode(), createASTNullNode());
        Dependencies(root);
        root = createASTNode(START, root, Main());
    }
    catch(int e)
    {
        ErrorReader::readError(e, currToken->entry);
        root =  NULL;
    }
}

void LLParser::Dependencies(ASTNode *& root)
{
    ASTNode * globalNode = createASTNode(DEPENDENCY_GLOBAL, NULL, NULL);
    ASTNode * functionNode = createASTNode(DEPENDENCY_FUNC, globalNode, NULL);
    root->right = functionNode;
    while (currToken->tType == T_KEYWORD)
    {
        if (currToken->entry == "function")
        {
            if ((currToken + 1)->entry == "MAIN")
            {
                return;
            }
            else 
            {
                functionNode->right = createASTNode(DEPENDENCY, Function(), NULL);
                functionNode = functionNode->right;
            }
        }
        else if ((currToken->entry == "let") || 
            (currToken->entry == "const") )
        {
            globalNode->right = createASTNode(DEPENDENCY, GlobalAssignment(), NULL);
            globalNode = globalNode->right;
        }
        else if (currToken->entry == "import")
        {
            validToken(T_KEYWORD, currToken->entry);

            ASTNode * importLib = ImportFile(root);
            
            validToken(T_SYMBOL, ";");
            
            globalNode->right = createASTNode(IMPORT, importLib, NULL);
        }
    }
}

ASTNode * LLParser::ImportFile(ASTNode * root)
{
    FrontEnd * importFrontEnd = new FrontEnd(symbol_table);
    //importFrontEnd->run(currToken->entry + ".w", root);
    delete importFrontEnd;
    currToken++;
    return root;
}

ASTNode * LLParser::Function()
{
    ASTNode * functionInner = createASTNode(FUNCTION, NULL, createASTNode(UNKNOWN, NULL, NULL));
    ASTNode * stmtList = functionInner->right;
    map<string, bool> placeholder_symbol_table = symbol_table;
    set<string> placeholder_array_table = arr_table;
    arr_table.clear();
    symbol_table.clear();
    validToken(T_KEYWORD, "function");
    if (IsValidFunction() == false) {throw ERROR_INVALID_SYMBOL;}
    functionInner->key = currToken->entry;
    function_table.insert(currToken->entry);
    validToken(T_VARIABLE, currToken->entry);
    validToken(T_SYMBOL, "(");
    while (currToken->entry != ")")
    {
        if (currToken->entry != ",")
        {
            validToken(T_KEYWORD, "let");
            ASTType ast_type;
            ASTNode * parameterName = createASTVariableNode(*currToken);
            if ((currToken + 1)->entry == "[")
            {
                arr_table.insert(currToken->entry);
                ast_type = PARAM_ARRAY_INST;
                currToken++;
                currToken++;
            }
            else
            {
                symbol_table.insert({currToken->entry, false});
                ast_type = PARAM_INST;
            }
            stmtList->left = createASTNode(ast_type, NULL, parameterName);
            stmtList = stmtList->left;
        }
        currToken++;
    }
    validToken(T_SYMBOL, ")");
    validToken(T_SYMBOL, "{");
    StmtList(functionInner->right);
    symbol_table = placeholder_symbol_table;
    arr_table = placeholder_array_table;
    validToken(T_SYMBOL, "}");
    return functionInner;
}

