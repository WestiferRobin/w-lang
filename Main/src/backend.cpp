#include "compiler_comp.hpp"

void BackEnd::run(ASTNode *& ast)
{
    if (ast != NULL)
    {
        initStandardLib();
        createAssembly(ast);
    }
}

void BackEnd::createAssembly(ASTNode *& root)
{
    if (root == NULL) return;

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
            endInstance->incAmmount();
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", "END" + to_string(endInstance->getAmmount()), ""));
            endInstance->push();
            break;
        case WHILE_LOOP_COND:
            whileInstance->incAmmount();
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", "WL" + to_string(whileInstance->getAmmount()), ""));
            whileInstance->push();
            break;
        case WHILE_LOOP_COND_P:
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", "WL" + to_string(whileInstance->peek()), ""));
            whileInstance->pop();
            break;
        case WHILE_LOOP_STATEMENT:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "WL" + to_string(whileInstance->peek()), "", "", ""));
            break;
        case FOR_LOOP_STATEMENT:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "FL" + to_string(forInstance->peek()), "", "", ""));
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
                endInstance->incAmmount();
                assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", "END" + to_string(endInstance->getAmmount()), ""));
                endInstance->push();
            }
            isLast = false;
            break;
        case SWITCH_COND:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), root->left->key));
            root->key = "r" + to_string(regIndex);
            break;
        case CASE_COND:
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "eq", "r" + to_string(regIndex), root->left->key));
            caseInstance->incAmmount();
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", "CS" + to_string(caseInstance->getAmmount()), ""));
            caseInstance->push();
            break;
        case CASE_STMT:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "CS" + to_string(caseInstance->peek()), "", "", ""));
            caseInstance->pop();
            break;
        case DEFAULT_STMT:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "DF" + to_string(defaultInstance->peek()), "", "", ""));
            defaultInstance->pop();
            break;
        case DEFAULT_COND:
            isLast = true;
            defaultInstance->incAmmount();
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", "DF" + to_string(defaultInstance->getAmmount()), ""));
            defaultInstance->push();
            break;
        case IF_COND:
            ifInstance->incAmmount();
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", "IFC" + to_string(ifInstance->getAmmount()), ""));
            ifInstance->push();
            regIndex = 0;
            break;
        case IF_STMT:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "IFC" + to_string(ifInstance->peek()), "", "", ""));
            ifInstance->pop();
            break;
        case ELIF_COND:
            elifInstance->incAmmount();
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", "EIC" + to_string(elifInstance->getAmmount()), ""));
            elifInstance->push();
            break;
        case ELIF_STMT:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "EIC" + to_string(elifInstance->peek()), "", "", ""));
            elifInstance->pop();
            break;
        case ELSE_COND:
            isLast = true;
            elseInstance->incAmmount();
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", "EC" + to_string(elseInstance->getAmmount()), ""));
            endInstance->incAmmount();
            endInstance->push();
            elseInstance->push();
            break;
        case ELSE_STMT:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "EC" + to_string(elseInstance->peek()), "", "", ""));
            elseInstance->pop();
            break;
        default:
            break;
    }
  
    createAssembly(root->right);

    ASTNode * elementPtr;
    
    switch (root->type)
    {
        case DEPENDENCY_GLOBAL:
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", "MAIN", ""));
            break;
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
        case FOR_LOOP_COND:
            forInstance->incAmmount();
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", "FL" + to_string(forInstance->getAmmount()), ""));
            forInstance->push();
            break;
        case FOR_LOOP_COND_P:
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jc", "FL" + to_string(forInstance->peek()), ""));
            forInstance->pop();
            break;
        case COND_CMPR:
        case WHILE_LOOP:
        case FOR_LOOP:
        case SWITCH_CMPR:
            assembly.push_back(new AssemblyEntry(JUMP_LABEL, programCounter++, "END" + to_string(endInstance->peek()), "", "", ""));
            endInstance->pop();
            break;
        case WHILE_LOOP_EXIT:
        case FOR_LOOP_EXIT:
        case IF_STMT:
        case ELIF_STMT:
        case ELSE_STMT:
        case CASE_STMT:
        case DEFAULT_STMT:
            assembly.push_back(new AssemblyEntry(JUMP_OPP, programCounter++, "", "jmp", "END" + to_string(endInstance->peek()), ""));
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
            assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter, "", "push", "RET_A", to_string(programCounter+1)));
            programCounter++;
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
            break;
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

void BackEnd::loadDependentLocals(ASTNode *& root)
{
    if (be_symbol_table.find(root->left->key) == be_symbol_table.end())
    {
        assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), root->left->key));
        root->left->key = "r" + to_string(regIndex);
    }
    if (be_symbol_table.find(root->right->key) == be_symbol_table.end())
    {
        assembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load",  "r" + to_string(++regIndex), root->right->key));
        root->right->key = "r" + to_string(regIndex);
    }
}