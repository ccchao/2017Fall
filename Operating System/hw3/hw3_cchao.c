/* CS201 Programming assignment 3 
 * Due Date: End of day Sun 11/12
 *
 * 1.) generate a linked list of at least 1000 random numbers greater than or equal to 0 and less than 64. These numbers are the page numbers.
 * 2.) make copy of the list of numbers and cull adjacent numbers that are equal.
 *     Your culled list should end up with at least 1000 numbers, so your unculled list may have greater than 1000.
 * 3.) simulate
 *     a.) FIFO (First in First out) algorithm
 *         first page in is the first to be swapped out second page in is the second to be swapped out etc.
 *     b.) optimal algorithm
 *         scan list of pages to be brought in and select the one that won’t be used for the longest time.
 *     c.) LRU
 *
 *     Run three algorithms for your original list and the culled list.
 *     All algorithms must be run on the same original list and the same culled lists. 
 *     Do NOT generate a new list for each algorithm. 
 *
 *     run 2 groups of 3 simulations on each and report the averages. 
 *     The first group will be 3 simulations using list of numbers between 0 and 64, and second group will be on numbers between 0 and 32
 
 * 4.) Report your page fault percentage.
 *     Report average page fault percentage of 3 runs on:
 *     0-64 culled FIFO
 *     0-64 non-culled FIFO 
 *     0-32 culled FIFO
 *     0-32 non-culled FIFO 
 *     same for LRU and Optimal
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define CREATECOUNT 1500
#define ATLEASTCOUNT 1000
#define FRAMECOUNT 16

struct numberList{
	int number;
	struct numberList *next;
};

struct numberList *nonCulledHead64, *culledHead64, *nonCulledHead32, *culledHead32;
int culledCount64, culledCount32;

void generate(int limit);
void copy(int limit);
int delete(int limit);
struct numberList * insert_node (struct numberList *current, int number);
void delete_next_node (struct numberList *current);
void print_list(struct numberList *head);
float FIFO(int limit, struct numberList *head);
float optimal(int limit, struct numberList *head, int totalCount);
float LRU(int limit, struct numberList *head, int totalCount);

int main (int argc, char *argv[])
{
	culledCount64 = 0;
	culledCount32 = 0;

	srand(time(NULL)); //Use srand() to avoid same random number

	//Generate the linked list of at least ATLEASTCOUNT random numbers greater than or equal to 0 and less than 64
	while (culledCount64 < ATLEASTCOUNT){
		generate(64);
		//print_list(nonCulledHead64);
		//Make copy of the list of numbers
		copy(64);
		//Cull adjacent numbers that are equal
		culledCount64 = delete(64);
		//print_list(culledHead64);
	}
	//Generate a linked list of CREATECOUNT random numbers greater than or equal to 0 and less than 32
	while (culledCount32 < ATLEASTCOUNT){
		generate(32);
        	copy(32);
        	culledCount32 = delete(32);
	}
	
	printf("======FIFO======\n");
	printf("0-64 culled FIFO:     %f\%\n", FIFO(64, culledHead64)*100/culledCount64);
	printf("0-64 non-culled FIFO: %f\%\n", FIFO(64, nonCulledHead64)*100/CREATECOUNT);
	printf("0-32 culled FIFO:     %f\%\n", FIFO(32, culledHead32)*100/culledCount32);
	printf("0-32 non-culled FIFO: %f\%\n", FIFO(32, nonCulledHead32)*100/CREATECOUNT);
	
	printf("======Optimal======\n");
	printf("0-64 culled Optimal:     %f\%\n", optimal(64, culledHead64, culledCount64)*100/culledCount64);
	printf("0-64 non-culled Optimal: %f\%\n", optimal(64, nonCulledHead64, CREATECOUNT)*100/CREATECOUNT);
	printf("0-32 culled Optimal:     %f\%\n", optimal(32, culledHead32, culledCount32)*100/culledCount32);
	printf("0-32 non-culled Optimal: %f\%\n", optimal(32, nonCulledHead32, CREATECOUNT)*100/CREATECOUNT);

	printf("======LRU======\n");
	printf("0-64 culled LRU:     %f\%\n", LRU(64, culledHead64, culledCount64)*100/culledCount64);
	printf("0-64 non-culled LRU: %f\%\n", LRU(64, nonCulledHead64, CREATECOUNT)*100/CREATECOUNT);
	printf("0-32 culled LRU:     %f\%\n", LRU(32, culledHead32, culledCount32)*100/culledCount32);
	printf("0-32 non-culled LRU: %f\%\n", LRU(32, nonCulledHead32, CREATECOUNT)*100/CREATECOUNT);

	return 0; //Finish
}

void generate(int limit)
{
	int count=0;
	struct numberList *generating;

	//Generate a number first
	generating = (struct numberList *) malloc (sizeof (struct numberList));
	generating->number = rand() % limit;
	generating->next = NULL;
	count++;

	//Assign the first number to the linked list
	if (limit == 64)
		nonCulledHead64 = generating;
	else
		nonCulledHead32 = generating;
	
	//Generate CREATECOUNT random numbers to the linked list
	while (count < CREATECOUNT){
		generating = insert_node (generating, rand() % limit);
		count++;
	}	
	return;
}

void copy (int limit)
{
	struct numberList *current, *newHead, *generating;

	//Point to head of linked list
	
	current = (limit == 64)? nonCulledHead64 : nonCulledHead32;
	
	//Copy the first number to a new linked list
	newHead = (struct numberList *) malloc (sizeof (struct numberList));
	newHead->number = current->number;
	newHead->next = NULL;
	generating = newHead;

	//Copy the rest numbers
	while (current->next != NULL){
		current = current->next;
		generating = insert_node (generating, current->number);
	}
	
	if (limit == 64)
		culledHead64 = newHead;
	else
		culledHead32 = newHead;
	
	return;
}

int delete(int limit)
{
	struct numberList *current;
	int count=CREATECOUNT;

	//Point to the specific linked list
	current = (limit == 64)? culledHead64 : culledHead32;

	while (current->next != NULL){
		//If next number is equal to current number, the next number should be deleted
		if (current->number == current->next->number){
			delete_next_node(current);
			count--;
		}
		else{
			current = current->next;
		}
	}

	return count;
}

struct numberList * insert_node (struct numberList *current, int number)
{
	struct numberList *element;

        //Insert a new number to the linked list
 	element = (struct numberList *) malloc (sizeof (struct numberList));
        element->number = number;
        element->next = NULL;

        current->next = element;
        current = element;

	//Return the new position of current pointer
	return current;            
}

void delete_next_node (struct numberList *current)
{
	//Delete next node of current node
	struct numberList *delete = current->next;

	if (delete->next == NULL){ //If the next number is the last number, we only need to free that space
		free (delete);
		current->next = NULL;
	}
	else{
		current->next = delete->next;
		free(delete);
	}
	return;
}

void print_list(struct numberList *head)
{
	struct numberList *current = head;
	
	while (current != NULL){
		printf ("%d ", current->number);
		current = current->next;
	}
	printf ("\n");	
	
	return;
}

float FIFO(int limit, struct numberList *listHead)
{
	int inMemory[limit], i, pageFault=0;
	int frameList[FRAMECOUNT];
	struct numberList *current=listHead;
	int first=0, rear=0;

	//Initialize the array to represent that all pages are not in memory
	for (i=0; i<limit; i++)
		inMemory[i] = -1;
	for (i=0; i<FRAMECOUNT; i++)
		frameList[i] = -1;

	while (current != NULL){
		if (inMemory[current->number] == -1){ //This page is not in memory, so we need to replace the page that is inserted in earliest with this page
			if (pageFault < FRAMECOUNT){ //The first 64 or 32 pages or more are not in memory in the beginning, so we only need to insert them to frame lis
				frameList[rear] = current->number;
				inMemory[current->number] = rear; //Store the position of this page
				rear++;
			}
			//After inserting pages to each frame, we need to replace the first inserted page with current page
			else{
				inMemory[frameList[first]] = -1; //Remove the first inserted page
                                frameList[first] = current->number; //Replace the first page with current page
                                inMemory[current->number] = first;
                                first = (first+1) % FRAMECOUNT; //Point to the new first inserted page
			}

			pageFault++;
			/*
			printf("insert: %d\tpage fault: %d\tframe: ", current->number, pageFault);
			for (i=0; i<FRAMECOUNT; i++)
				printf("%d ", frameList[i]);
			printf("\n");
			*/
		}
		else{ //This page is in memory, so we need to move this page to the previous position of the first inserted page
			if (pageFault < FRAMECOUNT){ //If the frame list is not filled, we need to move all pages later than this page backward and put this page in the end
				for (i=inMemory[current->number]; i<rear-1; i++){
					frameList[i] = frameList[i+1];
					inMemory[frameList[i]] = i;
				}
				frameList[i] = current->number;
				inMemory[current->number] = i;
			}
			else{ //After the frame list is filled, we need to move all pages later than this page backward and loop until it meets first page and put this page in the end
				if (inMemory[current->number] == first){ //If this page is inserted first, then we only need to make the next page as the first page
					first = (first+1)%FRAMECOUNT;
				}
				else{
					for (i=inMemory[current->number]; i!=(first-1+FRAMECOUNT)%FRAMECOUNT; i=(i+1)%FRAMECOUNT){
	                                        frameList[i] = frameList[(i+1)%FRAMECOUNT];
	                                        inMemory[frameList[i]] = i;
	                                }
					frameList[i] = current->number;
	                                inMemory[current->number] = i;
				}
			}
			/*
			printf("inMemory: %d\tpage fault: %d\tframe: ", current->number, pageFault);
			for (i=0; i<FRAMECOUNT; i++)
                                printf("%d ", frameList[i]);
                        printf("\n");
			*/
		}
		current = current->next;
	}

	return (float)pageFault;
}

