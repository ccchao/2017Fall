/* CS201 Programming assignment 2 
 * Due Date: End of day Sun 10/15
 * 
 * Write a program in C that generates a list of random numbers greater than or equal to 0 and less than an upper limit of at least 100, 
 * stores them in a linked list, deletes adjacent numbers that are within 8 of each other and then prints out the list.
 * 
 * The count of numbers and the upper limit on the numbers will be passed in on the command line.
 * You must check to make sure the count and upper limit have been passed in, print an error message and exit if not.
 * 
 * If you run this program multiple times with the same count and upper limit, you must get di erent lists of numbers.
 * Create a thread for initial generation of the numbers and a separate thread for deleting adjacent numbers
 *
 * Create both threads at once and have them work in a producer/consumer fashion.
 * Set up some sort of communication between threads and ensure that both threads do not access the data at the same time. I suggest a mutex lock.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define FALSE 0
#define TRUE 1

struct numberList{
	int number;
	struct numberList *next;
};

struct numberList *head, *generating, *checking;
int count, limit, generateNum, checkNum;
int available;

void *generate();
void *delete();
void print_list();

int main (int argc, char *argv[])
{
	pthread_t threadGenerate, threadDelete;
	int preturn;

	if (argc != 3){ //There should be exact 3 arguments passed in.
		printf ("There should be exact 3 arguments passed in.\n%s [count of numbers] [upper limit]\n", argv[0]);
		return 0;
	}

	//Store the count and the limit passed in from command line
	count = atoi(argv[1]);
	limit = atoi(argv[2]);

	if (count < 0){ //The count shoud be greater than or equal to 0
		printf ("The count of numbers should be greater than or equal to 0, but the input count is %d.\n", count);
		return 0;
	}
	if (limit < 100){ //The upper limit should be greater than 100
		printf ("The upper limit should be greater than 100, but the input limit is %d.\n", limit);
		return 0;
	}

	srand(time(NULL)); //Use srand() to avoid same random number
	

	//Initialize
	generateNum = 0; //How many numbers have been generated
	checkNum = 0; //How many numbers have been maintained after deleting
	head = NULL; //Head of the linked list
	generating = NULL; //The last generated number
	checking = NULL; //Current checked number
	available = TRUE; //A flag for mutex lock


	printf ("This program generates two numbers more than limit for easy coding, but these numbers will be deleted in the end.\n");

	//Create a thread to generate random numbers
	preturn = pthread_create (&threadGenerate, NULL, generate, NULL);
	if (preturn != 0){
		printf ("Create thread error\n");
		return(0);
	}
	//Create a thread to check adjacent numbers and delete a number if the difference is smaller than 8
	preturn = pthread_create (&threadDelete, NULL, delete, NULL);
	if (preturn != 0){
		printf ("Create thread error\n");
		return(0);
	}

	//Wait until these threads finish
	pthread_join (threadGenerate, NULL);
	pthread_join (threadDelete, NULL);


	//Delete two numbers because threadGenerate generated two more numbers
	free (checking->next->next);
	free (checking->next);
	checking->next = NULL;

	printf ("\n=========================================================\nFinal list of %d numbers smaller than %d:\n", count, limit);
	print_list();


	return 0; //Finish
}

void *generate()
{
	//Insert the first number to head
	head = (struct numberList *) malloc (sizeof (struct numberList));
	head->number = rand() % limit;
	head->next = NULL;
	generateNum++;
	generating = head;
	
	printf ("Generate %d. List: ", generating->number);
	print_list();

	while (checkNum < count){
		struct numberList *element;		
		
		if (generateNum == count+2) //If there are already enough random numbers, continue the loop
			continue;
	
		while (available == FALSE); //Wait until the lock is available
		
		available = FALSE; //Get the lock

		//Insert a new number to the linked list
		element = (struct numberList *) malloc (sizeof (struct numberList));
		element->number = rand() % limit;
		element->next = NULL;
		generating->next = element;
		generating = element;
		generateNum++;

		printf ("Generate %d. List: ", generating->number);
		print_list();

		available = TRUE; //Release the lock
	}

	pthread_exit(0);
}


void *delete()
{
	struct numberList *adjacent;

	while (generateNum == 0); //Wait until the first number has been generated
	//The first number doesn't need to check
	checking = head;
	checkNum++;

	while (checkNum < count){
		
		if (checkNum > generateNum-2) //If this thread checks numbers faster then the thread generating numbers, it continues the loop
			continue;
		
		while (available == FALSE); //Wait until the lock is available
		
		available = FALSE; //Get the lock

		adjacent = checking->next;

		//If adjacent numbers are within 8 of each other, the latter should be deleted
		if (checking->number - adjacent->number <= 8 && checking->number - adjacent->number >=-8){
	
			printf ("Compare %d and %d and delete %d. List: ", checking->number, adjacent->number, adjacent->number);

			checking->next = adjacent->next;
			generateNum--;

			print_list();
		
			free (adjacent);		
		}
		else{
			
			printf ("Compare %d and %d but do nothing. List: ", checking->number, adjacent->number);
			print_list();

			checking = adjacent;
			checkNum++;
		}


		available = TRUE; //Release the lock
	}

	pthread_exit(0);
}

void print_list()
{
	struct numberList *current = head;
	
	while (current != NULL){
		printf ("%d ", current->number);
		current = current->next;
	}
	printf ("\n");	

}

