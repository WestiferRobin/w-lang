#include "compiler_comp.hpp"

StdInit::StdInit(unsigned long long currCount) : programCounter(currCount) 
{ 
    initStd(); 
}

StdInit::StdInit() : programCounter(0)
{
    initStd();
}

void StdInit::initStd()
{
    initLimits();
    initStdConsts();
    initStdFuncs();
}

void StdInit::initLimits()
{
    limit_sym.push_back({"pintn", "0"});
    limit_sym.push_back({"pint", "1"});
    limit_sym.push_back({"pcharn", "2"});
    limit_sym.push_back({"pchar", "3"});
    limit_sym.push_back({"parr", "4"});
    limit_sym.push_back({"parrn", "5"});
    limit_sym.push_back({"pstr", "6"});
    limit_sym.push_back({"pstrn", "7"});
    limit_sym.push_back({"schar", "97"});
    limit_sym.push_back({"echar", "122"});
    limit_sym.push_back({"scchar", "65"});
    limit_sym.push_back({"ecchar", "90"});
    limit_sym.push_back({"mini", "-2147483648"});
    limit_sym.push_back({"maxi", "2147483647"});
}

void StdInit::addSymbolsToTable(map<string, bool> &global_sy)
{
    for (auto it = limit_sym.begin(); it != limit_sym.end(); it++)
    {
        global_sy.insert({get<0>(*it), true});
    }
}

void StdInit::initStdConsts()
{
    for (auto it = limit_sym.begin(); it != limit_sym.end(); it++)
    {
        stdInitAssembly.push_back(new AssemblyEntry(ALU_OPP, programCounter++, "", "load", get<0>(*it), get<1>(*it)));
    }
}

void StdInit::initStdFuncs()
{

}