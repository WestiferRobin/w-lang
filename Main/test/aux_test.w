
function MAIN()
{
    let ans = 0;
    RANDOM(ans, 0, 10);
    PRINT(ans, pintn);

    let array[] = [1,2,3,4];

    APPEND(5, array);
    PRINT(array, parrn);

    REMOVE(1, array);
    PRINT(array, parrn);

    LENGTH(ans, array);
    PRINT(ans, pintn);

    EQUAL(ans, array, [2,3,4,5]);
    PRINT(ans, pintn);
}