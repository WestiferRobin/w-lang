
/*function GET_RAND_ARRAY()
{
    let size;
    RANDOM(size, 10, 20);
    let ans[] = [];
    for (let index = 0; index < size; index += 1)
    {
        let value;
        RANDOM(value, 1, 100);
        APPEND(value, ans);
    }
    return ans;
}

function BUBBLE_SORT()
{
    let array[] = GET_RAND_ARRAY();
    PRINT(array, parrn);

    let size;
    LENGTH(size, array);
    
    for (let i = 0; i < size - 1; i += 1)
    {
        for (let j = 0; j < size - 1; j += 1)
        {
            if (array[j] > array[j + 1])
            {
                let temp = array[j + 1];
                array[j + 1] = array[j];
                array[j] = temp;
            }
        }
    }

    PRINT(array, parrn);
    PRINT(' ', pcharn);
}*/
function MAIN()
{
    // REWORK THE FUCKN GOD DAMN ARRAY
    let array[] = [1,2,3];
    let var = 2;

    PRINT(array, parrn);

    array[0] = 123;
    array[var] = 321;
    array[var - 1] = 111;

    PRINT(array, parrn);
}