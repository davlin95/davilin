/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
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
	/*Check for bounds */
	unsigned long long maxSize = (1024*1024*1024);
	maxSize = maxSize * 4;

	/* If greater than 4GB, exit*/
	if(size>maxSize){
		errno = ENOMEM;
		perror(strerror(errno));
		return NULL;
	}

	/* call sf_mem_init for 20MB*/
	/*sf_mem_init(20*1024*1024);*/
 

	/*Expand the heap if necessary */     

	void* currPtr ;

	/* Test */
	unsigned long currAdd =  ((unsigned long) sf_sbrk(0));
	unsigned long contentsCurrentAddress = *((unsigned long*)sf_sbrk(0));
	printf("\nIs current address divisible by 16? result of modulo: %lu ", (currAdd%16));
	printf("\nContents of current address: %lu\n",contentsCurrentAddress);
	
	/* Check for null cases */
	if( (currPtr = sf_sbrk(PAGE_SIZE)) == ((void*)-1)){
		perror(strerror(errno));
		return NULL;
	}

	if(!isAligned(currPtr)){
		/* Align the current header with padding*/

	}

	/* Initialize an sf_header struct*/
	sf_header headerList;
	printf("Initialized sf_header struct headerList\n");

	headerList.alloc = readAllocatedBit(currPtr);
	printf("headerList.alloc = %lu\n",readAllocatedBit(currPtr));
	headerList.block_size = readBlockSize(currPtr);
	printf("headerList.block_size = %lu\n",readBlockSize(currPtr));
	headerList.requested_size = size;
	writeRequestedSize(size,currPtr);
	printf("headerList.requested_size = %lu\n",readRequestedSize(currPtr));
		
	/* Traverse block until reach an empty block */
	while( headerList.alloc !=0 ){ /* Not a free block */

		printf("Traversing, looking for next free block");
		currPtr = currPtr + headerList.block_size;

		headerList.alloc = readAllocatedBit(currPtr);
		printf("headerList.alloc = %lu\n",readAllocatedBit(currPtr));

		headerList.block_size = readBlockSize(currPtr);
		printf("headerList.block_size = %lu\n",readBlockSize(currPtr));

		headerList.requested_size = readRequestedSize(currPtr);
		printf("headerList.requested_size = %lu\n",readRequestedSize(currPtr));
	}

	/* We have a free block */
	freelist_head = currPtr;
	/*Test */
	return freelist_head;

	/* Max size 4GB */
	/* size_t maxSize = 4*1024*1024*1024; */
	/* Check for size , if > 4GB return error. If 0, return null */

	/* if(size> maxSize ||size < 0)
		return NULL; */
	/* Check do we have enough memory?*/
	

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


    return NULL;
}

void* sf_calloc(size_t nmemb, size_t size) {

    return NULL;
}


/* This is a helper function that splits a free block into smaller blocks */
void split( ){

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

/* Helper Function: Pads block to aligned boundaries. */
void padToAlignment(){

}
 
/* Helper Function : Flip allocation Bit */
void flipAllocationBit(){

}
/* This is a helper function that searches the free list for the next available block 
 * according to the next-fit policy. 
 */
void* findNextFitPolicy(size_t requested_size, sf_free_header* curr_head, sf_free_header* freelist_head){
	/* pointer to an sf_free_head, in start of search*/
	sf_free_header* anchor = curr_head; 
	sf_free_header* curr = anchor; 

	/* Go through the link-list, up to one full cycle ending at the anchor point */
	/* Stop if we find the requested size before then */
	while ( ( (curr->next) != anchor) && (((curr->header).block_size)<requested_size) ){
		/* Reached end of list, continue search at the start of free_list */
		if(( (curr->next) == NULL) && (freelist_head!= anchor)){
			/* If we can continue iteration at the beginning of linked list*/
			curr = freelist_head;
		}
		else if(( (curr->next) == NULL) && (freelist_head== anchor)){
			/* Special Case: we are in a single node list. Don't continue iteration */
			return NULL;
		} 
		else curr = (curr->next); /* Regular iteration through range of linked list */
	}

	/* Either we completed one cycle, and are at the node before anchor*/
	/* or we have found our requested_size*/
	if((curr->header).block_size >= requested_size)
		return curr; /* Return a valid block */ 
	else{
		/* No block of size requested exists */
		sf_sbrk(PAGE_SIZE);
	} 

}
/*

/* This is a helper function that searches the free list for the next available block 
 * according to the first-fit policy 
 */
void* findFirstFitPolicy(size_t requested_size, sf_free_header* freelist_head){
	sf_free_header* curr = freelist_head; /* current pointer to an sf_free_head */

	/* Go through the link-list, looking for the requested size*/
	while (((curr->next) != NULL) && (((curr->header).block_size)<requested_size) ){
		curr = (curr->next);
	}
	/* Either we are at the last node of the free list, or we have found our requested_size*/
	if( ( (curr->header).block_size) >= requested_size)
		return curr; /* Return a valid block */ 
	else return NULL; /* No block of size requested exists */
}



/* This is a helper function that performs coalesce on the previous block */
void * coalescePrevBlock(sf_free_header* currHeader){

}

/* This is a helper function that performs coalesce on the next block */
void* coalesceNextBlock(){


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
/* Helper function that determines whether the address of the pointer is aligned, by modulo 16 */
bool isAligned(void* address){
	printf("\nisAligned() is at address: %p\n", address);
	uintptr_t addressInt = ((uintptr_t)address);
	printf("isAligned modulo: %lu\n", (addressInt%16));
	if( addressInt % 16 ==0)
		return true;
	else return false;
}

unsigned long readRequestedSize(void* ptr){
	/* Initialize Vars */
	unsigned long addressContents = (*((unsigned long*)ptr));
	unsigned long requestedSize = addressContents & 0xffffffff00000000;
	return requestedSize;

}
void writeRequestedSize(unsigned long size, void* ptr){
	/* Take the requested_size value and shift it to the request_size area */
	unsigned long requested_size = (size<<32);

	/* Convert the pointer, then retrieve its contents*/
	unsigned long addressContents = (*((unsigned long*)ptr));

	/* Clear the requested_size bits, then OR it to add. Assign to original pointer */
	addressContents = (addressContents & 0x00000000ffffffff); 
	*((unsigned long*)ptr) = (addressContents | requested_size);

}
unsigned long readBlockSize(void* ptr){
	/* Initialize Vars */
	unsigned long addressContents = (*((unsigned long*)ptr));
	unsigned long blockSize = addressContents & 0xfffffff0;
	return blockSize;
}

/* This is a helper function that reads the allocation bit and determines if the block is free*/
unsigned long readAllocatedBit(void* ptr){
	printf("Testing readAllocatedBit(): pointer size = %lu, pointer val = %p \n", sizeof(ptr), ptr);
	 unsigned long addressContents = (*((unsigned long*)ptr));
	if((addressContents & 0x1) == 1) return true;
	else return false;
}


