#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REQUEST  100
#define PIVOT    200
#define LARGE    300
#define SMALL    400
#define READY    500
#define DONE     600
#define TRUE       1
#define FALSE      0

void main(){
	
	//variables
	int ptc[5][2];
    int ctp[5][2];
	int childID;
	int childReady = 0;
	int receivedReady = 0;
	int randomArrayParent;
	int randomArrayChild;
	int parentReady = 0;
	int isReady = TRUE;
	
	int codeFromParent;
	int codeReceivedByChild;
	int codeFromChild;
	int pivotCode;
	int cmdCode;
	int isAlg = TRUE;
	int count = 0;
	
	//file I/O
	int numbers[5][5] = {0};
	FILE *fp;
	char ch;
    for(int i = 0; i < 6; i++){
        pipe(ptc[i]);
        pipe(ctp[i]);   
    }
	//saving the integers of the txt file
	for(int k = 0; k < 5; k++){
		char* filename;	
		
		//getting file names
		if(k == 0) {
			filename = "input_1.txt";
		}
		if(k == 1) {
			filename = "input_2.txt";
		}
		if(k == 2) {
			filename = "input_3.txt";
		}
		if(k == 3) {
			filename = "input_4.txt";
		}
		if(k == 4) {
			filename = "input_5.txt";
		}
		char dest[20];
		strcpy(dest, filename);
		
		//opening file in read mode
		fp = fopen(dest, "r");
		if(fp == NULL) {
			return;
		}
		else {
		}
		//saving the data to a 2 dimensional array
		for(int j = 0; j < 5; j++) {
			fscanf(fp,"%d\t", &numbers[k][j]);
		}
		if (!fclose(fp)) {
			childReady = READY;
		}
	}
	
	//for loop creating 5 parent child pairs
	for(int k = 0; k < 5; k++){
		pivotCode = 0;
		childID = k + 1;
		if(fork() == 0){
			//reading ids from parent
			int tempChildID;
			close(ptc[k][1]);
			read(ptc[k][0], &tempChildID, sizeof(tempChildID));
			
			//returning ready to parent
			if(tempChildID == childID && childReady){
				printf("child %d returns ready\n", tempChildID);
				close(ctp[k][0]);
				write(ctp[k][1], &childReady, sizeof(childReady));
			}
			//getting ready from parent
			close(ptc[5][1]);
			read(ptc[5][0], &receivedReady, sizeof(receivedReady));
			while(receivedReady == READY){
				printf("in while loop now\n");
				//checking the random number from randomly chosen array
				close(ptc[5][1]);
				read(ptc[5][0], &randomArrayChild, sizeof(randomArrayChild));
				printf("received random array %d \n", randomArrayChild);
				
				if(numbers[randomArrayChild]==NULL){
					codeFromChild = -1;
				}
				else{
					codeFromChild = numbers[randomArrayChild][0];
				}
				//sending the random chosen value from child to parent
				close(ctp[randomArrayChild][0]);
				write(ctp[randomArrayChild][1], &codeFromChild, sizeof(codeFromChild));
				printf("sent value %d from child\n",codeFromChild);
				
				for(int c = 0; c < 5; c++){
					//applying pivot value to everything
					close(ptc[c][1]);
					read(ptc[c][0], &pivotCode, sizeof(pivotCode));
					printf("child %d received pivot number %d from parent\n",c+1, pivotCode);
					count = 0;
					//counting and returning number of elements larger than pivot
					for(int j = 0; j < 5; j++){
						if(pivotCode < numbers[c][j]){
							count++;
						}
					}
					//returning number of elements
					close(ctp[c][0]);
					write(ctp[c][1], &count, sizeof(count));
					printf("Child %d counted %d numbers larger than pivot\n", c+1, count);
				}
				if(cmdCode!=DONE){
					//if code is not done
					for(int c = 0; c < 5; c++){
						//checking commands for what to do
						close(ptc[c][1]);
						read(ptc[c][0], &cmdCode, sizeof(cmdCode));
						printf("child %d received command %d from parent\n",c+1, cmdCode);
					}
					printf("cmdCode == %d", cmdCode);
					if(cmdCode == SMALL){
						//small iteration
						int row, col=0;
						for(int c = 0; c < 5; c++){
							for(int j = 0; j < 5; j++){
								if(pivotCode > numbers[c][j]){
									numbers[c][j]=0;
								}
							}
						}
					}
					else{
						//when command is large
						for(int c = 0; c < 5; c++){
							for(int j = 0; j < 5; j++){
								if(pivotCode < numbers[c][j]){
									numbers[c][j]=0;
								}
							}
						}
					}
					for(int c = 0; c < 5; c++){
						//returning the values to the parent for 2nd iteration
						close(ptc[c][1]);
						read(ptc[c][0], &pivotCode, sizeof(pivotCode));
						printf("child %d received pivot number %d from parent\n",c+1, pivotCode);
						count = 0;
						for(int j = 0; j < 5; j++){
							if(pivotCode < numbers[c][j]){
								count++;
							}
						}
						close(ctp[c][0]);
						write(ctp[c][1], &count, sizeof(count));
						printf("Child %d counted %d numbers larger than pivot\n", c+1, count);
					}
				}
				else{
					//we found the median
					for(int i = 0; i < 5; i++){
						printf("child %d terminated", i+1);
						kill(ctp[i]);
					}
				}
				break;
			}
		exit(0);
		}
		else{
			//writing childrens ids onto children
			close(ptc[k][0]);
			write(ptc[k][1], &childID, sizeof(childID));
		}
	}
	
	//making sure all the chjildren are ready
	int readyCount = 0;
	for(int i = 0; i < 5; i++){
		close(ctp[i][1]);
		read(ctp[i][0], &receivedReady, sizeof(receivedReady));
		if(receivedReady == READY){
			readyCount++;
		}
	}
	if(readyCount == 5){
		//letting children know parent is ready
		printf("parent trying to send ready\n");
		receivedReady = READY;
		printf("parent trying to send ready\n");
		close(ptc[5][0]);
		write(ptc[5][1], &receivedReady, sizeof(receivedReady));
		printf("parent returns ready %d\n",receivedReady);
	}
	//choosing a random array and sending it to the child
	randomArrayParent = printRandoms(0,4,1);
	close(ptc[5][0]);
	write(ptc[5][1], &randomArrayParent, sizeof(randomArrayParent));
	printf("sending random array %d \n", randomArrayParent);
	
	//receiving the value of the random element from random array
	close(ctp[randomArrayParent][1]);
	read(ctp[randomArrayParent][0], &pivotCode, sizeof(pivotCode));
	printf("pivot number %d received from child\n", pivotCode);
	
	//sending the pivot number to all children
	for(int i = 0; i < 5; i++){
		close(ptc[i][0]);
		write(ptc[i][1], &pivotCode, sizeof(pivotCode));
		printf("child %d sent its pivot number from parent %d\n",i+1,pivotCode);
	}
	printf("wrote pivot number %d received from child to all children\n", pivotCode);
	
	int numLarger;
	int mValue=0;
	//getting mValue of every child and adding them
	for(int i = 0; i < 5; i++){
		close(ctp[i][1]);
		read(ctp[i][0], &numLarger, sizeof(numLarger));
		printf("# of values larger than pivot code is %d as received from child %d \n", numLarger, i+1);	
		mValue=mValue+numLarger;
	}

	int rows = sizeof(numbers)/sizeof(numbers[0]);
    int cols = sizeof(numbers[0])/sizeof(numbers[0][0]);
	int kValue = rows*cols/2;
	printf("mValue = %d kValue = %d\n", mValue, kValue);
	
	//checking what command to send to the children
	while(pivotCode != DONE){
		//code is DONE
		if(mValue == kValue){
			pivotCode = DONE;
			for(int i = 0; i < 5; i++){
				close(ptc[i][0]);
				write(ptc[i][1], &pivotCode, sizeof(pivotCode));
				printf("child %d sent command %d\n",i+1,pivotCode);
			}
		}
		//code is SMALL
		else if(mValue > kValue){
			pivotCode = SMALL;
			for(int i = 0; i < 5; i++){
				close(ptc[i][0]);
				write(ptc[i][1], &pivotCode, sizeof(pivotCode));
				printf("child %d sent command %d\n",i+1,pivotCode);
			}
		}
		//code is LARGE
		else if(mValue < kValue){
			pivotCode = LARGE;
			for(int i = 0; i < 5; i++){
				close(ptc[i][0]);
				write(ptc[i][1], &pivotCode, sizeof(pivotCode));
				printf("child %d sent command %d\n",i+1,pivotCode);
				kValue = kValue-mValue;
			}
		}
		//sending commands to children
		for(int i = 0; i < 5; i++){
			for(int i = 0; i < 5; i++){
				close(ctp[i][1]);
				read(ctp[i][0], &numLarger, sizeof(numLarger));
				printf("# of values larger than pivot code is %d as received from child %d \n", numLarger, i+1);	
				mValue=mValue+numLarger;
			}
		}
		printf("mValue = %d", mValue);
	}
}

void printRandoms(int lower, int upper, 
                             int count)
{
    int i;
    for (i = 0; i < count; i++) {
        int num = (rand() %
           (upper - lower + 1)) + lower;
        printf("%d ", num);
    }
}