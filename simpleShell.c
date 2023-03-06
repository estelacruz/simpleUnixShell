#include <stdio.h>
#include <fcntl.h> // open 
#include <unistd.h> //close
#include <string.h> 
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

//run program: gcc -o nyush nyush.c 

/** RESOURCES 
https://en.cppreference.com/w/c/string/byte/strrchr

Cite above explanined strrchr function in c and how it finds the last occurrence of a char that you pass as an arg
If we want to exclude the char, we can just move 1 char over by adding 1 (add 1 byte to access next char)

https://linuxhint.com/getline-function-c/ and https://opensource.com/article/22/5/safely-read-user-input-getline

Cites explain how getline function works and how it automaticaly inceases the memory block as necessary. It made me realize how it
reads from stdin and how it returns a ptr to that stream that it read. it will resize the str
w realloc so that there is enough memory. getline uses a newline as the delimeter


https://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/

explains strok() and how it splits str[] by a delimeter, in order to get all tokens, we have to loop and will be null
when there are no more tkns

strok_r() is reentrant meaning it can interrupted and reentered w.o a problem


https://www.qnx.com/developers/docs/6.5.0SP1.update/com.qnx.doc.neutrino_lib_ref/c/chdir.html#:~:text=The%20chdir()%20function%20changes,or%20an%20absolute%20path%20name.
explained how chdir() function can accept relative or absolute path

https://stackoverflow.com/questions/15053776/how-do-you-disable-the-unused-variable-warnings-coming-out-of-gcc-in-3rd-party-c

Directed me towards a possible solution to fix the unused variable error i get when I try to make nyush

*/


/**
 Notes: 
 Makefile 
*/

void signalHandle(int signalNumber){
    if(signalNumber == SIGINT){
        //ignore, go back to while, is this enough?
        //continue;
    }else if (signalNumber == SIGQUIT)
    {
        //continue;
    }else if (signalNumber == SIGTSTP)
    {
        //continue;
    }
    
    
}

// function use: gets last token of current directory, prints out prompt
void getLastToken(char *tokens){

 // want very last tkn, start with what tokens points to and then we will iterate to last one
  char * lastTkn = tokens;
  //iterate through while to get to last one
  while (tokens != NULL){
    lastTkn= tokens;
    //update token position
    tokens = strtok(NULL, "/");
  }
  //finally, print out Last toke
  //printf("%s %s", "Last Token: ", lastTkn);
  //printf("\n");
  //flush out, check if this is suffient 
  fflush(stdout);
  /**
     Keep in mind that STDOUT is line-buffered by default.
    don’t forget to flush STDOUT immediately after you print the prompt. 
    Otherwise, your program may not work correctly with the autograder.
 */
  
}
// take away the spaces of an argument


char * removespace( char *inputstr){
   
   //printf("%s%s\n", "string is: ", inputstr);
   //printf("%s%lu\n", "string len is: ", strlen(inputstr));
   
   char *newString = (char *)malloc(strlen(inputstr) * sizeof(char));
   int g;
   int track = 0;

  for(g = 0; g < strlen(inputstr) + 1; g++){

   char ctemp = inputstr[g];
   int inttemp = inputstr[g];
   if(inttemp == 32){ //a space
      continue;
   }else{
      newString[track] = ctemp;
      track++;
   }
   
   //printf("%s%c%s\n", "char is: '", ctemp, "'");
  }

  newString[track] = '\0';
  // printf("%s%s%s\n", "new string is: '", newString, "'");
  
  return newString;

}

//working dir

void shellworkingdir() {

   char currworkdir[256];
   getcwd(currworkdir, sizeof(currworkdir));
  
   char *LastToken = strrchr(currworkdir, '/');
   //conditional: if lastoken is true, then set basename, else return the whole working dir
   char *baseName = LastToken ? LastToken + 1: currworkdir;
   

   
   //print out the basename
   printf("%s %s%s", "[nyush", baseName, "]$ ");
   // user has to enter command so do not print out new line yet
  // printf("\n");
  //flush out, check if this is suffient 
   fflush(stdout);
  /**
     Keep in mind that STDOUT is line-buffered by default.
    don’t forget to flush STDOUT immediately after you print the prompt. 
    Otherwise, your program may not work correctly with the autograder.
 */
}


