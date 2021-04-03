# W Language

## DEV TODOS
- Write test cases for test_runner
    - Use the manual to help shape the tests and make them atomic as possible
- Create the needed demos
    - All basic Algorithms
    - Basic Calculator
    - Text RPG Demo
    - Rock Paper Scissors
    - Keyboard Encryptor
- Try it out/configure on mac and windows projects
- Update the README.md for release
    - finish the import shit....

## What is W Lang and why was it created

Back when I was in college I took a compiler course. The course was writing a compiler for C which generated sudo assembly using some tools like GNU Bision. I was inspired by this course and wanted to create my own programing language with a compiler without the use of a framework. I should of done this project in C since the majority of compilers are written in C generally but I wanted to create something to show my skills in C++ as a portfolio project. If this language has a purpose besides writing it for fun it would be to help teach basic concepts of programming. I don't expect this language to get picked up of the sort. 
  
W Lang is a weak typed procedual language when compiled creates bytelike code (psudo assembly) when then the bytecode gets interpreted. W Lang can be thought up like BASIC in terms on being a simple language. This language only conatins intetegers and characters as primitive data types with the support of arrays and strings. I didn't include floating point numbers or boolean primitive values in this iteration because in order to get the development process flowing in order to develop the Abstract Syntax tree design, LL(1) vs LR(1) parser selection, and bytecode processor. Since the foundation of the language is laid out, I plan to revist this project later to include floats, doubles, and booleans as primitaive types. Just like the reason for the primitive types choosing W lang to be a procedual language seem to make sense, and I will also plan to create this language to be Object Oriented as well. It doesn't have any frameworks or anything of that sort to help build applications using W Lang. Maybe in the future there will be such a need of a framework, but as of now thats not the case. 

## How to build and use W_lang compiler

For Mac and Windows
- Go onto Xcode/VisualStudio and run a build. Then take the executable and run the file from the terminal.

If using the Makefile in Main
- just run the make file and take wlang executable file and run file from the terminal.

If you look at the demos folder, copy and paste those files where your executable is located. Once there you can do the following.

./wlang algro.w

or with display

./wlang algro.w -d

## Challenges
- Creating what the systax and type its going to be. Weak types were a better choice on parsing and then able to distinguish.  
- Deciding what type of Parser to use, and which one was easy to implement without a framework.
- Drawing up the Grammar and test cases for the compiler to work.
- Strings literally was a pain through the whole process.
- Writing an accurate error reader. I do plan to JUST focus on the error reader once I get more features out.

## Bugs?
As of now I'm very content what I have released, although I am 99% positive that it is not perfect. The error log is really not helpful at all. Best thing to do is to debug throught the compiler code and your W Lang code and if there are any bugs you find just reach out to me.

## Future Features
- Write and execute unittests and an automation test suite
- Add switch statements.
- Include floating point numbers, bytes, and bool.
- Add try catch functionality.
- Add do while loops and foreach loops
- Replace LL(1) parser to LR(1).
- More system functions like reading files, threads, math class, limits, and etc.
- Make the language object oriented with access modifiers.
- A more detailed error catcher.
- Create Frameworks for Web, Mobile, Applications, and Tools. This will need more system apis like reading files and so forth.

# How To Code in W Language

Wanna try W lang? Here is how you code it

## Variables

Variable naming convention for W Lang is very similar to languages like C#, Java, and C++. 
```javascript
// correct
let alllower = 234;
let cammelCase = 432;
let _canUseUnderScores = 333;
let or_be_like_python = 222;
let useNumbers123 = 123;

// WRONG
let NOCAPS = 'N';
let 123NumbersFirst = -234;
let $pecialCharacters = 'n';
```

There are only two types of variables. Integers and characters.
```javascript
let integer = 234;
let character = 'W';
```

Don't want your variables to change? Thats fine you can make constants.
```javascript
const theW = 'W'
```

## Operators

W Lang supports Arithmetic (+, -, *, /, %), Relational (==, !=, <, >, <=, >=), Logical (&&, ||, !) and Bitwise operators (&, |, ^, <<, >>). The W Lang also supports a sepcial operator called 'delete' which removes the variable from scope. Below are a few examples.

Arithmetic
```javascript
let add = 123 + 321;
let thisExample = 2;
thisExample *= 3;
let mod = add % thisExample;
```

 Relational
```javascript
let one = 1;
let two = 2;
let answer = (one == 2);
```

Logical 
```javascript
let number = 5;
let negatedNumber = !number && number == 5;
```

Bitwise
```javascript
let andExample = 4 & 8;
let orExample = 0;
orExample |= 1;
let xorExample = andExample ^ orExample;
xorExample <<= 1;
```

delete
```javascript
let number = 3;
delete number;
let number = 3;
```

## Arrays and Strings

Arrays are like lists in python and strings are like arrays but just chars. You can initalize an array with chars and ints, but if you print them out as an array it will display as ints. Here is some correct and wrong examples on what arrrays and strings look like in W Lang.

```javascript
// correct ways
let array1[] = [1,2,3];
let array2[7];
let array3[] = [];
let string[] = "Hello there";
let mixExample[3] = [1, 'W', 3];

// wrong ways
let array3[];
let array4[3] = [2,3,4,5,6];
let array4 = [3,2,1];
let array5 = "Wrong";
```

## Conditionals

There are only if, elif, and else statements. If statements means that if the condition is met then do the operation. Elif statements means if the previous condition wasn't met and if this statement is met then do the operation. Else statements means whatever previous conditions fail then do the else statement. 

```javascript
let flag = 1;

if (flag == 2)
{
    PRINT("WOW FIRST TRY!!!", pstrn);
}
elif (flag == 3 || flag == 0)
{
    PRINT("ITS A NUMBER!!!", pstrn);
}
else
{
    PRINT("ERROR", pstrn);
}
```

## Loops

As of now there are only two types of loops which are for loops and while loops. For loops enables a particular set of conditions to be executed repeatedly until a condition is satisfied while the while loop is more loose and based on a conditional statement. Below are some examples of these loops.

```javascript
// for loop
for (let index = 0; index < 5; index += 1)
{
    PRINT(index, pintn);
}

// while loop
let index = 0;
while (index < 5)
{
    PRINT(index, pintn);
    index += 1;
}
```

## Functions

A function is a block of organized, reusable code that is used to perform a single, related action. In the W Language you can pass parameters, return values, and just do whatever you want to accomplish in the scope. For naming convention it has to be capitalized letter that can include underscores and not nammed after system aux functions (more on that later). Below are some examples.


```javascript
function P_MESSAGE(let message[], let mode)
{
    PRINT(message, mode);

    return 0;
}


function MAIN()
{
    let result = P_MESSAGE();
    PRINT(result, pintn);
}
```

## System Aux Functions

The W Language has some aux functions for the developer to use. RANDOM just generates a random number and its up to you as the developer to choose which interval number you want. APPEND and REMOVE just adds or remove each element from a string or array. LENGTH just get the length of array or string. EQUAL just gets the result if two arrays or strings are the same. PRINT just prints the result on screen with different limits. SCAN just gets strings or variables of information from the user.

```javascript
let ans = 0;
RANDOM(ans, 0, 10);

let array[] = [1,2,3,4];
APPEND(5, array);
REMOVE(1, array);

LENGTH(ans, array);
EQUAL(ans, array, [2,3,4,5]);
PRINT("MESSAGE TIME", pstrn);
SCAN(ans);
```

## Import Files

