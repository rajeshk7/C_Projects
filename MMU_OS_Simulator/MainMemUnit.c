#include <stdio.h>             //For standard input and output.
#include <signal.h>            //For inter process communication
#include <stdlib.h>            //General purpose standard library of C programming
#include <unistd.h>            //POSIX operating system API
#include <string.h>            //For string manipulation.
#include <sys/ipc.h>           //For inter process communication
#include <sys/shm.h>           //For time based functions.
#include <sys/types.h>         //For clock and system times
#include "SharedMCred.h"       //Shared memory credentials
#include <readline/readline.h> //For command line input
#include <readline/history.h>  //To add input in buffer

//Maximum size of any address being provided in the trace file.
#define max_letters 100
//The Key choosen for the shared variable.
#define SharedKey 12345

//Mode stores the type of operation. R for read and W for write.
char* Mode;
//The variables act as a counter for Hit and Miss of the page.
int Hit,Miss;
//These are the variables of the header file that has been included.
PageTable Handle;
//The counter counts the number of requests that has been processed.
int global_counter = 1;
//The Key choosen for the shared variable.
key_t Key = SharedKey;
//To store number of pages, frames, page bits and offset.
int TotalPages, TotalFrames, PageBit, Offest, OSPID, page;

//For checking page faults.
int  Checker();
//Prints the entire page table with all available credentials.
void PageTableShow(); //Page table printer
//To take all the data as input.
void InputData();
//To detach the MMU from the shared memory.
int  ReleaseControl();
//To convert the provided hexadecimal address in binary and process.
int  Converter(char *item);
//To Handle the signal passed by the operating system.
void MyHandler(int signum);

int main(void)
{
  FILE *fp;
  int SharedId;
  Hit = Miss = 0;
  char line[max_letters], action[max_letters], **strpiped, *item;
  InputData();
  if (signal(SIGUSR2, MyHandler) == SIG_ERR)
      printf("\nNot able to catch SIGUSR2\n");
  Key = OSPID;
  int size = TotalPages * sizeof(Handle) ;
  if ((SharedId = shmget(Key,size, 0777 )) < 0)
  {
      perror("shmget error : Not able to find shared memory");
      exit(1);
  }
  if ((Handle = (PageTable)shmat(SharedId, NULL, 0)) == NULL)
  {
      perror("shmat error : attatching MMU to memory not successful");
      exit(1);
  }
  puts("\n\033[H\033[2J");
  fp=fopen("memtrace.txt","r");
  if(!fp)
    printf("Error, file doesn't exist\n");
  while (fgets(line,max_letters,fp))
  {
    item = strtok(line," ");
    Mode = strtok(NULL," ");
    page = Converter(item);
    page--;
    Handle[page].TimeStamp = global_counter++;
    Checker(page);
    PageTableShow();
  }
  ReleaseControl();
}

void MyHandler(int signum)
{
    if (signum == SIGUSR2)
  		printf("\tControl returned to MMU\n");
}

void PageTableShow()
{
  for(int i = 0; i < TotalPages; i++ )
    printf("%d. Valid = %d Frame = %d Dirty = %d Requested = %d TimeStamp = %d\n",(i),Handle[i].Valid,(Handle[i].Frame),Handle[i].Dirty,Handle[i].Requested,Handle[i].TimeStamp);
}

int Checker(int page)
{
  if(page > TotalPages)
  {
      printf("Error : Page number exceeding limits\n");
      printf("Total Pages = %d Given Page Number = %d\n",TotalPages,page);

  }
  else
  {
    printf("\n%d . The Page Requested is %d And Mode is %c\n",global_counter - 1 ,page,Mode[0]);
    if(Handle[page].Valid != 0)
      {
          printf("The Page has already got a frame = %d\n",Handle[page].Frame);
          Hit++;
      }
    else
    {
      printf("The page hasn't receiced a frame, Shifting control to OS \n");
      Handle[page].Requested = getpid();
      sleep(1);
      if (kill(OSPID,SIGUSR1) < 0)
      {
          perror("Killing to OS, error encountered\n");
          exit(EXIT_FAILURE);
      }
    while( Handle[page].Requested != 0 )
        sleep(1);
      if(Handle[page].Valid != 1)
        printf("Error, OS not working properly\n");
      Miss++;
    }
    if(Mode[0] == 'W')
      {
          printf("Write operation is being performed \n");
          Handle[page].Dirty = 1;
      }
  }
}

int Converter(char *hex)
{
  char binser[1000];
  binser[0]=0;
  for(int i=2; hex[i] != '\0'; i++)
  {
      switch( hex[i] )
      {
          case '0':
              strcat(binser, "0000");
              break;
          case '1':
              strcat(binser, "0001");
              break;
          case '2':
              strcat(binser, "0010");
              break;
          case '3':
              strcat(binser, "0011");
              break;
          case '4':
              strcat(binser, "0100");
              break;
          case '5':
              strcat(binser, "0101");
              break;
          case '6':
              strcat(binser, "0110");
              break;
          case '7':
              strcat(binser, "0111");
              break;
          case '8':
              strcat(binser, "1000");
              break;
          case '9':
              strcat(binser, "1001");
              break;
          case 'a':
          case 'A':
              strcat(binser, "1010");
              break;
          case 'b':
          case 'B':
              strcat(binser, "1011");
              break;
          case 'c':
          case 'C':
              strcat(binser, "1100");
              break;
          case 'd':
          case 'D':
              strcat(binser, "1101");
              break;
          case 'e':
          case 'E':
              strcat(binser, "1110");
              break;
          case 'f':
          case 'F':
              strcat(binser, "1111");
              break;
      }
  }
int a,b = strlen(binser) - Offest;
a = 0;
for(int i = 0; i < b; i++ )
{
  switch (binser[i])
  {
    case '0': a *= 2;
      break;
    case '1': a = a*2 + 1;
  }
}
return a;
}

void InputData()
{
  printf("\nEnter Total Frames : ");
  scanf("%d",&TotalFrames);
  printf("\nEnter Total Pages : ");
  scanf("%d",&TotalPages);
  printf("\nEnter the OFFSET Bits : ");
  scanf("%d",&Offest);
  char line[100];
  FILE *fp1;
  here:   fp1 = fopen("OSPID.txt","r");
            if(fp1 == NULL)
                goto here;
  fgets(line,10,fp1);
  OSPID = atoi(line);
  fclose (fp1);
}

int ReleaseControl()
{
  if (shmdt(Handle) < 0)
  {
      perror("Detachment Error\n");
  }
  printf("\n\n\tTotal Requests = %d\n", global_counter - 1);
  printf("\tTotal Misses = %d\tTotal Hits = %d\n",Miss,Hit);
  printf("\tMMU Terminating \n");
  FILE * fp;
  int i = -1;
  fp = fopen ("OSPID.txt","w");
  fprintf (fp,"%d",i);
  fclose (fp);
  sleep(1);
  if (kill(OSPID,SIGUSR1) < 0)
  {
      perror("Error during termination \n");
  }
  return(0);
}
