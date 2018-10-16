/* Programming assignment 4
 * Due Date: 8:00 am Monday 12/4.
 * 
 * Write a program in C that implements the FCFS and SCAN disk scheduling algorithms.
 * The disk in question has 5000 tracks.
 * 
 * 1.) generate a list of 100000 random track numbers that are greater than or equal to 0 and less than 5000.
 * 2.) segment your track addresses into groups of 1000, sort them according to the algorithm 
 * 3.) Calculate the number of tracks traveled for each group
 * 4.) Report the total number of tracks traveled for each algorithm.
 * 
 * Grad students will simulate: 
 * 1.) FCFS
 * 2.) SCAN 
 * 3.) C-SCAN
 * Grad students will run 3 groups of 3 simulations on each and report the averages for each algorithm.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define NUMVALS 100000
#define MAXVAL	5000
#define GROUPNUM 1000
#define ITERATE 3

/*
#define NUMVALS 100
#define MAXVAL	50
#define GROUPNUM 10
#define ITERATE 1
*/

typedef struct input_data_struct_type {
	int val;
	struct input_data_struct_type *next;
}input_data_type;

input_data_type *head = NULL,*tail = NULL;

int fcfsTotal=0, scanTotal=0, cscanTotal=0;

void *generate();
void *fcfs();
void *scan();
void *cscan();
int displaynums();
int increasingCompare (const void * a, const void * b);
int decreasingCompare (const void * a, const void * b);


int main(int argc, char *argv[])
{
	int i;   
	pthread_t generate_thread;
	pthread_t fcfs_thread;
	pthread_t scan_thread;
	pthread_t cscan_thread;

	srand( (int)time(NULL) );

	for (i=0; i<ITERATE; i++){
		//generate a list of 100000 random track numbers that are greater than or equal to 0 and less than 5000
		if(pthread_create(&generate_thread,NULL,generate,NULL) != 0) {
			perror("Generate numbers thread create");
		}
		if(pthread_join(generate_thread,NULL) != 0) { 
			perror("Generate thread join");
		}

		//print the 100000 random numbers
		//displaynums();

		//use FCFS to sort 100 groups of 1000 numbers and show the number of tracks traveled for each group
		if(pthread_create(&fcfs_thread,NULL,fcfs,NULL) != 0) {
			perror("FCFS thread create");
		}
		if(pthread_join(fcfs_thread,NULL) != 0) { 
			perror("FCFS thread join");
		}

		//use SCAN to sort 100 groups of 1000 numbers and show the number of tracks traveled for each group
		if(pthread_create(&scan_thread,NULL,scan,NULL) != 0) {
			perror("SCAN thread create");
		}
		if(pthread_join(scan_thread,NULL) != 0) { 
			perror("SCAN thread join");
		}

		//use C-SCAN to sort 100 groups of 1000 numbers and show the number of tracks traveled for each group
		if(pthread_create(&cscan_thread,NULL,cscan,NULL) != 0) {
			perror("C-SCAN thread create");
		}
		if(pthread_join(cscan_thread,NULL) != 0) { 
			perror("C-SCAN thread join");
		}
		//print the 100000 random numbers
		//displaynums();

		
		head = NULL;
		tail = NULL;
	}
	printf("===========================================================\n");
	printf("The average number of tracks traveled for FCFS: %d\n", fcfsTotal/ITERATE);
	printf("The average number of tracks traveled for SCAN: %d\n", scanTotal/ITERATE);
	printf("The average number of tracks traveled for C-SCAN: %d\n", cscanTotal/ITERATE);



	return 0;
}

void *generate() {

	input_data_type *tofill;
	//int insertcount = 0, newcount;
	int count = 0;

	while (count < NUMVALS) {
		/*	if((count %1000) == 1) {
			printf("count = %d\n",count);
			}*/
		tofill = (input_data_type *)malloc(sizeof(input_data_type));
		tofill->val = rand() % MAXVAL;

		tofill->next=NULL;
		if (head == NULL) {
			head = tofill;
		}
		else {
			tail->next = tofill;
		}
		tail = tofill;
		count++;
	}
}



