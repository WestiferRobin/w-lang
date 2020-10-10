import math;

function MAIN()
{
    let character = 1;
    let message[] = "Welcome To the Calculator Demo";
    let firstNumber = 0;
    let secondNumber = 0;

    while (character != 0)
    {
        PRINT(message, pstrn);
        
        message[] = "Please select your first number: ";
        PRINT(message, pstr);
        SCAN(firstNumber);
        
        message[] = "Please select your second number: ";
        PRINT(message, pstr);
        SCAN(secondNumber);

        let ans = firstNumber + secondNumber;
        PRINT(ans, pintn);

        message[] = "Do you want to exit? Enter 0: ";
        PRINT(message, pstr);

        character = 0;
        SCAN(character);
        PRINT(character, pintn);

        message[] = "Welcome To the Calculator Demo";
    }
}