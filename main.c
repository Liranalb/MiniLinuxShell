#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#define maxLetters 510

pid_t pid; //global process id


void printUserNPath() {    //a method that printing the current username and path
    char cwd[1024];
    char* username = getenv("USER");
    getcwd(cwd, sizeof(cwd));
    printf("\n%s@%s>",username, cwd);
}

void error() {
    printf("ERR\n");
    exit(1); //1 = ERROR TO STATUS
}

void freeArrayInput(char** input)
{
    int i = 0;
    while (input[i])
        free(input[i++]);
    free(input);
}

int main() {

    char str[510];			// creating a new string
    char **tokens;

    while(strcmp(str, "done")) {
        printUserNPath();			// print the path and username
        fgets(str , maxLetters , stdin); // getting a string from the user

        // creating a new proccess
        pid = fork();

        if(pid < 0) {  // error creating the son proccess
            error();
        }

        if(pid == 0) {
            char temp[maxLetters];		// creating a temp array
            strcpy(temp,str);		// copy the current string to the temp string

            int wordCounter = 0;
            char * command = strtok(temp , ". ,;?!"); // add more marks

            while (command != NULL) {
                wordCounter++;
                command = strtok(NULL , " \n");
            }
            //printf("%d", wordCounter);
            wordCounter++; // for the last place to be NULL
            char **arr = (char **) malloc(wordCounter*sizeof(char*));

            if(arr == NULL)
                exit(1);
            command = strtok(str , " \n");

            for (int i = 0; i < wordCounter-1 ; i++) {
                arr[i] = (char*)malloc((strlen(command)+1)*sizeof(char));
                strcpy(arr[i],command);
                command = strtok(NULL , " \n");
                //printf("%s", arr[i]);
            }

            execvp(arr[0], arr);   //WORKSSSSSSSS

            freeArrayInput(arr);


        }
        else {
            wait(NULL);
            continue;
        }
    }
    printf("\nBye !");
    exit(0);




}

/*
 *
 *
 * char *remove_cr(char *string)
{
int i=0;
while(string[i])
{
if (string[i] == '\n')
string[i] = '\0';
i++;
}
return string;
}
*
*
int main() {
	pid_t x;

	char* firstCommand[] = {"ls", "-l", NULL};
	char* secoundCommand[] = {"pwd", NULL};
	x = fork();

	if(x == 0) {
		execvp(firstCommand[0], firstCommand);
		return 0;
	}
	else
	wait(NULL);
	execvp(secoundCommand[0], secoundCommand);
	return 0;
}
	*/


/*

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define maxLetters 510

int main()
{
char str[maxLetters];
printf("please type a line \n");
fgets(str , maxLetters , stdin);
                                         //printf("%c\n" , str[1]);
char temp[maxLetters];
strcpy(temp,str);
                                            //printf("%c\n" , temp[1]);
int count = 0;
char * ptr = strtok(temp , " \n");
while (ptr!=NULL)
 {
	count++;
	ptr = strtok(NULL , " \n");
 }
                                            //printf("%d\n" , count);
count++;   				// for the last place to be NULL
char **arr = (char **) malloc(count*sizeof(char*));
if(arr == NULL)
	exit(1);
ptr = strtok(str , " \n");
int i;
for (i = 0; i<count-1 ; i++)
 {
	arr[i] = (char*)malloc((strlen(ptr)+1)*sizeof(char));
	strcpy(arr[i],ptr);
	ptr = strtok(NULL , " \n");
 }
arr[count-1] = NULL;


return 0;
}

*/

/* #include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main() {
	printf("I am the parent process, creating a new process\n");
	pid_t x;
	x = fork();

	if(x == 0) {
		for(int i = 0; i < 6; i++) {
			printf("I am the child process: Counting %i\n", i);
		}
		return 0;
	}
	else
		wait(NULL);
                printf("The child process with ID <%d> is terminated\n", x);

	return 0;

}

*/



/* #include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>

int main() {
	pid_t x;

	char* firstCommand[] = {"ls", "-l", NULL};
	char* secoundCommand[] = {"pwd", NULL};
	x = fork();

	if(x == 0) {
		execvp(firstCommand[0], firstCommand);
		return 0;
	}
	else
	wait(NULL);
	execvp(secoundCommand[0], secoundCommand);
	return 0;
}


*/
