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

    AssemblyEntry * entry;
    string assemblyLabel;

    createAssembly(root->left);
    
    switch (root->type)
    {
        case MAIN:
            entry = new AssemblyEntry(JUMP_LABEL, programCounter++, "MAIN", "", "", "");
            assembly.push_back(*entry);
            break;
        case FUNCTION:
            entry = new AssemblyEntry(JUMP_LABEL, programCounter++, root->key, "", "", "");
            assembly.push_back(*entry);
            break;
        case WHILE_LOOP:
        case FOR_LOOP:
        case SWITCH_END:
            assemblyLabel = "END" + to_string(++endStatementInstance);
            entry = new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", assemblyLabel, "");
            assembly.push_back(*entry);
            endInstances.push(endStatementInstance);
            break;
        case WHILE_LOOP_COND:
            assemblyLabel = "WL" + to_string(++whileLoopInstance);
            entry = new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", assemblyLabel, "");
            assembly.push_back(*entry);
            whileInstances.push(whileLoopInstance);
            break;
        case WHILE_LOOP_COND_P:
            assemblyLabel = "WL" + to_string(whileInstances.top());
            entry = new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", assemblyLabel, "");
            assembly.push_back(*entry);
            whileInstances.pop();
            break;
        case WHILE_LOOP_STATEMENT:
            assemblyLabel = "WL" + to_string(whileInstances.top());
            entry = new AssemblyEntry(JUMP_LABEL, programCounter++, assemblyLabel, "", "", "");
            assembly.push_back(*entry);
            break;
        case FOR_LOOP_STATEMENT:
            assemblyLabel = "FL" + to_string(forInstances.top());
            entry = new AssemblyEntry(JUMP_LABEL, programCounter++, assemblyLabel, "", "", "");
            assembly.push_back(*entry);
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
                assemblyLabel = "END" + to_string(++endStatementInstance);
                entry = new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", assemblyLabel, "");
                assembly.push_back(*entry);
                endInstances.push(endStatementInstance);
            }
            isLast = false;
            break;
        case SWITCH_COND:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), root->left->key);
            assembly.push_back(*entry);
            root->key = "r" + to_string(regIndex);
            break;
        case CASE_COND:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "eq", "r" + to_string(regIndex), root->left->key);
            assembly.push_back(*entry);
            assemblyLabel = "CS" + to_string(++caseStatementInstance);
            entry = new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", assemblyLabel, "");
            assembly.push_back(*entry);
            caseInstance.push(caseStatementInstance);
            break;
        case CASE_STMT:
            assemblyLabel = "CS" + to_string(caseInstance.front());
            entry = new AssemblyEntry(JUMP_LABEL, programCounter++, assemblyLabel, "", "", "");
            assembly.push_back(*entry);
            caseInstance.pop();
            break;
        case DEFAULT_STMT:
            assemblyLabel = "DF" + to_string(defaultInstance.top());
            entry = new AssemblyEntry(JUMP_LABEL, programCounter++, assemblyLabel, "", "", "");
            assembly.push_back(*entry);
            defaultInstance.pop();
            break;
        case DEFAULT_COND:
            isLast = true;
            assemblyLabel = "DF" + to_string(++defaultStatementInstance);
            entry = new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", assemblyLabel, "");
            assembly.push_back(*entry);
            defaultInstance.push(defaultStatementInstance);
            break;
        case IF_COND:
            assemblyLabel = "IFC" + to_string(++ifStatmentInstance);
            entry = new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", assemblyLabel, "");
            assembly.push_back(*entry);
            ifInstances.push(ifStatmentInstance);
            regIndex = 0;
            break;
        case IF_STMT:
            assemblyLabel = "IFC" + to_string(ifInstances.top());
            entry = new AssemblyEntry(JUMP_LABEL, programCounter++, assemblyLabel, "", "", "");
            assembly.push_back(*entry);
            ifInstances.pop();
            break;
        case ELIF_COND:
            assemblyLabel = "EIC" + to_string(++elifStatementInstance);
            entry = new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", assemblyLabel, "");
            assembly.push_back(*entry);
            elifInstances.push(elifStatementInstance);
            break;
        case ELIF_STMT:
            assemblyLabel = "EIC" + to_string(elifInstances.front());
            entry = new AssemblyEntry(JUMP_LABEL, programCounter++, assemblyLabel, "", "", "");
            assembly.push_back(*entry);
            elifInstances.pop();
            break;
        case ELSE_COND:
            isLast = true;
            assemblyLabel = "EC" + to_string(++elseStatementInstance);
            entry = new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", assemblyLabel, "");
            assembly.push_back(*entry);
            endInstances.push(++endStatementInstance);
            elseInstances.push(elseStatementInstance);
            break;
        case ELSE_STMT:
            assemblyLabel = "EC" + to_string(elseInstances.top());
            entry = new AssemblyEntry(JUMP_LABEL, programCounter++, assemblyLabel, "", "", "");
            assembly.push_back(*entry);
            elseInstances.pop();
            break;
        default:
            break;
    }
  
    createAssembly(root->right);

    ASTNode * elementPtr;
    switch (root->type)
    {
        case IMPORT:
            return;
        case ARRAY_INIT_SIZE:
        case ARRAY_EXCH:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "arrL", root->left->key, root->right->key);
            assembly.push_back(*entry);
            break;
        case ARRAY_INIT:
            if (root->right->type == ARRAY_INIT_PRE_ELM)
            {
                entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "arrL", root->left->key, root->right->left->key);
                assembly.push_back(*entry);
                elementPtr = root->right->right->right;
                while(elementPtr)
                {
                    entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "arrE", root->left->key, elementPtr->key);
                    assembly.push_back(*entry);
                    elementPtr = elementPtr->right;
                }
            }
            else
            {
                entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "arrL", root->left->key, root->right->key);
                assembly.push_back(*entry);
            }
            break;
        case PARAM_ARRAY_INST:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "arrL", root->right->key, "PARAM_ARR");
            assembly.push_back(*entry);
            break;
        case PARAM_INST:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "load", root->right->key, "PARAM");
            assembly.push_back(*entry);
            break;
        case PARAM_ARRAY_CALL:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "push", "PARAM_ARR", root->left->key);
            assembly.push_back(*entry);
            break;
        case PARAM_CALL:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "push", "PARAM", root->left->key);
            assembly.push_back(*entry);
            break;
        case DEPENDENCY_GLOBAL:
            entry = new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", "MAIN", "");
            assembly.push_back(*entry);
            break;
        case FOR_LOOP_COND:
            assemblyLabel = "FL" + to_string(++forLoopInstance);
            entry = new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", assemblyLabel, "");
            assembly.push_back(*entry);
            forInstances.push(forLoopInstance);
            break;
        case FOR_LOOP_COND_P:
            assemblyLabel = "FL" + to_string(forInstances.top());
            entry = new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", assemblyLabel, "");
            assembly.push_back(*entry);
            forInstances.pop();
            break;
        case COND_CMPR:
        case WHILE_LOOP:
        case FOR_LOOP:
        case SWITCH_CMPR:
            assemblyLabel = "END" + to_string(endInstances.top());
            entry = new AssemblyEntry(JUMP_LABEL, programCounter++, assemblyLabel, "", "", "");
            assembly.push_back(*entry);
            endInstances.pop();
            break;
        case WHILE_LOOP_EXIT:
        case FOR_LOOP_EXIT:
        case IF_STMT:
        case ELIF_STMT:
        case ELSE_STMT:
        case CASE_STMT:
        case DEFAULT_STMT:
            assemblyLabel = "END" + to_string(endInstances.top());
            entry = new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", assemblyLabel, "");
            assembly.push_back(*entry);
            break;
        case DELETE:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "delete", root->left->key, "");
            assembly.push_back(*entry);
            break;
        case RETURN_ARR:
        case RETURN:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", root->type == RETURN ? "load" : "arrL", "RET_V", root->left->key);
            assembly.push_back(*entry);
            root->key = "RET_V";
            break;
        case FUNCTION:
            assemblyLabel = "RET_A";
            entry = new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", assemblyLabel, "");
            assembly.push_back(*entry);
            break;
        case FUNCTION_CALL:
            entry = new AssemblyEntry(ALU_OPP, programCounter, "", "push", "RET_A", to_string(programCounter + 1));
            programCounter++;
            assembly.push_back(*entry);
            entry = new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", root->key, "");
            assembly.push_back(*entry);
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "pop", "RET_A", "");
            assembly.push_back(*entry);
            root->key = "RET_V";
            break;
        case NUM:
        case CHAR:
            if (!isOneNumber)
            {
                entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), to_string(root->value));
                assembly.push_back(*entry);
                root->key = "r" + to_string(regIndex);
                break;
            }
            else 
                return;
        case BOOL_AND:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "and", root->left->key, root->right->key);
            assembly.push_back(*entry);
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR");
            assembly.push_back(*entry);
            root->key = "r" + to_string(regIndex);
            break;
        case BOOL_OR:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "or", root->left->key, root->right->key);
            assembly.push_back(*entry);
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR");
            assembly.push_back(*entry);
            root->key = "r" + to_string(regIndex);
            break;
        case BIT_AND:
            loadDependentLocals(root);
        case ASSIGN_AND:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "b_and", root->left->key, root->right->key);
            assembly.push_back(*entry);
            root->key = root->left->key;
            break;
        case BIT_OR:
            loadDependentLocals(root);
        case ASSIGN_OR:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "b_or", root->left->key, root->right->key);
            assembly.push_back(*entry);
            root->key = root->left->key;
            break;
        case BIT_XOR:
            loadDependentLocals(root);
        case ASSIGN_XOR:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "xor", root->left->key, root->right->key);
            assembly.push_back(*entry);
            root->key = root->left->key;
            break;
        case BIT_LEFT:
            loadDependentLocals(root);
        case ASSIGN_SHIFT_LEFT:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "sh_left", root->left->key, root->right->key);
            assembly.push_back(*entry);
            root->key = root->left->key;
            break;
        case BIT_RIGHT:
            loadDependentLocals(root);
        case ASSIGN_SHIFT_RIGHT:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "sh_right", root->left->key, root->right->key);
            assembly.push_back(*entry);
            root->key = root->left->key;
            break;
        case ADD:
            loadDependentLocals(root);
        case ASSIGN_ADD:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "add", root->left->key, root->right->key);
            assembly.push_back(*entry);
            root->key = root->left->key;
            break;
        case MINUS:
            loadDependentLocals(root);
        case ASSIGN_MINUS:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "sub", root->left->key, root->right->key);
            assembly.push_back(*entry);
            root->key = root->left->key;
            break;
        case DIV:
            loadDependentLocals(root);
        case ASSIGN_DIV:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "div", root->left->key, root->right->key);
            assembly.push_back(*entry);
            root->key = root->left->key;
            break;
        case MULTI:
            loadDependentLocals(root);
        case ASSIGN_MULTI:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "multi", root->left->key, root->right->key);
            assembly.push_back(*entry);
            root->key = root->left->key;
            break;
        case MODULUS:
            loadDependentLocals(root);
        case ASSIGN_MODULUS:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "mod", root->left->key, root->right->key);
            assembly.push_back(*entry);
            root->key = root->left->key;
            break;
        case ASSIGN_NONE:
            root->right->key = "0";
        case ASSIGN:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "load", root->left->key, root->right->key);
            assembly.push_back(*entry);
            root->key = root->left->key;
            regIndex = 0;
            break;
        case NOT:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "not", root->right->key, "");
            assembly.push_back(*entry);
            root->key = root->right->key;
            break;
        case PRINT:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "print", root->left->key, root->right->key);
            assembly.push_back(*entry);
            root->key = root->left->key;
            break;
        case SCAN:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "scan", root->left->key, "");
            assembly.push_back(*entry);
            root->key = root->left->key;
            break;
        case LENGTH:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "len", root->left->key, root->right->key);
            assembly.push_back(*entry);
            root->key = root->left->key;
            break;
        case EQUAL_FUNC:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "eqf", root->right->left->key, root->right->right->key);
            assembly.push_back(*entry);
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "load", root->left->key, "CMPR");
            assembly.push_back(*entry);
            root->key = root->left->key;
            break;
        case COPY:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "copy", root->left->key, root->right->key);
            assembly.push_back(*entry);
            root->key = root->left->key;
            break;
        case EQUALS:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "eq", root->left->key, root->right->key);
            assembly.push_back(*entry);
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR");
            assembly.push_back(*entry);
            root->key = "r" + to_string(regIndex);
            break;
        case NOT_EQUALS:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "neq", root->left->key, root->right->key);
            assembly.push_back(*entry);
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR");
            assembly.push_back(*entry);
            root->key = "r" + to_string(regIndex);
            break;
        case LESS_THAN:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "lt", root->left->key, root->right->key);
            assembly.push_back(*entry);
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR");
            assembly.push_back(*entry);
            root->key = "r" + to_string(regIndex);
            break;
        case GREATER_THAN:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "gt", root->left->key, root->right->key);
            assembly.push_back(*entry);
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR");
            assembly.push_back(*entry);
            root->key = "r" + to_string(regIndex);
            break;
        case LESS_THAN_EQ:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "lte", root->left->key, root->right->key);
            assembly.push_back(*entry);
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR");
            assembly.push_back(*entry);
            root->key = "r" + to_string(regIndex);
            break;
        case GREATER_THAN_EQ:
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "gte", root->left->key, root->right->key);
            assembly.push_back(*entry);
            entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), "CMPR");
            assembly.push_back(*entry);
            root->key = "r" + to_string(regIndex);
            break;
        default:
            return;
    }
    delete entry;
}

void BackEnd::initStandardLib()
{
    StdInit stdInit(programCounter);
    vector<AssemblyEntry> ans = stdInit.getAssembly();
    programCounter = stdInit.getCounter();
    for (auto i = ans.begin(); i != ans.end(); i++)
    {
        assembly.push_back(*i);
    }
}


void BackEnd::loadDependentLocals(ASTNode * root)
{
    AssemblyEntry * entry;
    if (symbol_table.find(root->left->key) == symbol_table.end())
    {
        entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), root->left->key);
        assembly.push_back(*entry);
        root->left->key = "r" + to_string(regIndex);
    }
    if (symbol_table.find(root->right->key) == symbol_table.end())
    {
        entry = new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), root->right->key);
        assembly.push_back(*entry);
        root->right->key = "r" + to_string(regIndex);
    }
    delete entry;
}