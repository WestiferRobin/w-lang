# W Language

## What is W Lang and why was it created

  Back when I was in college I took a compiler course. The course was writing a compiler for C which generated sudo assembly using some tools like GNU Bision. I was inspired by this course and wanted to create my own programing language with a compiler without the use of a framework. I should of done this project in C since the majority of compilers are written in C generally but I wanted to create something to show my skills in C++ as a portfolio project. If this language has a purpose besides writing it for fun it would be to help teach basic concepts of programming. I don't expect this language to get picked up of the sort. 
  
  W Lang is a weak typed procedual language when compiled creates bytelike code (psudo assembly) when then the bytecode gets interpreted. W Lang can be thought up like BASIC in terms on being a simple language. It doesn't have any frameworks or anything of that sort to help build applications using W Lang. Maybe in the future there will be such a need of a framework, but as of now thats not the case. 

## How to build and use W_lang compiler

For Mac and Windows
- Go onto Xcode/VisualStudio and run a build. Then take the executable and run the file from the terminal.

If using the Makefile in Main
- just run the make file and take wlang executable file and run file from the terminal.

## Demos

If you look at the demos folder, copy and paste those files where your executable is located. Once there you can do the following.

./wlang algro.w

or with display

./wlang algro.w -d

## Challenges
1. Creating what the systax and type its going to be. Weak types were a better choice on parsing and then able to distinguish.  

## Known Bugs
1. Raw strings can't be passed as parameters. Work around is to assign a variable and then pass that variable in.
2. In switch statements, you cannot have multiple cases stack on each other. You have to use case and then follow by a break. 

## Future Features
1. Fix know bugs. I just wanted a break from the project before addressing those bugs again.
2. Make the language object oriented.
3. Include floating point numbers.
3. Create Frameworks for Web, Mobile, Applications, and Tools. This will need more system apis like reading files and so forth.

# How To Code in W Language

## Variables

## Arrays and Strings

## Functions

## Files

## System Functions
