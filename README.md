# CS 3503 Project 1 - BitBoard Checkers

## Author
Alexander Duty

## Description
In this project I design a checkers game using clever bitwise manipulation, packed with proper mechanics that makes up a traditional checkers game.

## Build Instructions
Open the repository folder in terminal to get the path, then input the following:

```
gcc -o game main.c bitmanip.c
./game
```
A copy of the repository can be downloaded from GitHub.
 
## Project Results
This was definitely a difficult task for me to figure out, but I think the end result is quite nice. The game includes game logic such as move legality checks, piece movement, piece capturing, king promotion/movement, 2-player versus, and win condition. 

An additional testing function is included before the game starts, and this function is made to demonstrate the bit manipulation functions that were used to make the checkers game work. Some functions aren't actually used in the inner workings of the checkers game, but they are in the testing function regardless.

Debugging is still annoying when it comes to C programming, and that contributed to the amount of time taken for me to finalize the product. Probably my favorite debugging moment was making the isLegalMove() function return different values depending on which legality check failed. This grealty sped up the debugging process on the isLegalMove() function and some others since I could pinpoint where exactly the code is being executed.

Even if this submission is very late (and I apologize for that), I'm quite satisified with the result and had lots of fun with using bitwise manipulation
