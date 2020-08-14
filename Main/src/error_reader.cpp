#include "general_util.h"

void ErrorReader::readError(int errorCode, string token)
{
    string errorMessage = "ERROR: ";
    switch (errorCode)
    {
        case ERROR_INVALID_BOOL_OP:
            errorMessage += "Expected to see a boolean operator and not \'" + token + "\'.";
            break;
        case ERROR_INVALID_SYMBOL:
            errorMessage += "This symbol \'" + token + "\' does not exist and/or make sense in this current implementation.";
            break;
        case ERROR_INVALID_ARITH_OP:
            errorMessage += "Expected to see an arithmetic operator and not \'" + token + "\'";
            break;
        case ERROR_INVALID_KEYWORD:
            errorMessage += "Expected to see a keyword and not \'" + token + "\'";
            break;
        case ERROR_VAR_UNKNOWN:
            errorMessage += "this variable \'" + token + "\' hasn't been initalize or not in scope.";
            break;
        case ERROR_INVALID_LOAD:
            errorMessage += "Was unable to load to register and/or data memeory";
            break;
        case ERROR_INVALID_OP_CODE:
            errorMessage += "This is an invalid op code to execute.";
            break;
        default:
            errorMessage = "Current error is UNKNOWN. " + to_string(errorCode);
            break;
    }
    cout << errorMessage << endl;
}