void *fcfs()
{
	//use this function to sort and calculate FCFS travel

	int i, j, groupCount=NUMVALS/GROUPNUM;
	input_data_type *current=head;
	int sortList[GROUPNUM], lastAddress=0, total=0;
 
	//there are 100000/1000 groups
	for (i=0; i<groupCount; i++){
		//total=0; //shows the number of tracks traveled for each group

		//every group contains 1000 numbers
		for (j=0; j<GROUPNUM; j++){
			sortList[j] = current->val;
			total = total + abs(sortList[j]-lastAddress);
			lastAddress = sortList[j];
			current = current->next;		
		}
		/*
		for (j=0; j<GROUPNUM; j++)
			printf("%d ", sortList[j]);
		printf("\ntotal=%d\n", total);
		*/
	}
	fcfsTotal = fcfsTotal + total;

	printf("The total number of tracks traveled for FCFS: %d\n", total);
}

void *scan()
{
	//use this function to sort and calculate SCAN travel

	int i, j, groupCount=NUMVALS/GROUPNUM;
	input_data_type *current=head;
	int sortList[GROUPNUM], lastAddress=0, total=0;
	
	//there are 100000/1000 groups
	for (i=0; i<groupCount; i++){
		//total=0; //shows the number of tracks traveled for each group

		//every group contains 1000 numbers
		for (j=0; j<GROUPNUM; j++){
			sortList[j] = current->val;
			current = current->next;		
		}

		if (i%2 == 0)
			qsort(sortList, GROUPNUM, sizeof(int), increasingCompare);
		else
			qsort(sortList, GROUPNUM, sizeof(int), decreasingCompare);


		for (j=0; j<GROUPNUM; j++){
			total = total + abs(sortList[j]-lastAddress);			
			lastAddress = sortList[j];
		}

		//moving from inner end to outer end
		if (i%2 == 0){
			total = total + (MAXVAL-1)-sortList[GROUPNUM-1];
			lastAddress = MAXVAL-1;
		}
		//moving from outer end to inner end
		else{
			total = total + sortList[GROUPNUM-1];
			lastAddress = 0;
		}
		/*
		for (j=0; j<GROUPNUM; j++)
			printf("%d ", sortList[j]);
		printf("\ntotal=%d\n", total);
		*/
	}
	scanTotal = scanTotal + total;

	printf("The total number of tracks traveled for SCAN: %d\n", total);
}

void *cscan()
{
	//use this function to sort and calculate C-SCAN travel

	int i, j, groupCount=NUMVALS/GROUPNUM;
	input_data_type *current=head;
	int sortList[GROUPNUM], lastAddress=0, total=0;
	
	//there are 100000/1000 groups
	for (i=0; i<groupCount; i++){
		//total=0; //shows the number of tracks traveled for each group

		//every group contains 1000 numbers
		for (j=0; j<GROUPNUM; j++){
			sortList[j] = current->val;
			current = current->next;		
		}

		qsort(sortList, GROUPNUM, sizeof(int), increasingCompare);

		for (j=0; j<GROUPNUM; j++){
			total = total + abs(sortList[j]-lastAddress);			
			lastAddress = sortList[j];
		}

		//moving from inner end to outer end
		total = total + (MAXVAL-1)-sortList[GROUPNUM-1];
		total = total + (MAXVAL-1)-0;
		lastAddress = 0;	
		
		/*		
		for (j=0; j<GROUPNUM; j++)
			printf("%d ", sortList[j]);
		printf("\ntotal=%d\n", total);
		*/
	}
	cscanTotal = cscanTotal + total;

	printf("The total number of tracks traveled for C-SCAN: %d\n\n", total);

}

int displaynums()
{
	//show all numbers in the linked list

	input_data_type *x;
	int count = 0;

	x = head;
	while(x) {
		printf("%d\n",x->val);
		++count;
		x = x->next;
	}
	printf("\ncount = %d\n\n",count);

	return(count);
}

int increasingCompare (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}
int decreasingCompare (const void * a, const void * b) {
   return ( *(int*)b - *(int*)a );
}
