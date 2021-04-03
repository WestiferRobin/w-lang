
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

function BUBBLE_SORT()
{
    let array[] = GET_RAND_ARRAY();
    PRINT("HERE IS BUBBLE_SORT", pstrn);
    PRINT(array, parrn);

    let size;
    LENGTH(size, array);
    
    // FIX THIS STUPID FUCKN NEGATIVE VS MINUS NUMBER!!!!!!!!!
    for (let i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (array[j] > array[j+1])
            {
                int temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
            }
        }
    }


    PRINT("BUBBLE_SORT IS SORTED", pstrn);
    PRINT(array, parrn);
    PRINT(' ', pcharn);
}

function INSERTION_SORT()
{
    let array[] = GET_RAND_ARRAY();
    PRINT("HERE IS INSERTION_SORT", pstrn);
    PRINT(array, parrn);

    
    PRINT("INSERTION_SORT IS SORTED", pstrn);
    PRINT(array, parrn);
    PRINT(' ', pcharn);
}

function MERGE_SORT()
{
    let array[] = GET_RAND_ARRAY();
    PRINT("HERE IS MERGE_SORT", pstrn);
    PRINT(array, parrn);


    PRINT("MERGE_SORT IS SORTED", pstrn);
    PRINT(array, parrn);
    PRINT(' ', pcharn);
}

function QUICK_SORT()
{
    let array[] = GET_RAND_ARRAY();
    PRINT("HERE IS QUICK_SORT", pstrn);
    PRINT(array, parrn);


    PRINT("QUICK_SORT IS SORTED", pstrn);
    PRINT(array, parrn);
    PRINT(' ', pcharn);
}

function MAIN()
{
    PRINT("WELCOME! Here is a demo of some basic algorithms", pstrn);
    PRINT(' ', pcharn);

    BUBBLE_SORT();
    INSERTION_SORT();
    MERGE_SORT();
    QUICK_SORT();
}