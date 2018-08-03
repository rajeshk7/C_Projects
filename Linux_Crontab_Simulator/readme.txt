			Linux Crontab Simulator
			-----------------------

File Name - crontab.c

File Size - 11kB

Total Lines - 311

------------------------------------------------------------------------
Linking readline and pthreads while compiling
------------------------------------------------------------------------

Sample Compilation - gcc crontab.c -lreadline -lpthread

Sample Execution   - ./a.out

Since the program uses functons from readline and POSIX library the same has be 
be linked while the compilation of the program. The program can also be executed
 using Makefile provided.

Compilation - make compile

Run         - make
		
------------------------------------------------------------------------
Expected Output
------------------------------------------------------------------------

rockitraj:/home/rockitraj/Desktop/Assignment/Q_2$ crontab abc

		abc - Is the sample command provided
		
------------------------------------------------------------------------
Additional Dependencies
------------------------------------------------------------------------

The following command has to be executed before compiling the code.

	sudo apt-get install libreadline6 libreadline6-dev

The package libreadline is for running applications using readline commandand 
the package libreadline-dev is for compiling and building readline application.
Furthermore, the configuration file must be stored in home/user/mycrontab/ and 
must be renamed to configuration.txt. The program accepts text file with space
 seperated values.

------------------------------------------------------------------------
Program Flow
------------------------------------------------------------------------

The program has been divided into two parts. The first part takes care of the 
task scheduling and the second part takes care of the console for crontab.
The scheduler runs after every minute and checks for tasks which are eligible
in that instance of time and schedules them. We can edit, remove or modify the
scheduled tasks using the terminal.
							   
PS : program_flow.png has been attatched.

------------------------------------------------------------------------
Supported Commands
------------------------------------------------------------------------

crontab -e/-l/-r/-i, help crontab, clear, quit

------------------------------------------------------------------------
Credits
------------------------------------------------------------------------
https://askubuntu.com/questions/194523/how-do-i-install-gnu-readline

