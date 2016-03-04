/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "sfmm.h"
#include <sfmmHelper.h>

/*
 * You should store the head of your free list in this variable.
 * Doing so will make it accessible via the extern statement in sfmm.h
 * which will allow you to pass the address to sf_snapshot in a different file.
 */
sf_free_header* freelist_head = NULL;
#define PAGE_SIZE 4096

void testPrint(){
/*Test: test the following principles of block creation
	sf_free_header* block = sf_sbrk(PAGE_SIZE);
	(block -> header).alloc = 0;
	(block -> header).block_size = 4096;
	(block -> header).requested_size = size;

	sf_free_header* block2 = sf_sbrk(PAGE_SIZE);
	(block2 -> header).alloc = 1;
	(block2 -> header).block_size = 4095;
	(block2 -> header).requested_size = 4094;
	
	freelist_head = block ;
	(block -> prev) = NULL;	
	(block -> next) = block2;
	(block2 -> next) = NULL;
	(block2 -> prev) = block;
	freelist_head = block ;

	sf_header sfheader = (block->header);
	printf("SF_HEADER BLOCK alloc : %d\n",sfheader.alloc);
	printf("SF_HEADER BLOCK requestedsize: %d\n",sfheader.requested_size);
	printf("SF_HEADER BLOCK blocksize : %d\n",sfheader.block_size);

	sf_header sfheader2 = ((block->next)->header);
	printf("SF_HEADER2 BLOCK2 alloc : %d\n",sfheader2.alloc);
	printf("SF_HEADER2 BLOCK2 requestedsize: %d\n",sfheader2.requested_size);
	printf("SF_HEADER2 BLOCK2 blocksize : %d\n",sfheader2.block_size);*/

}

			/* Main Methods */
/*
 * A function that implements malloc
 * @param size : size of bytes to be malloced. 
 */
void* sf_malloc(size_t size) {
	/*Upper limit of size is 4GB */
	unsigned long long maxSize = (1024*1024*1024);
	maxSize = maxSize * 4;

	/* If greater than 4GB, exit*/
	if(size>maxSize){
		errno = ENOMEM;
		perror(strerror(errno));
		return NULL;
	}else if(size ==0){
		return NULL;
	}

	sf_free_header* freeblock = findFirstFitPolicy(size, freelist_head);
	if(freeblock == NULL){
		initializeFreelistHeader(size);
		freeblock = findFirstFitPolicy(size,freelist_head);
	}
	updateBlockToAllocated(freeblock,size);
	
	/*Update the freelist in regards to this block */
	if((freeblock->prev) != NULL)
		((freeblock->prev)->next) = (freeblock->next);
	else if((freeblock-> next)!=NULL){ /* The fit block is the freelist_head */
		freelist_head = (freeblock->next); 
	}else{  
		/* The fit block is the only node, which is the freelist_head*/
		/* Replace the freelist_head with new free block */

		alignHeap();
		freelist_head = createAlignedBlock(1);
		freelist_head->next = NULL;
		freelist_head->prev = NULL; 
		printf("\nPOINTER TO PAYLOAD OF FREELISTHEAD: %p\n",getPayloadPtr(freelist_head));
	}
	
	/* return the payload */
	void* payloadAddress = getPayloadPtr(freeblock);
	return payloadAddress;

}

void sf_free(void *ptr) {
	/* Check for valid ptr to be freed */
	if(validateAddress(ptr) == true){
		/* Update Header Block*/
		sf_free_header* freeThisBlock = ptr;
		(freeThisBlock->header).alloc = 0;
		(freeThisBlock->header).requested_size=0;
		unsigned long blockSize = (((freeThisBlock->header).block_size)<<4);

		/* Update the footer block */
		void* footerBlock = (void*)(((char*)ptr)+blockSize-8); /* footer starts 8 bytes before end of block*/
		sf_footer* footerStructPtr = (void*)(((char*)footerBlock)+4); /* Grab the struct*/
		footerStructPtr->alloc=0; /* Update the footer alloc bit */

		/*Put the free block into the front of the freelist */
		freeThisBlock->next = freelist_head;
		freelist_head = freeThisBlock;
	}
}

void* sf_realloc(void *ptr, size_t size) {

    return NULL;
}

void* sf_calloc(size_t nmemb, size_t size) {

    return NULL;
}


						/* Memory Policy Methods */

/* A function that finds the free block according to the find next fit policy */
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