int my_system(char *command, char **commandArr) {
   // if in the child process, execute the following command
   //if it includes a slash, PATH is ignored and the file at the specficied pathname is executed
   //char* cmdNospace;
   //cmdNospace = removespace(command);
   //printf("%s%s%s\n", "command: '", command, "'");
   //printf("%s%c%s\n", "first char is: '", cmdNospace[0], "'");
   pid_t pid = fork();
   int check_exec;
   if (pid == 0) {
      check_exec = execvp(command, commandArr);
      exit (-1);
   }
   
   wait(NULL);
   if(check_exec == -1){
      printf("%s\n", "Error: Invalid Command");
   }
   return check_exec;
}

int exec_noslash(char *command, char **commandArr) {
   //have to make sure path enviornment = usr/bin
   char pathName[100];
   strcpy(pathName, "/usr/bin/");
   strcat(pathName,command);
   pathName[strlen(pathName)] = '\0';
   //printf("%s%s%s\n", "path to search for: '", pathName, "'");
   // if in the child process, execute the following command
   //int send = my_system(pathName, commandArr);
   
   pid_t processpid = fork();
   int check_exec;
   if (processpid == 0) {
      //printf("%s\n", "success fork!");
      check_exec = execvp(command, commandArr);
      exit(-1);
   }

   wait(NULL);
   //printf("%s%d\n", "check_exec = ", check_exec);
   if(check_exec == -1){
      printf("%s\n", "Error: Invalid Command");
   }
   return check_exec;
}


//BUILT IN COMMAND: CD change directory
int changedir(char *newdirectory){
   //will return int: 0 if successful, -1 if directory was unchanged, otherwise errno tells us error type
   return chdir(newdirectory);
}


//executing input redirection

int exec_inputredir(char * firstcmd, char *fileName, char ** c_arr){

   //char f_Name[100];
   //strcpy(f_Name, "\"");
   //strcat(f_Name,fileName);
   // strcat(f_Name, "\"");
   //printf("%s%s\n", "filename: ",fileName);
   //printf("%s%s\n", "first command: ",firstcmd);
   /*
   int k;
   for(k =0; c_arr[k] != NULL; k++){
      printf("%s%s\n", "argument: ", c_arr[k]);
   }
   */
   // argument = everything before the <  
   if(fork() == 0){
      //in child
      int fdIn = open(fileName, O_RDONLY);
      if(fdIn == -1){
         printf("%s\n", "Error: Invalid File");
         exit(-1);
      }
      dup2(fdIn,0);
      close(fdIn);
      execvp(firstcmd, c_arr);
      perror("Error: Invalid File");
      exit (-1);
      
   }
   wait(NULL);
   return 0;

}



int inputRedirection(char * user_command, int numRedirections, char **cArr){
   //shell only supports at maximum 1 input redirection
   //Input redirection is achieved by a < symbol followed by a file name
  // verify numRedirections
   
   
  // -------------copying user command --------------------
  char *copyCommand;
  copyCommand = (char*)malloc(strlen(user_command) * sizeof(char));
  strcpy(copyCommand, user_command);
  //printf("%s%s%s\n", "cpy usercommand = '",copyCommand, "'");
  //-------------Done copying user command --------------------

   //-------------Verify Num of Redirections (can only have 1) --------------------
   // cat < input.txt
   //cat < input.txt > output.txt
  // ---Getting all chars before input redirection '<'-----
   char *savingptr;
   char *tokens_inputredir;
   // if you have cat -l < ... tokens_input.. has 'cat -l'
   tokens_inputredir = strtok_r(user_command, "<", &savingptr);
   //want to parse this command into tokens and store in redirectionCommand
   int x = 0;
   //printf("%s%s%s\n", "first token = '",tokens_inputredir, "'");

   //---------------storing command tokens---------------
   char ** redirectionCommandArr = malloc(1000 * sizeof(char)); 
   
   while(strcmp(cArr[x], "<") != 0){
      // add to comman arr
      char * thisarg = cArr[x];
      //thisarg[strlen(cArr[x])] = '\0';
      redirectionCommandArr[x] = thisarg;
      //make sure it is null terminated 
      //printf("%s%c%s\n", "first char: '", thisarg[0], "'");
      //printf("%s%c%s\n", "last char: '", thisarg[strlen(cArr[x])], "'");  
      x++;
   }
   
   //make array null terminated
   redirectionCommandArr[x] = NULL;
   // checking that correct values were stored
   /*
   int j;
   for(j = 0; j < x; j++){
      printf("%s%s\n", "check this arg: ", redirectionCommandArr[j]);
   }
   */       
      
   //--------------------Execute Input Redirection---------------------------
  
   //get filename by updating tokens position, check if null
   tokens_inputredir = strtok_r(NULL, " ", &savingptr);
   if(tokens_inputredir == NULL){
      //printf("%s\n", "null file");
      return -1;
   }
   /*
   else{
      printf("%s%s%s\n", "filename = '", tokens_inputredir, "'");
   }
   */

   char *inputredFile = removespace(tokens_inputredir);

   // have all nec information
   //printf("%s%s%s\n", "file = '", inputredFile, "'");
   //printf("%s%s%s\n", "first cmd = '", redirectionCommandArr[0], "'");
   int exec_input = exec_inputredir(redirectionCommandArr[0], inputredFile, redirectionCommandArr);
   //printf("%s%d%s\n", "Num returned: ", exec_input, "'");
   //return exec_input;
   return exec_input;
}



