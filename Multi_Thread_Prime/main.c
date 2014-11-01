//
//  main.c
//  Multi_Thread_Prime
//
//  Created by Victor Chen on 6/25/14.
//  Copyright (c) 2014 Victor Chen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

typedef struct {
    int id_num;
    char name[10];
} id;

id database[10] = {
    {0, "Miden"},
    {1, "Alpha"},
    {2, "Beta"},
    {3, "Gamma"},
    {4, "Delta"},
    {5, "Epsilon"},
    {6, "Digamma"},
    {7, "Zeta"},
    {8, "Eta"},
    {9, "Theta"},
};

//NOTES: fd[0] is set up for reading, fd[1] is set up for writing
int main(void)
{
    int     fd[2], nbytes_read;
    id greek_name;
    pid_t   childpid;
    char    readbuffer[10];
    printf("Please enter a number for 0-9 for its Greek name: ");
    scanf("%d", &greek_name.id_num);
    
    pipe(fd);
    
    if((childpid = fork()) == -1)
    {
        perror("Unable to fork a child process");
        exit(1);
    }
    
    /* Child Process looks into the database and match the numeric number with its Greekname
       and send it via the pipe to the parent process for output */
    if(childpid == 0)
    {
        /* Child process closes up input side of pipe */
        close(fd[0]);
        
        /* Copy from the database its greek_name */
        strcpy(database[greek_name.id_num].name, greek_name.name);
        
        /* Send "string" through the output side of pipe */
        write(fd[1], greek_name.name, (strlen(greek_name.name)+1));
        exit(0);
    }
    else
    {
        /* Parent process closes up output side of pipe */
        close(fd[1]);
        
        /* Read in a string from the pipe */
        nbytes_read = read(fd[0], readbuffer, sizeof(readbuffer));
        printf("\nThe Greek Name is: %s", readbuffer);
    }
    
    return(0);
}