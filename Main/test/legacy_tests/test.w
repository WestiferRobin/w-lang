
function MAIN()
{
    let ans = 1;
    for (let i = 0; i < 10; i += 1)
    {
        RANDOM(ans, 'A', 'C');
        PRINT(ans, pintn);
    }

    let array[] = [1,2,3,'W'];
    APPEND('A', array);
    PRINT(array, parrn);
    REMOVE('W', array);
    PRINT(array, parrn);
    PRINT(array, pstrn);
}