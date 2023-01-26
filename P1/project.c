#include <stdio.h>
#include <stdlib.h>

#define REQUEST 100;
#define PIVOT 	200;
#define LARGE 	300;
#define SMALL	400;
#define READY 	500;

char string0[] = "Hello, this is the parent process";
char string1[] = "Hi, this is the child 1";
char string2[] = "Hi, this is the child 2";
char string3[] = "Hi, this is the child 3";
char string4[] = "Hi, this is the child 4";
char string5[] = "Hi, this is the child 5";
void main() {
	char buf[1024];
	int fds0[2], fds1[2], fds2[2], fds3[2], fds4[2], fds5[2];
	pipe(fds0); //pipe used by the parent process
	pipe(fds1); //pipe used by the child 1 process
	pipe(fds2);
	pipe(fds3);
	pipe(fds4);
	pipe(fds5);
	
	//The first child process is created
	if (fork() == 0) {
		close(fds0[1]);
		//read from the parent
		read(fds0[0], buf, sizeof(string0));
		printf("child 1 reads: %s\n", buf);
		//write child message to parent via its pipe
		close(fds1[0]);
		write(fds1[1], string1, sizeof(string1)); 
		exit(0);
	}
	//The second child process
	else if (fork() == 0) {
		sleep(1);
		close(fds0[1]);
		//Get something from the parent process
		read(fds0[0], buf, sizeof(string0));
		printf("child 2 reads: %s\n", buf);
		//write child message into fds2
		close(fds2[0]);
		write(fds2[1], string2, sizeof(string2));
		exit(0);
	}
	//3rd
	else if (fork() == 0) {
		sleep(1);
		close(fds0[1]);
		//Get something from the parent process
		read(fds0[0], buf, sizeof(string0));
		printf("child 3 reads: %s\n", buf);
		//write child message into fds2
		close(fds3[0]);
		write(fds3[1], string3, sizeof(string3));
		exit(0);
	}
	//4th
	else if (fork() == 0) {
		sleep(1);
		close(fds0[1]);
		//Get something from the parent process
		read(fds0[0], buf, sizeof(string0));
		printf("child 4 reads: %s\n", buf);
		//write child message into fds2
		close(fds4[0]);
		write(fds4[1], string4, sizeof(string4));
		exit(0);
	}//5th
	else if (fork() == 0) {
		sleep(1);
		close(fds0[1]);
		//Get something from the parent process
		read(fds0[0], buf, sizeof(string0));
		printf("child 5 reads: %s\n", buf);
		//write child message into fds2
		close(fds5[0]);
		write(fds5[1], string5, sizeof(string5));
		exit(0);
	}
	else {//Parent process starts
		//write parent message into fds0
		close(fds0[0]);
		write(fds0[1], string0, sizeof(string0));
		//read child 1 message from its associated pipe
		close(fds1[1]);
		read(fds1[0], buf, sizeof(string1));
		printf("parent reads from Child 1: %s\n", buf);
		
		//write something into fds0 again to child 2
		close(fds0[0]);
		write(fds0[1], string0, sizeof(string0));
		//read child 2 message from its associated pipe
		close(fds2[1]);
		read(fds2[0], buf, sizeof(string2));
		printf("parent reads from Child 2: %s\n", buf);		
		
		//write something into fds0 again to child 3
		close(fds0[0]);
		write(fds0[1], string0, sizeof(string0));
		//read child 2 message from its associated pipe
		close(fds3[1]);
		read(fds3[0], buf, sizeof(string3));
		printf("parent reads from Child 3: %s\n", buf);		
		
		//write something into fds0 again to child 4
		close(fds0[0]);
		write(fds0[1], string0, sizeof(string0));
		//read child 2 message from its associated pipe
		close(fds4[1]);
		read(fds4[0], buf, sizeof(string4));
		printf("parent reads from Child 4: %s\n", buf);		
		
		//write something into fds0 again to child 5
		close(fds0[0]);
		write(fds0[1], string0, sizeof(string0));
		//read child 2 message from its associated pipe
		close(fds5[1]);
		read(fds5[0], buf, sizeof(string5));
		printf("parent reads from Child 5: %s\n", buf);		
		exit(0);
	}
}