import next_step;


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


function MAIN()
{
    const d = 234;
    const f[] = "Wesley Webb";
    let asdf[] = "asdfasdf";
    let fdsa = 432;
    asdf[] = "fdsa";
    PRINT(asdf, pstrn);
    let a = 'W' - 2;
    COPY(message, asdf);
    PRINT(message, pstrn);
    P_STATE(5);
    PRINT(a, pcharn);
}