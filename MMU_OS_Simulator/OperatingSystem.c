//ipcrm -M <key> 987456321
#include <time.h>               //For time()
#include <stdio.h>              //For standard input and output.
#include <signal.h>             //For inter process communication
#include <stdlib.h>             //POSIX operating system API
#include <unistd.h>             //POSIX operating system API
#include <sys/ipc.h>            //For inter process communication
#include <sys/shm.h>            //For time based functions.
#include <sys/types.h>          //For clock and system times
#include "SharedMCred.h"        //Shared memory credentials

#define SharedKey 12345
#define MB_1 (1024*1024)
#define MB_8 (8*MB_1)

time_t t;
//These are the variables of the header file that has been included.
PageTable Handle;
//The Key choosen for the shared variable.
key_t Key;
//To store number of pages and frames. And if operation is FIFO or NMRU.
int TotalFrames, TotalPages, OperationMode;
//The Array keeps tracks of the frame distributed. Then total disk accesses amd total requests entertained.
int *FIFOArray, DiskAccess, GlobalCounter;

//To provide PID of OS to MMU.
void write_pid();
//Not Most Recently Used page replacement policy simulation.
int NRU(int page);
//First In First Out page replacement policy simulation.
int FIFO(int page);
//Next Most Recently Used page replacement policy simulation.
int NMRU(int page);
//Gets activated when MMU finishes execution.
void WaiterCaller();
//Creates the shared memory and attatches itself to it.
void SharedMemoryCreator();
//Handles signals passed by the MMU.
void MyHandler(int signum);

int main(void)
{
  Key = getpid();
  int OSKey,Frame;
  srand((unsigned) time(&t));
  if(signal(SIGUSR1, MyHandler) == SIG_ERR)
      printf("\nCan't catch SIGINT\n");
  GlobalCounter = 1;
  DiskAccess = 0;
  SharedMemoryCreator();
  FIFOArray = (int *)malloc(TotalFrames*sizeof(int));
  write_pid();
  if(TotalFrames == 2)
    OperationMode = 1;
  printf("\nCalling Main Memory Unit for further action \n");
  while (1)
  {
  for(int i = 0; i < TotalPages; i++)
    {
        if(Handle[i].Requested != 0)
        {
            sleep(1);
            OSKey = Handle[i].Requested;
            if(OperationMode == 1)
              Frame = FIFO(i);
            else if(OperationMode == 2)
              Frame = NMRU(i);
            else
              Frame = NRU(i);
            printf("Page Number %d is NOW at Frame %d\n",i,Frame);
            Handle[i].Requested = 0;
            Handle[i].Frame = Frame;
            Handle[i].Valid = 1;
            kill(OSKey,SIGUSR2);
        }
    }
    sleep(1);
    WaiterCaller();
  }
}

int NRU(int page)
{
  for(int i = 0 ; i < TotalFrames ; i++ )
      if(FIFOArray[i] == (-1))   //If we have empty frame we allocate one
        {
          FIFOArray[i] = page;
          return i;
        }
    int Random,Highest = 0;
    int Counter = 0;
    int Array[TotalFrames - 1];
    for(int i = 0; i < TotalPages ; i++ )
      if( Handle[i].TimeStamp > Highest )
          Highest = Handle[i].TimeStamp;
    printf("Highest Frame Stamp %d\n",Highest);
    for(int i = 0; i < TotalPages ; i++ )
      if((Handle[i].Valid == 1) && (Handle[i].TimeStamp < (Highest - 1)) && (!Handle[i].Dirty))
            Array[Counter++] = i;
    if(Counter == 0)
    {
      printf("All pages are dirty, choosing among dirty pages leaving the most Recently used\n");
      for(int i = 0 ; i < TotalPages ; i++ )
      {
        if((Handle[i].Valid))
        {
          if(Handle[i].TimeStamp < (Highest - 1))
          {
              Random = i;
          }
        }
      }
      printf("Page chosen for replacement = %d\n", Random);
      printf("Dirty Page Encountered, writing back . . . . \n");
      sleep(1);
      printf("Disk Access successful \n");
      Handle[Random].Dirty = 0;
      DiskAccess++;
    }
    else
    {
      printf("Choosing a page at random among the non dirty pages \n");
      if(Counter == 1)
      {
        printf("Only one non dirty page encountered\n");
        Random = Array[0];
      }
      else
      {
      Random = random()  % Counter;
      Random = Array[Random];
      }
      printf("Page chosen for replacement = %d\n", Random);
    }
    Handle[Random].Valid = 0;
    Handle[page].Frame = Handle[Random].Frame;
    Handle[Random].Frame = -1;
    return Handle[page].Frame;
}

