#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>
#include<assert.h>
#include<time.h>

#define f  10 // Number of flights
#define c  150 // Capacity of each flight
#define n_min   2 //Minimum number of tickets booked once
#define n_max  5 //Maximum number of tickets booked once=
#define MAX  5 //Maximum number of active queries at any given time
#define t  20 //Number of working threads
#define T_min   1 // Minimum Total running time =  1 minute
#define T_max  5 // Maximum total running time =  5 minutes

pthread_t user[t];			//For t users we have t threads

int global_counter,mutex=0;			//To maintain not more than 5 users
int global_tracker;
int count_watch;
int reading[f],writing[f];		//Binary semaphores

struct flight			//Maintaining Data regarding each flight
{
	int seats_total;
	int seats_available;
} flights[f];

struct elog			//rMaintaining data regarding each Thread
{
	int how;
	int operation[1000];
	int flight[1000];
	int amount[1000];
} each[20];
//Reading the CSV file
int readfile_csv(int count);
//Initializing the parameters
void initialize();
//Calling the function to activate threads
void call_driver();
//Prints the log of booked tickets
void finisher();
//Counts total lines in the csv File
int getcount();
// Chooses the appropriate thread
void actor( char action[], int flight_no, int thread_no);
// Edits the status of each threads private data
void log_changer(int process, int flight, int operation);
// Ask for choosing thread for printing private data
void thread_access(int thread_number);
// Prints the private data of threads
void thread_printer();
//Function for booking for each individual thread
void *changer_book1(void *flight_no );
void *changer_book2(void *flight_no );
void *changer_book3(void *flight_no );
void *changer_book4(void *flight_no );
void *changer_book5(void *flight_no );
void *changer_book6(void *flight_no );
void *changer_book7(void *flight_no );
void *changer_book8(void *flight_no );
void *changer_book9(void *flight_no );
void *changer_book10(void *flight_no );
void *changer_book11(void *flight_no );
void *changer_book12(void *flight_no );
void *changer_book13(void *flight_no );
void *changer_book14(void *flight_no );
void *changer_book15(void *flight_no );
void *changer_book16(void *flight_no );
void *changer_book17(void *flight_no );
void *changer_book18(void *flight_no );
void *changer_book19(void *flight_no );
void *changer_book20(void *flight_no );
//Function for cancelling for each individual thread
void *changer_cancel1(void *flight_no );
void *changer_cancel2(void *flight_no );
void *changer_cancel3(void *flight_no );
void *changer_cancel4(void *flight_no );
void *changer_cancel5(void *flight_no );
void *changer_cancel6(void *flight_no );
void *changer_cancel7(void *flight_no );
void *changer_cancel8(void *flight_no );
void *changer_cancel9(void *flight_no );
void *changer_cancel10(void *flight_no );
void *changer_cancel11(void *flight_no );
void *changer_cancel12(void *flight_no );
void *changer_cancel13(void *flight_no );
void *changer_cancel14(void *flight_no );
void *changer_cancel15(void *flight_no );
void *changer_cancel16(void *flight_no );
void *changer_cancel17(void *flight_no );
void *changer_cancel18(void *flight_no );
void *changer_cancel19(void *flight_no );
void *changer_cancel20(void *flight_no );
//Function for inquiry for each individual thread
void *changer_enquire1(void *flight_no );
void *changer_enquire2(void *flight_no );
void *changer_enquire3(void *flight_no );
void *changer_enquire4(void *flight_no );
void *changer_enquire5(void *flight_no );
void *changer_enquire6(void *flight_no );
void *changer_enquire7(void *flight_no );
void *changer_enquire8(void *flight_no );
void *changer_enquire9(void *flight_no );
void *changer_enquire10(void *flight_no );
void *changer_enquire11(void *flight_no );
void *changer_enquire12(void *flight_no );
void *changer_enquire13(void *flight_no );
void *changer_enquire14(void *flight_no );
void *changer_enquire15(void *flight_no );
void *changer_enquire16(void *flight_no );
void *changer_enquire17(void *flight_no );
void *changer_enquire18(void *flight_no );
void *changer_enquire19(void *flight_no );
void *changer_enquire20(void *flight_no );

