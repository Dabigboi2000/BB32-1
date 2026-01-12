# BB32-1
Welcome to the open-source repository for the homebrew BB32-1 CPU. here you'll find all the documentation, bugs and software related to this project.

The BB32-1 CPU is a 32-bit processor with a custom architecture. It includes 32 dedicated I/O ports (16 input, 16 output) and naitively supports up to 32 MB of RAM (24 bits per address) and 48 MB of ROM (32 bits per address).

# Registers

The CPU has 12 inbuilt 24-bit registers, split into 4 groups of 3. Each register in a group is either A, B or C, and the way that each register works does not change when you switch banks to another set of registers, and it only changes the physical chip the CPU is reading.

# Assembly
Here is the basic custom assembly instruction set:

(the A and B values in the following table are not to be confused with the registers A and B, they are only placeholders in this case)
| Function | Operands | Type | Description | Extra Conditions |
|---------|----------|------|-------------|------------------|
| SET | A, B | Memory | Sets A to value B, where B is a register or value | N/A |
| ADD | A, B | Arithmetic | Adds value B to A, where B is a register or value; outputs to register C | N/A |
| SUB | A, B | Arithmetic | Subtracts value B from A, where B is a register or value; outputs to register C | N/A |
| MUT | A, B | Arithmetic | Multiplies value B by A, where B is a register or value; outputs to register C | N/A |
| DIV | A, B | Arithmetic | Divides A by value B, where B is a register or value; outputs to register C | N/A |
| WRITE | A, B | Memory | Sets RAM address A to value B, where A and B are a register or value | N/A |
| READ | A, B | Memory | Sets A to the value loaded from RAM address B, where B is a register or value | N/A |
| IF | A, B | Conditional | If condition A is true, jump to address B | C = D, C > D, C < D, ZER, CAR, AND, OR, NOT, XOR |
| IN | A, B | Input/Output | Sets A to the current state of input B, where B is a register or value | N/A |
| OUT | A, B | Input/Output | Sets output A to value B | N/A |
| JMP | A, 0 | Structure | Jumps to address A | N/A |
| REG | A, 0 | Memory | Sets the address bank to A, where A is a register or value | N/A |
| HLT | 0, 0 | Structure | Halts the program | N/A |

This instruction set directly correlates to the binary loaded into ROM.
Using the instruction SET C, 12 as an example:

SET is encoded as 0000

Register C is encoded as 10

The value 12 is replaced with 11

This means it points to the final 24 bits of the instruction, where the literal value of 12 is now stored:

000000000000000000001100

So the final 32-bit ROM address is:

00001011000000000000000000001100
