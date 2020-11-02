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
    
    holder[] = array;

    AUX_QUICK_SORT(0, size);

    array[] = holder;

    PRINT(array, parrn);
}

function AUX_QUICK_SORT(let low, let high)
{
    if (low < high)
    {
        let partition = PARTITION(low, high);
        
        // ALL PARAMETERS SHOULD BE EXPRESSIONS
        let asdf = partition - 1;
        let fdsa = partition + 1;
        AUX_QUICK_SORT(low, asdf);
        AUX_QUICK_SORT(fdsa, high);
    }
}

function PARTITION(let l, let h)
{
    let pivot = holder[h];

    let i = l - 1;

    for (let j = l; j <= h; j += 1)
    {
        if (holder[j] < pivot)
        {
            i += 1;
            let temp = holder[i];
            holder[i] = holder[j];
            holder[j] = temp;  
        }
    }

    let temp = holder[i + 1];
    holder[i + 1] = holder[h];
    holder[h] = temp; 

    return (i + 1);
}

function MAIN()
{
    // TODOS!!!!
    //  1. Fix the pass array reference.
    //  2. Fix the expression parameters bullshit.
    TEST_SORT();
}