int exec_outputredir_overwrite(char * firstcmd, char *fileName, char ** c_Arr){
   //char file_name[100];
   //strcpy(file_name, "\"");
   //strcat(file_name,fileName);
   //strcat(file_name, "\"");
   
   //printf("%s%s\n", "filename: ",fileName);
   //printf("%s%s\n", "first command: ",firstcmd);
   /*
   int k;
   for(k =0; c_Arr[k] != NULL; k++){
      printf("%s%s\n", "executing overwrite array argument: ", c_Arr[k]);
   }
   */
  
   if(fork() == 0){
      //in child
      int fdOut = open(fileName,  O_CREAT|O_WRONLY| O_TRUNC, S_IRUSR|S_IWUSR);
      if(fdOut == -1){
         printf("%s\n", "Error: Invalid File");
         exit(-1);
      }
      dup2(fdOut,1);
      close(fdOut);
      execvp(firstcmd, c_Arr);
      perror("Error: Invalid File");
      exit (-1);
      
   }
   wait(NULL);
   return 0;
}


int outputRedirection_overwrite(char * user_command, int numRedirections, char **cArr){
  
  // ---Getting all chars before output redirection '>'-----
   char *savingptr;
   char *tokens_outputredir;
   // // outputredir overwrite only > 
   tokens_outputredir = strtok_r(user_command, ">", &savingptr);
   //want to parse this command into tokens and store in redirectionCommand
   int j = 0;
   //printf("%s%s%s\n", "first token = '",tokens_outputredir, "'");
   //---------------storing command tokens---------------
   char ** redirectionCommandArr = malloc(1000 * sizeof(char)); 
   //cat < input.txt > output.txt
   while(strcmp(cArr[j], ">") != 0){
      // add to comman arr
      char * thisarg = cArr[j];
      //thisarg[strlen(cArr[j])] = '\0';
      redirectionCommandArr[j] = thisarg;
      //make sure it is null terminated 
      //printf("%s%c%s\n", "first char: '", thisarg[0], "'");
      //printf("%s%c%s\n", "last char: '", thisarg[strlen(cArr[j])], "'");  
      j++;
   }
   
   //make array null terminated
   redirectionCommandArr[j] = NULL;
   //get filename by updating tokens position, check if null
   tokens_outputredir = strtok_r(NULL, " ", &savingptr);
   if(tokens_outputredir == NULL){
      //printf("%s\n", "null file");
      return -1;
   } 

   
   char *outputredFile = removespace(tokens_outputredir);


   // have all nec information
   //printf("%s%s%s\n", "output filename = '", outputredFile, "'");
   //printf("%s%s%s\n", "first cmd = '", redirectionCommandArr[0], "'");
   int exec_output = exec_outputredir_overwrite(redirectionCommandArr[0], outputredFile, redirectionCommandArr);
   //printf("%s%d%s\n", "Num returned: ", exec_output, "'");
   return exec_output;
    //return 0;
}


// take off quotes
int exec_outputredir_append(char * firstcmd, char *fileName, char ** c_array){
   /*
   char file_name[100];
   strcpy(file_name, "\"");
   strcat(file_name,fileName);
   strcat(file_name, "\"");
   */
  
   //printf("%s%s\n", "filename: ",fileName);
   //printf("%s%s\n", "first command: ",firstcmd);
   /*
   int k;
   for(k =0; c_array[k] != NULL; k++){
      printf("%s%s\n", "exec output c_array: ", c_array[k]);
   }
   */
  
   if(fork() == 0){
      //in child
      int fdout = open(fileName, O_CREAT|O_WRONLY| O_APPEND, S_IRUSR|S_IWUSR);
      if(fdout == -1){
         printf("%s\n", "Error: Invalid File ");
         exit(-1);
      }
      dup2(fdout,1);
      close(fdout);
      execvp(firstcmd, c_array);
      perror("Error: Invalid File");
      exit (-1);
      
   }
   return 0;
}



