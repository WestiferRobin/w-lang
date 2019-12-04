#include "processor.hpp"

void Processor::init()
{
  // setups registers and initalizes its value
  for (int i = 1; i <= MAX_NUM_REGISTERS; i++)
  {
    registers.insert({"r" + to_string(i), 0});
  }
  registers.insert({"CMPR", 0});
  
  // setups the labels needed for the program
  for (int i = 0; i < assembly.size(); i++)
  {
    if (assembly[i].type == JUMP_LABEL)
    {
      jump_labels.insert({assembly[i].label, assembly[i].counter});
    }
  }
}

void Processor::run()
{
  try
  {
    init();
    while (programCounter < assembly.size())
    {
      AssemblyEntry instance = assembly[programCounter];
      switch (instance.type)
      {
        case JUMP_LABEL:
          break;
        case JUMP_OPP:
          if (instance.operatorLabel == "jc")
          {
            programCounter = registers["CMPR"] > 0 ? jump_labels[instance.firstOp] : programCounter;
          }
          else if (instance.operatorLabel == "jmp")
          {
            if (instance.firstOp == "MAIN")
            {
              isGettingGlobal = false;
            }
            programCounter = instance.firstOp == "RET_A" ? return_addresses.top() : jump_labels[instance.firstOp];
          }
          break;
        case ALU_OPP:
          readALUop(instance);
          break;
        default:
          throw ERROR_INVALID_OP_CODE;
      }
      programCounter++;
    }
  }
  catch(int e)
  {
    ErrorReader::readError(e, "");
  }
}

void Processor::readALUop(AssemblyEntry assemblyLine)
{
  int left = 0;
  int right = 0;
  if (assemblyLine.operatorLabel == "load")
  {
    if (isGettingGlobal) { global_vars.insert({assemblyLine.firstOp, stoi(assemblyLine.secondOp)}); return;}
    if (this->isRegister(assemblyLine.firstOp))
    {
      registers[assemblyLine.firstOp] = this->getValue(assemblyLine.secondOp);
    }
    else if (this->isDataMem(assemblyLine.firstOp))
    {
      data_mem[assemblyLine.firstOp] = this->getValue(assemblyLine.secondOp);
    }
    else if (this->isGlobal(assemblyLine.firstOp))
    {
      global_vars[assemblyLine.firstOp] = this->getValue(assemblyLine.secondOp);
    }
    else if (data_mem.find(assemblyLine.firstOp) == data_mem.end()) 
    {
      data_mem.insert({assemblyLine.firstOp, this->getValue(assemblyLine.secondOp)});
    }
    else
    {
      throw ERROR_INVALID_LOAD;
    }
  }
  else if (assemblyLine.operatorLabel == "push")
  {
    if (assemblyLine.firstOp == "RET_A")
    {
      return_addresses.push(stoi(assemblyLine.secondOp));
    }
    else if (assemblyLine.firstOp == "PARAM")
    {
      params.push(this->getValue(assemblyLine.secondOp));
    }
  }
  else if (assemblyLine.operatorLabel == "pop")
  {
    return_addresses.pop();
  }
  else if (assemblyLine.operatorLabel == "and")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    this->setValue(assemblyLine.firstOp, left && right);
    registers["CMPR"] = left && right;
  }
  else if (assemblyLine.operatorLabel == "or")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    this->setValue(assemblyLine.firstOp, left || right);
    registers["CMPR"] = left || right;
  }
  else if (assemblyLine.operatorLabel == "b_and")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    this->setValue(assemblyLine.firstOp, left & right);
  }
  else if (assemblyLine.operatorLabel == "b_or")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    this->setValue(assemblyLine.firstOp, left | right);
  }
  else if (assemblyLine.operatorLabel == "xor")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    this->setValue(assemblyLine.firstOp, left ^ right);
  }
  else if (assemblyLine.operatorLabel == "sh_left")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    this->setValue(assemblyLine.firstOp, left << right);
  }
  else if (assemblyLine.operatorLabel == "sh_right")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    this->setValue(assemblyLine.firstOp, left >> right);
  }
  else if (assemblyLine.operatorLabel == "add")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    this->setValue(assemblyLine.firstOp, left + right);
  }
  else if (assemblyLine.operatorLabel == "sub")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    this->setValue(assemblyLine.firstOp, left - right);
  }
  else if (assemblyLine.operatorLabel == "mod")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    this->setValue(assemblyLine.firstOp, left % right);
  }
  else if (assemblyLine.operatorLabel == "div")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    this->setValue(assemblyLine.firstOp, left / right);
  }
  else if (assemblyLine.operatorLabel == "multi")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    this->setValue(assemblyLine.firstOp, left * right);
  }
  else if (assemblyLine.operatorLabel == "not")
  {
    left = getValue(assemblyLine.firstOp);
    this->setValue(assemblyLine.firstOp, !left);
  }
  else if (assemblyLine.operatorLabel == "print")
  {
    left = this->getValue(assemblyLine.firstOp);
    right = this->getValue(assemblyLine.secondOp);
    char tmp = (char) left;
    switch (right)
    {
      case 0:
        cout << left << endl;
        break;
      case 1:
        cout << left;
        break;
      case 2:
        cout << tmp << endl;
        break;
      case 3:
        cout << tmp;
        break;
      default:
        throw ERROR_INVALID_OP_CODE;
        break;
    }
  }
  else if (assemblyLine.operatorLabel == "eq")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    registers["CMPR"] = left == right;
  }
  else if (assemblyLine.operatorLabel == "neq")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    registers["CMPR"] = left != right;
  }
  else if (assemblyLine.operatorLabel == "lt")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    registers["CMPR"] = left < right;
  }
  else if (assemblyLine.operatorLabel == "gt")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    registers["CMPR"] = left > right;
  }
  else if (assemblyLine.operatorLabel == "lte")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    registers["CMPR"] = left <= right;
  }
  else if (assemblyLine.operatorLabel == "gte")
  {
    left = getValue(assemblyLine.firstOp);
    right = getValue(assemblyLine.secondOp);
    registers["CMPR"] = left >= right;
  }
}

