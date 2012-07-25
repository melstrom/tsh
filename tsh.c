/*
 * File:   main.c
 * Author: melstrom
 *
 * Created on June 8, 2012, 3:53 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //include header for chdir()
#include <sys/resource.h> //include header for getrusage()
#include <string.h> //include header for strtok()
#include <sys/wait.h> //include header for wait()
#include <errno.h>

#define MAX_LINE 80
#define MAX_ARGS 32


long int nivcsw = 0;
long int nvcsw = 0;
long int majflt = 0;
long int minflt = 0;

/*
char * parser (char * input) {
    char * cmd = (char *) malloc(MAX_LINE);
    cmd = input;
    char * strings[MAX_ARGS];// srting after divding each word ot token

    int j;
    for (j = 0; j < MAX_ARGS; j++) {
        strings[j] = (char *) malloc (80);
    }

    int i = 0;
    strings[i] = (strtok (cmd," \t\n"));
    while (strings[i] != NULL)
    {
        i++;
        strings[i] = strtok (NULL," \t\n");
    }
    return *strings;
}
 */

void execute (char * commands[MAX_ARGS]) {
          pid_t pid;
            pid = fork();
            if (pid < 0) {                     /* error occurred */
                fprintf(stderr, "execUS: Fork Failed");
                exit (1);
                }

                else if (pid > 0) {
                    wait(NULL);
                }

                else {
                    execvp (commands[0],commands);
                }
}

void print (struct rusage *usage) {
    /*
    a)	The amount of CPU time used (both user and system time) (in milliseconds),
    b)	The elapsed “wall-clock” time for the command to execute (in milliseconds),
    c)	The number of times the process was preempted involuntarily (e.g. time slice expired,
    preemption by higher priority process, etc.),
    d)	the number of times the process gave up the CPU voluntarily (e.g. waiting for a resource),
    e)	The number of page faults, and
    f)	The number of page faults that could be satisfied from the kernel’s internal cache (e.g. did not
    require any input/output operations).
    */

    /*
    a) timeval ru_utime, timeval ru_stime
    b)
    */

    //code idea source: http://www.unix.com/hp-ux/38937-getrusage.html
    
    nivcsw = (usage->ru_nivcsw) - nivcsw;
    nvcsw = (usage->ru_nvcsw) - nvcsw;
    majflt = (usage->ru_majflt) - majflt;
    minflt = (usage->ru_minflt) - minflt;
    

    printf("\tNumber of times the process was preempted involuntarily: %lu .\n",
            nivcsw);    //c
    printf("\tNumber of times the process gave up the CPU voluntarily: %lu .\n",
            nvcsw);     //d
    printf("\tNumber of page faults: %lu .\n",
            majflt);    //e
    printf("\tNumber of page faults that could be satisfied \n"
            "\tfrom the kernel’s internal cache: %lu .\n",
            minflt);    //f
}

void getstats (void) {
    int who = RUSAGE_SELF;
    //struct timeval tim;
    struct rusage usage;
    getrusage(who, &usage);
    print (&usage);
}

int main (char initialcommand[MAX_LINE]) {
    extern int errno;

    int c;
    int length;

    char inputbuffer[MAX_LINE];// the string the user inputs
    char * cmd = (char *) malloc(MAX_LINE);
    char * strings[MAX_ARGS];

    //memcpy (inputbuffer, args, strlen(args)+1);
    read (initialcommand, inputbuffer,MAX_LINE);
    cmd = inputbuffer;

    int j;
    for (j = 0; j < MAX_ARGS; j++) {
        strings[j] = (char *) malloc (80);
    }

    int i = 0;
    strings[i] = (strtok (cmd," \t\n"));
    while (strings[i] != NULL)
    {
        i++;
        strings[i] = strtok (NULL," \t\n");
    }

    execute(strings);
    getstats();
    
    while (1){

        printf("Enter a command:\n");

        length = read (STDIN_FILENO, inputbuffer,MAX_LINE);
        if (length == 1) {// no text was entered
            //exit(0);
        }

        /*if (length<0)
        {
            exit(-1); // somthing went wrong
        }*/
        else {

            cmd = inputbuffer;
// srting after divding each word ot token

            int j;
            for (j = 0; j < MAX_ARGS; j++) {
                strings[j] = (char *) malloc (80);
            }

            int i = 0;
            strings[i] = (strtok (cmd," \t\n"));
            while (strings[i] != NULL)
            {
                i++;
                strings[i] = strtok (NULL," \t\n");
            }

            if (strcmp(strings[0],"cd") == 0) {
                //If the command is cd, call chdir(): see man page for calling parameters
                //http://linux.die.net/man/3/chdir
                /*char * workdirectory[50];
                getwd(workdirectory);
                strcat(workdirectory,strings[1]);*/
                if (strcmp(strings[1],"..") == 0) {
                    chdir(strings[1]);
                }

                else {
                    char buff[MAX_LINE];
                    getwd(buff);
                    strcat(buff,"/");
                    strcat(buff,strings[1]);
                    chdir(buff);
                }

                //if (errno != 0) {
                    //fprintf(stderr, "Change directory failed.\n");
                //}
            }

            else if (strcmp(strings[0],"exit") == 0) {
                    printf("Exiting tsh\n");
                    exit(-1);
                }

            else {

                execute(strings);
                getstats();
                }

            /*int k;
            for (k = 0; k < MAX_ARGS; k++) {
                free(strings[k]);
            }*/
            //free(cmd);
        }
    }
    exit(-1);
}
/*
void main (void){
    tsh("cat /etc/motd");
}*/
