const the_constant = 'C';
let _global_var = 'a';

function MAIN()
{
    let alllower = 'd';
    PRINT(alllower, pcharn);

    let camelCase = 'B';
    PRINT(camelCase, pcharn);

    const numLocalConst123 = 'Z';
    PRINT(numLocalConst123, pcharn);

    let assignConst;
    assignConst = the_constant;
    PRINT(assignConst, pcharn);

    PRINT(_global_var, pcharn);
    _global_var = 'A';
    PRINT(_global_var, pcharn);
}