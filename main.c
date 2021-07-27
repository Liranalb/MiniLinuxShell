#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include <sys/stat.h>
#include<sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include<pwd.h>

#define MAX_LETTERS 510
#define NO_WAIT_4_SON -20 // for using &
#define STDIN 0  // for the use of redir_handler method
#define STDOUT 1
#define STDERR 2
#define CAT 1
#define NOCAT 0

void printUserPath();
char ** argOrginizer(char str[] , int count);
int argCounter(char str[]);
void cd(char *pth);
void error();
void freeArrayInput(char** arr , int arr_length);

char * Pipe_Ampersand_Handler (char * str , char x);
void sig_handler (int signum);
int redir_handler(char ** redirArr , int redirLength , int add , int target, char * command);

int main() {

    signal(SIGINT , sig_handler);
    char str[MAX_LETTERS];
    char ** arr;
    pid_t pid , pid2;
    int arr_length =0;
    int numOfCommands = 0;
    int lenOfCommands = 0;
    int status = 0;

    char * Special_sign = NULL;		// for "&" and "|"s signs
    int pip[2]; // pip
    int right_length;
    char * ptr_pip = NULL;
    char ** right;

    char * leftArrow = ">";
    char * leftArrowDouble = ">>";
    char * arrow_2Left = "2>";
    char * rightArrow = "<";

    int file_left;
    int file_left_double;
    int file_left_2;
    int file_right;

    while(strcmp(str, "done")) { // keep going till the user types "done"
        printUserPath();			//print the title
        fgets(str , MAX_LETTERS , stdin); //get the command

        Special_sign = Pipe_Ampersand_Handler (str ,'&'); // search for & sign
        if(Special_sign != NULL)
            status = NO_WAIT_4_SON; // change status. Fathers process won't wait for the son's process

        ptr_pip = Pipe_Ampersand_Handler (str ,'|'); // search for | sign
        if(ptr_pip != NULL) {
            pipe(pip);			//create a pip
            right_length = argCounter (ptr_pip);		// count the command
            right = argOrginizer(ptr_pip , right_length); // organize the command in an array
        }

        arr_length = argCounter (str);
        arr = argOrginizer(str , arr_length); // organize the command in an array
        if (arr[0] == NULL)	// in a case of empty command - keep running
            continue;

        pid = fork(); // create a new process

        if(pid == -1) // fork checker
            error();

        if(pid == 0) { 					// son's process
            if(ptr_pip != NULL) {		// close the pipe
                close(pip[0]);
                dup2(pip[1] , STDOUT_FILENO);
                close(pip[1]);
            }

            else {					//check for redirection in this son
                file_left = redir_handler(arr ,arr_length ,NOCAT ,STDOUT ,leftArrow);
                file_left_double = redir_handler(arr ,arr_length ,CAT ,STDOUT ,leftArrowDouble);
                file_left_2 = redir_handler(arr ,arr_length ,CAT ,STDERR ,arrow_2Left);
                file_right = redir_handler(arr ,arr_length ,CAT ,STDIN ,rightArrow);
                if(file_left == -1 || file_left_double == -1 || file_left_2 == -1 || file_right == -1)
                    exit(1);
            }

            if(strcmp(arr[0], "cd") == 0) {
                if(arr[1]==NULL)
                    exit(1);
                exit(2);
            }

            if(execvp(arr[0] , arr) <0) {             //  execv checker
                if(strcmp(arr[0] ,"done") == 0) {
                    exit(1);
                }
                exit(0);
            }

        }
        if(pid>0) { // father's process
            if(ptr_pip != NULL)	{ // check if a pipe is on
                pid2 = fork();	 //create second process son

                if(pid2 == 0) {
                    close(pip[1]);
                    dup2(pip[0] , STDIN_FILENO);
                    close(pip[0]);
                    file_left = redir_handler(right ,right_length ,NOCAT ,STDOUT ,leftArrow);
                    file_left_double = redir_handler(right ,right_length ,CAT ,STDOUT ,leftArrowDouble);
                    file_left_2 = redir_handler(right ,right_length ,CAT ,STDERR ,arrow_2Left);
                    file_right = redir_handler(right ,right_length ,CAT ,STDIN ,rightArrow);
                    if(file_left == -1 || file_left_double == -1 || file_left_2 == -1 || file_right == -1)
                        exit(1);
                    execvp(right[0] , right);
                }

                if(pid2 > 0) {
                    close(pip[0]);		// close the father's pipe
                    close(pip[1]);
                }

            }

            if(status!= NO_WAIT_4_SON) { // there is no waiting
                waitpid(pid , &status , 0);

                if(WEXITSTATUS(status)== 0)	{	//exec passed
                    numOfCommands++;
                    lenOfCommands += strlen(arr[0]);
                }

                if(WEXITSTATUS(status)== 2) {
                    cd(arr[1]);
                }
            }

            else {                   //There is & then don't wait
                numOfCommands++;
                lenOfCommands += strlen(arr[0]);
                status = 0;			//reset the status

            }
            freeArrayInput(arr , arr_length); 				// and clear the memory
        }
    }

    printf("Num of cmd: %d\n" , numOfCommands);
    printf("Cmd length: %d\n" , lenOfCommands);
    printf("Bye !\n");

    return 0;
}

