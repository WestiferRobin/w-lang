const six_constant = 6;

// (==, !=, <, >, <=, >=)
function MAIN()
{
    let one_hundred = 100;
    let answers[6];

    answers[0] = 7 == six_constant;
    answers[1] = 7 != one_hundred;
    answers[2] = 7 < one_hundred;
    answers[3] = six_constant > one_hundred;
    answers[4] = six_constant <= 1234;
    answers[5] = six_constant >= six_constant;

    PRINT(answers, parrn);
}