import math;

function CONFIG()
{
    let firstNumber = 0;
    let secondNumber = 0;
    let character;

    let message[] = "Please select your first number: ";
    PRINT(message, pstr);
    SCAN(firstNumber);
    
    message[] = "Please select your second number: ";
    PRINT(message, pstr);
    SCAN(secondNumber);

    message[] = "Please select desired operator (-, +, *, /, %): ";
    PRINT(message, pstr);

    SCAN(character);

    let ans = IS_VALID(character);
    while (ans < 0)
    {
        message[] = "Please select desired operator (-, +, *, /, %): ";
        PRINT(message, pstr);

        SCAN(character);

        ans = IS_VALID(character);
    }

    if (character == '+')
    {
        ans = ADD(firstNumber, secondNumber);
    }
    elif (character == '-')
    {
        ans = SUBTRACT(firstNumber, secondNumber);
    }
    elif (character == '*')
    {
        ans = MULTI(firstNumber, secondNumber);
    }
    elif (character == '/')
    {
        ans = DIV(firstNumber, secondNumber);
    }
    elif (character == '%')
    {
        ans = MOD(firstNumber, secondNumber);
    }
    else
    {
        ans = -1;
    }

    PRINT(ans, pintn);
}

function IS_VALID(let character)
{
    let validOps[] = ['-', '+', '*', '/', '%'];
    let size;
    LENGTH(size, validOps);

    for (let index = 0; index < size; index += 1)
    {
        if (validOps[index] == character)
        {
            return 0;
        }
    }

    return -1;
}


function MAIN()
{
    let character = 'N';

    while (character != 'E')
    {
        let message[] = "Welcome To the Calculator Demo";
        PRINT(message, pstrn);

        CONFIG();

        message[] = "Do you want to exit? Enter E: ";
        PRINT(message, pstr);

        SCAN(character);
    }
}