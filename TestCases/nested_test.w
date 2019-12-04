function +main()
{
    for (let i = 0; i < 5; i++)
    {
        for (let j = 0; i < 5; i++)
        {
            print(0, 0);
        }
    }
    let i = 0;
    let j = 0;
    while (i < 5)
    {
        while (j < 5)
        {
            print(1, 0);
            j += 1;
        }
        i += 1;
    }
    let k = 0;
    while (k < 5)
    {
        for (let l = 0; l < 5; l++)
        {
            print(2, 0);
        }
        k++;
    }
    for (let l = 0; l < 5; l++)
    {
        let z = 0;
        while (z < 5)
        {
            print(3, 0);
            z++;
        }
    }
}