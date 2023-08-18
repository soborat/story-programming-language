# Story programming language
[story-language.vercel.app](https://story-language.vercel.app)

I started this project as a joke, but it turned out to be more than this.  
The language has a interpreter written in C++, and also a C++ transpiler.  

The syntax is weird on purpose, but also very easy to read for non programmers, easier than Python.  
The language features variables, input/output, arithmetics, if statements, for loops and functions.  

I also created a Web Assembly frontend for the language, and a small documentation.


## Rules

1. Identifiers must be capitalized
2. Numbers can be written only in letters, 57❌ fifty-seven✔️
3. Can't use numbers outside the range [zero, ninety-nine]
4. Indentation must be a multiple of 4

## Variable declaration

`let X` (defaults as 0)

`let Y be ten`

`let Z be Y`

## Output

`say X`

`say two`

`say space` (evaluates as '&nbsp;')

`say line` (evaluates as '\n')

## Input

`read X`

## Arithmetic

`add one to X`

`subtract one from X`

`multiply X by two`

`divide X by two`

## If statement

`if X is odd`

`if X is even`

`if X equals nine`

`if X not-equals nine`

`if X is less-than nine`

`if X is greater-than nine`

`if X is less-equal nine`

`if X is greater-equal nine`

`if X is divisible-by nine`

## Else statement
```
if X is divisible-by nine
    say X
else
    say zero
```

## For loop
```
for every N from five to Z
    for every M from Z to N
```
## Comment

`code//`

## Function declaration

```
function F
    say one
```

## Function call

`call F`

## Recursive Fibonacci sequence example
```
let A be zero

let B be one
let N be one

function F
    let C be A
    add B to C
    let A be B
    let B be C

    say C
    say space

    add one to N
    
    if N less-equal ten
        call F

call F
```

Output:

```
1 2 3 5 8 13 21 34 55 89
```

## Requirements
1. C++ 11 compiler
2. [Emscripten](https://emscripten.org) for Web Assembly

## How to run
1. In the root directory `emcc main.cpp --bind -o public/main.js -s NO_DISABLE_EXCEPTION_CATCHING` 
2. Serve `public/index.html`

[![Story website screenshot](https://i.imgur.com/URLas7U.png)](https://story-language.vercel.app)