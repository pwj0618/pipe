#include <stdio.h>
#include <stdlib.h>

char string1[] = "Hello from parent";
char string2[] = "Hi from child";
#define READY = 1;
#define SMALL = 2;
#define LARGE = 3;
#define REQUEST = 4;

void main()
{
	char buf[1024];
	int i, fds1[2], fds2[2];
	pipe(fds1);
	pipe(fds2);
	
	if (fork()>0){//Parent process starts
		//parent code
		for (i=0; i<5; i++) {
			//write parent message into fds1
			close(fds1[0]);
			write(fds1[1], string1, sizeof(string1));
			//read child message from fds2
			close(fds2[1]);
			read(fds2[0],buf,sizeof(string2));
			printf("parent read %s\n", buf);
		}
	}
	else {
		//child code
		for (i=0; i<5; i++) {
			//write child message into fds2
			close(fds2[0]);
			write(fds2[1], string2, sizeof(string2));
			//read parent message from fds1
			close(fds1[1]);
			read(fds1[0], buf, sizeof(string1));
			printf("child read %s\n", buf);
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