
function doThis(let asdf)
{
    print(asdf, 0);
}

function orThis()
{
    return 666;
}

function orThat(let a, let b)
{
    let temp = a + b;
    print(temp, 0);
}

function orWhatever(let a, let b, let c)
{
    let temp = (a + b) / c;
    print(temp, 0);
}

function +main()
{
    let asdf = 234;
    doThis(asdf);
    asdf = orThis();
    orThat(asdf, 234);
    orWhatever(asdf, asdf, asdf);
}