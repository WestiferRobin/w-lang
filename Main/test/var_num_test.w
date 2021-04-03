const the_constant = 123;
let _global_var = 1;

function MAIN()
{
    let alllower = 234;
    PRINT(alllower, pintn);

    // THE FUCKING NEG NUMBERS IS BROKEN!!!!!
    let camelCase = -123;
    PRINT(alllower, pintn);

    const numLocalConst123 = 33333;
    PRINT(numLocalConst123, pintn);

    let assignConst;
    assignConst = the_constant;
    PRINT(assignConst, pintn);

    PRINT(_global_var, pintn);
    _global_var = 111;
    PRINT(_global_var, pintn);
}