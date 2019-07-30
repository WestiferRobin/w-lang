#include "processor_comp.hpp"


bool Interpreter::isValidALUOp(string opcode)
{
    char flag = opcode[1];
    switch(flag)
    {
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case 'E':
            return true;
        case '8':
        case '9':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
            return opcode[0] == '0' ? true : false;
        default:
            return false;
    }
}

bool Interpreter::isValidJumpOp(string opcode)
{
    char flag = opcode[1];
    switch(flag)
    {
        case 'A':
        case 'B':
        case 'C':
        case 'D':
            return true;
        case '8':
        case '9':
            return opcode[0] == '1' ? true : false;
        default:
            return false;
    }
}

bool Interpreter::isValidAuxOp(string opcode)
{
    char flag = opcode[1];
    switch(flag)
    {
        case 'F':
            return opcode[0] == '0' ? true : false;
        case '0':
            return opcode[0] == '1' ? true : false;
        case '1':
            return true;
        default:
            return false;
    }
}

void Interpreter::calculate(map<string, short> * reg, Entry * instance)
{
    short reg1 = reg->at(instance->getRegister());
    short reg2 = instance->getType() == REGISTER ? reg->at(instance->getRemaining()) : Converter::hexToDec(instance->getRemaining());
    short ans = 0;
    switch(instance->getOppCode()[1])
    {
        case '2':
            ans = reg1 + reg2;
            break;
        case '3':
            ans = reg1 - reg2;
            break;
        case '4':
            ans = reg1 * reg2; 
            break;
        case '5':
            ans = reg1 / reg2;
            break;
        case '6':
            ans = reg1 & reg2;
            break;
        case '7':
            ans = reg1 | reg2;
            break;
        case 'E':
            ans = !reg1;
            break;
        case '8':
            ans = reg1 == reg2 ? 1 : 0;
            break;
        case '9':
            ans = reg1 != reg2 ? 1 : 0;
            break;
        case 'A':
            ans = reg1 < reg2 ? 1 : 0;
            break;
        case 'B':
            ans = reg1 > reg2 ? 1 : 0;
            break;
        case 'C':
            ans = reg1 <= reg2 ? 1 : 0;
            break;
        case 'D':
            ans = reg1 >= reg2 ? 1 : 0;
            break;
    }
    reg->at(instance->getRegister()) = ans;
}

void Interpreter::auxExec(map<string, short> * reg, Entry * instance)
{
    switch(instance->getOppCode()[1])
    {
        case '0':
            cout << reg->at(instance->getRegister()) << endl;
            break;
        case '1':
            if (instance->getOppCode()[1] == '1')
            {
                cout << reg->at(instance->getRegister()) << endl;
                cout << Converter::hexToDec(instance->getRemaining()) << endl;
                reg->at(instance->getRegister()) = Converter::hexToDec(instance->getRemaining());
                cout << reg->at(instance->getRegister()) << endl;
            }
            else
            {
                reg->at(instance->getRegister()) = reg->at(instance->getRemaining());
            }
            break;
        case 'F':
            short tmp = reg->at(instance->getRegister());
            reg->at(instance->getRegister()) = reg->at(instance->getRemaining());
            reg->at(instance->getRemaining()) = tmp;
            break;
    }
}

int Converter::hexToDec(string code_line)
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