float optimal(int limit, struct numberList *head, int totalCount)
{
	struct numberList *current=head, *tmp;
	int i, pageFault=0, index=0, max;
	int inMemory[limit];
        int frameList[FRAMECOUNT], frame=0;
	int nextOccur[limit];

	//Initialize the array to represent that all pages are not in memory
	for (i=0; i<limit; i++){
		inMemory[i] = -1;
		nextOccur[i] = totalCount;
	}
	for (i=0; i<FRAMECOUNT; i++)
		frameList[i] = -1;

	while (current != NULL){
		if (inMemory[current->number] == -1){ //This page is not in memory, so we need to replace the page that will not be used for the longest period of time with this page
			if (pageFault < FRAMECOUNT){ //The first 64 or 32 pages or more are not in memory in the beginning, so we only need to insert them to frame list
				frameList[frame] = current->number;
				inMemory[current->number] = frame; //Store the position of this page
				frame++;
				
				//Find the index of when this page number occurs next time
				tmp = current->next;
				for (i=index+1; i<totalCount; i++){
					if (tmp->number == current->number){
						nextOccur[current->number] = i;
						break;
					}
					tmp = tmp->next;
				}
			}
			else{ //After inserting pages to each frame, we need to replace the page that will not be used for the longest period of time with this page
				
				//Find the page that will not be used for the longest period of time
				max=0;
				for (i=0; i<FRAMECOUNT; i++){
					if (nextOccur[frameList[i]] > max){
						max = nextOccur[frameList[i]];
						frame = frameList[i]; //frame is the page that will not be used for the longest period of time
					}
				}
				
				//replace the page that will not be used for the longest period of time with this page
				inMemory[current->number] = inMemory[frame];
				frameList[inMemory[frame]] = current->number;
				inMemory[frame] = -1; //Remove the frame that will not be used for the longest period of time from memory
				
				//Find the index of when this page number occurs next time
				tmp = current->next;
				for (i=index+1; i<totalCount; i++){
					if (tmp->number == current->number){
                                                nextOccur[current->number] = i;
                                                break;
                                        }
					tmp = tmp->next;
                                }
			}

			pageFault++;
			/*
			printf("insert: %d\tpage fault: %d\tframe: ", current->number, pageFault);
                        for (i=0; i<FRAMECOUNT; i++)
                                printf("%d ", frameList[i]);
                        printf("\n");
			*/
		}
		else{ //This page is in memory, so we only need to find the index of when this page number occurs next time
			tmp = current->next;
			for (i=index+1; i<totalCount; i++){
				if (tmp->number == current->number){
					nextOccur[current->number] = i;
					break;
				}
				tmp = tmp->next;
			}
			//printf("inMemory: %d\n", current->number);		
		}

		current = current->next;
		index++; //Store the index of current page
	}

	return (float)pageFault;
}