//------------------------------------------------------------------------------

void sig_handler (int signum) {
    if(signum == SIGINT) {
        if(getpid != NULL && getpid ==NULL)
            exit(1);			// done count is
    }
}


int redir_handler(char ** redirArr , int redirLength , int add , int target, char * command) {
    int file;
    if(redirLength < 3) // checking if the whole command is too short
        return 0;
    if(redirArr[redirLength -3] == NULL || redirArr[redirLength -2] == NULL)	// checking if the command is too short
        return 0;

    if (strcmp(redirArr[redirLength -3] , command) == 0) {
        if(add == 0) { // the command is't add
            file = open(redirArr[redirLength -2] , O_RDWR | O_CREAT ,S_IRWXU,S_IRUSR,S_IWUSR,S_IXUSR);  
			//There is not using O_APPEND. the data is overriding the existed data
            if(file == -1) {
                printf("cannot open %s file\n" , file);
                return -1;
            }
        }
        if(add == 1) {
			// Using O_APPEND. The data will be add to the existed data
            file = open(redirArr[redirLength -2] , O_RDWR | O_CREAT | O_APPEND ,S_IRWXU,S_IRUSR,S_IWUSR,S_IXUSR); 
            if(file == -1) {
                printf("cannot open %s file\n" , file);
                return -1;
            }
        }

        if(target == 0) 
            dup2(file , STDIN_FILENO); // make a copy of the file descriptor
        if(target == 1)
            dup2(file , STDOUT_FILENO);
        if(target == 2)
            dup2(file , STDERR_FILENO);

        redirArr[redirLength -3] = NULL;
        redirArr[redirLength -2] = NULL;
        return file;
    }
    return 0;
}

char * Pipe_Ampersand_Handler (char * str , char x) { // find the special char (pip or &)
    char * ptr = strchr(str, x);

    if(ptr != NULL) {
        size_t num_x = strlen(str) - strlen(ptr);
        str[num_x] = '\0';
        ptr++;

    }
    return ptr;
}

void cd(char *pth) {
    char path[MAX_LETTERS];
    path[0] = '\0';
    strcpy(path,pth);
    chdir(path);
}

void printUserPath() { // printing the path and username
    struct passwd *pwd;
    uid_t uid=0;
    pwd=getpwuid(uid);
    char* cwd;
    char buff[510];
    cwd = getcwd( buff, 510);

    printf( "%s@%s>", pwd->pw_name,cwd);
}

int argCounter (char * str) { // count the command in order to allocate memory space for the array
    char temp[MAX_LETTERS];
    strcpy(temp,str);
    int count = 0;
    str = strtok(temp , " \n");
    while (str!=NULL) {
        count++;
        str = strtok(NULL , " \n");
    }

    count++;
    return count;
}

char ** argOrginizer(char* str , int count) { //putting the command and arg into the array
    char **arr = (char **) malloc(count*sizeof(char*));
    if(arr == NULL)
        exit(1);

    str = strtok(str , " \n");
    int i;

    for (i = 0; i<count-1 ; i++) {
        arr[i] = (char*)malloc((strlen(str)+1)*sizeof(char));
        strcpy(arr[i],str);
        str = strtok(NULL , " \n");
    }
    arr[count-1] = NULL;
    return arr;
}

void error() {
    printf("EER\n");
    exit(1);
}

void freeArrayInput(char** arr , int arrLength) {
    for(int i=0 ; i < arrLength ; i++) {
        free(arr[i]);
    }
    free(arr);
}
