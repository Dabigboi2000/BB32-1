This is the first early pre-release version of the BB32-1 CPU emulator.



This version was written in Code::Blocks
Expect bugs and a lot of spaghetti code!!!

NOTE: when selecting the path to file in the shell, spaces are not supported, and it is recommended to store your programs in a folder on the root of your drive. if you wish to do so, the included BB32-1 folder should be moved to the root of your drive as it contains the test program.

For testing:
when testing a program there is currently no supported output, so if you wish to see a variable or output from the emulated CPU, the first register block begins at index 0 of the array 'registers', and goes in the order of:
0-A1
1-B1
2-C1
3-A2
4-B2
5-C2
...

A test program containing the Collatz sequence is included in this folder. where the sequence of numbers is stored in register A1. in the Main function, there is a commenting line at index 331:
//cout << registers[0] << '\n';
remove the // at the beginning and rebuild the program to see the collatz sequence in action.