/* A helper function that helps execute the findFirstFitPolicy
 * @param requested_size: the size of the requested block 
 * @param freelist_head: the header of the freelist.
 * @returns: sf_free_header* node containing the first block to fit the policy, else NULL. 
 */
sf_free_header* findFirstFitPolicy(size_t requested_size, sf_free_header* head){
	/* Store the current node */
	sf_free_header* curr = head; 

	/*Base case*/
	if(curr == NULL)
		return NULL;

	/* Check current head for the free block before going through the list */
	if ( ( ( curr->header).block_size) >= requested_size){ 
		/* We've found the block */
		return curr;
	}

	/* first block is not a fit, go through the link-list, looking for the requested size*/
	while (((curr->next) != NULL) && (((curr->header).block_size) < requested_size) ){
		curr = (curr->next);
	}

	/* Either we are at the last node of the free list, or we have found our requested_size*/
	if(((curr->header).block_size)>= requested_size)
		return curr; /* Return node containing the valid block */ 
	else
		return NULL; /* No block of size requested exists */
}

/* A function that sets the allocated bit as non-free, as well as writes the requested size for the block. 
 * It also makes sure to update the freelist. 
 * @param blockPointer: the pointer to the beginning of the block
 * @param size: the size of the block's requested_size field to be set. 
 */
void updateBlockToAllocated(sf_free_header* block,size_t size){
	/* Get access to footer struct */
	unsigned long blockSize = (block->header).block_size; /* Grab Block size  */
	void* endOfBlock = (void*)(((char*)block) + blockSize); /* ptr to end of block */
	void* footerStartPtr = (((char*)endOfBlock)-8); /* ptr to beginning of footer */
	sf_footer* footer = footerStartPtr;

	/* Update header struct */
	((block->header).alloc) = 1;
	(block->header).requested_size = (size>>4);

	/* Update footer struct */
	(footer->alloc) = 1;
}

/* A function that requests for more free heap blocks to be inserted into the explicit free list
 * @param size: the size of the requested free block to be inserted into the front of the freelist
 */
void initializeFreelistHeader(size_t size){
	/*size for at least payload, boundary tags*/
	int requestSize = calculateRequestSize(size);

	/* extra size for padding */
	if( (requestSize - size) < (4*sizeof(unsigned long))) /*Room for padding/ header tags*/
		requestSize = requestSize + PAGE_SIZE; /* Increment by whole new multiple of page_size*/

	/*Test*/
	printf("\nrequestedSize in initializeFreelistHeader() is %d\n", requestSize);

	/* expand heap to create new block */
	unsigned long multipleOfPageSize = requestSize/4096;
	sf_free_header* newBlock = createAlignedBlock(multipleOfPageSize);

	/*Attach the node to the front of the list*/
	(newBlock->next) = freelist_head;
	freelist_head = newBlock;	
}

				/*Methods to Create Blocks*/

/* A function that creates a header boundary within the existing heap. Not asking for additional memory. 
 * @param startingAddress : starting address of the header
 * @param alloc : allocated bits
 * @param block_size: block size
 * @param requested_size: requested size 
 * @return : return address of where header ends.
*/
void* insertHeader(void* startingAddress, int alloc, int block_size, int requested_size){
	sf_free_header* headerStart = startingAddress;
	headerStart->header.alloc = alloc;
	headerStart->header.block_size = (block_size>>4);
	headerStart->header.requested_size = requested_size;

	/*Test */
	printf("\nIn insertHeader(): sizeof(sf_free_header) = : %lu \n", sizeof(sf_free_header));
	printf("\nIn insertHeader(): sizeof(sf_footer) = : %lu \n", sizeof(sf_footer));

	return ((char*) startingAddress + sizeof(sf_free_header));
}

/* A function that inserts a footer boundary, by asking for more heap memory.
 * Appends to the end of the (header + payload).
 * Note: Must be able to allocate enough space, since this is an insertion. 
 * @param startingAddress : starting address of the footer
 * @param alloc : allocated bits
 * @param block_size: block size
 * @return : return address of where footer ends. 
 */
void* insertFooter(void* startingAddress, int alloc, unsigned long block_size){
	sf_footer* footer = (void*)((char*)startingAddress); /* since sf_footer is only 4 bytes, but footer bound is 8 bytes */
	footer->alloc = alloc;
	footer->block_size = (block_size>>4);
	return ((char*) startingAddress+8);
}

/* A function that creates a block in the heap, in the following format 
 * header + space + footer = (multiple * PAGE_SIZE bytes)
 * function returns block of size (multiple * PAGE_SIZE bytes)
 * @param multipleOfPageSize : the multiple amount of PAGE_SIZE. 
 * @return: The return address of the starting point of the block 
 */
