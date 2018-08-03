							Linux Shell Simulator
							---------------------

File Name - linux_shell.c

File Size - 3.3kB

Total Lines - 351

------------------------------------------------------------------------
Linking readline while compiling
------------------------------------------------------------------------

Sample Compilation - gcc linux_shell.c -lreadline

Sample Execution   - ./a.out

Since the program uses functons from readline library the same has be be linked while the compilation of 
the program.
		
------------------------------------------------------------------------
Expected Output
------------------------------------------------------------------------

rockitraj:/home/rockitraj/Desktop/Assignment/Q_2$ abc

		abc - Is the sample command provided
		
------------------------------------------------------------------------
Additional Dependencies
------------------------------------------------------------------------

The following command has to be executed before compiling the code.

	sudo apt-get install libreadline6 libreadline6-dev

The package libreadline is for running applications using readline commandand the package libreadline-dev 
is for compiling and building readline application.

------------------------------------------------------------------------
Program Flow
------------------------------------------------------------------------

-------------------------------		---------------------	 NO	    --------------------------------
- Taking input user command(1)---->----Detecting pipes------>--------->Checking for known commands -
-------------------------------		---------------------	  		---------------------------------
											|														|
										YES	|														|
											|														|
-----------------------------------------------------       -----------------------------			|
- Calling Fork and Performing two commands seperately       If found execute else error -<----------|
-----------------------------------------------------       -----------------------------
												|					|
												|					|
										   ----------------------------------
										   -Repeat, take input from user (1)-
										   ----------------------------------
										   
program_flow.png has been attatched.

We take input from the user using readline and store the same in buffer. We the check for pipes.
If it is not having pipes we use string matching to match with the commands available. If a match is found
the command is executed and we ask for next command else if no match is found we throw error message or 
we ask if the user wants a system call.
For piped execution we initialize fork. For the first comand we call fork and wait for its execution 
and after its execution gets over we call next fork with the next command. This works exactly as 
the pipe being implemented in Ubuntu.

------------------------------------------------------------------------
Supported Commands
------------------------------------------------------------------------

Help , ls , mkdir , cat , top , cd , exit , clear

------------------------------------------------------------------------
Credits
------------------------------------------------------------------------
https://askubuntu.com/questions/194523/how-do-i-install-gnu-readline