int outputRedirection_append(char * user_command, int numRedirections, char **cArr){
  
  // ---Getting all chars before input redirection '>'-----
   char *savingptr;
   char *tokens_output_redir;
   // // outputredir overwrite only > 
   tokens_output_redir = strtok_r(user_command, ">>", &savingptr);
   //want to parse this command into tokens and store in redirectionCommand
   int j = 0;
   //printf("%s%s%s\n", "first token = '", tokens_output_redir, "'");
   

   //---------------storing command tokens---------------
   char ** redirectionCommandArr = malloc(1000 * sizeof(char)); 
   
   while(strcmp(cArr[j], ">>") != 0){
      // add to comman arr
      char * thisarg = cArr[j];
      //thisarg[strlen(cArr[j])] = '\0';
      redirectionCommandArr[j] = thisarg;
      //make sure it is null terminated 
      //printf("%s%s%s\n", "this arg: '", thisarg, "'");
      //printf("%s%c%s\n", "last char: '", thisarg[strlen(cArr[j])], "'");  
      j++;
   }
   
   //printf("%s%s%s\n", "next arg: '", cArr[j], "'");
   //printf("%s%s%s\n", "first token again = '",tokens_output_redir, "'");


   //make array null terminated
   redirectionCommandArr[j] = NULL;
   //get filename by updating tokens position, check if null
   tokens_output_redir = strtok_r(NULL, " ", &savingptr);
   tokens_output_redir = strtok_r(NULL, ">", &savingptr);
   //printf("%s%s%s\n", "NEXT TKN after two updates = '",tokens_output_redir, "'");


   if(tokens_output_redir == NULL){
      //printf("%s\n", "null file");
      return -1;
   }
   /*
   else{
      printf("%s%s%s\n", "filename = '", tokens_outputredir, "'");
   }
   */
   char *outputredfile = removespace(tokens_output_redir);

   // have all nec information
   //printf("%s%s%s\n", "file = '", outputredfile, "'");
   //printf("%s%s%s\n", "first cmd = '", redirectionCommandArr[0], "'");
   int exec_outputNum = exec_outputredir_append(redirectionCommandArr[0], outputredfile, redirectionCommandArr);
   //printf("%s%d%s\n", "Num returned: ", exec_output, "'");
   return exec_outputNum;
   //return 0;
}
int bothredirections(char * wholestr, char ** parsedcommandArray, char ** noredirectionArray, int commandCount){
   int k;
   int flagIn = 0;
   int flagOut = 0;
   //printf("%s\n", "Made it in the function");
   //in child  
   pid_t pid = fork();
   // argument = everything before the <  
   if(pid == 0){
      //printf("%s\n", "Made it to child process");

      for(k =0; k < commandCount; k++) {
         char * currentcommand = parsedcommandArray[k];
         if( strcmp(currentcommand, "<") == 0 ){ 

            if(flagIn == 1){
               printf("%s\n", "Error: Invalid Command");
               return -1;
            }
            if(k == 0 || k == commandCount -1){
               printf("%s\n", "Error: Invalid Command");
               return -1;
            }
            flagIn = 1;
            //do the following
            //next is file name
            int fdIn = open(parsedcommandArray[k+1], O_RDONLY);
            if(fdIn == -1){
               printf("%s\n", "Error: Invalid File");
               exit(-1);
            }
            dup2(fdIn,0);
            close(fdIn);
      } //end <
         if( strcmp(currentcommand, ">") == 0 ){
            // do the following
            if(flagOut == 1){
               printf("%s\n", "Error: Invalid Command");
               return -1;
               }
               if(k == 0 || k == commandCount -1){
                  printf("%s\n", "Error: Invalid Command");
                  return -1;
               }

               flagOut = 1;
               //do the following
               //next is file name
               int fdOut = open(parsedcommandArray[k+1],  O_CREAT|O_WRONLY| O_TRUNC, S_IRUSR|S_IWUSR);
               if(fdOut == -1){
                  printf("%s\n", "Error: Invalid File");
                  exit(-1);
               }
               dup2(fdOut,1);
               close(fdOut);
        
              
            } //end >
         if( strcmp(currentcommand, ">>") == 0 ){
            //do the following
            if(flagOut == 1){
               printf("%s\n", "Error: Invalid Command");
               return -1;
            }
            if(k == 0 || k == commandCount -1){
               printf("%s\n", "Error: Invalid Command");
               return -1;
            }
            flagOut = 1;
            //do the following
            //next is file name
            int fdOut = open(parsedcommandArray[k+1], O_CREAT|O_WRONLY| O_APPEND, S_IRUSR|S_IWUSR);
            if(fdOut == -1){
               printf("%s\n", "Error: Invalid File");
               exit(-1);
            }
            dup2(fdOut,1);
            close(fdOut);

         } //end >>
   } //end for loop
   
   //printf("%s\n", "Before Execution!");
   
   execvp(noredirectionArray[0], noredirectionArray);
   perror("Error: Invalid File");
   exit (-1);   
 
   } // end child process
   //wait(NULL);
   waitpid(pid,0,0);
   return 0;

}


