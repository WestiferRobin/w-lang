let holder[] = [];

function GET_RAND_ARRAY()
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

function TEST_SORT()
{
    let array[] = GET_RAND_ARRAY();
    //let array[] = [5,4,3,2,1];
    PRINT(array, parrn);

    let size;
    LENGTH(size, array);
    size -= 1;
    
    //holder[] = array;

    AUX_QUICK_SORT(array, 0, size);

    //array[] = holder;

    PRINT(array, parrn);
}

function AUX_QUICK_SORT(let wholeArray[], let low, let high)
{
    if (low < high)
    {
        let partition = PARTITION(wholeArray, low, high);
        
        AUX_QUICK_SORT(wholeArray, low, partition - 1);
        AUX_QUICK_SORT(wholeArray, partition + 1, high);
    }
}

function PARTITION(let targetArray[], let l, let h)
{
    let pivot = targetArray[h];

    let i = l - 1;

    for (let j = l; j <= h; j += 1)
    {
        if (targetArray[j] < pivot)
        {
            i += 1;
            let temp = targetArray[i];
            targetArray[i] = targetArray[j];
            targetArray[j] = temp;  
        }
    }

    let temp = targetArray[i + 1];
    targetArray[i + 1] = targetArray[h];
    targetArray[h] = temp; 

    return (i + 1);
}

function MAIN()
{
    PRINT("HELLO THERE ASSHOLE!", pstrn);
    PRINT("I LOVE YOU BRI GOD FUCKN DAMN IT!!!!!!", pstrn);
    // TODOS!!!!
    //  1. Fix the pass array reference.
    TEST_SORT();
}