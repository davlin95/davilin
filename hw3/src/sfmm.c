/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include <stdio.h>
#include <stdlib.h>
#include "sfmm.h"
#include <sfmmHelper.h>
/**
 * You should store the head of your free list in this variable.
 * Doing so will make it accessible via the extern statement in sfmm.h
 * which will allow you to pass the address to sf_snapshot in a different file.
 */
sf_free_header* freelist_head = NULL;

#define PAGE_SIZE 4096

void* sf_malloc(size_t size) {
	fprintf(stdout,"Working SF_MALLOC");

	/* Check for size , if > 4GB return error. If 0, return null */
	int * heapPtr = sf_brk(0);
	if(heapPtr != NULL)
	    fprintf(stdout, "HEAP PTR IS AT: %d", heapPtr);

	/* Check, do we have enough memory?*/
	


	/* If not enough memory, set Errno */

	/* check flags for -DFIRST and -DNEXT*/

	/*DFIRST = first placement strategy for locating block (More info p. 849)*/


	/* Call sfbrk(), which works on multiples of size 4096 bytes */

	/* Perform Immediate coalescing (p.850) checks*/

	/* Header and Footer Boundaries */

	/* Split blocks which are too large (p.849), do not make splinters( < 16 bytes free bytes)  */

	/* Avoid splinters by overallocation of mem*/


	/* Check alignment issues */

	/* Must implement boundary tags */


    return NULL;
}

void sf_free(void *ptr) {
fprintf(stdout,"Working SF_FREE");

	/* Check for Free NULL, middle of allocation block, non-sf _allocated address */

	/* Check above with helper function testing valid address ?*/
    /* Must check if ptr is allocated or freed */

    /* If already free, do nothing and exit*/

    /* If allocated, continue operating on it*/

}

void* sf_realloc(void *ptr, size_t size) {
fprintf(stdout,"Working SF_REALLOC");


    return NULL;
}

void* sf_calloc(size_t nmemb, size_t size) {
fprintf(stdout,"Working SF_CALLOC");


    return NULL;
}


/* This is a helper function that splits a free block into smaller blocks */
void split( ){

}

/* This is a helper function that reads the allocation bit and determines if the block is free*/
bool isBlockFree(){

}

/* This is a helper function that gets the previous block */
void* getPrevBlock(){

}

/* This is a helper function that gets the next block */
void* getNextBlock(){

}

/* Helper Function: Gets size of the next block */
void getNextBlockSize(){

}

/*HelperFunction: Get size of the previous block */
void getPrevBlockSize(){

}

/* This is a helper function to request extra heap space: mem_sbrk()? */
void heapIncrementRequest(){

}

/* Helper Function: Pads block to aligned boundaries. */
void padToAlignment(){

}
 
/* Helper Function : Flip allocation Bit */
void flipAllocationBit(){

}
/* This is a helper function that searches the free list for the next available block 
 * according to the next-fit policy
 */
void* findNextFitPolicy(){

}

/* This is a helper function that searches the free list for the next available block 
 * according to the first-fit policy 
 */
void * findFirstFitPolicy(){

}
/* This is a helper function that searches the free list for the next available block 
 * according to the best fit policy 
 */
void * findBestFitPolicy(){
}

/* This is a helper function that performs coalesce on the previous block */
void * coalescePrevBlock(){

}

/* This is a helper function that performs coalesce on the next block */
void * coalesceNextBlock(){

}
/* This is a helper function that performs coalesce on both the next and previous block */
void* coalescePrevNextBlock(){

}
/* This is a bottom level merge of two blocks */
void* mergeBlocks(){
}

/* Helper Function: Check Valid address for this program's mem allocation */
bool validateAddress(){

}



