#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char string1[] = "Hello from parent";
char string2[] = "Hi from child";
#define REQUEST = 100;
#define PIVOT = 200;
#define LARGE = 300;
#define SMALL = 400;
#define READY = 500;
#define TRUE  = 1;
#define FALSE = 0;

void main()
{
	char buf[1024];
	int isFirstIter = FALSE;
	int isRunning   = TRUE;
	int isReady     = FALSE;
	int childID;
	int ptc[5][2];
	int ctp[5][2];
	pipe(ptc);
	pipe(ctp);
	
	while(isRunning){
		if(isFirstIter == FALSE){
			//parent process
			if(fork()>0){
				for(int i = 0; i < 5; i++){
					close(ptc[i][0]);
					write(ptc[i][i], &i, sizeof(i));
					print("child %d is ready \n", i);
					close(ctp[i][0]);
					write(ctp[i][i], &i, sizeof(i));
					print("parent read from child child %d is ready \n", i);
				}
				
			}
			//child process
			else{
				for (int i=0; i<5; i++) {
					//write child message into fds2
					close(ctp[i][0]);
					write(ctp[i][1], &i, sizeof(i));
					//read parent message from fds1
					close(ptc[1]);
					read(ptc[0], i, sizeof(i));
					printf("child read %s\n\n", i);
						
				}	
			}
		}
	}
}

/*
parent 
	- allots ids 1 - 5 to each children and sends the same along parent-> child 
	- waits on child-> to receive a ready code
	- instantiates k = n/2 where n = number of elements in the array
	- select random child and queries it for a random element
    - request to a random child, then reads response. if response = -1, repeat. else, continue
	- first non-negative = our pivot element
	- parent subsequently broadcasts the pivot element to all its child processes.
	- reads response from each child, which should be # of elements larger than the pivot
	- sums all responses, into m. 
	- if m=k, then there are n/2 elements larger than the pivot in the data set, so our pivot is the median
	- if m>k, then it sends command small to all its children, which tells the children that it should drop all elements smaller than the pivot element
	- if m<k, then it sends the command large to all its children, which tells the children to drop all elements larger than the pivot
*/

/*
child
The child waits upon the parent?child pipe to receive its id i.
• It then reads an array of 5 integers from its corresponding file (input_i.txt).
• Upon doing so, it sends the code READY along the child?parent pipe.
• It then enters a while loop (broken by a user defined signal - which is sent by the parent to terminate the child process).
• In each iteration it waits on the parent?child pipe to respond according to the codes it gets.
• If it receives the command REQUEST from parent:
¦ If its array is empty, write -1 on the child->parent pipe
¦ Else choose a random element from its array and write it to the child?parent pipe
• If it receives the command PIVOT from parent:
¦ It waits to read another integer (and store it as pivot).
¦ It then writes the number of integers greater than pivot on the child?parent pipe. If it has an empty array, the number would be 0.
• If it receives the command SMALL from parent:
¦ It deletes the elements smaller than the pivot and updates the array.
• If it receives the command LARGE from parent:
¦ It deletes the elements larger than the pivot and updates the array
*/