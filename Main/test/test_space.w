
let message[2];

function P_STATE(let times)
{
    let i = 0;
    while (i < times)
    {
        for (let j = i; j < (times); j+=1)
        {
            PRINT(j, pint);
            PRINT(' ', pchar);
        }
        PRINT(' ', pcharn);
        i += 1;
    }
}

function P_MESSAGE(let asdf[])
{
    PRINT(asdf, pstrn);
}

function MAIN()
{
    let a = 'W' - 2;
    let asdf[] = "asdfasdf";
    COPY(message, asdf);
    delete asdf;
    P_STATE(5);
    P_MESSAGE(message);
    PRINT('W', pcharn);
}