int main()
{
	struct timespec begin, end;
	double elapsed;
	clock_gettime(CLOCK_MONOTONIC, &begin);
	initialize();
	puts("\n");
	int pid=fork();
	int i = 0;
	if(!pid)				//Child takes over
	{
			int count = getcount();
			static int i;
			for( i=1; i<(count+1) ; i++)
			{
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = (end.tv_nsec - begin.tv_nsec) / 10000000.0;		//Calculating the elapsed time
				if(elapsed >= T_min)
					{
						sleep(3);
						printf("\a\n\n2 Minutes over, Time for maintainence\n\n");
						goto now;
					}
				if( global_counter > 5)		//Not more than 5 users run at a time
						sleep(2);
							global_counter++;
          		global_tracker++;
							if(global_tracker % 10)
									sleep(2);
							printf("%d\t",i);
          		readfile_csv(i-1);		//Calling the reader function
          }
now:
          sleep(2);
          mutex = 1;			//When it ens we activate parent process
	}
here:
	if(mutex == 0)
	{
		sleep(2);
		goto here;
  	}
  	else
  	{
  			waitpid( pid, NULL, 0 );
  			fflush(stdout);
				puts("\n\033[H\033[2J");
  			finisher();								//Printing log
				thread_printer();					//Printing thread data
				sleep(1);
				printf("\n Terminating server . . . . ");
				exit(EXIT_SUCCESS);
		}
  		return 0;
}

void thread_printer()
{
	int i,k;
	while(1)
	{
		printf("\nDo you want to see private date of any thread \n");
		printf("\n Press (1) for yes or (0) for no . . . ");
		scanf("%d",&k);
		if(k == 0)
				return NULL;
		printf("\nEnter thread number : ");
		scanf("%d",&i);
		if(i > 21)
			{
					printf("\n Please enter between 1 - 20 \n");
					continue;
			}
		sleep(1);
		puts("\n");
		thread_access(i);
	}
}

void thread_access(int thread_number)
{
	int j,i = each[thread_number - 1].how;
	if(!i)
			printf("No operation to print \n");
	for(int j = 0; j<i ; j++ )
	{
			if(each[thread_number - 1].operation[j] == 1)
					printf("Operation Number : %d			Operation Type : Booking			Flight Number %d\n\n",j+1,each[thread_number - 1].flight[j]);
			else if(each[thread_number - 1].operation[j] == 2)
					printf("Operation Number : %d			Operation Type : Cancellation			Flight Number %d\n\n",j+1,each[thread_number - 1].flight[j]);
			else
					printf("Operation Number : %d			Operation Type : Inquiry			Flight Number %d\n\n",j+1,each[thread_number - 1].flight[j]);
	}
}

void finisher()
{
	sleep(1);
	wait(NULL);
	printf("Generating the Log File . . . \n\n");
	sleep(1);
	printf("  Flight Number       Booked Seats     Available Seats\n\n");
	int i;
	for(i = 0; i < f ; i++)
		printf("%8d%20d%22d\n",i+1,flights[i].seats_total - flights[i].seats_available,flights[i].seats_available);
	printf("\nSaving Files . . . \nExiting\n");
}

void log_changer(int process, int operation, int flight)
{
		each[process-1].operation[each[process-1].how] = (int)(operation);
		each[process-1].amount[each[process-1].how] = 2;
		each[process-1].flight[each[process-1].how] = flight;
		each[process-1].how++;
}

int readfile_csv(int var)
{
  		FILE *fp;
        char line[15],action[7];
        char *item;
        int flight_no, thread_no ;
        int counter = 0;
        fp=fopen("DES.csv","r");
		while (fgets(line,120,fp))
	      	{
	      		if(var != (counter++))
	      			continue;
	      		else
	      			{
          					item=strtok(line,",");
          					item=strtok(NULL,",");
          					strcpy(action,item);
          					item=strtok(NULL,",");
          					flight_no = atoi(item);
          					item=strtok(NULL,"\n");
          					thread_no = atoi(item);
          					actor(action, flight_no,thread_no);
          					fclose(fp);
  							return  1;
        			}
        	}
  		return 0;
}

void initialize()
{
	int i;

	for(i=0 ; i < f  ; i++)
	{
		flights[i].seats_total = c;
		flights[i].seats_available = c;
	}
	for( i=0; i< t; i++ )
		each[i].how = 0;
}

int getcount()
{
  	    FILE *fp;
        char line[25],action[7];
        char *item;
        int new = 0;
        fp=fopen("DES.csv","r");
		while (fgets(line,120,fp))
	      			{
	      				new++;
        			}
							fclose(fp);
	count_watch = new;
  	return new;
}

