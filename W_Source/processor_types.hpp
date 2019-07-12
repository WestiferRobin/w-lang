#ifndef _PROCESSOR_TYPES_H_
#define _PROCESSOR_TYPES_H_
#include <iostream>

using namespace std;

// using capital letters cause 'register' is a keyword
enum EntryType
{
    R,
    I,
    M,
    H,
};

class Entry
{
private:
    string org_line[4];
    EntryType control_type;
public:
    Entry(string raw, EntryType type) : control_type(type) 
    {  
        for (int i = 0; i < 4; i++)
        {
            org_line[i] = raw.substr(2 * i, 2);
        }
    }
    ~Entry() {}
    string getOppCode(void);
    string getRegister(void);
    string getRemaining(void);
    EntryType getType(void);
    void print() { for (int i = 0; i < 4; i++) { cout << org_line[i] << "|"; } cout << endl; }
};

#endif 