bool Processor::isNumber(string target)
{
  bool isNumber = true;
  for(string::const_iterator k = target.begin(); k != target.end(); ++k)
      isNumber = isdigit(*k) && isNumber;
  return isNumber;
}

bool Processor::isRegister(string target)
{
  for (auto i=registers.begin(); i != registers.end(); i++)
  {
    if (i->first == target)
    {
      return true;
    }
  }
  return false;
}

bool Processor::isDataMem(string target)
{
  string ans;
  for (auto i=data_mem.begin(); i != data_mem.end(); i++)
  {
    if (i->first == target)
    {
      return true;
    }
  }
  return false;
}

bool Processor::isGlobal(string target)
{
  string ans;
  for (auto i=global_vars.begin(); i != global_vars.end(); i++)
  {
    if (i->first == target)
    {
      return true;
    }
  }
  return false;
}

int Processor::getValue(string target)
{
  int ans = INT_MIN;
  if (this->isDataMem(target))
  {
    ans = data_mem[target];
  }
  else if (this->isRegister(target))
  {
    ans = registers[target];
  }
  else if (this->isNumber(target))
  {
    ans = stoi(target);
  }
  else if (this->isGlobal(target))
  {
    ans = global_vars[target];
  }
  else if (target == "PARAM")
  {
    ans = params.front();
    params.pop();
  }
  else
  {
    cout << target << " isn\'t working. " << endl;
    throw ERROR_INVALID_ARITH_OP;
  }
  return ans;
}

void Processor::setValue(string target, int value)
{
  if (this->isDataMem(target))
  {
    data_mem[target] = value;
  }
  else if (this->isRegister(target))
  {
    registers[target] = value;
  }
  else if (this->isGlobal(target))
  {
    global_vars[target] = value;
  }
  else
  {
    cout << target << " isn\'t working. " << endl;
    throw ERROR_INVALID_ARITH_OP;
  }
}