void actor( char *action, int flight_no, int thread_no)
{
int pale;
if 		((strcmp(action,"Inquiry"))== 0)
		pale = 1;
else if ( strcmp(action,"Book") == 0 )
		pale = 2;
else 	pale = 3;

switch(pale)
{
case 1:
	switch (thread_no)
	{
	case 1: pthread_create(&user[1], NULL, changer_enquire1 ,(void *)flight_no);
					break;
	case 2: pthread_create(&user[2], NULL, changer_enquire2 ,(void *)flight_no);
					break;
	case 3: pthread_create(&user[3], NULL, changer_enquire3 ,(void *)flight_no);
					break;
	case 4: pthread_create(&user[4], NULL, changer_enquire4 ,(void *)flight_no);
					break;
	case 5: pthread_create(&user[5], NULL, changer_enquire5 ,(void *)flight_no);
					break;
	case 6: pthread_create(&user[6], NULL, changer_enquire6 ,(void *)flight_no);
					break;
	case 7: pthread_create(&user[7], NULL, changer_enquire7 ,(void *)flight_no);
					break;
	case 8: pthread_create(&user[8], NULL, changer_enquire8 ,(void *)flight_no);
					break;
	case 9: pthread_create(&user[9], NULL, changer_enquire9 ,(void *)flight_no);
					break;
	case 10: pthread_create(&user[10], NULL, changer_enquire10 ,(void *)flight_no);
					break;
	case 11: pthread_create(&user[11], NULL, changer_enquire11 ,(void *)flight_no);
					break;
	case 12: pthread_create(&user[12], NULL, changer_enquire12 ,(void *)flight_no);
					break;
	case 13: pthread_create(&user[13], NULL, changer_enquire13 ,(void *)flight_no);
					break;
	case 14: pthread_create(&user[14], NULL, changer_enquire14 ,(void *)flight_no);
					break;
	case 15: pthread_create(&user[15], NULL, changer_enquire15 ,(void *)flight_no);
					break;
	case 16: pthread_create(&user[16], NULL, changer_enquire16 ,(void *)flight_no);
					break;
	case 17: pthread_create(&user[17], NULL, changer_enquire17 ,(void *)flight_no);
					break;
	case 18: pthread_create(&user[18], NULL, changer_enquire18 ,(void *)flight_no);
					break;
	case 19: pthread_create(&user[19], NULL, changer_enquire19 ,(void *)flight_no);
					break;
	case 20: pthread_create(&user[20], NULL, changer_enquire20 ,(void *)flight_no);
					break;
	}
	break;

case 2:
	switch(thread_no)
	{
		case 1: pthread_create(&user[1], NULL, changer_book1 ,(void *)flight_no );
						break;
		case 2: pthread_create(&user[2], NULL, changer_book2 ,(void *)flight_no );
						break;
		case 3: pthread_create(&user[3], NULL, changer_book3 ,(void *)flight_no );
						break;
		case 4: pthread_create(&user[4], NULL, changer_book4 ,(void *)flight_no );
						break;
		case 5: pthread_create(&user[5], NULL, changer_book5 ,(void *)flight_no );
						break;
		case 6: pthread_create(&user[6], NULL, changer_book6 ,(void *)flight_no );
						break;
		case 7: pthread_create(&user[7], NULL, changer_book7 ,(void *)flight_no );
						break;
		case 8: pthread_create(&user[8], NULL, changer_book8 ,(void *)flight_no );
						break;
		case 9: pthread_create(&user[9], NULL, changer_book9 ,(void *)flight_no );
						break;
		case 10: pthread_create(&user[10], NULL, changer_book10 ,(void *)flight_no );
						break;
		case 11: pthread_create(&user[11], NULL, changer_book11 ,(void *)flight_no );
						break;
		case 12: pthread_create(&user[12], NULL, changer_book12 ,(void *)flight_no );
						break;
		case 13: pthread_create(&user[13], NULL, changer_book13 ,(void *)flight_no );
						break;
		case 14: pthread_create(&user[14], NULL, changer_book14 ,(void *)flight_no );
						break;
		case 15: pthread_create(&user[15], NULL, changer_book15 ,(void *)flight_no );
						break;
		case 16: pthread_create(&user[16], NULL, changer_book16 ,(void *)flight_no );
						break;
		case 17: pthread_create(&user[17], NULL, changer_book17 ,(void *)flight_no );
						break;
		case 18: pthread_create(&user[18], NULL, changer_book18 ,(void *)flight_no );
						break;
		case 19: pthread_create(&user[19], NULL, changer_book19 ,(void *)flight_no );
						break;
		case 20: pthread_create(&user[20], NULL, changer_book20 ,(void *)flight_no );
						break;
	}
 break;

case 3:

	switch (thread_no)
	{
	case 1: pthread_create(&user[1], NULL, changer_cancel1 ,(void *)flight_no);
					break;
	case 2: pthread_create(&user[2], NULL, changer_cancel2 ,(void *)flight_no);
					break;
	case 3: pthread_create(&user[3], NULL, changer_cancel3 ,(void *)flight_no);
					break;
	case 4: pthread_create(&user[4], NULL, changer_cancel4 ,(void *)flight_no);
					break;
	case 5: pthread_create(&user[5], NULL, changer_cancel5 ,(void *)flight_no);
					break;
	case 6: pthread_create(&user[6], NULL, changer_cancel6 ,(void *)flight_no);
					break;
	case 7: pthread_create(&user[7], NULL, changer_cancel7 ,(void *)flight_no);
					break;
	case 8: pthread_create(&user[8], NULL, changer_cancel8 ,(void *)flight_no);
					break;
	case 9: pthread_create(&user[9], NULL, changer_cancel9 ,(void *)flight_no);
					break;
	case 10: pthread_create(&user[10], NULL, changer_cancel10 ,(void *)flight_no);
					break;
	case 11: pthread_create(&user[11], NULL, changer_cancel11 ,(void *)flight_no);
					break;
	case 12: pthread_create(&user[12], NULL, changer_cancel12 ,(void *)flight_no);
					break;
	case 13: pthread_create(&user[13], NULL, changer_cancel13 ,(void *)flight_no);
					break;
	case 14: pthread_create(&user[14], NULL, changer_cancel14 ,(void *)flight_no);
					break;
	case 15: pthread_create(&user[15], NULL, changer_cancel15 ,(void *)flight_no);
					break;
	case 16: pthread_create(&user[16], NULL, changer_cancel16 ,(void *)flight_no);
					break;
	case 17: pthread_create(&user[17], NULL, changer_cancel17 ,(void *)flight_no);
					break;
	case 18: pthread_create(&user[18], NULL, changer_cancel18 ,(void *)flight_no);
					break;
	case 19: pthread_create(&user[19], NULL, changer_cancel19 ,(void *)flight_no);
					break;
	case 20: pthread_create(&user[20], NULL, changer_cancel20 ,(void *)flight_no);
					break;
	}
}
}

