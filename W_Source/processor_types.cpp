#include "processor_types.hpp"

string Entry::getOppCode(void)
{
    return org_line[0];
}

string Entry::getRegister(void)
{
    return org_line[1];
}

string Entry::getRemaining(void)
{
    switch (control_type)
    {
        case R:
            return org_line[2];
        case I:
            return org_line[2] + org_line[3];
        default:
            return "";   
    }
}

EntryType Entry::getType(void)
{
    return control_type;
}