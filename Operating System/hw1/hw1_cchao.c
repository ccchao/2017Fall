/* Programming assignment 1
 * Due Date: End of day Sun. 9/24.
 *
 * Write a program in C that generates a list of random numbers greater than or
 * equal to 0 and less than an upper limit, stores them in an array and then prints
 * out the array. The count of numbers and the upper limit on the numbers will
 * be passed in on the command line. You must check to make sure the count
 * and upper limit have been passed in, print an error message and exit if not.
 *
 * If you run this program multiple times with the same count and upper limit, you
 * must get diferent lists of numbers.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char *argv[])
{
	int count, limit, i;
	int *array;

	if (argc != 3){ //There should be exact 3 arguments passed in.
		printf ("There should be exact 3 arguments passed in.\n%s [count of numbers] [upper limit]\n", argv[0]);
		return 0;
	}

	//Store the count and the limit passed in from command line
	count = atoi(argv[1]);
	limit = atoi(argv[2]);

	if (count < 0 || limit < 0){ //The count and the limit should be greater than 0.
		printf ("The count of numbers and the upper limit should be greater than 0.\n");
		return 0;
	}

	srand(time(NULL)); //Use srand() to avoid same random number
	array = (int *) malloc (sizeof (int) * count); //Allocate memory to store random numbers 
	for (i=0; i<count; i++){ //Store the random number in the array
		array[i] = rand() % limit; //rand() creates a random integer. rand()%limit will generage a number larger than 0 and less than limit 
	}

	for (i=0; i<count; i++){ //Print out the array
		printf ("%d\n", array[i]);
	}

	return 0; //Finish
}
