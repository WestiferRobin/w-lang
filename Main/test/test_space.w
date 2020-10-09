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
    P_STATE(5);
    PRINT(f[0], pcharn);

    switch (d)
    {
        case 234:
            PRINT(f, pstrn);
            break;
        default:
            PRINT(666, pintn);
            break;
    }
}