#include<readline/readline.h>
#include<readline/history.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h> //For getcwd
#include<dirent.h>
#include<sys/stat.h>
#include<sys/wait.h>

// Function to take input
int input_console(char* );
// function for finding pipe
int pipe_seperator(char* , char** );
// help command builtin
void help_menu(int case_);
// function for parsing command words
void space_seperator(char* , char** );
// Function to execute builtin commands
int known_commands(char** );
//Seperates commands and arguments
int input_extractor(char* , char** , char** );
// Function where the system command is executed
void Not_found(char** );
// Function where the piped system commands is executed
void process_piped(char** , char** );

const int max_letters = 1000; // max number of letters to be supported in input
const int max_command_list =  100; // max number of commands to be supported
char pres[200];
struct stat mdir = {0};

int main()
{
    puts("\n\033[H\033[2J");    //Acts as clear
    puts("\n**************** WELCOME TO SHELL SIMULATION *****************\n");
    char get_inputString[max_letters], cur_dir[max_letters], *chopped_input[max_command_list];
    char* chopped_input_piped[max_command_list];
    int execFlag = 0,holder;
    while (4)
    {
        getcwd(cur_dir, sizeof(cur_dir)); //Fetches the present directory
        printf("\n%s:%s",getenv("USER"), cur_dir);
        if(input_console(get_inputString))    // take input
        {
        // process and check if it is a recognised command
        execFlag = input_extractor(get_inputString,chopped_input, chopped_input_piped);
        if(!execFlag)   // returns zero if there is no commandor it is a builtin command,
          continue;
        else    // 1 if it is not known command or 2 if it is including a pipe.
          (execFlag==1)?Not_found(chopped_input):process_piped(chopped_input, chopped_input_piped);
        }
    }
}

int input_console(char* str)
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

int input_extractor(char* str, char** chopped, char** afterpipe)
{
    char* strpiped[2];
    int piped = 0;
    piped = pipe_seperator(str, strpiped);    //Here we detect pipes
    if (piped)
    {
        space_seperator(strpiped[0], chopped);      //Command before pipe
        space_seperator(strpiped[1], afterpipe);    //Command after pipe
    }
    else
        space_seperator(str, chopped);          //Else we just remove spaces
    if(!piped)
    {
        if(known_commands(chopped))
            return 0;
        else
            return 1;
    }
    else
        return 1 + piped;
}

int pipe_seperator(char* str, char** strpiped)
{
    for (int i = 0; i <= 1; i++)
    {
        strpiped[i] = strsep(&str, "|");  //mod_2
          if (strpiped[i] == NULL)
              break;
    }
    if(strpiped[1] == NULL)
      return 0;
    else
      return 1;       // returns zero if no pipe is found.
}

void space_seperator(char* str, char** chopped)
{
    for (int i = 0; i < max_command_list; i++)
    {
        chopped[i] = strsep(&str, " ");         //We are chopping based on space
        if (chopped[i] == NULL)
            break;
        if (strlen(chopped[i]) == 0)
            i--;
    }
}

int known_commands(char** chopped)
{
    int total_commands = 7, i, j = 0,n;
    char* stored_commands[] = {"exit","cd","help","ls","clear","mkdir","cat","top"};
    char* cat[] = {">",">>"};
    pid_t process_id;
    char touch[200];
    touch[5] = ' ';
    touch[0] = 't';
    touch[1] = 'o';
    touch[2] = 'u';
    touch[3] = 'c';
    touch[4] = 'h';
    struct dirent **namelist;

    for (i = 0; i < total_commands; i++)
        if (strcmp(chopped[0], stored_commands[i]) == 0)
            break;
    switch (i)
    {
        case 0:
            puts("\nlogout\n\a"
                   "Saving session...\n"
                   "...copying shared history...\n"
                   "...saving history...truncating history files...\n"
                   "...completed.\n\n"
                   "[Process completed]\n\n");
            exit(0);
            break;
        case 1:
            chdir(chopped[1]);
            return 1;
        case 2:
            if(chopped[1] == NULL)
              help_menu(0);
            else if(strcmp(chopped[1],stored_commands[1]) == 0)
              help_menu(1);
            else if(strcmp(chopped[1],stored_commands[3]) == 0)
              help_menu(2);
            else if(strcmp(chopped[1],stored_commands[5]) == 0)
              help_menu(3);
            else if(strcmp(chopped[1],stored_commands[6]) == 0)
              help_menu(4);
            else if(strcmp(chopped[1],stored_commands[7]) == 0)
              help_menu(5);

            return 1;
        case 4:
            puts("\n\033[H\033[2J");
            return 1;
        case 3:
            n=scandir(".",&namelist,NULL,alphasort);
            while (n--)
        		{
        			printf("%s\t\t",namelist[n]->d_name);
        			free(namelist[n]);
        		}
        		free(namelist);
            printf("\n");
            return 1;
        case 5:
            getcwd(pres,sizeof(pres));
            strcat(pres,"/");
            strcat(pres,chopped[1]);
            if(stat(pres,&mdir)==-1)
               mkdir(pres,0700);
            else
              printf("Directory already present");
            return 1;
        case 6:
            if(strcmp(chopped[1],cat[0]) == 0)
            {
              getcwd(pres,sizeof(pres));
              strcat(pres,"/");
              strcat(pres,chopped[2]);
              if(stat(pres,&mdir)==-1)
              {
                strcat(touch,chopped[2]);
                system(touch);
              }
             FILE *fptr = fopen(chopped[2],"w");
             char c;
             while((c=getchar())!=EOF)
                putc(c,fptr);
             fseek(fptr,0,SEEK_SET);
             fclose(fptr);
            }
            else if(strcmp(chopped[1],cat[1]) == 0)
            {
              getcwd(pres,sizeof(pres));
              strcat(pres,"/");
              strcat(pres,chopped[2]);
              if(stat(pres,&mdir)==-1)
              {
                strcat(touch,chopped[2]);
                system(touch);
              }
              FILE *fptr = fopen(chopped[2],"a");
              char c;
              while((c=getchar())!=EOF)
                 putc(c,fptr);
              fseek(fptr,0,SEEK_SET);
              fclose(fptr);
            }
            else
            {
            FILE *fptr = fopen(chopped[1],"r");
            char c = fgetc(fptr);
            while( c !=EOF )
            {
              printf("%c",c);
              c=fgetc(fptr);
            }
            if(!fptr)
              printf("File doesn't exist\n");
            fclose(fptr);
            }
            return 1;
        case 7 : if(!strcmp(chopped[0],"top"))
                {
                  printf("Inovking system for the same \n");
                  process_id = vfork();
                 if (process_id == 0)
                  {
                    if (execvp(chopped[0], chopped) < 0)
                    puts("\nCould not execute command..");
                    exit(0);
                  }
                  waitpid( process_id, NULL, 0 );
                  return 1;
                }
          }
    return 0;
  }

