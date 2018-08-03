#include <stdio.h>             //For standard input and output.
#include <time.h>              //For time based functions.
#include <string.h>            //For string manipulation.
#include <unistd.h>            //POSIX operating system API.
#include <dirent.h>            //For manipulationof directory of streams.
#include <stdlib.h>            //General purpose standard library of C programming.
#include <sys/stat.h>          //Stat structure inclusion.
#include <sys/wait.h>          //For time based functions.
#include <pthread.h>           //For Implementing threads.
#include <readline/readline.h> //For command line input.
#include <readline/history.h>  //To add input in buffer.

//Maximum size of any input being provided in the configuration file.
#define max_letters 150
//Create the configuration file when its not existing.
char* config = "touch /home/rajesh/mycrontab/configuration.txt";

//To use and access threads.
pthread_t driver;

//For providing help menu in terminal.
void  Help();
//For matching the given time with the current time.
int   MatchTime();
void  EditHandler();
//To seperate the current time for matching.
int*  TimeSeperator();
//For input console and display.
void  ConsoleDriver();
//For console management.
int   InputConsole(char* str);
//To read the provided configuration file.
void  ReadFileConfiguration(int type);
//Driver Runs throughout the program, checking and tracing changes in configuration
void* DriverFunction(void *arg);
//To tokenize input with respect to space.
void  SpaceSeperator(char* str, char** chopped);

int main()
{
    pthread_create(&driver, NULL, &DriverFunction ,NULL);
    ConsoleDriver();
    return 0;
}

void* DriverFunction(void *arg)
{
  pid_t process_id;         // Forking a child
  while(1)
  {
        FILE *fp;
        char line[max_letters],action[max_letters];
        char *item;
        const char* main = "*";
        int date[5],i;
        fp=fopen("/home/rajesh/mycrontab/configuration.txt","r");
        if(!fp)
          system(config);
        while (fgets(line,max_letters,fp))
        {
          item=strtok(line," ");
          if(strcmp(main,item)== 0)
            date[0] = -1;
          else
            date[0] = atoi(item);

          item=strtok(NULL," ");
          if(strcmp(main,item)== 0)
            date[1] = -1;
          else
            date[1] = atoi(item);

          item=strtok(NULL," ");
          if(strcmp(main,item)== 0)
            date[2] = -1;
          else
            date[2] = atoi(item);

          item=strtok(NULL," ");
          if(strcmp(main,item)== 0)
            date[3] = -1;
          else
            date[3] = atoi(item);

          item=strtok(NULL," ");
          if(strcmp(main,item)== 0)
            date[4] = -1;
          else
            date[4] = atoi(item);

          item=strtok(NULL," ");
          strcpy(action,item);

          item=strtok(NULL," ");
          while(item != NULL)
          {
            strcat(action," ");
            strcat(action,item);
            item=strtok(NULL," ");
          }
          if(!MatchTime(date))
          {
            if ((process_id = vfork()) < 0)
              puts("\nFailed forking child..");
            else if (process_id == 0)
              system(action);
            waitpid( process_id, NULL, 0 );
          }
        }
    sleep(59);
  }
}

int MatchTime(int* array)
{
  int i,j,*tds = TimeSeperator(),counter = 0;


  for(i=0;i<5;i++)
  {
    if(array[i]==tds[i] || array[i] == (-1))
    {
      counter++;
      if(counter == 5 && i == 5)
        return 0;
    }
    else
      return 1;
  }
  return 0;
}

void ConsoleDriver()
{
  //For manipulating the configuration file from console
  puts("\n\033[H\033[2J");    //Acts as clear
  puts("\n**************** WELCOME TO CRONTAB SIMULATION *****************\n");
  char get_inputString[max_letters], cur_dir[max_letters], *chopped_input[2];
  char* chopped_input_piped[2];
  int execFlag = 0,holder;
  char* stored_commands[] = {"crontab","Help","exit","clear"};
  char* cron_commands[] = {"-l","-r","-e","-i"};
  while (4)
  {
      getcwd(cur_dir, sizeof(cur_dir)); //Fetches the present directory
      printf("\n%s:%s",getenv("USER"), cur_dir);
      int i;
      if(InputConsole(get_inputString))    // take input
      {
        SpaceSeperator(get_inputString, chopped_input);
        if (strcmp(chopped_input[0], stored_commands[0]) == 0)
        {
          for (i = 0; i < 4; i++)
              if (strcmp(chopped_input[1], cron_commands[i]) == 0)
                  break;
          switch (i)
          {
            case 0: ReadFileConfiguration(0);
              break;
            case 1: ReadFileConfiguration(1);
              break;
            case 2: EditHandler();
              break;
            case 3: printf("To confirm removal press 1 else 0\n");
                    scanf("%d",&i);
                    if(i)
                      ReadFileConfiguration(1);
              break;
            default: printf("\n Command Not recognised,\n Type 'Help' for Help or 'exit' to quit\n");
          }
        }
        else if (strcmp(chopped_input[0], stored_commands[1]) == 0)
          Help();
        else if (strcmp(chopped_input[0], stored_commands[2]) == 0)
        {
          printf("\n\nTerminating Running Instances \n");
          sleep(1);
          printf("\nExiting . . . . \n\n");
          sleep(1);
          exit(0);
          puts("\n\033[H\033[2J");
        }
        else if (strcmp(chopped_input[0], stored_commands[3]) == 0)
          puts("\n\033[H\033[2J");    //Acts as clear
        else
          printf("\nCommand Not recognised,\n Type 'Help' for Help or 'exit' to quit\n");
      }
  }
}

