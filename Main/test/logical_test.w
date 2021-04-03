const six_constant = 6;

// (&&, ||, !)
function MAIN()
{
    let one_hundred = 100;
    let answers[3];

    answers[0] = 7 == six_constant && six_constant < one_hundred;
    answers[1] = 7 != one_hundred || six_constant <= 1234;
    answers[2] = !(7 > one_hundred);

    PRINT(answers, parrn);
}