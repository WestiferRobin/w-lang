
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

    PRINT("BUBBLE_SORT IS SORTED", pstrn);
    PRINT(array, parrn);
}

function INSERTION_SORT()
{
    let array[] = GET_RAND_ARRAY();
    PRINT("HERE IS INSERTION_SORT", pstrn);
    PRINT(array, parrn);

    let size;
    LENGTH(size, array);

    for (let i = 1; i < size; i += 1)
    {
        let key = array[i];
        let j = i - 1;

        while (j >= 0 && array[j] > key)
        {
            array[j + 1] = array[j];
            j -= 1;
        }

        array[j + 1] = key;
    }
    
    PRINT("INSERTION_SORT IS SORTED", pstrn);
    PRINT(array, parrn);
    PRINT(' ', pcharn);
}
/*
function QUICK_SORT()
{
    let array[] = GET_RAND_ARRAY();
    PRINT("HERE IS QUICK_SORT", pstrn);
    PRINT(array, parrn);


    PRINT("QUICK_SORT IS SORTED", pstrn);
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
*/

function MAIN()
{
    let asdf = -1;
    PRINT("WELCOME! Here is a demo of some basic algorithms", pstrn);
    PRINT(' ', pcharn);

    BUBBLE_SORT();
    INSERTION_SORT();
    MERGE_SORT();
    QUICK_SORT();
}