#include "parser_comp.hpp"

ASTNode * Parser::function()
{
    ASTNode * functionInner = ASTUtility::createASTNode(FUNCTION, NULL, ASTUtility::createASTNode(UNKNOWN, NULL, NULL));
    ASTNode * stmtList = functionInner->right;

    map<string, bool> placeholder_symbol_table = symbol_table;
    map<string, bool> placeholder_array_table = arr_table;
    map<string, string> paramAdapter;

    arr_table.clear();
    symbol_table.clear();

    ASTUtility::validToken(T_KEYWORD, "function", currToken);

    if (isValidFunction() == false) 
    {
        throw (int) ErrorInvalidSymbol;
    }

    functionInner->key = currToken->entry;

    function_table.insert(currToken->entry);

    ASTUtility::validToken(T_VARIABLE, currToken->entry, currToken);
    ASTUtility::validToken(T_SYMBOL, "(", currToken);

    while (currToken->entry != ")")
    {
        if (currToken->entry != ",")
        {
            ASTUtility::validToken(T_KEYWORD, "let", currToken);

            ASTType ast_type;
            ASTNode * parameterName = ASTUtility::createASTVariableNode(*currToken);
            
            paramAdapter.insert({ parameterName->key, parameterName->key + "_PARAM" });
            parameterName->key +=  "_PARAM";

            if ((currToken + 1)->entry == "[")
            {
                arr_table.insert({ currToken->entry, false });
                ast_type = PARAM_ARRAY_INST;

                currToken++;
                currToken++;
            }
            else
            {
                symbol_table.insert({currToken->entry, false});
                ast_type = PARAM_INST;
            }

            stmtList->left = ASTUtility::createASTNode(ast_type, NULL, parameterName);
            stmtList = stmtList->left;
        }

        currToken++;
    }
    ASTUtility::validToken(T_SYMBOL, ")", currToken);
    ASTUtility::validToken(T_SYMBOL, "{", currToken);

    this->stmtList(functionInner->right);

    symbol_table = placeholder_symbol_table;
    arr_table = placeholder_array_table;

    ASTUtility::validToken(T_SYMBOL, "}", currToken);
    ASTUtility::cleanParameter(paramAdapter, functionInner->right);

    return functionInner;
}

ASTNode * Parser::functionCall()
{
    ASTNode * functionCall = ASTUtility::createASTNode(FUNCTION_CALL, ASTUtility::createASTNode(UNKNOWN, NULL, NULL), NULL);
    ASTNode * stmtList = functionCall->left;

    functionCall->key = currToken->entry;
    currToken++;

    ASTUtility::validToken(T_SYMBOL, "(", currToken);
    
    int count = 0;
    const string paramArrayTitle = "PARAM_ARRAY_";

    while (currToken->entry != ")")
    {
        
        if (currToken->entry != "," && currToken->entry != "\'" && currToken->entry != "\"")
        {
            ASTType ast_type = arr_table.find(currToken->entry) != arr_table.end() ? PARAM_ARRAY_CALL : PARAM_CALL;
            
            stmtList->right = ASTUtility::createASTNode(ast_type, ASTUtility::createASTVariableNode(*currToken), NULL);
            stmtList = stmtList->right;
            
            currToken++;
        }
        else if (currToken->entry == "\"")
        {
            ASTUtility::validToken(T_SYMBOL, "\"", currToken);
            
            TokenEntry variableName(T_VARIABLE, paramArrayTitle + to_string(count));
            ASTNode * varAssign = ASTUtility::createASTVariableNode(variableName);
            ASTNode * resultNode = ASTUtility::createASTNode(ARRAY_INIT_PRE_ELM, ASTUtility::createASTNullNode(), ASTUtility::createASTNullNode());
            ASTNode * char_elements = resultNode->right;
            
            while (currToken->entry != "\"")
            {
                char_elements->right = ASTUtility::createASTNumberNode(*currToken);
                char_elements = char_elements->right;
                currToken++;
            }
            
            TokenEntry te(T_NUMBER, "0");
            resultNode->left = ASTUtility::createASTNumberNode(te);
            
            ASTUtility::validToken(T_SYMBOL, "\"", currToken);
            
            ASTNode * rawStringNode = ASTUtility::createASTNode(ARRAY_INIT, varAssign, resultNode);
            rawStringNode->key = paramArrayTitle + to_string(count++);
            stmtList->right = ASTUtility::createASTNode(PARAM_ARRAY_CALL, rawStringNode , ASTUtility::createASTNullNode());
            stmtList = stmtList->right;
        }
        else
        {
            currToken++;
        }
    }
    
    ASTUtility::validToken(T_SYMBOL, ")", currToken);
    ASTUtility::validToken(T_SYMBOL, ";", currToken);
    
    return functionCall;
}

ASTNode * Parser::importFile(ASTNode * root)
{
    FrontEnd * importFrontEnd = new FrontEnd(symbol_table);
    importFrontEnd->run(currToken->entry + ".w", root);     // TODO: try adding a bool flag to distinguish if this is Main file or a dependency file.
    delete importFrontEnd;
    currToken++;
    return root;
}

bool Parser::isValidFunction()
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

ASTNode * Parser::returnCall()
{
    ASTNode * returnNode = ASTUtility::createASTNode(UNKNOWN, NULL, NULL);
    
    ASTUtility::validToken(T_KEYWORD, "return", currToken);
    if (isValidFunction())
    {
        returnNode = functionCall();
    }
    else
    {
        returnNode = expression();
        ASTUtility::validToken(T_SYMBOL, ";", currToken);
    }
    
    return ASTUtility::createASTNode(arr_table.find(returnNode->key) == arr_table.end() ? RETURN : RETURN_ARR, returnNode, NULL);
}
