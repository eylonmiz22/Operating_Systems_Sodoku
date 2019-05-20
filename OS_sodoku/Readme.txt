Des: The program accepts 4 files of sodoku solution board- 
It has to print if a given solution is legal or not by sodoku game rules.
This assignment is done by 4 ways:
1. Working with processes- Create 3 children processes:
	A. By using exec, pipe, dup syscalls and the stdin/out file descriptors of father and children.
	B. By using mmap syscall to share a memory zone.
2. Working with Threads- Creates 27 (9 rows * 9 cols * 9 smallMats) : with and without mutex locks and condition variables.

P.S. 
Needs to be compiled in ubuntu. 
There are given txt files to send to the main.
There are executable files to compile fast in the shell.