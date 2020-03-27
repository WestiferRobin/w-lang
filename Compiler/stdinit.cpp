#include "compiler_comp.hpp"

void StdInit::initStdConsts()
{
    stdInitAssembly.push_back(*(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", "pintn", "0")));
    stdInitAssembly.push_back(*(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", "pint", "1")));
    stdInitAssembly.push_back(*(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", "pcharn", "2")));
    stdInitAssembly.push_back(*(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", "pchar", "3")));
    stdInitAssembly.push_back(*(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", "parr", "4")));
    stdInitAssembly.push_back(*(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", "parrn", "5")));
    stdInitAssembly.push_back(*(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", "pstr", "6")));
    stdInitAssembly.push_back(*(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", "pstrn", "7")));
    stdInitAssembly.push_back(*(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", "schar", "97")));
    stdInitAssembly.push_back(*(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", "echar", "122")));
    stdInitAssembly.push_back(*(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", "scchar", "65")));
    stdInitAssembly.push_back(*(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", "ecchar", "90")));
}

void StdInit::initStdFuncs()
{

}