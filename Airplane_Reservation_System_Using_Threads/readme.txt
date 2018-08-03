								Airline Reservation System
								--------------------------

File Name - airline.c

File Size - 70.2kB

Total Lines - 2605

------------------------------------------------------------------------
Linking readline while compiling
------------------------------------------------------------------------

Sample Compilation - gcc airline.c -lpthread

Sample Execution   - ./a.out

Since the program uses functons from pthread library the same has be be 
linked while the compilation of the program.

------------------------------------------------------------------------
Warnings
------------------------------------------------------------------------

Since the program follows certain conversion rules, gcc shows some 
warnings regrding compilstion.Those warnings have to be ignored. 
The program is bug free and runs flawleslly.
		
------------------------------------------------------------------------
Expected Output
------------------------------------------------------------------------

Thread Number : 02				Query Type : Book
Thread ID : 140012428601088		Wait time : 0.00000108 seconds		Flight Number : 3
Query Output : Booked ticket Successfully 

Where the respective thread number, query input, thread ID , total wait time of the thread,
the flight on which the query was and the output of the user query will be printed.

Aditionally a log file will also be printed containing all the details of the flights.
That is the booked seats and the available seats.
		
------------------------------------------------------------------------
Additional Dependencies
------------------------------------------------------------------------

The program expects DES.csv file in the same directory where the program resides.
Moreover the queries should be in the format

		<Time Stamp>,<Query Type>,<Flight Number>,<Thread Number>
		
Query Type 	   : Book/Cancel/Inquiry
Time Stamp 	   : Natural Number values (0 -  n)
Flight Nubmber : Natural Number values (0 - 10)
Thread Number  : Natural Number values (0 - 20)

------------------------------------------------------------------------
Program Flow
------------------------------------------------------------------------
screenshot.png has been attatched to demonstrate the program flow.

Initially the main() function initializes the parameters, that is , we have 10 flights, 20 threads, and
private data structure for each thread and 150 seats capacity for each corresponding flight.
Then the flow is given to child which in turn reads the csv file line by line and decodes each query 
and calls the corresponding thread. No data is saved in the program regarding the csv file,
The threads run in a synchronised manner and at a time not more than 5 threads can run. And on a same 
flight we have applied mutex for both read and write operations. Threads execute and get terminated and
either after the csv file ends or after 2 minutes the program halts and generates a log of the number of
tickets available in each flight. Moreover we can look into private data of each thread one by one.

------------------------------------------------------------------------
Supported Commands
------------------------------------------------------------------------

Help , ls , mkdir , cat , top , cd , exit , clear

------------------------------------------------------------------------
Credits
------------------------------------------------------------------------
https://en.wikipedia.org/wiki/C_POSIX_library
https://www.gnu.org/software/libc/manual/html_node/POSIX.html

