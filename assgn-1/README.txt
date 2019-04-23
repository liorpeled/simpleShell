This program is the first assignment in cmps111 spring 2019

It is a simple shell program that uses system calls to execute various different command.

To use the shell:
1) go the directory that contains the files --> Makefile, myshell.c and lex.l

2) using make
   A) if it is the first time running, simply type make.
   B) if you want to run it again with changes you made to either myshell.c or lex.l types make clean and then make again,
      make clean will remove the object files and make will rebuild it.
   C) Assuming infer is installed on the machine being used to run the program, type make infer to view any potential bugs. 
   D) I ran infer, and as of April 22,2019 there are no bugs.   

3) After using make or combination of make clean and make an exectuable file called    shell    will be present, to use it
   type ./shell   this will execute the program.

4) To exit the program, simply type exit or Exit and the program will terminate.

----------------

BUGS TO BE AWARE OF

1) CANNOT  execute different types of commands in one line, example: sort < test.txt > sortedtest.txt

2) In command chainning used to work, but for some reason, it does not work well with piping.
3) CANNOT change directories

4) There is single pipe function, but no multipipe.

  



