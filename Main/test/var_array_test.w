// TODO: global constant arrays are broken and needs to be fixed
//const the_constant[] = [123];
let _global_var[3];

function MAIN()
{
    let alllower[] = [234, 321];
    PRINT(alllower, parrn);

    let camelCase[] = [1,-2,3,-4,5];
    PRINT(camelCase, parrn);

    const numLocalConst123[] = ['a', 'A', 123, 321];
    PRINT(numLocalConst123, parrn);

    // BROKEN DUE TO GLOBAL CONSTANTS
    //let assignConst[] = the_constant;
    //PRINT(assignConst, pintn);

    _global_var[0] = 123;
    PRINT(_global_var, parrn);
    _global_var[2] = 111;
    PRINT(_global_var, parrn);
}