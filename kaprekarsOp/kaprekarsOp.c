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

/**
 * Concatenates all of the array elements into a single integer to return.
 * @param arr array to concat
 * @param len length of the array
 * @return integer of combined array elements.
 */
int concat(int* arr, size_t len)
{
    int result = 0;

    for (int i = 0; i < len; i++)
    {
        int digits = floor(log10(arr[i])) + 1;
        result *= pow(10, digits);
        result += arr[i];
    }
    return result;
}

/**
 * Creates an array of length: lengthN from the integer n
 * @param n integer to use to create the array
 * @param lengthN length of the integer (123 = lengthN of 3)
 * @return an array with the single digits from the original integer
 */
int *createArray(int n, int lengthN)
{
    // log10(123) = 2.001 + 1 == 3 (the length of 123)
    // allocating an array with the size of lengthN * the byte size of ints (4)
    int *numArray = malloc(lengthN * sizeof(int));
    int i;
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
    return numArray;
}
/**
 * Take in a number, convert it to an array, and sort that array in ascending order
 * @param n an integer value of the number to be sorted
 * @return the smaller version of the original number
 */
int sortAscending(int n, int lengthN)
{
    int *ascendingArr = createArray(n, lengthN);
    // sort the array in ascending order here (smallest to largest)
    for (int i = 0; i < lengthN - 1; i++)
    {
        for (int j = 0; j < lengthN - 1; j++)
        {
            // if the num on left is greater than num on right, swap them.
            if (ascendingArr[j] > ascendingArr[j+1])
            {
                int temp = ascendingArr[j];
                ascendingArr[j] = ascendingArr[j+1];
                ascendingArr[j+1] = temp;
            }
        }
    }
    int ascendingNum = concat(ascendingArr, lengthN);
    free(ascendingArr);
    printf("ascendingNum: %d\n", ascendingNum);
    return ascendingNum;
}

/**
 * Take in a number n, convert it to an array, and sort that array in descending order
 * @param n an integer value of the number to be sorted
 * @return the larger version of the original number
 */
int sortDescending(int n, int lengthN)
{
    int *descendingArr = createArray(n, lengthN);
    // sort the array in descending order here (largest -> smallest)
    for (int i = 0; i < lengthN - 1; i++)
    {
        for (int j = 0; j < lengthN - 1; j++)
        {
            // if the num on left is greater than num on right, swap them.
            if (descendingArr[j] < descendingArr[j+1])
            {
                int temp = descendingArr[j];
                descendingArr[j] = descendingArr[j+1];
                descendingArr[j+1] = temp;
            }
        }
    }
    int descendingNum = concat(descendingArr, lengthN);
    free(descendingArr);
    printf("descendingNum: %d\n", descendingNum);
    return descendingNum;
}

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


    // creating the pipe for communication.
    // unidirectional pipe
    // fd[0] is read-end of pipe, fd[1] is the write end of the pipe.
    int read_msg[BUFFER_SIZE];
    int fd[2];
    if (pipe(fd) == -1)
    {
        fprintf(stderr, "something broke in pipe.");
        return 1;
    }

    // forking the process here. Child will perform calculations, write to buffer, and return to parent.
    int id = fork();
    if (id < 0)
    {
        fprintf(stderr, "Something failed in id:   %d", id);
    }

    if (id == 0)
    { // CHILD PROCESS
        printf("\n CHILD PROCESS ID: %d\n", id);
        int diff = 0;
        int lengthN = log10(n) + 1;
        int finalResult = 495;

        while (diff != finalResult)
        {
            int asc = sortAscending(n, lengthN);
            int des = sortDescending(n, lengthN);
            diff = des - asc;
            printf("diff: %d\n", diff);
            n = diff;
        }
        char result[sizeof(diff) * 4+1];
        sprintf(result, "%d", diff);

        // piping number to parent
        int write_msg[BUFFER_SIZE] = {diff};
        close(fd[READ_END]);
        write(fd[WRITE_END], write_msg, sizeof(diff));
        close(fd[WRITE_END]);
    } else if (id > 0) {
        // PARENT PROCESS
        wait(NULL); // wait for execution to finish in child process
        read(fd[READ_END], read_msg, BUFFER_SIZE);
        printf("\n PARENT PROCESS ID: %d", id);
        printf("\nFinal Result: %d\n", read_msg[0]);

        close(fd[READ_END]);

    }
    return 0;
}
