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
	int randomArray1 = rand() % 5;
	int randomArray2 = rand() % 5;
	int randomArrayChild1, randomArrayChild2 = 0;
	int parentReady = 0;
	int isReady = 0;
	int countLarger[5]; 
	int codeFromParent;
	int codeReceivedByChild;
	int codeFromChild;
	int pivotVal;
	int cmdCode;
	int isAlg = TRUE;
	int count = 0;
	int iCount = 0;
	int kVal, mVal;
	int nVal = 1;
	
	//file I/O
	int numbers[25] = {0};
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
			int arrayNum = k * 5 + j;
			fscanf(fp,"%d\t", &numbers[arrayNum]);
			nVal++;
		}
		if (!fclose(fp)) {
			childReady = READY;
		}
	}
	
	
	//for loop creating 5 parent child pairs
	for(int k = 0; k < 5; k++){
		iCount++;
		childID = k + 1;
		if(fork() == 0){
			//reading ids from parent
			int tempChildID;
			close(ptc[k][1]);
			read(ptc[k][0], &tempChildID, sizeof(tempChildID));
			
			//returning ready to parent
			if(tempChildID == childID){
				close(ctp[k][0]);
				write(ctp[k][1], &childReady, sizeof(childReady));
				printf("child %d returns ready\n", tempChildID);
			}
			
			if(iCount == 5){
					isReady = READY;
					printf("all children are ready\n");
					close(ctp[5][0]);
					write(ctp[5][1], &isReady, sizeof(isReady));
					
					close(ptc[5][1]);
					read(ptc[5][0], &receivedReady, sizeof(receivedReady));
					printf("received ready code %d from parent\n", receivedReady);
					//getting ready from parent
					while(receivedReady == READY){
						
						close(ptc[5][1]);
						read(ptc[5][0], &cmdCode, sizeof(cmdCode));
						printf("received command %d from parent\n", cmdCode);
						
						if(cmdCode == REQUEST){
							printf("command was request\n");
							
							close(ptc[5][1]);
							read(ptc[5][0], &randomArrayChild1, sizeof(randomArrayChild1));
							printf("received array number %d from parent\n", randomArrayChild1);
							
							close(ptc[randomArrayChild1][1]);
							read(ptc[randomArrayChild1][0], &randomArrayChild2, sizeof(randomArrayChild2));
							pivotVal = numbers[randomArrayChild1*randomArrayChild2+randomArrayChild2];
							printf("received array element %d from parent\n", randomArrayChild2);
							
							close(ctp[5][0]);
							write(ctp[5][1], &pivotVal, sizeof(pivotVal));
							printf("wrote pivot value %d as requested by parent\n", pivotVal);
							
							close(ptc[5][1]);
							read(ptc[5][0], &pivotVal, sizeof(pivotVal));
							printf("child %d received pivot value %d\n", 5, pivotVal);
							for(int i = 0; i < 5;i++){
								countLarger[i]=0;
								int temp = 0;
								for(int j = 0; j < 5; j++){
									int arrayNum = i * j + j;
									if(pivotVal < numbers[arrayNum]){
										temp++;
									}
									countLarger[i]=temp;
								}
								printf("child %d counted %d number of elements larger than pivot value\n",i+1, countLarger[i]);
							}	
							
							close(ctp[5][0]);
							write(ctp[5][1], &countLarger, sizeof(countLarger));
							printf("sending countLarger\n");
						}
						break;
					}
				}
		exit(0);
		}
		else{
			//writing childrens ids onto children
			close(ptc[k][0]);
			write(ptc[k][1], &childID, sizeof(childID));
		}
	}
	close(ctp[5][1]);
	read(ctp[5][0], &receivedReady, sizeof(receivedReady));
	printf("parent received ready value %d\n", receivedReady);
	if(receivedReady == READY){
		printf("parent is ready\n");
		cmdCode = REQUEST;
	}
	close(ptc[5][0]);
	write(ptc[5][1], &receivedReady, sizeof(receivedReady));
	printf("parent sent code %d to children\n", receivedReady);
	
	if(receivedReady == READY){
		cmdCode = REQUEST;
		printf("code to be sent is %d \n", cmdCode);
	}
	close(ptc[5][0]);
	write(ptc[5][1], &cmdCode, sizeof(cmdCode));
	printf("parent sent code %d to children\n", cmdCode);
	
	close(ptc[5][0]);
	close(ptc[5][0]);
	write(ptc[5][1], &randomArray1, sizeof(randomArray1));
	printf("parent random array element %d to child \n", randomArray1);
	
	close(ptc[randomArray1][0]);
	write(ptc[randomArray1][1], &randomArray2, sizeof(randomArray2));
	printf("parent random array element %d to child %d \n", randomArray2, randomArray1);
	
	close(ctp[5][1]);
	read(ctp[5][0], &pivotVal, sizeof(pivotVal));
	printf("parent received pivot value %d\n", pivotVal);
	
	int largerChild[5];
	close(ptc[5][0]);
	write(ptc[5][1], &pivotVal, sizeof(pivotVal));
	printf("parent random array element %d to child %d\n", pivotVal, 5);
	
	close(ctp[5][1]);
	close(ctp[5][1]);
	read(ctp[5][0], &countLarger, sizeof(countLarger));
	printf("parent received countLarger\n");
	mVal = 0;
	for(int i = 0; i < 5;i++){
		printf("child %d counted %d number of elements larger than pivot value\n", i+1, countLarger[i]);
		mVal = mVal + countLarger[i];
	}
	kVal = nVal/2;
	printf("%d ", numbers[10]);
	printf("kVal = %d, nVal = %d, mVal = %d\n", kVal, nVal-1, mVal);
	

	printf("\n");
	close(ctp[5][1]);
	close(ctp[5][1]);
	read(ctp[5][0], &countLarger, sizeof(countLarger));
	if(kVal != mVal){
		while(kVal != mVal){
			if(mVal < kVal){
				//LARGE
				cmdCode = LARGE;
			}
			else{
				//SMALL
				cmdCode = SMALL;
			}
			
		}
	}
	else{
		printf("Median has been found!\n");
		printf("kill all child processes \n");
	}
}