void EditHandler()
{
  int n=1;
  printf("\n\tExecution is taking you to Terminal editor.");
  printf("\tAfter editing press 'ctrl + X' to save\n");
  printf("\n\tAfter that press 'shift + Y' and 'Enter'\n");
  printf("\n\tThe Order : Minute Hours Days Month Year Command\n");
  printf("\n\tTo initiate System call press 1 else 0 . . ");
  scanf("%d",&n);
  char *chopped = "nano /home/rajesh/mycrontab/configuration.txt";
  if(n == 1)
  {
  pid_t process_id;         // Forking a child
  if ((process_id = vfork()) < 0)
      puts("\nFailed forking child..");
  else if (process_id == 0)
          system(chopped);
  waitpid( process_id, NULL, 0 );
  }
}

void ReadFileConfiguration(int a)
{
  	FILE *fp;
    int counter = 0;
    if(!a)
    {
      char line[max_letters];
      printf("\n");
      fp=fopen("/home/rajesh/mycrontab/configuration.txt","r");
      if(!fp)
        system(config);
		  while (fgets(line,max_letters,fp))
      {
          printf("%d) ",counter+1);
	      	puts(line);
          counter++;
      }
      if(!counter)
        printf("\n\tNo crontab to be displayed\n");
    }
    else
      fp=fopen("/home/rajesh/mycrontab/configuration.txt","w");
    fclose(fp);
}

int InputConsole(char* str)
{
    char* buffer = readline(" $ ");      //To output dollar in the console
    if (strlen(buffer) != 0)
    {
        add_history(buffer);
        strcpy(str, buffer); //We copy the input to the string str
        return 1;
    }
    else
        return 0;
}

void SpaceSeperator(char* str, char** chopped)
{
    for (int i = 0; i < 2; i++)
    {
        chopped[i] = strsep(&str, " ");         //We are chopping based on space
        if (chopped[i] == NULL)
            break;
        if (strlen(chopped[i]) == 0)
            i--;
    }
}

int* TimeSeperator()
{
  time_t timer;
  char buffer[26];
  time(&timer);
  struct tm* tm_info = localtime(&timer);
  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
  int * array = (int *)malloc((sizeof(int))*5);
  int h1,h2,h3,h4;
  h1 = buffer[14]; h2 = buffer[15]; h1 = 10*h1 + h2 - 528 ;  array[0] = h1;  //Calculating Minutes
  h1 = buffer[11]; h2 = buffer[12]; h1 = 10*h1 + h2 - 528 ;  array[1] = h1;  //Calculating Hours
  h1 = buffer[8];  h2 = buffer[9];  h1 = 10*h1 + h2 - 528 ;  array[2] = h1;  //Calculating Days
  h1 = buffer[5];  h2 = buffer[6];  h1 = 10*h1 + h2 - 528 ;  array[3] = h1;  //Calculating Month
  h1 = buffer[0];  h2 = buffer[1];  h3 = buffer[2];   h4 = buffer[3];
  h1 = 1000*h1 + 100*h2 + 10*h3 + h4 - 53328; array[4] = h1; //Calculating Year
  return array; // Minute Hours Days Month Year
}

void Help()
{
puts("CRONTAB(1)                                                             General Commands Manual                                                            CRONTAB(1)\n\n");
puts("NAME\n");
puts("\t\tcrontab - maintain crontab files for individual users (Vixie Cron)\n\n");
puts("SYNOPSIS\n");
puts("\tcrontab [ -u user ] file\n");
puts("crontab [ -i ] { -e | -l | -r }\n\n");
puts("\tThe first form of this command is used to install a new crontab from some named file or standard input if the pseudo-filename ``-'' is given.\n");
puts("\tThe -l option causes the current crontab to be displayed on standard output. See the note under DEBIAN SPECIFIC below.\n");
puts("\tThe -r option causes the current crontab to be removed.\n");
puts("\tThe -e option is used to edit the current crontab using the editor specified by the VISUAL or EDITOR environment variables.  After you exit from  the  editor,\n"
    "\tthe modified crontab will be installed automatically. If neither of the environment variables is defined, then the default editor /usr/bin/editor is used.\n"
    "\tThe -i option modifies the -r option to prompt the user for a 'y/Y' response before actually removing the crontab.\n"
    "\tCommands  are  executed by cron(8) when the minute, hour, and month of year fields match the current time, and when at least one of the two day fields (day of\n"
    "\tmonth, or day of week) match the current time (see ``Note'' below).  cron(8) examines cron entries once every minute.  The time and date fields are:\n"
                            "\t\t\t\tfield          allowed values\n"
                            "\t\t\t\t -----          -------------- \n"
                            "\t\t\t\t minute         0-59 \n"
                            "\t\t\t\t hour           0-23 \n"
                            "\t\t\t\t day of month   1-31 \n"
                            "\t\t\t\t month          1-12 (or names, see below) \n"
                            "\t\t\t\t day of week    0-7 (0 or 7 is Sun, or use names) \n"
    "\t\tA field may be an asterisk (*), which always stands for ``first-last''. \n");
puts("DIAGNOSTICS\n");
puts("\tcron requires that each entry in a crontab end in a newline character. If the last entry in a crontab is missing a newline (ie, terminated by EOF), cron  will\n"
    "\tconsider the crontab (at least partially) broken. A warning will be written to syslog.\n\n");
puts("AUTHOR\n"
     "\tRajesh Kumar Jha  <rajes174101007@iitg.ernet.in>  is  the  author of cron and original creator of this manual page.\n");
puts("4th Berkeley Distribution                                                   07 March 2018                                                                  CRONTAB(5)\n");

}
