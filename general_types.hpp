#ifndef _GENERAL_TYPES_H_
#define _GENERAL_TYPES_H_
#include <iostream>

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
    int counter;
    string label;
    string operatorLabel;
    string firstOp;
    string secondOp;
    AssemblyEntryType type;
    AssemblyEntry(AssemblyEntryType type,int counter, string label, string operatorLabel, string first, string second) 
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


#endif