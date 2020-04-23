#include "compiler_comp.hpp"
#include "ast_types.hpp"

/*
    Public Function: run
    - runs the operation of BackEnd.
*/
void BackEnd::run(ASTNode* ast)
{
    if (ast != NULL)
    {
        initStandardLib();
        createAssembly(ast);
    }
}

void BackEnd::createAssembly(ASTNode * root)
{
    if (root == NULL) {return;}

    createAssembly(root->left);
    
    switch (root->type)
    {
        case MAIN:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "MAIN", "", "", ""));
            break;
        case FUNCTION:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, root->key, "", "", ""));
            break;
        case WHILE_LOOP:
        case FOR_LOOP:
        case SWITCH_END:
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", "END" + to_string(++endStatementInstance), ""));
            endInstances.push(endStatementInstance);
            break;
        case WHILE_LOOP_COND:
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", "WL" + to_string(++whileLoopInstance), ""));
            whileInstances.push(whileLoopInstance);
            break;
        case WHILE_LOOP_COND_P:
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", "WL" + to_string(whileInstances.top()), ""));
            whileInstances.pop();
            break;
        case WHILE_LOOP_STATEMENT:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "WL" + to_string(whileInstances.top()), "", "", ""));
            break;
        case FOR_LOOP_STATEMENT:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "FL" + to_string(forInstances.top()), "", "", ""));
            break;
        case ASSIGN:
        case ASSIGN_ADD:
        case ASSIGN_AND:
        case ASSIGN_DIV:
        case ASSIGN_MINUS:
        case ASSIGN_MULTI:
        case ASSIGN_OR:
            isOneNumber = root->right->right == NULL && root->right->left == NULL;
            break;
        case COND_CMPR:
            if (!isLast) 
            {
                assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", "END" + to_string(++endStatementInstance), ""));
                endInstances.push(endStatementInstance);
            }
            isLast = false;
            break;
        case SWITCH_COND:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), root->left->key));
            root->key = "r" + to_string(regIndex);
            break;
        case CASE_COND:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "eq", "r" + to_string(regIndex), root->left->key));
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", "CS" + to_string(++caseStatementInstance), ""));
            caseInstance.push(caseStatementInstance);
            break;
        case CASE_STMT:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "CS" + to_string(caseInstance.front()), "", "", ""));
            caseInstance.pop();
            break;
        case DEFAULT_STMT:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "DF" + to_string(defaultInstance.top()), "", "", ""));
            defaultInstance.pop();
            break;
        case DEFAULT_COND:
            isLast = true;
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", "DF" + to_string(++defaultStatementInstance), ""));
            defaultInstance.push(defaultStatementInstance);
            break;
        case IF_COND:
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", "IFC" + to_string(++ifStatmentInstance), ""));
            ifInstances.push(ifStatmentInstance);
            regIndex = 0;
            break;
        case IF_STMT:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "IFC" + to_string(ifInstances.top()), "", "", ""));
            ifInstances.pop();
            break;
        case ELIF_COND:
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", "EIC" + to_string(++elifStatementInstance), ""));
            elifInstances.push(elifStatementInstance);
            break;
        case ELIF_STMT:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "EIC" + to_string(elifInstances.front()), "", "", ""));
            elifInstances.pop();
            break;
        case ELSE_COND:
            isLast = true;
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", "EC" + to_string(++elseStatementInstance), ""));
            endInstances.push(++endStatementInstance);
            elseInstances.push(elseStatementInstance);
            break;
        case ELSE_STMT:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "EC" + to_string(elseInstances.top()), "", "", ""));
            elseInstances.pop();
            break;
        default:
            break;
    }
  
    createAssembly(root->right);

    ASTNode * elementPtr;
    auto functionCallCounter = 0;
    switch (root->type)
    {
        case IMPORT:
            return;
        case ARRAY_INIT_SIZE:
        case ARRAY_EXCH:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "arrL", root->left->key, root->right->key));
            break;
        case ARRAY_INIT:
            if (root->right->type == ARRAY_INIT_PRE_ELM)
            {
                assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "arrL", root->left->key, root->right->left->key));
                elementPtr = root->right->right->right;
                while(elementPtr)
                {
                    assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "arrE", root->left->key, elementPtr->key));
                    elementPtr = elementPtr->right;
                }
            }
            else
            {
                assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "arrL", root->left->key, root->right->key));
            }
            break;
        case PARAM_ARRAY_INST:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "arrL", root->right->key, "PARAM_ARR"));
            break;
        case PARAM_INST:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", root->right->key, "PARAM"));
            break;
        case PARAM_ARRAY_CALL:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "push", "PARAM_ARR", root->left->key));
            break;
        case PARAM_CALL:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "push", "PARAM", root->left->key));
            break;
        case DEPENDENCY_GLOBAL:
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", "MAIN", ""));
            break;
        case FOR_LOOP_COND:
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", "FL" + to_string(++forLoopInstance), ""));
            forInstances.push(forLoopInstance);
            break;
        case FOR_LOOP_COND_P:
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", "FL" + to_string(forInstances.top()), ""));
            forInstances.pop();
            break;
        case COND_CMPR:
        case WHILE_LOOP:
        case FOR_LOOP:
        case SWITCH_CMPR:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "END" + to_string(endInstances.top()), "", "", ""));
            endInstances.pop();
            break;
        case WHILE_LOOP_EXIT:
        case FOR_LOOP_EXIT:
        case IF_STMT:
        case ELIF_STMT:
        case ELSE_STMT:
        case CASE_STMT:
        case DEFAULT_STMT:
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", "END" + to_string(endInstances.top()), ""));
            break;
        case DELETE:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "delete", root->left->key, ""));
            break;
        case RETURN_ARR:
        case RETURN:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", root->type == RETURN ? "load" : "arrL", "RET_V", root->left->key));
            root->key = "RET_V";
            break;
        case FUNCTION:
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", "RET_A", ""));
            break;
        case FUNCTION_CALL:
            functionCallCounter = ++programCounter;
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter, "", "push", "RET_A", to_string(functionCallCounter)));
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", root->key, ""));
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "pop", "RET_A", ""));
            root->key = "RET_V";
            break;
        case NUM:
        case CHAR:
            if (!isOneNumber)
            {
                assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), to_string(root->value)));
                root->key = "r" + to_string(regIndex);
                break;
            }
            else 
                return;
        case BOOL_AND:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "and", root->left->key, root->right->key));
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR"));
            root->key = "r" + to_string(regIndex);
            break;
        case BOOL_OR:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "or", root->left->key, root->right->key));
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR"));
            root->key = "r" + to_string(regIndex);
            break;
        case BIT_AND:
            loadDependentLocals(root);
        case ASSIGN_AND:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "b_and", root->left->key, root->right->key));
            root->key = root->left->key;
            break;
        case BIT_OR:
            loadDependentLocals(root);
        case ASSIGN_OR:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "b_or", root->left->key, root->right->key));
            root->key = root->left->key;
            break;
        case BIT_XOR:
            loadDependentLocals(root);
        case ASSIGN_XOR:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "xor", root->left->key, root->right->key));
            root->key = root->left->key;
            break;
        case BIT_LEFT:
            loadDependentLocals(root);
        case ASSIGN_SHIFT_LEFT:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "sh_left", root->left->key, root->right->key));
            root->key = root->left->key;
            break;
        case BIT_RIGHT:
            loadDependentLocals(root);
        case ASSIGN_SHIFT_RIGHT:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "sh_right", root->left->key, root->right->key));
            root->key = root->left->key;
            break;
        case ADD:
            loadDependentLocals(root);
        case ASSIGN_ADD:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "add", root->left->key, root->right->key));
            root->key = root->left->key;
            break;
        case MINUS:
            loadDependentLocals(root);
        case ASSIGN_MINUS:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "sub", root->left->key, root->right->key));
            root->key = root->left->key;
            break;
        case DIV:
            loadDependentLocals(root);
        case ASSIGN_DIV:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "div", root->left->key, root->right->key));
            root->key = root->left->key;
            break;
        case MULTI:
            loadDependentLocals(root);
        case ASSIGN_MULTI:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "multi", root->left->key, root->right->key));
            root->key = root->left->key;
            break;
        case MODULUS:
            loadDependentLocals(root);
        case ASSIGN_MODULUS:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "mod", root->left->key, root->right->key));
            root->key = root->left->key;
            break;
        case ASSIGN_NONE:
            root->right->key = "0";
        case ASSIGN:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", root->left->key, root->right->key));
            root->key = root->left->key;
            regIndex = 0;
            break;
        case NOT:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "not", root->right->key, ""));
            root->key = root->right->key;
            break;
        case PRINT:;
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "print", root->left->key, root->right->key));
            root->key = root->left->key;
            break;
        case SCAN:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "scan", root->left->key, ""));
            root->key = root->left->key;
            break;
        case LENGTH:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "len", root->left->key, root->right->key));
            root->key = root->left->key;
            break;
        case EQUAL_FUNC:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "eqf", root->right->left->key, root->right->right->key));
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", root->left->key, "CMPR"));
            root->key = root->left->key;
            break;
        case COPY:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "copy", root->left->key, root->right->key));
            root->key = root->left->key;
            break;
        case EQUALS:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "eq", root->left->key, root->right->key));
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR"));
            root->key = "r" + to_string(regIndex);
            break;
        case NOT_EQUALS:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "neq", root->left->key, root->right->key));
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR"));
            root->key = "r" + to_string(regIndex);
            break;
        case LESS_THAN:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "lt", root->left->key, root->right->key));
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR"));
            root->key = "r" + to_string(regIndex);
            break;
        case GREATER_THAN:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "gt", root->left->key, root->right->key));
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR"));
            root->key = "r" + to_string(regIndex);
            break;
        case LESS_THAN_EQ:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "lte", root->left->key, root->right->key));
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR"));
            root->key = "r" + to_string(regIndex);
            break;
        case GREATER_THAN_EQ:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "gte", root->left->key, root->right->key));
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR"));
            root->key = "r" + to_string(regIndex);
            break;
        default:
            return;
    }
}

void BackEnd::initStandardLib()
{
    StdInit * stdInit = new StdInit(programCounter);
    vector<AssemblyEntry*> ans = stdInit->getAssembly();
    programCounter = stdInit->getCounter();
    for (auto i = ans.begin(); i != ans.end(); i++)
    {
        assembly.push_back(*i);
    }
    delete stdInit;
}


void BackEnd::loadDependentLocals(ASTNode * root)
{
    if (symbol_table.find(root->left->key) == symbol_table.end())
    {
        assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), root->left->key));
        root->left->key = "r" + to_string(regIndex);
    }
    if (symbol_table.find(root->right->key) == symbol_table.end())
    {
        assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), root->right->key));
        root->right->key = "r" + to_string(regIndex);
    }
}