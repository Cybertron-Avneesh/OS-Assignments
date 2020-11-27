Question Statement:

1) 	Create your own SHELL-commands to simulate (mimic) the functionality of the well-known
	SHELL command:- mycut [-d] "delimiter" [-f] field-number filename
	
NOTE-1: You have to implement your commands using only C/C++ programs. You cannot
use any SHELL script to implement the commands.

NOTE-2: In order to do file-I/O operations (excluding I/O with standard input-output
devices), you MUST use System Calls, NOT C library functions.	

2)	Write SHELL scripts inside which you should execute, along with other SHELL commands,
	your own command, which you have already created in the previous question i.e:

		Write a SHELL Script which takes a source pathname/filename and a destination
		pathname from the user, checks whether the source file and the destination pathname
		(directory) exists. If not, prompts to the user to correctly provide the inputs. When it
		receives correct inputs, asks the user to input values for a delimiter and a field-number. It
		then executes your mycut command with the -d and -f options with the specified values
		of delimiter and field-number on the input file. The output of the execution is not
		displayed on the terminal; rather, it is redirected to a newly created file called output
		which is placed in the destination directory as mentioned by the user.