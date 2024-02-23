//
// Created by wheezeus on 1/31/24.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>


#define BUFFER_SIZE 10
#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{

    int n;

    if (argc == 1) {
        fprintf(stderr,"Usage: ./simpleCProg <starting value>\n");

        return -1;
    }

    n = atoi(argv[1]); //  n is the input value

    printf("\n Input number is %d\n", n);

    return 0;
}