void* createAlignedBlock(int multipleOfPageSize){
	/* Create space for header and payload/pointers */
	unsigned long wholeBlockSize = (multipleOfPageSize * PAGE_SIZE);
	int headAndPayload =  wholeBlockSize -8;      /* -8 for span of footer boundary */
	void* headPtr = sf_sbrk(headAndPayload); 	 /* Ask for space*/
	if(headPtr == ((void*)-1)){
		perror(strerror(errno));
		return NULL;
	}
	void* topOfHeap = sf_sbrk(0);                        /* top of heap*/

	/*TEST CODE
	void* endOfHeadPayloadBlock =(void*) ((char*)headPtr + headAndPayload);*/

	/*Initialize header */
	void* endOfHeadPtr = insertHeader(headPtr,0,wholeBlockSize,0); 

	/*Initialize Footer */ 		   
	void* footerStartPtr = sf_sbrk(8);
	if(footerStartPtr == ((void*)-1)){
		perror(strerror(errno));
		return NULL;
	}

	/*Test
	assert(footerStartPtr == topOfHeap);
	assert(endOfHeadPayloadBlock == topOfHeap);*/

	void* footerEndPtr = insertFooter(footerStartPtr,0,wholeBlockSize);
	return headPtr;
}

/* A function returning the number of bytes that is a multiple of 4096, that will fit the requested size 
 * @param size: size request
 * @return : the returned multiple of 4096.
 */
int calculateRequestSize(size_t size){
	/*Calculate what multiple of 4096 to use */
	int multiple = size/4096;
	int leftover = size%4096;

	/*Calculate request size */
	int requestSize=0;
	if(leftover==0)
	    requestSize = multiple*4096;
	else requestSize = ((multiple+1)*4096);

	return requestSize;
}


						/* Methods Dealing With Heap Pointers */

/* A function that takes the start pointer to a block, then returns the pointer to the
 * payload, by skipping the header block, then aligning. 
 * @param node: the pointer to the block
 * @return : the pointer to the payload of the block. 
 */
void* getPayloadPtr(void* node){
	void* tempPtr = (void*)(((char*)node)+8); /*address after skipping the sf_header*/
	void* alignedPayloadStartAddress = alignPtr(tempPtr);
	return alignedPayloadStartAddress;

	/* IGNORE OLD CODE
	char* offset = (((char*)node)+8); 
	if( ((uintptr_t)offset)%16 != 0){
		offset = offset +(((uintptr_t)offset)%16);
	}
	return (void*)offset; */
}

void* alignHeap(){
	void* currentHeap = sf_sbrk(0);
	uintptr_t heapAddress = ((uintptr_t)currentHeap);
	if( heapAddress % 16 ==8)
		return currentHeap;
	else{

		int val = heapAddress%16;
		int offset = 0;
		if(val < 8)
			offset = 8 - val;
		else if(val>8)
			offset = (16-val)+8;
		currentHeap = sf_sbrk(offset);
		currentHeap = (void*)(((char*)currentHeap)+offset);

		/*ALTERNATE CODE FOR WHEN ALIGNING HEAP ADDRESS TO 16.
		int remainder = heapAddress%16;
		int offset = 16-remainder;
		currentHeap = sf_sbrk(offset);
		currentHeap = (void*)(((char*)currentHeap)+offset);*/
	}
	return currentHeap;
}

/* A function that aligns the pointer within the heap */
void* alignPtr(void* pointer){
	uintptr_t pointerAddress = ((uintptr_t)pointer);
	if( pointerAddress % 16 ==0)
		return pointer;
	else{
		printf("\nIN alignPtr(): \n");
		int remainder = pointerAddress%16;
		printf("\nremainder is:  %d\n pointerAddress is : %p \n",remainder, pointer);
		int offset = 16-remainder;
		printf("\noffset is: %d \n",offset);
		pointer = (void*)(((char*)pointer)+offset);
		printf("\n Aligned pointer is : %p \n",pointer);
	}
	return pointer;
}



			/* To Be Implemented Methods */

/* This is a helper function that performs coalesce on the next block */
void* coalesceNextBlock(sf_header* sf_header){
	/* Go through this block's header boundary tag */
	/* Jump to the next block */
	/* Store next block's size*/
	/* Add next block size to this block's size */
	/* Go to next block's footer and update */
	/* the offset to the next block size + this block size */
	/* Return this block's pointer */
}


			/* HELPER METHODS FOR SF_FREE() */