void *changer_book1(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 01\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(1,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book2(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 02\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(2,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book3(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 03\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(3,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book4(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 04\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(4,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book5(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 05\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(5,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book6(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 06\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(6,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book7(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 07\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(7,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book8(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 08\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(8,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book9(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 09\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(9,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book10(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 10\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(10,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book11(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 11\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(11,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book12(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 12\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(12,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book13(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 13\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(13,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book14(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 14\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(14,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book15(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 15\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(15,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book16(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 16\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(16,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book17(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 17\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(17,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book18(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 18\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(18,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book19(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 19\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(19,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_book20(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			  pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 20\t");
				printf("Query Type : Book\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available < 2 )
					printf("Query Output : Not enought seats in the flight\n\n");
				else
				{
					flights[i-1].seats_available -= 2;
					printf("Query Output : Booked ticket Successfully \n\n");
					log_changer(20,1,i);
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel1(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 01\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(1,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel2(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 02\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(2,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel3(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 03\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(3,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel4(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 04\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(4,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel5(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 05\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(5,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel6(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 06\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(6,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel7(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 07\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(7,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel8(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 08\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(8,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel9(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 09\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(9,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel10(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 10\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(10,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel11(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 11\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(11,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel12(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 12\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(12,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel13(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 13\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(13,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel14(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 14\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(14,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel15(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			writing[i] = 1;
			printf("Thread Number : 15\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			clock_gettime(CLOCK_MONOTONIC, &end);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(15,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel16(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 16\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(16,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel17(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 17\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(17,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel18(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
			pthread_t self = pthread_self();
			clock_gettime(CLOCK_MONOTONIC, &end);
			writing[i] = 1;
			printf("Thread Number : 18\t");
			printf("Query Type : Cancel\n");
			printf("Thread ID : %lu\t",self);
			elapsed = end.tv_sec - begin.tv_sec;
			elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
			printf("Wait time : %.8f seconds\t",elapsed );
			printf("Flight Number : %d\n",i );
			if( flights[i-1].seats_available > 148 )
				printf("Query Output : Not enought seats to cancel\n\n");
			else
			{
				flights[i-1].seats_available += 1;
				log_changer(18,2,i);
				printf("Query Output : Cancelled Successfully\n\n");
			}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel19(void *flight_no )
{
	struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
				pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				writing[i] = 1;
				printf("Thread Number : 19\t");
				printf("Query Type : Cancel\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available > 148 )
						printf("Query Output : Not enought seats to cancel\n\n");
				else
						{
							flights[i-1].seats_available += 1;
							log_changer(19,2,i);
							printf("Query Output : Cancelled Successfully\n\n");
						}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_cancel20(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 || reading[i]==1 )
			continue;
		else
		{
				pthread_t self = pthread_self();
				writing[i] = 1;
				clock_gettime(CLOCK_MONOTONIC, &end);
				printf("Thread Number : 20\t");
				printf("Query Type : Cancel\n");
				printf("Thread ID : %lu\t",self);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				if( flights[i-1].seats_available > 148 )
					printf("Query Output : Not enought seats to cancel\n\n");
				else
				{
					flights[i-1].seats_available += 1;
					log_changer(20,2,i);
					printf("Query Output : Cancelled Successfully\n\n");
				}
				global_counter--;
				writing[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire1(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(1,3,i);
				pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Thread Number : 01\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire2(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(2,3,i);
				pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Thread Number : 02\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire3(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(3,3,i);
				pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Thread Number : 03\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire4(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(4,3,i);
				pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Thread Number : 04\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire5(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(5,3,i);
				pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Thread Number : 05\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire6(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(6,3,i);
				pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Thread Number : 06\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire7(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(7,3,i);
				pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Thread Number : 07\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire8(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(8,3,i);
				pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Thread Number : 08\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire9(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(9,3,i);
				pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Thread Number : 09\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
 				return NULL;
		}
	}
}

void *changer_enquire10(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(10,3,i);
				pthread_t self = pthread_self();
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Thread Number : 10\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire11(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(11,3,i);
				pthread_t self = pthread_self();
				printf("Thread Number : 11\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire12(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(12,3,i);
				pthread_t self = pthread_self();
				printf("Thread Number : 12\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire13(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(13,3,i);
				pthread_t self = pthread_self();
				printf("Thread Number : 13\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire14(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(14,3,i);
				pthread_t self = pthread_self();
				printf("Thread Number : 14\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire15(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(15,3,i);
				pthread_t self = pthread_self();
				printf("Thread Number : 15\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire16(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(16,3,i);
				pthread_t self = pthread_self();
				printf("Thread Number : 16\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
 				return NULL;
		}
	}
}

void *changer_enquire17(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(17,3,i);
				pthread_t self = pthread_self();
				printf("Thread Number : 17\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire18(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(18,3,i);
				pthread_t self = pthread_self();
				printf("Thread Number : 18\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire19(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				log_changer(19,3,i);
				pthread_t self = pthread_self();
				printf("Thread Number : 19\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  				return NULL;
		}
	}
}

void *changer_enquire20(void *flight_no )
{
struct timespec begin, end;
double elapsed;
clock_gettime(CLOCK_MONOTONIC, &begin);
int i = (int)flight_no;
	while(1)
	{
		if( writing[i]==1 )
			continue;
		else
		{
				reading[i] = 1;
				pthread_t self = pthread_self();
				log_changer(20,3,i);
				printf("Thread Number : 20\t");
				printf("Query Type : Inquiry\n");
				printf("Thread ID : %lu\t",self);
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed = end.tv_sec - begin.tv_sec;
				elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
				printf("Wait time : %.8f seconds\t",elapsed );
				printf("Flight Number : %d\n",i );
				printf("Query Output : Total available seats =  %d\n\n",flights[i-1].seats_available );
				global_counter--;
				reading[i]--;
				pthread_cancel(pthread_self());
  			return NULL;
		}
	}
}
