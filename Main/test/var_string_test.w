// TODO: global constant arrays are broken and needs to be fixed
//const the_constant[] = [123];
let _global_var[3];

function MAIN()
{
    let alllower[] = "Hi";
    PRINT(alllower, pstrn);

    let camelCase[] = "W Language";
    PRINT(camelCase, pstrn);

    const numLocalConst123[] = ['b', 'B', 'x', 'X'];
    PRINT(numLocalConst123, pstrn);

    // BROKEN DUE TO GLOBAL CONSTANTS
    //let assignConst[] = the_constant;
    //PRINT(assignConst, pstrn);

    _global_var[0] = 'a';
    PRINT(_global_var, pstrn);
    _global_var[2] = 'z';
    PRINT(_global_var, pstrn);
}