bool checkPointerToAllocatedBlockHead(void*ptr){
	/* Check for size of ptr, avoid seg fault*/
	uintptr_t topOfHeapAddress = ((uintptr_t) sf_sbrk(0)); 
	uintptr_t ptrAddress = ((uintptr_t) ptr); 
	if((topOfHeapAddress - ptrAddress)< 0) return false; /* ptr address is beyond heap */
	else if ((topOfHeapAddress-ptrAddress)< 32) return false; /*At least 32 bytes */

	/* Assured enough memory to cast after above checks*/
	sf_header* headPtr = ptr;
	/* Grab data*/
	unsigned long headerAlloc = headPtr->alloc;
	if(headerAlloc!=1) return false;

	/*Compare header's block size and request size */
	unsigned long headerBlockSize = ((headPtr->block_size)<<4); 
	unsigned long headerRequestedSize = (headPtr->requested_size); 
	if(headerRequestedSize>headerBlockSize) return false; /* violates definition of valid block struct */

	/*Compare footer's block size and request size */
	uintptr_t totalRangeOfBlock = ((uintptr_t)ptr + headerBlockSize); /*End of footer */
	if( (topOfHeapAddress-totalRangeOfBlock) < 0) return false; /* Footer can't exist past top of heap */
	uintptr_t footerAddress = totalRangeOfBlock-8; /* start of the presumed footer address*/
	uintptr_t footerStructAddress = footerAddress + 4; /* start of the struct for the footer tag*/

	/*Should now NOT cause a segfault*/
	sf_footer* footerPtr = (void*)footerStructAddress;
	unsigned long footerBlockSize = ((footerPtr->block_size)<<4); 
	unsigned long footerAlloc = (footerPtr->alloc); 
	if(footerAlloc!=1) return false; /*should be an allocated block footer */
	if(footerBlockSize!=headerBlockSize) return false;

	return true; /* Passed all tests */
}


bool validateAddress(void* ptr){
	/* Check null */
	if(ptr==NULL)
		return false;
	/*Check for cases */
	if(checkPointerToAllocatedBlockHead(ptr)==true ){
		return true;
	}
	/* failed tests */
	return false;
}


			/* EXTRA UNNECESSARY METHODS */

/* A function that retrieves the long at address of the block pointer */
unsigned long  getLongAtPointer(void* pointer){
	return (*(unsigned long *)pointer);
}
/* A function that puts the long at address of the block pointer */
void putLongAtPointer(void* pointer, unsigned long value){
	(*(unsigned long *)(pointer)) = value;
}
/*A function that reads the requested size from the pointer block */
unsigned long readRequestedSize(void* ptr){
	/* Initialize Vars */
	unsigned long addressContents = (*((unsigned long*)ptr));
	unsigned long requestedSize = addressContents & 0xffffffff00000000;
	return (requestedSize>>32);
}
/* Helper function that determines whether the address of the pointer is aligned, by modulo 16 */
bool isAligned(void* address){
	uintptr_t addressInt = ((uintptr_t)address);
	printf("\n %p isAligned() modulo 16 is: %lu\n", address, (addressInt%16));
	if( addressInt % 16 ==0)
		return true;
	else return false;
}

/* A function that writes the requested size into the pointer's header 
 * @param size: the size of the block 
 * @param ptr : the pointer of the block.
*/
void writeRequestedSize(unsigned long size, void* ptr){
	/* Take the requested_size value and shift it to the request_size area */
	unsigned long max = 1;
	max = (max << 32);
	if(size < max){
		unsigned long requested_size = (size<<32);
		/* Convert the pointer, then retrieve its contents, then set it*/
		sf_header header;
		header.requested_size = size;
		memcpy(ptr,&header, sizeof(header));

		/*
		addressContents = (addressContents & 0x00000000ffffffff); 
		*((unsigned long*)ptr) = (addressContents | requested_size);*/
	}
}
/* A function that reads the block size from the block header */
unsigned long readBlockSize(void* ptr){
	/* Initialize Vars */
	unsigned long addressContents = (*((unsigned long*)ptr));
	unsigned long blockSize = addressContents & 0xfffffff0;
	return blockSize;
}
/* This is a helper function that reads the allocation bit and determines if the block is free*/
unsigned long readAllocatedBit(void* ptr){
	unsigned long addressContents = (*((unsigned long*)ptr));
	if((addressContents & 0x1) == 1) return true;
	else return false;
}