int NMRU(int page)
{
  for(int i = 0 ; i < TotalFrames ; i++ )
      if(FIFOArray[i] == (-1))   //If we have emHandley frame we allocate one
        {
          FIFOArray[i] = page;
          return i;
        }
  int Highest , SecondHighest, HighestPage, SecondHighestPage;
  HighestPage = SecondHighestPage = 0;
  Highest = SecondHighest = -1 ;
  for(int i = 0; i < TotalPages ; i++ )
  {
    if( Handle[i].Valid && Handle[i].Frame != (-1))
    {
    if(Handle[i].TimeStamp > SecondHighest)
      {
        if(Handle[i].TimeStamp > Highest)
        {
        SecondHighest = Highest;
        Highest = Handle[i].TimeStamp;
        SecondHighestPage = HighestPage;
        HighestPage = i;
      }
    else
        {
          SecondHighest = Handle[i].TimeStamp;
          SecondHighestPage = i;
        }
      }
    }
  }
  printf("Page chosen for replacement = %d\n", SecondHighestPage);
  printf("Highest %d and Stamp %d\n",HighestPage,Highest );
  printf("Second Highest %d and Stamp %d\n",SecondHighestPage,SecondHighest );
  if( Handle[SecondHighestPage].Dirty )
  {
    printf("Dirty Page Encountered, writing back . . . . \n");
    sleep(1);
    printf("Disk Access successful \n");
    Handle[SecondHighestPage].Dirty = 0;
    DiskAccess++;
  }
  Handle[SecondHighestPage].Valid = 0;
  Handle[page].Frame = Handle[SecondHighestPage].Frame;
  Handle[SecondHighestPage].Frame = -1;
  return Handle[page].Frame;
}

int FIFO(int page)
{
  for(int i = 0 ; i < TotalFrames ; i++ )
    {
      if(FIFOArray[i] == (-1))   //If we have emHandley frame we allocate one
        {
          FIFOArray[i] = page;
          return i;
        }
    }
  if(Handle[FIFOArray[0]].Dirty)
  {
    printf("Dirty Page Encountered, writing back . . . . \n");
    sleep(1);
    printf("Disk Access successful \n");
    Handle[FIFOArray[0]].Dirty = 0;
    DiskAccess++;
  }
  Handle[FIFOArray[0]].Valid = 0;
  Handle[FIFOArray[0]].Frame = -1;
  printf("Page chosen for replacement %d \n",FIFOArray[0]);
  for(int i = 0 ; i < (TotalFrames - 1) ; i++ )
  {
    Handle[FIFOArray[i + 1]].Frame = i;
    FIFOArray[i] = FIFOArray[i + 1];
  }
  FIFOArray[TotalFrames - 1] = page;
  Handle[page].Valid = 1;
  Handle[page].Frame = TotalFrames - 1 ;
  return (TotalFrames - 1);
}

void MyHandler(int signum)
{
    if (signum == SIGUSR1)
  		printf("\n%d. Control returned to the Operating System \n",GlobalCounter++);
}

void WaiterCaller()
{
  FILE *fp1;
  int pid;
  char line[100];
  fp1=fopen("OSPID.txt","r");
  fgets(line,10,fp1);
  pid = atoi(line);
  fclose (fp1);

  if(pid < 0)
  {
    if (shmdt(Handle) < 0)
        perror("Detachment Error\n");
    char* new = "rm OSPID.txt";
    system(new);
    sleep(2);
    printf("\n\n\tTotal Disk Accesses = %d\n",DiskAccess);
    printf("\tMMU terminated , OS terminating . . . \n");
    sleep(1);
    exit(1);
  }
}

void SharedMemoryCreator()
{
  int shared_id;
  printf("\nEnter the mode of operation : \t 1.First In First Out\n");
  printf("                            \t 2.Next Most Recently Used\n");
  printf("                            \t 3.Not Most Recently Used\n");
  scanf("%d",&OperationMode);
  if( OperationMode == 1)
    printf("\n\t First In First Out mode turned on . . .\n");
  else if(OperationMode == 2)
    printf("\n\t Next Most Recently Used mode turned on . . .\n");
  else
    printf("\n\t Not Most Recently Used mode turned on . . .\n");
  printf("\nEnter the number of Frames : ");
  scanf("%d",&TotalFrames);
  printf("\nEnter the number of Pages : ");
  scanf("%d",&TotalPages);
  if(TotalPages <= TotalFrames)
    {
      printf("Exiting : Total Pages are less or equal to total frames \n");
      exit(0);
    }
  printf("\n\tCreating virtual address space . . . \n");
  int size = TotalPages * sizeof(Handle);
  printf("\n\tAlloacting a total size of %d Bytes\n",size);
  //
  //if((shared_id = shmget(2, MB_8, SHM_HUGETLB | IPC_CREAT | SHM_R | SHM_W)))
  if ((shared_id = shmget(Key, size , IPC_CREAT | 0777 )) < 0)
  {
      perror("shmget error : memory allocation error");
      exit(1);
  }
  if ((Handle = (PageTable)shmat(shared_id, NULL, 0)) == NULL)
  {
      perror("shmat error : attatching OS to memory not successful");
      exit(1);
  }
  for( int i=0 ; i < TotalPages; i++)
  {
      Handle[i].Valid = Handle[i].Requested = Handle[i].Dirty = 0;
      Handle[i].Frame = Handle[i].TimeStamp = (-1);
  }
  printf("\n\tAll valid bit fields set to zero  \n");
}

void write_pid()
{
  pid_t pidl = getpid();
  FILE * fp = fopen ("OSPID.txt","w");
  fprintf (fp,"%d",pidl);
  fclose (fp);
  for( int i = 0; i < TotalFrames ; i++ )
      FIFOArray[i] = -1 ;
}