int main(int argc, char  **argv) {

   // ----------------Signal Handling-----------------
   //SIGINT, SIGQUIT, and SIGTSTP
   
   signal(SIGINT, signalHandle);
   signal(SIGQUIT, signalHandle);
   signal(SIGTSTP, signalHandle);
   while(1) {

      // -----------------Reading User Input -----------------
      //string ptr is set to NULL to say no memory has been allocated
      char *userCommand = NULL;
      size_t size = 0;
      ssize_t numCharsRead;
      
      //print prompt
      shellworkingdir();
      // using get line to get the user command
      numCharsRead = getline(&userCommand, &size, stdin);
      // getline also saves the delimeter so set to NULL instead of new line
      userCommand[numCharsRead-1] = '\0';
      //copy so that we have an unchanged user input
      char *wholeUserCommand;
      wholeUserCommand = (char*)malloc(numCharsRead);
      strcpy(wholeUserCommand, userCommand);
      //printf("%s%s%s\n", "user input: '", wholeUserCommand, "'");

      //flush immediately after u print smthg that u want to continuously print without a buffer
      // -----------------End Section -----------------


      // -----------------Tokenizing User Input -----------------
      //Section: Splitting User input / tokenizing user input
      //split user input into tokens, can assume delimiter will be spaces
      char *token;
      char *saveptr;
      int cmdCount = 0;
      // we can check here if there are pipes, I/O redirection
      int pipeCount = 0;
      
      // > followed by filename : overwrite
      int outputRedir_overwrite = 0;
      // >> followed by filename : append
      int outputRedir_append = 0;   
      // < followed by filename
      int inputRedirCount = 0;
      //array to store all commands
      char ** commandArr = malloc(1000 * sizeof(char));
      //strtok_r is reentrant function
      token = strtok_r(userCommand, " ", &saveptr);
      
      //char *lastTkn;
      //iterate to access all tokens, can get last token if nec
      while (token != NULL){
         //update here to avoid getting null value
         //lastTkn= token;
       
        // check for pipe and F/O Redirection
        
         if(strcmp(token, "|") == 0){
            pipeCount++;
         }else if (strcmp(token, "<") == 0)
         {
            inputRedirCount++;
         }else if (strcmp(token, ">") == 0 )
         {
            outputRedir_overwrite++;
         }else if (strcmp(token, ">>") == 0)
         {
            outputRedir_append++;
         }else if (strcmp(token, "<<") == 0)
         {
            break;

         }
         
         //store token in array   
         commandArr[cmdCount] = token;
         cmdCount++;
         //update token position
         token = strtok_r(NULL, " ", &saveptr);
      } //end while
      
      //last token equal to null
      commandArr[cmdCount] = NULL;

      char * commandName = commandArr[0];

      //---------take care of possible error-----------
      /*
      if(strcmp(token, "<<") == 0){
         printf("%s\n", "Error: Invalid Command");
         continue;
      }
      */


      // check for both input and output redir
      int bothInputOutput = inputRedirCount + outputRedir_overwrite + outputRedir_append;
      int check_io_pipe = inputRedirCount + outputRedir_overwrite + outputRedir_append + pipeCount;
      
      /*
      //summary 
      if(bothInputOutput > 0){
         printf("%s%d", "Input + Out Redirections: ", bothInputOutput);
         printf("\n");

      } 
      printf("%s%d", "Num of Pipes: ", pipeCount);
      printf("\n");
      
      printf("%s%d", "Num of Input Redir(<): ", inputRedirCount);
      printf("\n");
      printf("%s%d", "Num of Output Redir(>): ", outputRedir_overwrite);
      printf("\n");
      printf("%s%d", "Num of Output Redir(>>): ", outputRedir_append);
      printf("\n");
      */
   

      //printf("%s%s%s\n", "command = '", commandName, "'");
         
      //finally, print out Last token
      //printf("%s%d", "Token count: ", cmdCount);
      //printf("\n");
      //printf("%s", "Iterating through token array");
      //printf("\n");
      /*
      int i;
      for(i = 0; i<cmdCount; i++){
         printf("%s\n", commandArr[i]);
      }
      */
         
      // -----------------End Section -----------------
      // -----------------Built In Commands -----------------
      
      /*
      KEY
      userCommand = whole string that user input
      commandArr = parsed tokens
      commandName = commandArr[0] 
      cmdCount = how many tokens were parsed
      
      Arguments
      // > followed by filename : overwrite
      int outputRedir_overwrite = 0;
      // >> followed by filename : append
      int outputRedir_append = 0;   
      // < followed by filename
      int inputRedirCount = 0;
      */


       
      
      //Built in Commands: cd, exit
      // When the user issues a command, the shell should first check if it is a built-in command.
      // If so, it should not be executed like other programs.
      // Built in commands CANNOT be I/O redirected or piped.

      //Base command is first so we can check the first arg in our arry using strcmp
      // if strcmp returns 0, then the strings are equal

      //printf("%s%s%s\n", "this is what the first arg[0] is: '", commandArr[0], "'");

      //built in function: change directory

      if (strcmp(commandName, "cd") == 0) {
         //printf("%s\n", "change dir command");
         // built in cmds cannot be redirected or piped
         if(check_io_pipe > 0){
            printf("%s\n", "Error: Invalid Command");
            continue;
         }
         // check valid command count
         if(cmdCount == 1 || cmdCount > 2){
            //attempted to call cd with  0 or 2+ arguments
            printf("%s\n", "Error: Invalid Command");
            continue;
         }
   
         //new directory can be absolute or relative path, chdir() handles both
         int checkcd = changedir(commandArr[1]);
         //printf("%s%s\n", "New Directory: ", commandArr[1]);
         //check error
         if (checkcd != 0){
            //check if this even works here
            //check command count to see if 0 or 2+ cmds were sent
            if(cmdCount == 1 || cmdCount > 2){
               //attempted to call cd with  0 or 2+ arguments
               printf("%s\n", "Error: Invalid Command");
               continue;
            }else{
               //-1 if directory was unchanged
               if(checkcd != -1){
                  printf("%s\n", "Error: Invalid Command");
                  continue;
               } else if(checkcd == -1){
                  // check 
                  printf("%s\n", "Error: Invalid Directory");
                  continue;             
               }
            
            }
         } //end if
         // it returned 0, so continue
         continue;
          // check if it worked
         //printf("%s\n", "new wd");
         //shellworkingdir();
         //printf("\n");
      } // end cd 




      
  

   
      //EXIT SHELL: HAVE TO UPDATE with jobs

      //Note that if the STDIN of your shell is closed
      // (e.g., by pressing Ctrl-D at the prompt), your shell should terminate 
      //regardless of whether there are suspended job
      
      // check if user input was exit 
      if (strcmp(userCommand, "exit") == 0) {

         // built in cmds cant be redirected or piped
         if(check_io_pipe > 0){
            printf("%s\n", "Error: Invalid Command");
            continue;
         }
   

         //it should print the following error message to STDERR and prompt for the next command.
         //Error: there are suspended jobs

         //ELSE, terminate
         //printf("%s\n", "Leaving shell, goodbye!");
         free(userCommand);
         free(commandArr);
         exit (0); // change to exit once we incorp while loop
      } //end if exit
      

      // -----------------End Section -----------------
      
      
      
   
      //Section: Interpreting the Command: External Programs

      //shell searches for the executable program at some directory
      //searches for executable files in search path, $path in environmental vars
      //execvp : put first parsed tkn as the first argument, followed by whole command
      
      // so commandName can be an absolute path, relative path, or contain the BASENAME

      
     
    // printf("%s%s\n", "command is: ", commandName);



    //-------------taking care of some possible errors---------------
   if (strcmp(commandName, "cat") == 0){
      if(cmdCount == 1){
         printf("%s\n", "Error: Invalid Command");
         continue;
      } 
      
   }
   if (strcmp(commandName, "echo") == 0){
      if(cmdCount == 1){
         printf("%s\n", "Error: Invalid Command");
         continue;
      }
   }
   

   int k;
   int slashCount = 0;
   //is absolute path
   int isAbsolu = 0;
   // check path type
   for(k =0; k <strlen(commandName); k++) {
   int c = commandName[k];
   if( c == 47){ //slah num is 47
      slashCount++;
      //printf("%s%c\n", "slash found: ", c);
      if(k == 0){
         isAbsolu++;
      }
   }
   } //end for loop

   //-----------------Base Name-------------------
   // Base Name: shell must search for program under /usr/bin
   // If that fails, it is an error.   search path of /usr/bin applies to programs only. 
   //In this case, your shell should NOT search the current working directory. 

   //In any case, if the program cannot be located, 
   //your shell should print the following error message to STDERR and prompt for the next command.
   //Error: invalid program

    // printf("%s%d\n", "Total Slash Count: ", slashCount);
    // printf("%s%d\n", "Is Absolute: ", isAbsolu);

   
   
   //Runs a program with arguments, such as ls -l and runs ls
   //search path of /usr/bin applies to programs only. 
   /*
      KEY
      userCommand = whole string that user input
      commandArr = parsed tokens
      commandName = commandArr[0] 
      cmdCount = how many tokens were parsed
      
      Arguments
      // > followed by filename : overwrite
      int outputRedir_overwrite = 0;
      // >> followed by filename : append
      int outputRedir_append = 0;   
      // < followed by filename
      int inputRedirCount = 0;
    */
   if(slashCount==0 && pipeCount == 0 && outputRedir_overwrite == 0 && outputRedir_append == 0 && inputRedirCount == 0 && isAbsolu==0){
      //base name
      //printf("%s\n", "base name");
      int exec_check = exec_noslash(commandName, commandArr);
      //check if there was an error
      //printf("%s%d\n", "the int returned from my_sys: ", exec_check);
      //execvp will return -1 if unsuccessful
      if(exec_check == -1){
         printf("%s\n", "Error: Invalid Command");
      }
      continue;
   }else if (isAbsolu > 0 && pipeCount == 0  && outputRedir_overwrite == 0 && outputRedir_append == 0 && inputRedirCount == 0){        
      //-----------------Absolute Path-------------------
      //Absolute path: begins with a slash, shell must run the program at that location
      // if it includes a slash, PATH is ignored and the file at the specficied pathname is executed

      int exec_absoluteCheck = my_system(commandName, commandArr);
      //check if there was an error
      //printf("%s%d\n", "the int returned from my_sys: ", exec_absoluteCheck);
      // execvp will return -1 if unsuccessful
      if(exec_absoluteCheck == -1){
         printf("%s\n", "Error: Invalid Command");
   }
   continue;
   }else if (slashCount > 0 && pipeCount == 0  && outputRedir_overwrite == 0 && outputRedir_append == 0 && inputRedirCount == 0){
      //-----------------Relative Path-------------------
      //Relative Path: contains, but does not begin with slash:  
      //shell should locate the program by following the path from the current working directory. 

      int exec_relativeCheck = my_system(commandName, commandArr);
      //check if there was an error
      //printf("%s%d\n", "the int returned from my_sys: ", exec_relativeCheck);
      // execvp will return -1 if unsuccessful
      if(exec_relativeCheck == -1){
         printf("%s\n", "Error: Invalid Command");
   }
   continue;
   }

   // ------------------ Input and Output Redirection ------------------------------------------
   // <, >,>> are present
   /*
      Arguments
   // > followed by filename : overwrite
   int outputRedir_overwrite = 0;
   // >> followed by filename : append
   int outputRedir_append = 0;   
   // < followed by filename
   int inputRedirCount = 0;
   */

   //int bothInputOutput = inputRedirCount + outputRedir_overwrite + outputRedir_append;
   int checkdoubleoutput = outputRedir_append + outputRedir_overwrite;
   if(checkdoubleoutput > 1){
      printf("%s\n", "Error: Invalid Command");
      continue;
   }
   if(inputRedirCount > 0 && outputRedir_append == 0 && outputRedir_overwrite == 0 && pipeCount == 0 ){

      //-----------------input redirection only---------------------
      if(inputRedirCount > 1){
         printf("%s\n", "Error: Invalid Command");
         continue;
         }if(cmdCount < 3){
         printf("%s\n", "Error: Invalid Command");
         continue;
         }
         
      int inputred = inputRedirection(wholeUserCommand, inputRedirCount, commandArr);
      /*
      if(inputred != 0){
         fprintf(stderr, "%s%s\n", "Error: ", strerror(errno));
      }
      */
   } else if (outputRedir_append > 0 && outputRedir_overwrite == 0 && inputRedirCount == 0 && pipeCount == 0 )
   {

      // outputredir only >> 
      //printf("%s\n", "---output redirection append ---");
     // printf("%s%d\n", ">> count: ", outputRedir_append);
      if(outputRedir_append > 1){
         printf("%s\n", "Error: Invalid Command");
         continue;
         }if(cmdCount < 3){
         printf("%s\n", "Error: Invalid Command");
         continue;
         }
         
         int outputappend = outputRedirection_append(wholeUserCommand, outputRedir_append, commandArr);
         //printf("%s%d\n", "Num returned: ", outputappend);

   }else if (outputRedir_overwrite > 0 && outputRedir_append == 0 && inputRedirCount == 0 && pipeCount == 0 )
   {
      // outputredir overwrite only > 
      //printf("%s%s%s\n", "output redir overwrite: '", wholeUserCommand, "'");
      if(outputRedir_overwrite > 1){
         printf("%s\n", "Error: Invalid Command");
         continue;
         }if(cmdCount < 3){
         printf("%s\n", "Error: Invalid Command");
         continue;
         }


      int output_overwrite = outputRedirection_overwrite(wholeUserCommand, outputRedir_overwrite, commandArr);

      /*
      if(output_overwrite != 0){
         fprintf(stderr, "%s%s\n", "Error: ", strerror(errno));
      }
      */

   } else if (bothInputOutput > 0 && pipeCount == 0 ){

      //printf("%s\n", "--redirc site--");


      int caseOne = outputRedir_overwrite + inputRedirCount;
      int caseTwo = inputRedirCount + outputRedir_append;

      if(caseOne > 1 && caseTwo > 1){
         printf("%s\n", "Error: Invalid Command");
         continue;
      }

      if(bothInputOutput > 2){ //what if they pass 3 of one
         printf("%s\n", "Error: Invalid Command");
         continue;
      }
      if(cmdCount < 5){
            printf("%s\n", "Error: Invalid Command");
            continue;
      }

      
      //printf("%s\n", "...Welcome to output and input redir....");

      // output append and input redirection
      //cat >> output.txt < input.txt
      // -------------copying user command --------------------
      char *copyingcmd;
      copyingcmd = (char*)malloc(strlen(wholeUserCommand) * sizeof(char));
      strcpy(copyingcmd, wholeUserCommand);
      //printf("%s%s%s\n", "cpy usercommand = '",copyingcmd, "'");
      //-------------Done copying user command -------------------- 
      char **noredirectionsArray = malloc(1000 * sizeof(char));
      char *cpyFirst;
      cpyFirst = (char*)malloc(strlen(wholeUserCommand) * sizeof(char));

      
      //int h;
      //int cnt = 0;
      noredirectionsArray[0] = commandArr[0];
      noredirectionsArray[1] = NULL;

      /*
      for(h = 0; h < cmdCount; h++){
         if(strcmp(commandArr[h], "<") == 0){
            //skip
            continue;
         }
         else if (strcmp(commandArr[h], ">") == 0)
         {
            continue; 
         }
         else if (strcmp(commandArr[h], ">>") == 0 )
         {
            continue;       
         }
         //store argument in array   
         noredirectionsArray[cnt] = commandArr[h];
         //printf("%s%s%s\n", "arg val = '", noredirectionsArray[cnt], "'");
         cnt ++;
      } //end for loop
      */

      
      //printf("%s\n", "made it out of the loop");
      //bothredirections(str wholestr, char ** parsedcommandArray, char ** noredirectionArray, int commandCount)
      int goredirect = bothredirections(copyingcmd, commandArr, noredirectionsArray, cmdCount);
      
   } //end of this case
    
      
   } // end while
   return 0;
}

    