float LRU(int limit, struct numberList *head, int totalCount)
{
	struct numberList *current=head;
	int i, pageFault=0, index=0, min;
	int inMemory[limit];
        int frameList[FRAMECOUNT], frame=0;
	int lastOccur[limit];

	//Initialize the array to represent that all pages are not in memory
	for (i=0; i<limit; i++){
		inMemory[i] = -1;
		lastOccur[i] = totalCount;
	}
	for (i=0; i<FRAMECOUNT; i++)
		frameList[i] = -1;

	while (current != NULL){
		if (inMemory[current->number] == -1){ //This page is not in memory, so we need to replace the page that has not been used for the longest period of time with this page
			if (pageFault < FRAMECOUNT){ //The first 64 or 32 pages or more are not in memory in the beginning, so we only need to insert them to frame list
				frameList[frame] = current->number;
				inMemory[current->number] = frame; //Store the position of this page
				frame++;
				
				lastOccur[current->number] = index; //Store the index of when this page number occurs
			}
			else{ //After inserting pages to each frame, we need to replace the page that has not been used for the longest period of time with this page
				
				//Find the page that has not been used for the longest period of time
				min=totalCount;
				for (i=0; i<FRAMECOUNT; i++){
					if (lastOccur[frameList[i]] < min){
						min = lastOccur[frameList[i]];
						frame = frameList[i]; //frame is the page that has not been used for the longest period of time
					}
				}
				
				//replace the page that has not been used for the longest period of time with this page
				inMemory[current->number] = inMemory[frame];
				frameList[inMemory[frame]] = current->number;
				inMemory[frame] = -1; //Remove the frame that has not been used for the longest period of time from memory
				
				
				lastOccur[current->number] = index; //Store the index of when this page number occurs
			}

			pageFault++;
			/*	
			printf("insert: %d\tpage fault: %d\tframe: ", current->number, pageFault);
                        for (i=0; i<FRAMECOUNT; i++)
                                printf("%d ", frameList[i]);
                        printf("\n");
			*/
		}
		else{ //This page is in memory, so we only need to store the index of when this page number occurs
			lastOccur[current->number] = index; 
			//printf("inMemory: %d\n", current->number); 
		}

		current = current->next;
		index++; //Store the index of current page
	}

	return (float)pageFault;
}
