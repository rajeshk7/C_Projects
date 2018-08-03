			MMU and OS Simulator
			---------------------

File Name - MainMemUnit.c , OperatingSystem.c

File Size - 6.7Kb , 9Kb

Total Lines - 250 , 305 
------------------------------------------------------------------------
Compiling and Running
------------------------------------------------------------------------

Makefile has been provided with the programs. Both the programs can be compiled 
using the makefile. And OperatingSystem.c can be run using the makefile. We have
to run the MainMemUnit.c explixitely.

Compilation - make compile

Run (OS)    - make

Run (MMU)   - ./MMU

------------------------------------------------------------------------
Additional Files
------------------------------------------------------------------------

For smooth execution, the program requires two files. One the header file that 
has to be included in both the files. And one trace file for taking the memory 
traces as input. The two files are :
                                        SharedMCred.h
                                        memtrace.txt
		
------------------------------------------------------------------------
Expected Output
------------------------------------------------------------------------

Operating System outputs the total requests entertained and total number of disk
accesses. Main Memory Unit outputs the total number of requests encountered and
total hits as well as total hits.

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
Comparison Report
------------------------------------------------------------------------

* FIFO V/S Not MOST RECENTLY USED

        NMRU chooses among the pages which are not recent and tries to avoid 
        pages with disk accesses. But FIFO doesn't look at all those apects. It 
        just replaces the pages in first in first out manner.
        1. Disk accesses in NMRU were very few with respect to FIFO.
        2. FIFO always gave more page fault than NMRU.
        3. NMRU, because of the randomness involved, out performs FIFO in almost
           every aspect.
        4. Sometimes page faults increase in case of FIFO when we increase 
           frames, but that never happens with NMRU.

* FIFO V/S Next MOST RECENTLY USED

        NMRU chooses the page which is not the most recent. FIFO just replaces the 
        pages in first in first out manner. Both of them do not take into account
        whether the page is dirty or not and usually both result in high disk
        accesses.
        1. Disk accesses in NMRU and FIFO were relatively same.
        2. FIFO usually had more page fault than NMRU.
        3. Sometimes FIFO outperformed NMRU with respect to page faults.
        4. Sometimes page faults increase in case of FIFO when we increase frames, but
        that never happens with NMRU.

							   
PS     : Graphs have been attatched of all the three algorithms.
Namely : For FIFO        :- FIFO.png
         For Not MRU     :- NotMRU.png
         For Next MRU    :- NextMRU.png

------------------------------------------------------------------------
Credits
------------------------------------------------------------------------
https://www.gnu.org/software/hurd/microkernel/mach/ipc.html
https://www.gnu.org/software/hurd/ipc.html
https://www.gnu.org/software/hurd/gnumach-doc/Inter-Process-Communication.html
https://www.gnu.org/software/shmm/