void Not_found(char** chopped)
{
    int n;
    printf("\n\nCommand Not found in directory \n");
    printf("\nTo initiate System call press 1 else 0\n");
    scanf("%d",&n);
    if(n == 1)
    {
    pid_t process_id;         // Forking a child
    if ((process_id = vfork()) < 0)
    {
        puts("\nFailed forking child..");
        return;
    }
    else if (process_id == 0)
    {
        if (execvp(chopped[0], chopped) < 0)
            puts("\nCould not execute command..");
        exit(0);
    }
    waitpid( process_id, NULL, 0 );
    }
}

void process_piped(char** chopped, char** afterpipe)
{
    pid_t process_id_1, process_id_2;
    process_id_1 = vfork();
    if (process_id_1 < 0)
    {
        puts("Forking Error\n");
        return;
    }
    if (process_id_1 == 0)
    {
        known_commands(chopped);
    }
    waitpid( process_id_1, NULL, 0 );
    process_id_2 = vfork();
    if (process_id_2 < 0)
      {
          puts("Forking Error\n");
          return;
      }
    if (process_id_2 == 0)
        {
            known_commands(afterpipe);
        }
    waitpid( process_id_2, NULL, 0 );
}

void help_menu(int case_)
{
  switch(case_)
  {
  case 1: puts("\ncd: cd [-L|[-P [-e]] [-@]] [dir]\n"
               "Change the shell working directory.\n\n"
               "Exit Status:"
               "Returns 0 if the directory is changed, and if $PWD is set successfully when"
               "-P is used; non-zero otherwise.\n");
          break;
  case 2: puts("\nls - list directory contents\n"
               "ls [OPTION]... [FILE]...\n"
               "DESCRIPTION : \n "
               "\tList  information  about  the FILEs (the current directory by default). "
               "Sort entries alphabetically if none of -cftuvSUX nor --sort  is  speci‐ "
               "fied\n");
          break;
  case 3: puts("\nmkdir - make directories\n"
               "mkdir [NEW DIRECTORY NAME (NON EXISTING)]"
               "Create the DIRECTORY(ies), if they do not already exist."
               "Mandatory  arguments  to  long  options are mandatory for short options too.\n");
          break;
  case 4: puts("\ncat - concatenate files and print on the standard output\n"
               "cat [OPTION]... [FILE]... \n"
               "Concatenate FILE(s) to standard output.\n"
               "With no FILE, or when FILE is -, read standard input.\n"
               "> For overwriting existing or new file\n"
               ">> For appending existing or new file\n");
          break;
  case 5: puts("top - display Linux processes\n"
              "top -hv|-bcHiOSs -d secs -n max -u|U user -p pid -o fld -w [cols]\n"
              "The traditional switches `-' and whitespace are optional.\n");
          break;
  default : puts("\nGNU bash simulator, version 1.0.0(1)-release-2018 (x86_64)\n"
              "These shell commands are defined internally.  Type `help' to see this list.\n"
              "Use `info bash' to find out more about the shell in general.\n"
              "Use `man -k' or `info' to find out more about commands not in this list.\n\n"
              "A star (*) next to a name means that the command is disabled.\n\n"
              "List of supported commands : \n"
              "cd [-L|-P] [dir]\n"
              "help [-s] [pattern ...]\n"
              "ls [-s]\n"
              "exit [n]\n\n"
              "Available features of the prototype 1.0.0(1)-release-2018\n\n"
              "Variables - Some variable names an wait [n]\n"
              "while COMMANDS; do COMMANDS; done  { COMMANDS ; }\n");
    }
}
