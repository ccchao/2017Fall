#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SEPARATION 8

/* this structure has an input integer for the counter and an integer random number */

typedef struct input_data_struct_type {
    int val;
    struct input_data_struct_type *next;
}input_data_type;

input_data_type *numstruct = NULL,*tail = NULL,*traverse = NULL;

int numvals,uplimit,count=0;

void *getnums() {
    
    input_data_type *tofill;
    int insertcount = 0,newcount;

    while (count < numvals) {
	tofill = (input_data_type *)malloc(sizeof(numstruct));
	tofill->val = rand() % uplimit;
/*	    printf("inserting %d\t",tofill->val)*/
	if((abs(uplimit - tofill->val) <= SEPARATION) && (tofill->val <= SEPARATION)) {
	    printf("list cannot be completed -- value = %d, upper limit = %d\n",tofill->val,uplimit);
	    exit(1);
	}
	tofill->next=NULL;
	if (numstruct == NULL) {
	    numstruct = tofill;
	}
	else {
	    tail->next = tofill;
	}
	tail = tofill;
	++count;
	++insertcount;
    }
/*    printf("inserted %d\n",insertcount);*/
/*    newcount = displaynums();*/
}


int displaynums()
{
    input_data_type *x;
    int count = 0;

    x = numstruct;
    while(x) {
	printf("%d  ",x->val);
	++count;
	x = x->next;
    }
    printf("\ncount = %d\n\n",count);
    return(count);
}

void *cullnums()
{
    
    input_data_type *tocull;
    int newcount,cullcount= 0;

    traverse = numstruct;
    while (traverse && traverse->next) {
	if(abs(traverse->val - traverse->next->val) < SEPARATION) {
	    tocull = traverse->next;
	    traverse->next = tocull->next;
	    if(!(tocull->next)) {
		tail = traverse;
	    }
/*		printf("culling %d\n",tocull->val);*/
	    free(tocull);
	    ++cullcount;
	    --count;
	}
	else {
	    traverse = traverse->next;
	}
    }
/*    printf("culled %d\n",cullcount);*/
/*    newcount = displaynums();*/
}


help(argv) 
char *argv[]; {

    printf ("%s <numvals> <uplimit>\n",argv[0]);
}

main(argc,argv)
int argc;
char *argv[]; {
    
    pthread_t producer;
    pthread_t consumer;
    if(argc != 3){
	help(argv);
	exit(1);
    }

    numvals = atoi(argv[1]);
    uplimit = atoi(argv[2]);

    numstruct = NULL;
    srand( (int)time(NULL) );

    while(count < numvals) {
	if(pthread_create(&producer,NULL,getnums,NULL) != 0) {
	    perror("Consumer thread create");
	}
	if(pthread_join(producer,NULL) != 0) { 
	    perror("Producer thread join");
	}
	if(pthread_create(&consumer,NULL,cullnums,NULL) != 0) {
	    perror("Producer thread create");
	}

	if(pthread_join(consumer,NULL) != 0) { 
	    perror("Consumer thread join");
	}
    }
/*    printf("count = %d, numvals = %d\n",count,numvals);*/
    traverse = numstruct;
    while(traverse) {
	printf("%d\n",traverse->val);
	traverse = traverse->next;
    }
}
