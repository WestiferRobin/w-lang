#include "general_util.h"

void ErrorReader::readError(ErrorCode errorCode, string token)
{
    string errorMessage = "ERROR: ";
    switch (errorCode)
    {
        case ErrorInvalidBoolOp:
            errorMessage += "Expected to see a boolean operator and not \'" + token + "\'.";
            break;
        case ErrorInvalidSymbol:
            errorMessage += "This symbol \'" + token + "\' does not exist and/or make sense in this current implementation. please check your code.";
            break;
        case ErrorInvalidArithOp:
            errorMessage += "Expected to see an arithmetic operator and not \'" + token + "\'. please check your code.";
            break;
        case ErrorInvalidKeyword:
            errorMessage += "Expected to see a keyword and not \'" + token + "\'. please check your code.";
            break;
        case ErrorVariableUnknown:
            errorMessage += "this variable \'" + token + "\' hasn't been initalize or not in scope. please check your code.";
            break;
        case ErrorInvalidLoad:
            errorMessage += "Was unable to load to register and/or data memeory. please check your code.";
            break;
        case ErrorInvalidOpCode:
            errorMessage += "This is an invalid op code to execute. please check your code.";
            break;
        case ErrorInvalidConstant:
            errorMessage += "You cannot assign or delete a constant. please check your code.";
            break;
        case ErrorNoRawString:
            errorMessage += "You cannot pass in raw strings into system operators at this time. Please assign a string variable and proceed";
        default:
            errorMessage = "Current error is UNKNOWN. " + to_string(errorCode);
            break;
    }
    cout << errorMessage << endl;
}