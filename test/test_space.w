

function P_STATE(let times)
{
    let i = 0;
    while (i < times)
    {
        for (let j = i; j < (times); j+=1)
        {
            PRINT(j, pint);
            PRINT(32, pchar);
        }
        PRINT(32, pcharn);
        i += 1;
    }
}

function MAIN()
{
    let a = 'W' - 2;
    P_STATE(5);
    PRINT(a, pcharn);
}