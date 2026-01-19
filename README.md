# Introduction

Welcome to the open-source repository for the homebrew BB24-1 CPU. here you'll find all the documentation, bugs and software related to this project. The final goal of this project is to run DOOM.

(disclaimer: this is my first GitHub Repo, so beware, it kinda sucks)

The BB24-1 CPU is a 24-bit processor with a custom architecture. It has been a long-term passion project starting around mid 2025.

# The plan
To run DOOM on a totaly unique and custom CPU, there are a lot of steps to be take, and the first, most obvious step for anything like this, is to create a C interperator that runs precompiled C that has been converted into plain bytes. This ensures that no more assembly needs to be written then is needed, because assembly is insanely difficult to work with, especially one that was created by a teenager. Because the CPU is now able to run precompiled C scripts, it is possible to write a basic operating system kernel in C. Built into this kernel is a second C compiler that can run actual uncompiled C programs, which is the foundation for running other programs that arent built into the kernel. From here there are 3 paths:

Path 1 - Take the DOOM source code and assets and modify it to actually run on the CPU since the OS kernel and file layout differs a lot to a normal computer, because annoyingly enough: games have assets.

Path 2 - Program an emulator that emulates a DOOM capable device, like the Nintendo GBA or IBM 386 CPU.

Path 3 - Completely remake DOOM from scratch.

Path 3 is immediately out the window, as it literaly requres rebuilding it from the ground up, which leaves path 1 and 2. path 1 is the more difficult of the 2, because i would have to sort through the ≈ 60  thousand lines of C and C++ code and modify it accordingly, or write the onboard C interperater in such a way that it handles that for me. This means that option 2 probably requires the least difficult programming, but introduces the fact that this is now running on 6 layers of super sketchy programming, and a lot of lag.

As of Writing this last section (16th January 2026), i have not decided yet on what method i will take as this is an extremely long way away from the amount of progress i have made so far.

#  
Beneath is the basics covering the architecture of the BB24-1 CPU.

# ROM & RAM
The BB24-1 CPU naitively supports up to 48 MiB of RAM (16m x 24) and 64 MiB of ROM (16m x 32), and an 8 way DIP switch located next to the RAM/ROM chip/s tells the CPU how much is availible, supporting a minimum of 32b x 64K. 

For initial testing of the CPU i decided to use 3 128k x 8 IS61C1024 CMOS STATIC RAM in parallel to get a total of 128k x 24, or 384 KiB, which is much less then the maximum 48 Mib, and is very likely to not be enough for DOOM.

(Note: because the RAM is 24 bits wide, it is actually very difficult to find a single chip with that bit width, so it is almost certain that multiple chips will have to be connected in parallel)

# Registers
The CPU has 12 inbuilt 24-bit registers, split into 4 groups of 3. Each register in a group is either A, B or C, when you switch banks to another set of registers no data is changed, and it only changes the physical chip the CPU is reading.

# External Links
Instruction set: https://docs.google.com/spreadsheets/d/1baC-FZuSKrdwAQvM--H7stg7DwzFx2tct_jAeBdvTPM/edit?usp=sharing
