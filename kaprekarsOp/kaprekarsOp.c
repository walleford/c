//
// Created by wheezeus on 2/22/24.
//
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define BUFFER_SIZE 10
#define READ_END 0
#define WRITE_END 1

// argc is the count of arguments passed (1 min because the filename is 1 argument)
// *argv[] is a ref to the argument array as chars
int main(int argc, char *argv[])
{
    int n;
    if (argc == 1)
    {
        fprintf(stderr, "Usage: ./kaprekarsOp <starting value>\n");

        return -1;
    }

    // atoi = arg to int -> argv[1] == the number passed as an argument
    n = atoi(argv[1]);
    printf("Input number is %d\n", n);

    // log10(123) = 2.001 + 1 == 3 (the length of 123)
    int lengthN = log10(n) + 1;
    printf("%d : lengthN \n", lengthN);
    int i;
    // allocating an array with the size of lengthN * the byte size of ints (4)
    int *numArray = malloc(lengthN * sizeof(int));

    // for i = 0 to i = lengthN, decrement n by n / 10;
    // this will sort the number in reverse order from largest -> smallest
    for (i = 0; i < lengthN; n /= 10)
    {
        // 123 % 10 = 3
        // 12.3 % 10 = 2
        // 1.23 % 10 = 1
        numArray[i] = n % 10;
        i++;
    }

    for (int l = 0; l < lengthN; l++)
    {
        printf("%d ", numArray[l]);
    }
    free(numArray);
}