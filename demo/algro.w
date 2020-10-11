
function ADD(let x, let y)
{
    return x + y;
}

function MAIN()
{
    for (let i = 0; i < 5; i += 1)
    {
        let character = '+';
        let ans = 0;

        switch (character)
        {
            case '+':
                ans = ADD(2, 2);
                break;
            default:
                ans = -1;
                break;
        }

        PRINT(ans, pintn);
    }
}