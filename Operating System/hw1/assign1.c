#include <stdio.h>
#include <stdlib.h>
#include <time.h>

main(argc,argv)
int argc;
char *argv[]; {


    int count,upperlimit,x;

/*	check to make sure we pass in arguments  */
    if(argc <= 2) {
	printf("Usage:  %s Count UpperLimit\n",argv[0]);
	exit(1);
    }
/*	we've checked to make sure arguments are there,  */
/*	it is now save to access argv[1] and argv[2]     */
    count = atoi(argv[1]);
    upperlimit = atoi(argv[2]);

/*	we now know the count so initialize the size of the array   */
    int arr[count];

/*	seed random number generator   */
    srand( (int)time(NULL) );

/*    generate numbers, store in array, then print from array   */
    for(x=0;x< count; x++) {
	arr[x] = rand() % upperlimit;
	printf("%d\n",arr[x]);
    }

}
