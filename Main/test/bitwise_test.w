
// THIS ISNT WORING WITH GLOBAL VARS
//let asdf= 100;

// (&, |, ^, <<, >>)
function MAIN()
{
    let asdf= 100;
    let answers[5];

    answers[0] = 1 & 3;
    answers[1] = 1 | 2;
    answers[2] = 1 ^ 3;
    answers[3] = 1 << 1;
    answers[4] = 2 >> 1;

    PRINT(answers, parrn);

    PRINT(asdf, pintn);
    delete asdf;
    let asdf = 4321;
    PRINT(asdf, pintn);
}