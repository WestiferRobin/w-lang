const six_constant = 6;

// (+, -, *, /, %)
function MAIN()
{
    let answers[6];

    let first = 1;
    first += 2;
    first = first + 2;
    answers[0] += (first + six_constant);

    // fix the stupid negative number error.
    let second = 2;
    second -= 1;
    second = second - -1;
    //answers[1] -= (second - -six_constant);
    answers[1] = second;

    let third = 3;
    third *= 1;
    third = third * 11;
    answers[2] = (third + (six_constant * 100));

    let fourth = 4;
    fourth /= 1;
    fourth = fourth / 2;
    answers[3] = ((fourth / 2) + 6);

    let fifth = 5;
    fifth %= 2;
    fifth = fifth + six_constant;
    answers[4] = 20 % fifth;

    // THIS EXPRESSION IS FAILED
    let algerbra = (5 - (123 * 43) + (12 + (222 / 2))) + six_constant;
    answers[5] = algerbra;
    PRINT(answers, parrn);
}