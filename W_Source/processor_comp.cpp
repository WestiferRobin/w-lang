#include "processor_comp.hpp"


bool Interpreter::isValidALUOp(string opcode)
{
    return false;
}

bool Interpreter::isValidJumpOp(string opcode)
{
    return false;
}

bool Interpreter::isValidAuxOp(string opcode)
{
    return false;
}

int Converter::decToHex(string code_line)
{
    int ans = 0;
    int multiplier = 1;
    for (int i = code_line.size() - 1; i >= 0; i--)
    {
        int result = 0;
        switch (code_line.at(i))
        {
            case '0':
                result = 0;
                break;
            case '1':
                result = 1;
                break;
            case '2':
                result = 2;
                break;
            case '3':
                result = 3;
                break;
            case '4':
                result = 4;
                break;
            case '5':
                result = 5;
                break;
            case '6':
                result = 6;
                break;
            case '7':
                result = 7;
                break;
            case '8':
                result = 8;
                break;
            case '9':
                result = 9;
                break;
            case 'A':
                result = 10;
                break;
            case 'B':
                result = 11;
                break;
            case 'C':
                result = 12;
                break;
            case 'D':
                result = 13;
                break;
            case 'E':
                result = 14;
                break;
            case 'F':
                result = 15;
                break;
        }
        ans += multiplier * result;
        multiplier *= 16;
    }
    return ans;
}