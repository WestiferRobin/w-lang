#ifndef _GENERAL_TYPES_H_
#define _GENERAL_TYPES_H_
#include <iostream>
#include <stack>
#include <queue>
#include <climits>


using namespace std;


enum AssemblyEntryType
{
    JUMP_LABEL,
    JUMP_OPP,
    ALU_OPP,
};


class AssemblyEntry
{
public:
    unsigned long long counter;
    string label;
    string operatorLabel;
    string firstOp;
    string secondOp;
    AssemblyEntryType type;
    AssemblyEntry(AssemblyEntryType type, unsigned long long counter, string label, string operatorLabel, string first, string second)
    {
        this->type = type;
        this->counter = counter;
        this->label = label;
        this->operatorLabel = operatorLabel;
        this->firstOp = first;
        this->secondOp = second;
    }
    ~AssemblyEntry() {}
    string toString()
    {
        string ans = to_string(counter) + " ";
        ans += !label.empty() ? label + ": " : "\t";
        ans += !operatorLabel.empty() ? operatorLabel + " " : "";
        ans += !firstOp.empty() ? firstOp + " " : "";
        ans += !secondOp.empty() ? secondOp + " " : "";
        return ans;
    }
};


enum ASTType
{
   MODULUS,
   NOT,
   ADD,
   MINUS,
   BIT_AND,
   BOOL_AND,
   BOOL_OR,
   BIT_OR,
   BIT_XOR,
   BIT_LEFT,
   BIT_RIGHT,
   DIV,
   MULTI,

   NUM,
   VAR,
   VAR_ARRAY,
   CHAR,
   KEYWORD,

   EQUALS,
   NOT_EQUALS,
   LESS_THAN,
   GREATER_THAN,
   LESS_THAN_EQ,
   GREATER_THAN_EQ,

   ASSIGN,
   ASSIGN_NONE,
   ASSIGN_ADD,
   ASSIGN_MINUS,
   ASSIGN_AND,
   ASSIGN_OR,
   ASSIGN_DIV,
   ASSIGN_MULTI,
   ASSIGN_XOR,
   ASSIGN_SHIFT_LEFT,
   ASSIGN_SHIFT_RIGHT,
   ASSIGN_MODULUS,
   
   ARRAY_EXCH,
   ARRAY_INIT,
   ARRAY_INIT_SIZE,
   ARRAY_INIT_PRE_ELM,
   ARRAY_INIT_ELM,
   ARRAY_ACCESS,
   ARRAY_WHOLE,
   
   COND_CMPR,
   IF_STMT,
   IF_COND,
   ELIF_STMT,
   ELIF_COND,
   ELSE_STMT,
   ELSE_COND,

   WHILE_LOOP,
   WHILE_LOOP_COND,
   WHILE_LOOP_COND_P,
   WHILE_LOOP_STATEMENT,
   WHILE_LOOP_EXIT,

   FOR_LOOP,
   FOR_LOOP_COND,
   FOR_LOOP_COND_P,
   FOR_LOOP_STATEMENT,
   FOR_LOOP_EXIT,

   START,
   MAIN,
   STMTLIST,
   DEPENDENCY_FUNC,
   DEPENDENCY_GLOBAL,
   DEPENDENCY,
   
   FUNCTION,
   FUNCTION_CALL,
   PARAM_CALL,
   PARAM_INST,
   PARAM_ARRAY_CALL,
   PARAM_ARRAY_INST,
   RETURN,
   RETURN_ARR,

   PRINT,
   SCAN,
   COPY,
   LENGTH,
   RANDOM,
   APPEND,
   REMOVE,
   EQUAL_FUNC,
   DELETE,

   UNKNOWN
};


class ASTNode
{
public:
   ASTType type;
   int value;
   string key;
   ASTNode * left;
   ASTNode * right;
 
   ASTNode()
   {
      this->type = UNKNOWN;
      value = INT_MIN;
      right = NULL;
      left = NULL;
      key = "";
   }
 
   ~ASTNode()
   {
      delete left;
      delete right;
      key.clear();
      
   }
};

enum ErrorCode
{
    ErrorInvalidBoolOp = 1,
    ErrorInvalidSymbol = 2,
    ErrorInvalidArithOp = 3,
    ErrorInvalidKeyword = 4,
    ErrorVariableUnknown = 5,
    ErrorInvalidLoad = 6,
    ErrorInvalidOpCode = 7,
    ErrorInvalidConstant = 8
};

class Instance
{
private:
    unsigned int instAmmount;
public:
    Instance() { instAmmount = 0; }
    virtual ~Instance() {}
    unsigned int getAmmount() { return instAmmount; }
    void resetAmmount() { instAmmount = 0; }
    void incAmmount() { instAmmount++; }
    virtual void push() = 0;
    virtual void pop() = 0;
    virtual unsigned int peek() = 0;
};


class StackInstance : public Instance
{
private:
    stack<unsigned int> stackInstance;
public:
    StackInstance() : Instance() {}
    ~StackInstance() {}
    void push() { stackInstance.push(this->getAmmount()); }
    void pop() { stackInstance.pop(); }
    unsigned int peek() { return stackInstance.top(); }
};


class QueueInstance : public Instance
{
private:
    queue<unsigned int> queueInstance;
public:
    QueueInstance() : Instance() {}
    ~QueueInstance() {}
    void push() { queueInstance.push(this->getAmmount()); }
    void pop() { queueInstance.pop(); }
    unsigned int peek() { return queueInstance.front(); }
};


enum TokenType
{
    T_NUMBER,
    T_VARIABLE,
    T_KEYWORD,
    T_SYMBOL,
    T_ENDOFFILE,
    T_CHAR,
    T_STRING,
};


struct TokenEntry
{
    TokenType tType;
    string entry;
    TokenEntry(TokenType newType, string newEntry) : tType(newType), entry(newEntry) {}
    ~TokenEntry() {}
};

#endif
