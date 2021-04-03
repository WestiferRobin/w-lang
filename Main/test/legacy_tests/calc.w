import math;

function CONFIG()
{
    let firstNumber = 0;
    let secondNumber = 0;
    let character;

    PRINT("Please select your first number: ", pstr);
    SCAN(firstNumber);
    
    PRINT("Please select your second number: ", pstr);
    SCAN(secondNumber);

    PRINT("Please select desired operator (-, +, *, /, %): ", pstr);

    SCAN(character);

    let ans = IS_VALID(character);
    while (ans < 0)
    {
        PRINT("Please select desired operator (-, +, *, /, %): ", pstr);

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
    let size = 0;
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

function RETURN_W()
{
    return ('W');
}

function MAIN()
{
    let character;

    while (character != 'E')
    {
        PRINT("Welcome To the Calculator Demo", pstrn);

        CONFIG();

        PRINT("Do you want to exit? Enter E: ", pstr);

        SCAN(character);
    }
}