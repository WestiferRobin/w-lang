#include "parser_comp.h"

void Parser::dependencies(ASTNode *& root)
{
    /*
        TODO: 
            - Write an exaple on paper on what this new dependency will look like
                - Ideas: Have MAIN_GLOBAL vs IMPORT_GLOBAL
            - Clean up the stupid ENUMs
            - jmp MAIN is getting called everytime the DEPENCDY_GLOBAL is being iterated on.
    */
    ASTNode* importNode = ASTUtility::createASTNode(DEPENDENCY_IMPORT, NULL, NULL);
    ASTNode * globalNode = ASTUtility::createASTNode(DEPENDENCY_GLOBAL, importNode, NULL);
    ASTNode * functionNode = ASTUtility::createASTNode(DEPENDENCY_FUNC, globalNode, NULL);

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
                functionNode->right = ASTUtility::createASTNode(DEPENDENCY, this->function(), NULL);
                functionNode = functionNode->right;
            }
        }
        else if ((currToken->entry == "let") || 
            (currToken->entry == "const") )
        {
            globalNode->right = ASTUtility::createASTNode(DEPENDENCY, this->globalAssignment(), NULL);
            globalNode = globalNode->right;
        }
        else if (currToken->entry == "import")
        {
            ASTUtility::validToken(T_KEYWORD, currToken->entry, currToken);

            // this is a hack for using Xcode
            // currToken->entry = "/Users/wesitferrobin/Projects/workspace/W_Project/Main/test/" + currToken->entry;

            // this is a hack for using VS
            currToken->entry = "D:\\ProgramProjects\\W_Project\\Main\\test\\" + currToken->entry;
            
            ASTNode * importLib = this->importFile(root);
            
            ASTUtility::validToken(T_SYMBOL, ";", currToken);
            
            importNode->right = ASTUtility::createASTNode(IMPORT, importLib, NULL);
            importNode = importNode->right;
        }
    }
}

void Parser::initGrammar(bool isMainFile, TokenEntry * token_instance, ASTNode *& the_ast) 
{
    currToken = token_instance;
    start(isMainFile, the_ast);
}

ASTNode * Parser::main()
{
    ASTNode* mainNode = ASTUtility::createASTNode(MAIN, NULL, NULL);
    map<string, bool> placeholder_symbol_table = symbol_table;

    ASTUtility::validToken(T_KEYWORD, "function", currToken);
    ASTUtility::validToken(T_VARIABLE, "MAIN", currToken);
    ASTUtility::validToken(T_SYMBOL, "(", currToken);
    ASTUtility::validToken(T_SYMBOL, ")", currToken);
    ASTUtility::validToken(T_SYMBOL, "{", currToken);

    this->stmtList(mainNode);

    symbol_table.clear();

    ASTUtility::validToken(T_SYMBOL, "}", currToken);
    
    return mainNode;
}

void Parser::start(bool isMainFile, ASTNode *& root)
{
    try
    {
        root = ASTUtility::createASTNode(UNKNOWN, ASTUtility::createASTNullNode(), ASTUtility::createASTNullNode());
        dependencies(root);
        if (isMainFile)
        {
            root = ASTUtility::createASTNode(START, root, main());
        }
    }
    catch(int e)
    {
        ErrorReader::readError((ErrorCode)e, currToken->entry);
        root =  NULL;
    }
}
