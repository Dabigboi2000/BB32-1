This is the first early pre-release version of the BB32-1 CPU emulator that is built with an IDE.

This version was written in Code::Blocks and Python IDLE
Expect bugs and a lot of spaggetti code!!!


For testing:
when testing a program there is currently no supported output, so if you wish to see a variable or output from the emulated CPU, the first register block begins at index 0 of the array 'registers', and goes in the order of:
0-A0
1-B0
2-C0
3-A1
4-B1
5-C1
...

A test program containing the Collatz sequence is included in this folder. where the sequence of numbers is stored in register A0.
