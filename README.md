# Mini Shell v2
This program has been developed as part of my B.Sc in Software Engineering program at Jerusalem College of Engineering.

## Description
This program is a mini shell that supports a variance of shell commands.
The program using methods to print the username and path, taking input
from the user, and creating a pointers array that contains all of the user's input commands and arguments. If the command is valid, the array will be sent to the exevp command for running it. If the command is invalid, an error message will be displayed. The program also supports CD commands for navigating between folders. Ending the program by entering "done" and enter. At the ending, the program will print: The number of valid commands, the length of the command, and "Bye !"

## The Program
* The user now can use pipe using the sign "|"/
* Ctrl+C support
* background processes support using the sign "&".
* file redirection support
	* ">"  create a new file. Override if the file already exists.
	* ">>" Create a new file.  If the file already exists, add the new data to it.
	* "<"  Using data from file
	* "2>" Create a log file for the command's errors.
